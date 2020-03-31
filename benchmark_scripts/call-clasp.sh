#!/usr/bin/env bash

../../gringo encoding.asp "$2" | ../../clasp -o "$1"