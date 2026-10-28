CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: kvstore tests

kvstore: main.cpp kvstore.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o kvstore

tests: tests.cpp kvstore.hpp
	$(CXX) $(CXXFLAGS) tests.cpp -o tests

clean:
	rm -f kvstore tests kv.log test_kv.log
