#!/usr/bin/env bash

../../gringo encoding.asp "$2" -o smodels | ../../lp2normal | ../../lp2lp2 | ../../clasp -o "$1"