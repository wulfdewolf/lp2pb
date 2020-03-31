#!/usr/bin/env bash

../../gringo encoding.asp "$1" -o smodels | ../../lp2normal | ../../lp2lp2 | ../../lp2sat | ../../roundingsat