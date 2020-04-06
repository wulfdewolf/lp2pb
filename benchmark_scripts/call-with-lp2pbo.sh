#!/usr/bin/env bash

../../gringo encoding.asp "$1" -o smodels | ../../lp2pb | ../../roundingsat --opt-mode=linear 