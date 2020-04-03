#!/usr/bin/env bash

../../gringo encoding.asp "$1" | ../../clasp --quiet=2
ret=$?
exit $ret