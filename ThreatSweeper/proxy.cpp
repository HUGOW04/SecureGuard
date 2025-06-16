#include "proxy.h"
#include <algorithm>

Proxy::Proxy()
    : io_context(),
    acceptor(io_context, tcp::endpoint(tcp::v4(), 8080)) {
}

Proxy::~Proxy() {
    stop();
}

std::string Proxy::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \r\n\t");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \r\n\t");
    return s.substr(start, end - start + 1);
}

std::string Proxy::to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return std::tolower(c);
        });
    return s;
}

void Proxy::load_blocklist_file(const std::string& filename) {
    std::unordered_set<std::string> new_blocked;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::ofstream create_file(filename);
        if (!create_file) {
            std::cerr << "Failed to create blocklist file: " << filename << std::endl;
            return;
        }
        std::cout << "Blocklist file not found. Created empty file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (!line.empty()) {
            new_blocked.insert(to_lower(line));
        }
    }

    {
        std::unique_lock lock(m_BlocklistMutex);
        m_BlockedList = std::move(new_blocked);
    }
}

bool Proxy::is_blocked(const std::string& host) {
    std::string host_lower = to_lower(trim(host));
    std::shared_lock lock(m_BlocklistMutex);

    for (const auto& blocked : m_BlockedList) {
        if (host_lower == blocked ||
            (host_lower.size() > blocked.size() &&
                host_lower.compare(host_lower.size() - blocked.size(), blocked.size(), blocked) == 0 &&
                host_lower[host_lower.size() - blocked.size() - 1] == '.')) {
            return true;
        }
    }
    return false;
}

void Proxy::forward_data(tcp::socket& from, tcp::socket& to) {
    try {
        std::array<char, 8192> data;
        while (m_Running) {
            boost::system::error_code ec;
            std::size_t length = from.read_some(boost::asio::buffer(data), ec);
            if (ec || !m_Running) break;
            boost::asio::write(to, boost::asio::buffer(data, length));
        }
    }
    catch (...) {
        // Fail silently
    }

    // Cleanly shutdown both sockets
    boost::system::error_code ec;
    from.shutdown(tcp::socket::shutdown_both, ec);
    to.shutdown(tcp::socket::shutdown_both, ec);
}

void Proxy::handle_client(tcp::socket socket) {
    auto socket_ptr = std::make_shared<tcp::socket>(std::move(socket));
    {
        std::lock_guard lock(m_SocketMutex);
        m_ClientSockets.push_back(socket_ptr);
    }

    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(*socket_ptr, buffer, "\r\n");

        std::istream stream(&buffer);
        std::string request_line;
        std::getline(stream, request_line);
        if (!request_line.empty() && request_line.back() == '\r')
            request_line.pop_back();

        if (request_line.rfind("CONNECT ", 0) == 0) {
            std::string target = request_line.substr(8);
            target = target.substr(0, target.find(' '));
            auto colon_pos = target.find(':');
            std::string host = target.substr(0, colon_pos);
            std::string port = target.substr(colon_pos + 1);

            if (is_blocked(host)) {
                std::cout << "Blocked HTTPS: " << host << std::endl;
                return;
            }

            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(host, port);
            auto server_socket = std::make_shared<tcp::socket>(io_context);
            boost::asio::connect(*server_socket, endpoints);

            {
                std::lock_guard lock(m_SocketMutex);
                m_ClientSockets.push_back(server_socket);
            }

            std::string response = "HTTP/1.1 200 Connection established\r\n\r\n";
            boost::asio::write(*socket_ptr, boost::asio::buffer(response));

            std::thread t1(&Proxy::forward_data, this, std::ref(*socket_ptr), std::ref(*server_socket));
            std::thread t2(&Proxy::forward_data, this, std::ref(*server_socket), std::ref(*socket_ptr));
            t1.join();
            t2.join();
        }
        else {
            std::string full_request = request_line + "\r\n", host;
            std::string header_line;

            while (std::getline(stream, header_line) && header_line != "\r") {
                if (!header_line.empty() && header_line.back() == '\r')
                    header_line.pop_back();
                full_request += header_line + "\r\n";

                if (header_line.rfind("Host: ", 0) == 0)
                    host = trim(header_line.substr(6));
            }
            full_request += "\r\n";

            if (is_blocked(host)) {
                std::cout << "Blocked HTTP: " << host << std::endl;
                return;
            }

            std::istringstream req_line_stream(request_line);
            std::string method, url, version;
            req_line_stream >> method >> url >> version;

            std::string relative_path = "/";
            auto path_start = url.find('/', url.find("://") + 3);
            if (path_start != std::string::npos)
                relative_path = url.substr(path_start);

            std::ostringstream new_request;
            new_request << method << " " << relative_path << " " << version << "\r\n";
            new_request << full_request.substr(full_request.find("\r\n") + 2);

            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(host, "80");
            auto server_socket = std::make_shared<tcp::socket>(io_context);
            boost::asio::connect(*server_socket, endpoints);

            {
                std::lock_guard lock(m_SocketMutex);
                m_ClientSockets.push_back(server_socket);
            }

            boost::asio::write(*server_socket, boost::asio::buffer(new_request.str()));
            forward_data(*server_socket, *socket_ptr);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Proxy error: " << e.what() << std::endl;
    }
}

void Proxy::do_accept() {
    acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::thread client_thread(&Proxy::handle_client, this, std::move(socket));
            {
                std::lock_guard<std::mutex> lock(m_ClientsMutex);
                m_ClientThreads.emplace_back(std::move(client_thread));
            }
        }
        if (m_Running) {
            do_accept();
        }
        });
}

void Proxy::start() {
    if (m_Running) return;
    m_Running = true;

    load_blocklist_file("blocklist.txt");

    do_accept();

    std::thread([this]() {
        while (m_Running) {
            load_blocklist_file("blocklist.txt");
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        }).detach();

    io_thread = std::thread([this]() {
        io_context.run();
        });
}

void Proxy::stop() {
    if (!m_Running) return;
    m_Running = false;

    boost::system::error_code ec;
    acceptor.close(ec);
    io_context.stop();

    if (io_thread.joinable())
        io_thread.join();

    {
        std::lock_guard<std::mutex> lock(m_SocketMutex);
        for (auto& sock : m_ClientSockets) {
            if (sock && sock->is_open()) {
                boost::system::error_code e;
                sock->shutdown(tcp::socket::shutdown_both, e);
                sock->close(e);
            }
        }
        m_ClientSockets.clear();
    }

    {
        std::lock_guard<std::mutex> lock(m_ClientsMutex);
        for (auto& t : m_ClientThreads) {
            if (t.joinable()) t.join();
        }
        m_ClientThreads.clear();
    }
}
