#!/bin/bash

L1File=$1
HiForestList=$2
outputName=$3

echo $L1File
echo $HiForestList
echo $outputName

tar -xvf inputTarball.tar

root -b <<EOF
.O2
.L makeCaloTree.C+
makeCaloTree("$L1File", "$HiForestList", "$outputName")
.q
EOF
