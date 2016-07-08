#!/usr/bin/env python

import argparse

parser = argparse.ArgumentParser(
    description='Format SWIG-generated python module such that it can be '
    'parsed by doxygen')
parser.add_argument('infile', help='Path to python module')
parser.add_argument('outfile', help='Path to reformatted module')
args = parser.parse_args()

head = True
with open(args.outfile, 'w') as f:
    for line in open(args.infile):
        if head and (line.startswith('#') or not line.strip()):
            continue
        head = False
        f.write(line)
