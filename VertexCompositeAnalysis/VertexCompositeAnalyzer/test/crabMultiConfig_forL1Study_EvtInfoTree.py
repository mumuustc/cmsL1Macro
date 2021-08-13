from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

# We want to put all the CRAB project directories from the tasks we submit here into one common directory.
# That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsername
config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['HeavyIonRPRcd_PbPb2018_offline.db']
config.JobType.psetName = 'PbPbSkimAndTree2018_EvtInfoOnly_cfg.py'
config.JobType.maxMemoryMB = 2000
config.JobType.maxJobRuntimeMin = 1400 
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
#config.Data.splitting = 'LumiBased'
#config.Data.unitsPerJob = 30
#config.Data.totalUnits = 300
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/HI/PromptReco/Cert_326381-327564_HI_PromptReco_Collisions18_JSON_HF_and_MuonPhys.txt'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T1_US_*','T2_US_*', 'T2_CH_CERN']
#config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'
#config.Site.storageSite = 'T2_US_MIT'
#config.Site.storageSite = 'T3_US_Rice'

def submit(config):
    try:
        #crabCommand('submit', config = config, dryrun=True)
        crabCommand('submit', config = config, dryrun=False)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

#############################################################################################
## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
#############################################################################################

dataMap = {
        "run326776": {"mTrigger": "ZeroBias",    "mRunRange": "326776", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run326822": {"mTrigger": "ZeroBias",    "mRunRange": "326822", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run326859": {"mTrigger": "ZeroBias",    "mRunRange": "326859", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run326942": {"mTrigger": "ZeroBias",    "mRunRange": "326942", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327078": {"mTrigger": "ZeroBias",    "mRunRange": "327078", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327148": {"mTrigger": "ZeroBias",    "mRunRange": "327148", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327174": {"mTrigger": "ZeroBias",    "mRunRange": "327174", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327211": {"mTrigger": "ZeroBias",    "mRunRange": "327211", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327238": {"mTrigger": "ZeroBias",    "mRunRange": "327238", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327327": {"mTrigger": "ZeroBias",    "mRunRange": "327327", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327488": {"mTrigger": "ZeroBias",    "mRunRange": "327488", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
        "run327554": {"mTrigger": "ZeroBias",    "mRunRange": "327554", "mDataset": "/HIForward/HIRun2018A-04Apr2019-v1/AOD"},
#        "run326942": {"mTrigger": "MinimumBias", "mRunRange": "326942", "mDataset": "/HIMinimumBias2/HIRun2018A-04Apr2019-v1/AOD"},
        }

## Submit the muon PDs
for key, val in dataMap.items():
    config.General.requestName = 'EvtInfoTree_' + val['mTrigger'] + '_' + key
    config.Data.inputDataset = val['mDataset']
    config.Data.outputDatasetTag = config.General.requestName
    config.Data.runRange = val['mRunRange']
    config.Data.outLFNDirBase = '/store/group/phys_heavyions/%s/L1Emulator/EvtInfoTree/%s' %  (getUsername(), config.General.requestName)

    print("Submitting CRAB job for: " + key)
    submit(config)
