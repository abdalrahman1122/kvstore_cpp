#pragma once
#include <algorithm>
#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// In-memory key-value store with an append-only log for persistence.
// Every write is appended to the log; on startup the log is replayed.
class KVStore {
public:
    explicit KVStore(const std::string& logPath = "") : path_(logPath) {
        if (!path_.empty()) {
            replay();
            log_.open(path_, std::ios::app);
            if (!log_) throw std::runtime_error("cannot open log file: " + path_);
        }
    }

    void set(const std::string& key, const std::string& value) {
        data_[key] = value;
        append("SET " + key + " " + value);
    }

    std::optional<std::string> get(const std::string& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) return std::nullopt;
        return it->second;
    }

    bool del(const std::string& key) {
        bool removed = data_.erase(key) > 0;
        if (removed) append("DEL " + key);
        return removed;
    }

    // Increments an integer value (a missing key starts from 0).
    long long incr(const std::string& key) {
        long long n = 0;
        auto it = data_.find(key);
        if (it != data_.end()) {
            try {
                size_t pos = 0;
                n = std::stoll(it->second, &pos);
                if (pos != it->second.size()) throw std::invalid_argument("");
            } catch (const std::exception&) {
                throw std::invalid_argument("value is not an integer");
            }
        }
        ++n;
        set(key, std::to_string(n));
        return n;
    }

    size_t size() const { return data_.size(); }

    // Sorted keys that start with `prefix`.
    std::vector<std::string> keys(const std::string& prefix = "") const {
        std::vector<std::string> out;
        for (const auto& kv : data_)
            if (kv.first.compare(0, prefix.size(), prefix) == 0) out.push_back(kv.first);
        std::sort(out.begin(), out.end());
        return out;
    }

private:
    void append(const std::string& line) {
        if (!log_.is_open()) return;
        log_ << line << '\n';
        log_.flush();
    }

    void replay() {
        std::ifstream in(path_);
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream ss(line);
            std::string cmd, key;
            ss >> cmd >> key;
            if (cmd == "SET") {
                std::string value;
                std::getline(ss, value);
                if (!value.empty() && value[0] == ' ') value.erase(0, 1);
                data_[key] = value;
            } else if (cmd == "DEL") {
                data_.erase(key);
            }
        }
    }

    std::string path_;
    std::ofstream log_;
    std::unordered_map<std::string, std::string> data_;
};
