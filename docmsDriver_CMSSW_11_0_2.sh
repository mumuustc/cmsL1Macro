#!/bin/bash

globaltag=110X_dataRun2_v12
era=Run2_2018_pp_on_AA
#filein='file:/afs/cern.ch/work/w/wangj/public/HIZeroBiasReducedFormat/HIRun2018A-v1/FABCF51E-4182-C341-B159-BF5C73482FAA.root' ; rawlabel="rawDataReducedFormat" ; config=l1Ntuple_data2018RF ;
# filein='file:/afs/cern.ch/work/w/wangj/public/HIEmptyBX/HIRun2018A-v1/596BE573-BF9D-6548-A994-46113B0151EB.root' ; rawlabel="rawDataRepacker" ; config=l1Ntuple_data2018 ;
#filein='file:/afs/cern.ch/work/w/wangj/public/HIMinimumBias2/HIRun2018A-v1/FC3873C2-BDA3-D747-908D-7C29DC301129.root' ; rawlabel="rawDataRepacker" ; config=l1Ntuple_data2018 ;
filein='file:/eos/cms/store/group/phys_heavyions/shuaiy/L1Emulator/HIForwardRAW/skimedZeroBias.root' ; rawlabel="rawDataRepacker" ; config=l1Ntuple_data2018 ;

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --repacked \
    --python_filename=${config}.py -n 1000 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMUCalo \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_4_1 \
    --filein=${filein}
#    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAW \
#    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
#    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWCalo \
#    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMUCalo \


echo '
# Limit the output messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 200
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

##only store l1EventTree and l1CaloTowerTree (only works for customiseL1Ntuple.L1NtupleRAW)
#process.L1NtupleRAW = cms.Sequence(process.l1EventTree+process.l1CaloTowerTree)

process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(10) 
process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "MYDATA" )
' >> ${config}.py

# store HF depth, ADC, and energy information (significantly increase the size of l1Ntuple.root)
echo '
process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis")
)
process.hfadc = cms.Path(process.HFAdcana)
process.schedule.append(process.hfadc)
' >> ${config}.py
