#!/usr/bin/env bash

../../gringo encoding.asp "$1" -o smodels | ../../lp2normal -e | ../../lp2lp2 | ../../clasp