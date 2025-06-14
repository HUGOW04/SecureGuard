CXX     :=  g++
FLAGS   :=  -std=c++17 -Iinclude -g -Iinclude -Wall -Wextra

INCLUDE :=  include
SRC     :=  src
BIN     :=  bin

LIBS    :=  -lGL -lGLU -lglfw -lglut -lGLEW -lssl -lcrypto -lcurl -lzip
EXE     :=  main
DEPS    :=  $(wildcard $(SRC)/*.cpp)
OBJ     :=  $(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(DEPS))

$(BIN)/%.o: $(SRC)/%.cpp
	mkdir -p $(BIN)
	$(CXX) -c -o $@ $< $(FLAGS) -I$(INCLUDE)

$(BIN)/$(EXE): $(OBJ)
	$(CXX) $(FLAGS) -I$(INCLUDE) -o $@ $^ $(LIBS)

run: $(BIN)/$(EXE)
	./$(BIN)/$(EXE)

start:
	clear
	./$(BIN)/$(EXE)

clean:
	rm -rf $(BIN)/*

setup:
	mkdir -p $(BIN)
	mkdir -p $(SRC)
	mkdir -p $(INCLUDE)
	touch $(SRC)/main.cpp
