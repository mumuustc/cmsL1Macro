1) Produce L1Ntuple
   1-1) Set up L1 emulator framework: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions (CMSSW_11_0_2 is being used and the latest one is CMSSW_11_2_0)
   1-2) cp -r /yourPath/cmsL1Macro/ADC  CMSSW_11_0_2/src/
   1-3) 'scram b -j 8' in CMSSW_11_0_2/src/
   1-4) cp /yourPath/cmsL1Macro/docmsDriver_CMSSW_11_0_2.sh  CMSSW_11_0_2/src/
   1-5) cp /yourPath/cmsL1Macro/crabMultiConfig_ZB_L1Ntuple.py CMSSW_11_0_2/src (just an example for submitting crab job)
   1-5) ./docmsDriver_CMSSW_11_0_2.sh to generate cmsRun configuration (Please comment out the HF ADC information, whose size is huge, if you do not need it)
   1-6) cmsRun l1Ntuple_data2018.py


2) Produce offline event information. You can either use the standard CMS HiForest package or use a simple package only containing needed offline event information
   2-1) Setup CMSSW_10_3_3_patch1 environment
   2-2) cp -r /yourPath/cmsL1Macro/VertexCompositeAnalysis CMSSW_10_3_3_patch1/src
   2-3) 'scram b -j 8' in CMSSW_10_3_3_patch1/src
   2-4) cd VertexCompositeAnalysis/VertexCompositeAnalyzer/test
   2-5) cmsRun PbPbSkimAndTree2018_EvtInfoOnly_cfg.py

3) Combine L1 (HF, calorimeter, etc.) and offline information into plain root
   3-1) cd cmsL1Macro/makeCaloTree
   3-2) root -l -b -q makeCaloTree.C+ to do local test
   3-3) ./submitJob.sh to submit condor job (for test purpose, please excute './submitJob_test.sh')

4) For L1 trigger efficiency study
   4-1) EvtTowerInfoNTuple.h and EvtTowerInfoNTuple.C in cmsL1Macro/common directory are genereated by the ROOT MakeClass() function based on the output of 3-3)
        a) root -l /eos/cms/store/group/phys_heavyions/shuaiy/L1Emulator/L1AndOfflineEvtMatch/L1NtupleRAWEMUCalo_HFAdc_smallStat/output/EvtL1CaloInfo_326776_66.root
        b) EvtTowerInfoNTuple->MakeClass()
        NOTE, the array length of nHCALTPs, nTowers, nHCALTPsEmu, and nTowersEmu should be modified to a large number (e.g., 8000) to avoid memory leak

5) L1 Calorimeter study (check various distributions between noise and collisions)
   # coming soon

6) L1 HF study (check various distributions between noise and collisions)
   # coming soon
#   1) cd cmsL1Macro/HFNoiseStudy
#   2) check details in anaNoise.C (this macro generates needed histograms by combining L1 infomation and offline event infomation. The reason of not generating a plain root tree, is just because the size of HF L1 information is huge)
