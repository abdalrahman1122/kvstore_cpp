#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

const std::string FILE_NAME = "data.txt";

// File format: one pair per line -> "key value"
std::map<std::string, std::string> loadData() {
    std::map<std::string, std::string> data;
    std::ifstream in(FILE_NAME);
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string key, value;
        ss >> key;
        std::getline(ss, value);
        if (!value.empty() && value[0] == ' ') value.erase(0, 1);
        if (!key.empty()) data[key] = value;
    }
    return data;
}

void saveData(const std::map<std::string, std::string>& data) {
    std::ofstream out(FILE_NAME);
    for (const auto& pair : data) out << pair.first << ' ' << pair.second << '\n';
}

void printUsage() {
    std::cout << "Usage:\n"
                 "  ./kv set <key> <value>\n"
                 "  ./kv get <key>\n"
                 "  ./kv del <key>\n"
                 "  ./kv list\n";
}

int main(int argc, char** argv) {
    if (argc < 2) { printUsage(); return 1; }

    std::string cmd = argv[1];
    auto data = loadData();

    if (cmd == "set" && argc >= 4) {
        std::string value;
        for (int i = 3; i < argc; i++) value += (i > 3 ? " " : "") + std::string(argv[i]);
        data[argv[2]] = value;
        saveData(data);
        std::cout << "OK\n";
    } else if (cmd == "get" && argc == 3) {
        auto it = data.find(argv[2]);
        if (it == data.end()) { std::cout << "Error: key not found\n"; return 1; }
        std::cout << it->second << "\n";
    } else if (cmd == "del" && argc == 3) {
        if (data.erase(argv[2]) == 0) { std::cout << "Error: key not found\n"; return 1; }
        saveData(data);
        std::cout << "OK\n";
    } else if (cmd == "list") {
        if (data.empty()) std::cout << "No data\n";
        for (const auto& pair : data) std::cout << pair.first << " = " << pair.second << "\n";
    } else {
        printUsage();
        return 1;
    }
    return 0;
}
