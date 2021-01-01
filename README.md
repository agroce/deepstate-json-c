This is used to test https://github.com/json-c/json-c

The harness can be compiled after building/installing json-c like this:

```
clang++ -o TestJSON TestJSON.cpp -ldeepstate -I/usr/local/include/json-c/ /usr/local/lib/libjson-c.a
```

or for a specific fuzzer, e.g.,:

```
deepstate-afl --compile_test TestJSON.cpp  --out_test_name TestJSON --compiler_args "-I/usr/local/include/json-c/ /usr/local/lib/libjson-c.a"
```

Seeding with strings from a good JSON test suite can help fuzzers out, but you'll need to generate DeepState format files.  Will add a Python script to take a set of text files and transform into strings with the length in the first four bytes soon.