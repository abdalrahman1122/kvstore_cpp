# KVStore-CPP

A small key-value storage engine in C++17 with a command-line interface and crash-safe persistence using an append-only log (the same idea Redis AOF uses).

## Features
- In-memory storage with `std::unordered_map` (average O(1) get/set/delete)
- Append-only log: every write is saved to disk and replayed on startup
- Commands: `SET`, `GET`, `DEL`, `INCR`, `KEYS [prefix]`, `COUNT`, `EXIT`
- Input validation (e.g. `INCR` on a non-integer value returns an error)
- 4 unit tests using `assert`, including a persistence test

## Build and run
```bash
make          # builds ./kvstore and ./tests
./tests       # runs the unit tests
./kvstore     # starts the CLI (log file: kv.log)
```

## Example
```
> SET name Abdalrahman
OK
> INCR visits
1
> KEYS
name
visits
> DEL name
1
```

## Project structure
- `kvstore.hpp`: the `KVStore` class (storage + log replay)
- `main.cpp`: command parser / CLI
- `tests.cpp`: unit tests

## Possible next steps
- Key expiry (TTL), log compaction, a TCP server mode
