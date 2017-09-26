#!/usr/bin/env bash
python ./RMC_vasp.py $*
python ./RMC_rate.py output_surface.txt
