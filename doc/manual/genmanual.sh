#!/bin/bash
cat 01_preface.md 02_soft_introduction.md 03_porto_overview.md 04_usecase1.md 05_usecase2.md > manual.md
pandoc -V geometry:margin=1in -f markdown -o manual.pdf manual.md  --latex-engine=xelatex --toc --wrap=preserve
