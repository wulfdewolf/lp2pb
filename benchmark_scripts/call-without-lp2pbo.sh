#!/usr/bin/env bash

../../gringo encoding.asp "$1" -o smodels | ../../lp2normal -e | ../../lp2lp2 | ../../lp2sat | ../../roundingsat --opt-mode=linear --verbosity=0