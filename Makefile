kv: main.cpp
	g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o kv

clean:
	rm -f kv data.txt
