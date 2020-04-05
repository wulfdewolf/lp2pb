#!/usr/bin/env bash

ulimit -St 2 -Ht 5

gringo benchmarks/BayesianNL/encoding.asp benchmarks/BayesianNL/instances/058-voting-bic.lp | clasp --quiet=2 > ppp