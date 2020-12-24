This is used to test https://github.com/json-c/json-c

The harness can be compiled after building/installing json-c like this:

```
clang++ -o TestJSON TestJSON.cpp -ldeepstate -I/usr/local/include/json-c/ /usr/local/lib/libjson-c.a
```

or for a specific fuzzer, e.g.,:

```
deepstate-afl --compile_test TestJSON.cpp  --out_test_name TestJSON --compiler_args "-I/usr/local/include/json-c/ /usr/local/lib/libjson-c.a"
```

If you include numeric values in tests, then the RoundTrip will fail for some raw numerics:

```
TRACE: Initialized test input buffer with data from `r.fail`
TRACE: Running: JSON_RoundTrip from TestJSON.cpp(8)
TRACE: TestJSON.cpp(13): String 1:8,
TRACE: TestJSON.cpp(14): Length 1:2
TRACE: TestJSON.cpp(17): Type 1:3
TRACE: TestJSON.cpp(21): String 2:8
TRACE: TestJSON.cpp(22): Length 2:1
CRITICAL: TestJSON.cpp(25): Object 2 should not be null!
ERROR: Failed: JSON_RoundTrip
ERROR: Test case r.fail failed
```
