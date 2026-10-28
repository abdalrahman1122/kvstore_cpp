#include <iostream>
#include "kvstore.hpp"

int main(int argc, char** argv) {
    std::string logPath = argc > 1 ? argv[1] : "kv.log";
    KVStore store(logPath);
    std::cout << "KVStore ready (log: " << logPath << "). Commands: SET GET DEL INCR KEYS COUNT EXIT\n";

    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        std::istringstream ss(line);
        std::string cmd, key;
        ss >> cmd >> key;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        if (cmd == "EXIT") break;
        else if (cmd == "SET") {
            std::string value;
            std::getline(ss, value);
            if (!value.empty() && value[0] == ' ') value.erase(0, 1);
            if (key.empty() || value.empty()) { std::cout << "ERR usage: SET key value\n"; continue; }
            store.set(key, value);
            std::cout << "OK\n";
        } else if (cmd == "GET") {
            auto v = store.get(key);
            std::cout << (v ? *v : "(nil)") << '\n';
        } else if (cmd == "DEL") {
            std::cout << (store.del(key) ? "1" : "0") << '\n';
        } else if (cmd == "INCR") {
            try { std::cout << store.incr(key) << '\n'; }
            catch (const std::exception& e) { std::cout << "ERR " << e.what() << '\n'; }
        } else if (cmd == "KEYS") {
            for (const auto& k : store.keys(key)) std::cout << k << '\n';
        } else if (cmd == "COUNT") {
            std::cout << store.size() << '\n';
        } else if (!cmd.empty()) {
            std::cout << "ERR unknown command\n";
        }
    }
}
