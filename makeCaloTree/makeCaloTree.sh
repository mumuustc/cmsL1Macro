#!/bin/bash

nEvts=$1
L1File=$2
HiForestList=$3
outputName=$4

echo $nEvts
echo $L1File
echo $HiForestList
echo $outputName

tar -xvf inputTarball.tar

root -b <<EOF
.O2
.L makeCaloTree.C+
makeCaloTree($nEvts, "$L1File", "$HiForestList", "$outputName")
.q
EOF
