# KVStore-CPP

A simple key-value store for the command line, written in C++17. Data is saved in a text file, so it is still there the next time you run the program.

## Commands
```bash
./kv set name Abdalrahman   # save a value
./kv get name               # read a value
./kv del name               # delete a key
./kv list                   # show all keys and values
```

## Example
```
$ ./kv set city Assiut
OK
$ ./kv get city
Assiut
$ ./kv list
city = Assiut
```

## Build
```bash
make
```

## How it works
- Data is kept in a `std::map` (sorted by key) and stored in `data.txt`, one `key value` pair per line.
- Every command loads the file, changes the map, and saves the file again.
- Missing keys and wrong usage show an error message.
