#!/usr/bin/env python

################################################################################
#
# RMC-vasp
#
# Retriving output data from DFT calculations
#
# Part of the REMARC set of scripts for retriving output data
# from DFT calculations (using VASP).
#
# Specifically writes the DFT data to a text-file that is input for
# RMC-rate. RMC-vasp relies on vasp_data and vasp_outcar modules.
# The VASP DFT calculations must be organized according the manual.
# Requires that the VASP-files, CONTCAR and OUTCAR, are placed in
# the corresponding directories.
#
# Input: root_directory, path to the VASP calculations
# Optional: -g path to the directory for isolated atoms/molecules
#              (default: root_directory/gasphase)
#           -o name of output text-file (default: output)
#
# Output: Text-file with DFT data (input for RMC_rate)
#
# Version 1.0
#
################################################################################
from __future__ import print_function

import os
import os.path
from vasp_data import VASP_DATA

import argparse
from optparse import OptionParser


parser = OptionParser(usage="%prog rootdir [options]",
                      version='%prog 1.0')

parser.add_option('-g', dest='gasphase_directory', action='store',
                  default='gasphase',
                  help='path to directory containing gasphase speces - '
                  'default: rootdir/%default')
parser.add_option('-o', '--output', default='output',
                  help='Output uri. For the txt driver, this is the output '
                  'base name - default: %default')
parser.add_option('-d', '--driver', default='txt',
                  help='Output driver; can be any driver supported by SOFT '
                  '(hdf5, json, mongo...) in addition to txt '
                  '- default: %default')
parser.add_option('--options',
                  help='Additional options passed to the driver.')

opts, args = parser.parse_args()

if len(args) == 0:
    parser.error("The directory (rootdir) must be given")
elif len(args) >1:
    parser.error('Too many arguments. Only the directory (rootdir) must '
                 'be given')

# Append output file extension if it is missing
if opts.driver in ('hdf5', 'json') and not '.' in os.path.basename(opts.output):
    opts.output += '.' + opts.driver


rootdir = args[0]
if rootdir[-1] == '/':
    rootdir = rootdir[:-1]
print("\nExtracting VASP data from: " + str(rootdir))


if opts.gasphase_directory == 'gasphase':
    gasphase_dir = os.path.join(rootdir, 'gasphase')
else:
    gasphase_dir = opts.gasphase_directory

OA = VASP_DATA(rootdir, gasphase_dir)
OA.write_outputdata(uri=opts.output, driver=opts.driver,
                    options=opts.options)
