import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process('ANASKIM',eras.Run2_2018_pp_on_AA)

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

# Limit the output messages
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 200
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# Define the input source
process.source = cms.Source("PoolSource",
#   fileNames = cms.untracked.vstring('root://cmsxrootd.fnal.gov//store/hidata/HIRun2018A/HIMinimumBias2/AOD/04Apr2019-v1/50000/F134F372-A1D6-9844-AB54-77742E73027C.root'),
#   fileNames = cms.untracked.vstring('/store/hidata/HIRun2018A/HIMinimumBias2/AOD/04Apr2019-v1/50000/F134F372-A1D6-9844-AB54-77742E73027C.root'),

   # for Zero-bias data
   fileNames = cms.untracked.vstring('root://cmsxrootd.fnal.gov//store/hidata/HIRun2018A/HIForward/AOD/04Apr2019-v1/100000/6BEF51FA-3E17-7447-A6B0-075B4C067A68.root'),

#   # for EmptyBX data
#   fileNames = cms.untracked.vstring('/store/hidata/HIRun2018A/HIEmptyBX/AOD/27Feb2019-v1/110000/E927D83E-B26A-2341-BA56-9B61E731B4CA.root'),

#   # for Cosmic data
#   fileNames = cms.untracked.vstring('/store/hidata/HIRun2018A/Cosmics/AOD/PromptReco-v2/000/327/693/00000/5DD6B66F-24BD-3041-9301-8E56FD73EB01.root'),

#   fileNames = cms.untracked.vstring('root://cmsxrootd.fnal.gov//store/hidata/HIRun2018A/HIForward/AOD/04Apr2019-v1/50006/28E4F661-1674-2E42-B8BF-6C9F643E185A.root'),

   inputCommands=cms.untracked.vstring('keep *', 'drop *_hiEvtPlane_*_*')
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(3000))

# Set the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = cms.string('103X_dataRun2_v6')

# Add PbPb centrality
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')
process.hiCentrality.produceHFhits = False
process.hiCentrality.produceHFtowers = False
process.hiCentrality.produceEcalhits = False
process.hiCentrality.produceZDChits = True
process.hiCentrality.produceETmidRapidity = False
process.hiCentrality.producePixelhits = False
process.hiCentrality.produceTracks = False
process.hiCentrality.producePixelTracks = False
process.hiCentrality.reUseCentrality = True
process.hiCentrality.srcZDChits = cms.InputTag("QWzdcreco")
process.hiCentrality.srcReUse = cms.InputTag("hiCentrality","","reRECO")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.centralityBin.nonDefaultGlauberModel = cms.string("")
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("HeavyIonRcd"),
        tag = cms.string("CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run2v1033p1x01_offline"),
        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
        label = cms.untracked.string("HFtowers")
        ),
    ])
process.cent_seq = cms.Sequence(process.hiCentrality * process.centralityBin)

# Add PbPb event plane
process.load("RecoHI.HiEvtPlaneAlgos.HiEvtPlane_cfi")
process.load("RecoHI.HiEvtPlaneAlgos.hiEvtPlaneFlat_cfi")
process.hiEvtPlane.trackTag = cms.InputTag("generalTracks")
process.hiEvtPlane.vertexTag = cms.InputTag("offlinePrimaryVerticesRecovery")
process.hiEvtPlane.loadDB = cms.bool(True)
process.hiEvtPlane.useNtrk = cms.untracked.bool(False)
process.hiEvtPlane.caloCentRef = cms.double(-1)
process.hiEvtPlane.caloCentRefWidth = cms.double(-1)
process.hiEvtPlaneFlat.caloCentRef = cms.double(-1)
process.hiEvtPlaneFlat.caloCentRefWidth = cms.double(-1)
process.hiEvtPlaneFlat.vertexTag = cms.InputTag("offlinePrimaryVerticesRecovery")
process.hiEvtPlaneFlat.useNtrk = cms.untracked.bool(False)
process.CondDB.connect = "sqlite_file:HeavyIonRPRcd_PbPb2018_offline.db"
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
                                       process.CondDB,
                                       toGet = cms.VPSet(cms.PSet(record = cms.string('HeavyIonRPRcd'),
                                                                  tag = cms.string('HeavyIonRPRcd_PbPb2018_offline')
                                                                  )
                                                         )
                                      )
process.es_prefer_flatparms = cms.ESPrefer('PoolDBESSource','')
process.evtplane_seq = cms.Sequence(process.hiEvtPlane * process.hiEvtPlaneFlat)

# Add trigger selection
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltFilter.andOr = cms.bool(True)
process.hltFilter.throw = cms.bool(False)
process.hltFilter.HLTPaths = [
    # Double muon triggers
#    'HLT_HIL1DoubleMuOpen_OS_Centrality_40_100_v*', # Peripheral OS dimuons
#    'HLT_HIL1DoubleMuOpen_Centrality_50_100_v*', # Peripheral dimuons
#    'HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf_v*', # Bottomonia
#    'HLT_HIL1DoubleMu10_v*', # Z bosons
#    'HLT_HIUPC_DoubleMu0_NotMBHF2AND_v*', # UPC dimuons
    # Single muon triggers
#    'HLT_HIL1MuOpen_Centrality_80_100_v*', # Peripheral muons
#    'HLT_HIL3Mu12_v*', # Electroweak bosons
#    'HLT_HIUPC_SingleMuOpen_NotMBHF2AND_v*', # UPC muons
#    'HLT_HIL3Mu3_NHitQ10_v*',  # Low pT muons
    # MinimumBias 
    'HLT_HIZeroBias_v*', # ZeroBias trigger
    # EmptyBX
#    'HLT_HIL1NotBptxOR_v*' # Empty BX trigger
    ]

# Add PbPb collision event selection
process.load('VertexCompositeAnalysis.VertexCompositeProducer.collisionEventSelection_cff')
process.load('VertexCompositeAnalysis.VertexCompositeProducer.clusterCompatibilityFilter_cfi')
process.load('VertexCompositeAnalysis.VertexCompositeProducer.hfCoincFilter_cff')
process.load("VertexCompositeAnalysis.VertexCompositeProducer.OfflinePrimaryVerticesRecovery_cfi")
process.colEvtSel = cms.Sequence(process.hfCoincFilter2Th4 * process.primaryVertexFilter * process.clusterCompatibilityFilter)

# Define the event selection sequence
process.eventFilter_HM = cms.Sequence(
    process.hltFilter *
    process.offlinePrimaryVerticesRecovery
)
process.eventFilter_HM_step = cms.Path( process.eventFilter_HM )

# ZDC info
process.load('VertexCompositeAnalysis.VertexCompositeProducer.QWZDC2018Producer_cfi')
process.load('VertexCompositeAnalysis.VertexCompositeProducer.QWZDC2018RecHit_cfi')

# Define the analysis steps
process.pcentandep_step = cms.Path(process.eventFilter_HM * process.zdcdigi * process.QWzdcreco * process.cent_seq * process.evtplane_seq)

process.load("VertexCompositeAnalysis.VertexCompositeAnalyzer.eventinfotree_cff")
process.eventinfoana.selectEvents = cms.untracked.string("eventFilter_HM_step")

# Define the output
process.TFileService = cms.Service("TFileService", fileName = cms.string('evtinfo.root'))
process.p = cms.EndPath(process.eventinfoana)

#process.load("FlowCorrAna.DiHadronCorrelationAnalyzer.track_cff")
#process.ptrk = cms.Path(process.eventFilter_HM * process.track_ana)
#process.track_ana.IsDebug = cms.bool(True)

# Define the process schedule
process.schedule = cms.Schedule(
    process.eventFilter_HM_step,
    process.pcentandep_step,
#    process.ptrk,
    process.p
)

# Add the event selection filters
process.Flag_colEvtSel = cms.Path(process.eventFilter_HM * process.colEvtSel)
process.Flag_hfCoincFilter2Th4 = cms.Path(process.eventFilter_HM * process.hfCoincFilter2Th4)
process.Flag_primaryVertexFilter = cms.Path(process.eventFilter_HM * process.primaryVertexFilter)
process.Flag_clusterCompatibilityFilter = cms.Path(process.eventFilter_HM * process.clusterCompatibilityFilter)
process.Flag_hfPosFilterTh3 = cms.Path(process.eventFilter_HM * process.hfPosFilterTh3_seq)
process.Flag_hfPosFilterTh4 = cms.Path(process.eventFilter_HM * process.hfPosFilterTh4_seq)
process.Flag_hfPosFilterTh5 = cms.Path(process.eventFilter_HM * process.hfPosFilterTh5_seq)
process.Flag_hfPosFilterTh6 = cms.Path(process.eventFilter_HM * process.hfPosFilterTh6_seq)
process.Flag_hfPosFilterTh7 = cms.Path(process.eventFilter_HM * process.hfPosFilterTh7_seq)
process.Flag_hfPosFilterTh8 = cms.Path(process.eventFilter_HM * process.hfPosFilterTh8_seq)
process.Flag_hfPosFilterTh7p3 = cms.Path(process.eventFilter_HM * process.hfPosFilterTh7p3_seq)
process.Flag_hfNegFilterTh3 = cms.Path(process.eventFilter_HM * process.hfNegFilterTh3_seq)
process.Flag_hfNegFilterTh4 = cms.Path(process.eventFilter_HM * process.hfNegFilterTh4_seq)
process.Flag_hfNegFilterTh5 = cms.Path(process.eventFilter_HM * process.hfNegFilterTh5_seq)
process.Flag_hfNegFilterTh6 = cms.Path(process.eventFilter_HM * process.hfNegFilterTh6_seq)
process.Flag_hfNegFilterTh7 = cms.Path(process.eventFilter_HM * process.hfNegFilterTh7_seq)
process.Flag_hfNegFilterTh8 = cms.Path(process.eventFilter_HM * process.hfNegFilterTh8_seq)
process.Flag_hfNegFilterTh7p6 = cms.Path(process.eventFilter_HM * process.hfNegFilterTh7p6_seq)

eventFilterPaths = [ process.Flag_colEvtSel , process.Flag_hfCoincFilter2Th4 , process.Flag_primaryVertexFilter, process.Flag_clusterCompatibilityFilter, process.Flag_hfPosFilterTh3, process.Flag_hfNegFilterTh3,process.Flag_hfPosFilterTh4, process.Flag_hfNegFilterTh4, process.Flag_hfPosFilterTh5, process.Flag_hfNegFilterTh5, process.Flag_hfPosFilterTh6, process.Flag_hfNegFilterTh6, process.Flag_hfPosFilterTh7, process.Flag_hfNegFilterTh7, process.Flag_hfPosFilterTh8, process.Flag_hfNegFilterTh8, process.Flag_hfPosFilterTh7p3, process.Flag_hfNegFilterTh7p6 ]

for P in eventFilterPaths:
    process.schedule.insert(0, P)

# Add recovery for offline primary vertex
from HLTrigger.Configuration.CustomConfigs import MassReplaceInputTag
process = MassReplaceInputTag(process,"offlinePrimaryVertices","offlinePrimaryVerticesRecovery")
process.offlinePrimaryVerticesRecovery.oldVertexLabel = "offlinePrimaryVertices"
