#!/bin/bash

cp ../cmake-build-debug/bin/latestLog.txt ./opcodeCount.txt
python3 opcodeCompare.py
