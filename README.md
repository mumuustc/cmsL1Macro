Produce L1Ntuple
   1) Set up L1 emulator framework: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions (CMSSW_11_0_2 is being used and the latest one is CMSSW_11_2_0)
   2) cp -r /yourPath/cmsL1Macro/ADC  CMSSW_11_0_2/src/
   3) 'scram b -j 8' in CMSSW_11_0_2/src/
   4) cp /yourPath/cmsL1Macro/docmsDriver_CMSSW_11_0_2.sh  CMSSW_11_0_2/src/
   5) cp /yourPath/cmsL1Macro/crabMultiConfig_ZB_L1Ntuple.py CMSSW_11_0_2/src (just an example for submitting crab job)
   5) ./docmsDriver_CMSSW_11_0_2.sh to generate cmsRun configuration (Please comment out the HF ADC information, whose size is huge, if you do not need it)
   6) cmsRun l1Ntuple_data2018.py


Produce offline event information. You can either use the standard CMS HiForest package or use a simple package only containing needed offline event information
   1) Setup CMSSW_10_3_3_patch1 environment
   2) cp -r /yourPath/cmsL1Macro/VertexCompositeAnalysis CMSSW_10_3_3_patch1/src
   3) 'scram b -j 8' in CMSSW_10_3_3_patch1/src
   4) cd VertexCompositeAnalysis/VertexCompositeAnalyzer/test
   5) cmsRun PbPbSkimAndTree2018_EvtInfoOnly_cfg.py

For barrel calorimeter related study
   1) cd /yourPath/cmsL1Macro/anaCalo
   2) root -l -b -q makeCaloTree.C+ to do some local test (this macro generates a plain root tree containing calorimeter L1 information and offline event information)
   3) ./submitJob.sh to submit condor job

For HF related study
   1) cd /yourPath/cmsL1Macro/HFNoiseStudy
   2) check details in anaNoise.C (this macro generates needed histograms by combining L1 infomation and offline event infomation. The reason of not generating a plain root tree, is just because the size of HF L1 information is huge)

