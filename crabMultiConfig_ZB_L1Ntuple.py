from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

# We want to put all the CRAB project directories from the tasks we submit here into one common directory.
# That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
from CRABClient.UserUtilities import config
config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'l1Ntuple_data2018.py'
config.JobType.maxMemoryMB = 2000
config.JobType.maxJobRuntimeMin = 1000 
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
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
        crabCommand('submit', config = config, dryrun=False)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

#############################################################################################
## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
#############################################################################################

dataMap = {
        "run326776": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run326776-94cbf4add1123a786b867f646bf0f0da/USER"},
        "run326822": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run326822-94cbf4add1123a786b867f646bf0f0da/USER"},
        "run327078": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327078-94cbf4add1123a786b867f646bf0f0da/USER"},
        "run327174": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327174-94cbf4add1123a786b867f646bf0f0da/USER"},
        "run327327": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327327-94cbf4add1123a786b867f646bf0f0da/USER"},
        "run327554": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327554-94cbf4add1123a786b867f646bf0f0da/USER"},
#        "run326859": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run326859-94cbf4add1123a786b867f646bf0f0da/USER"},
#        "run326942": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run326942-94cbf4add1123a786b867f646bf0f0da/USER"},
#        "run327148": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327148-94cbf4add1123a786b867f646bf0f0da/USER"},
#        "run327211": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327211-94cbf4add1123a786b867f646bf0f0da/USER"},
#        "run327238": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327238-94cbf4add1123a786b867f646bf0f0da/USER"},
#        "run327488": { "mDataset": "/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run327488-94cbf4add1123a786b867f646bf0f0da/USER"},
        }

## Submit job for different datasets 
for key, val in dataMap.items():
    config.General.requestName = "L1NtupleRAWEMUCalo_ZeroBias_" + key
    config.Data.inputDataset = val['mDataset']
    config.Data.outputDatasetTag = config.General.requestName
#    config.Data.outLFNDirBase = '/store/user/shuaiy/RiceHIN/L1Emulator/%s' %  config.General.requestName
    config.Data.outLFNDirBase = '/store/group/phys_heavyions/shuaiy/L1Emulator/L1Ntuple/%s' %  config.General.requestName
    print("Submitting CRAB job for: "+val["mDataset"])
    submit(config)
