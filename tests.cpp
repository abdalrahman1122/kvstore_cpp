#include <cassert>
#include <cstdio>
#include <iostream>
#include "kvstore.hpp"

void testBasic() {
    KVStore s;
    s.set("a", "1");
    assert(*s.get("a") == "1");
    assert(!s.get("missing"));
    assert(s.del("a") && !s.del("a"));
}

void testIncr() {
    KVStore s;
    assert(s.incr("n") == 1 && s.incr("n") == 2);
    s.set("t", "abc");
    bool threw = false;
    try { s.incr("t"); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}

void testKeysPrefix() {
    KVStore s;
    s.set("user:2", "b"); s.set("user:1", "a"); s.set("post:1", "c");
    auto k = s.keys("user:");
    assert(k.size() == 2 && k[0] == "user:1" && k[1] == "user:2");
}

void testPersistence() {
    const char* path = "test_kv.log";
    std::remove(path);
    {
        KVStore s(path);
        s.set("name", "Ahmed Ali");
        s.set("x", "10");
        s.incr("x");
        s.del("name");
    }
    KVStore s2(path);  // replays the log
    assert(!s2.get("name"));
    assert(*s2.get("x") == "11");
    assert(s2.size() == 1);
    std::remove(path);
}

int main() {
    testBasic(); testIncr(); testKeysPrefix(); testPersistence();
    std::cout << "All tests passed\n";
}
