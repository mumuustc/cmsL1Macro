#!/bin/bash

#ntupleType="test"
ntupleType="L1NtupleRAWEMUCalo_HFAdc"

initDir="$PWD/condor_projects/$ntupleType"
mkdir -p $initDir
rm -rf $initDir/*

mkdir -p $initDir/log $initDir/output

tar -cvf $initDir/inputTarball.tar anaNoise.C *.h ZBEvtInfo_RunByRunList

condorJobCfg="$initDir/condorJob_${ntupleType}.con"

cat > $condorJobCfg <<- _EOF_
Universe     = vanilla
GetEnv       = False
InitialDir   = $initDir
+JobFlavour  = "testmatch"
Requirements = (OpSysAndVer =?= "CentOS7")
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = inputTarball.tar
Executable = anaNoise.sh
_EOF_

#+JobFlavour  = "espresso"      # 20 mins
#+JobFlavour  = "microcentury"  # 1 hour
#+JobFlavour  = "workday"       # 8 hours
#+JobFlavour  = "tomorrow"      # 1 day
#+JobFlavour  = "testmatch"     # 3 days

nEvts=-1;

for inputFile in `cat ${ntupleType}.list`
do
    echo $inputFile

    substr=${inputFile#*run} 
    runNum=${substr:0:6}
    echo $runNum

    substrWoRoot=${inputFile%.root} 
    idx=${substrWoRoot##*L1Ntuple_}
    echo $idx
    echo 

    outputName=L1HFHisto_${runNum}_$idx

cat >> $condorJobCfg <<- _EOF_

arguments      = $nEvts $inputFile ZBEvtInfo_RunByRunList/${runNum}.list $outputName
output         = log/${outputName}.out
error          = log/${outputName}.err
log            = log/${outputName}.olog
#transfer_output_files = ${outputName}.root 
transfer_output_remaps = "${outputName}.root = output/${outputName}.root"
Queue
_EOF_

# If only the outputName.root should be transferred, please turn on the transfer_output_files setting 

done

condor_submit $condorJobCfg
