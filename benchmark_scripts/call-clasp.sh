#!/usr/bin/env bash

# Set limits
ulimit -St 1200
ulimit -Ht 1203
ulimit -Hv 14680064 -Sm 12582912

../../gringo encoding.asp "$1" | ../../clasp --quiet=2