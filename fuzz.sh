#!/bin/sh
afl-fuzz -P examples/fuzzy_invp/fuzzy_invp.tst  -m none -i seed_corpus -o out -- /home/shang/CPFuzz/examples/fuzzy_invp/fuzz-target 10 1 0 0 3 1