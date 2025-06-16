#ifndef PROXY_H
#define PROXY_H

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <array>
#include <sstream>
#include <atomic>
#include <unordered_set>
#include <fstream>
#include <shared_mutex>
#include <vector>
#include <mutex>

using boost::asio::ip::tcp;

class Proxy {
public:
    Proxy();
    ~Proxy();

    void start();
    void stop();

private:
    void do_accept();
    void load_blocklist_file(const std::string& filename);
    bool is_blocked(const std::string& host);
    std::string trim(const std::string& s);
    std::string to_lower(std::string s);

    void handle_client(tcp::socket socket);
    void forward_data(tcp::socket& from, tcp::socket& to);

private:
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;

    std::thread io_thread;

    std::unordered_set<std::string> m_BlockedList;
    mutable std::shared_mutex m_BlocklistMutex;
    std::vector<std::shared_ptr<tcp::socket>> m_ClientSockets;
    std::mutex m_SocketMutex;

    std::atomic<bool> m_Running{ false };

    std::mutex m_ClientsMutex;
    std::vector<std::thread> m_ClientThreads;
};

#endif // PROXY_H
