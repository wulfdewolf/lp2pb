#!/usr/bin/env bash

../../gringo encoding.asp "$1" -o smodels | ../../lp2shift | ../../lp2pb | ../../roundingsat 