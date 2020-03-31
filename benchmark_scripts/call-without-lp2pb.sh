#!/usr/bin/env bash

../../gringo encoding.asp "$2" -o smodels | ../../lp2normal | ../../lp2lp2 | ../../lp2sat | ../../roundingsat -o "$1"