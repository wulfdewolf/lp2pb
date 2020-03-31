#!/usr/bin/env bash

../../gringo encoding.asp "$2" -o smodels | ../../lp2pb | ../../roundingsat -o "$1"