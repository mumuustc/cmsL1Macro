import FWCore.ParameterSet.Config as cms

# make calotowers into candidates
towersAboveThreshold = cms.EDProducer("CaloTowerCandidateCreator",
    src = cms.InputTag("towerMaker"),
    verbose = cms.untracked.int32(0),
    minimumE = cms.double(3.0),
    minimumEt = cms.double(0.0),
)

# make calotowers into candidates with threshold 4
towersAboveThresholdTh2 = towersAboveThreshold.clone(minimumE=cms.double(2.0))
towersAboveThresholdTh4 = towersAboveThreshold.clone(minimumE=cms.double(4.0))
towersAboveThresholdTh5 = towersAboveThreshold.clone(minimumE=cms.double(5.0))
towersAboveThresholdTh6 = towersAboveThreshold.clone(minimumE=cms.double(6.0))
towersAboveThresholdTh7 = towersAboveThreshold.clone(minimumE=cms.double(7.0))
towersAboveThresholdTh8 = towersAboveThreshold.clone(minimumE=cms.double(8.0))
towersAboveThresholdTh7p3 = towersAboveThreshold.clone(minimumE=cms.double(7.3))
towersAboveThresholdTh7p6 = towersAboveThreshold.clone(minimumE=cms.double(7.6))

# select HF+ towers above threshold
hfPosTowers = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("towersAboveThreshold"),
    ptMin   = cms.double(0),
    etaMin = cms.double(3.0),
    etaMax = cms.double(6.0)
)

# select HF- towers above threshold
hfNegTowers = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("towersAboveThreshold"),
    ptMin   = cms.double(0),
    etaMin = cms.double(-6.0),
    etaMax = cms.double(-3.0)
)

# select HF+/HF- towers above threshold 4 and 5
hfPosTowersTh2 = hfPosTowers.clone(src=cms.InputTag("towersAboveThresholdTh2"))
hfNegTowersTh2 = hfNegTowers.clone(src=cms.InputTag("towersAboveThresholdTh2"))
hfPosTowersTh4 = hfPosTowers.clone(src=cms.InputTag("towersAboveThresholdTh4"))
hfNegTowersTh4 = hfNegTowers.clone(src=cms.InputTag("towersAboveThresholdTh4"))
hfPosTowersTh5 = hfPosTowers.clone(src=cms.InputTag("towersAboveThresholdTh5"))
hfNegTowersTh5 = hfNegTowers.clone(src=cms.InputTag("towersAboveThresholdTh5"))
hfPosTowersTh6 = hfPosTowers.clone(src=cms.InputTag("towersAboveThresholdTh6"))
hfNegTowersTh6 = hfNegTowers.clone(src=cms.InputTag("towersAboveThresholdTh6"))
hfPosTowersTh7 = hfPosTowers.clone(src=cms.InputTag("towersAboveThresholdTh7"))
hfNegTowersTh7 = hfNegTowers.clone(src=cms.InputTag("towersAboveThresholdTh7"))
hfPosTowersTh8 = hfPosTowers.clone(src=cms.InputTag("towersAboveThresholdTh8"))
hfNegTowersTh8 = hfNegTowers.clone(src=cms.InputTag("towersAboveThresholdTh8"))
hfPosTowersTh7p3 = hfPosTowers.clone(src=cms.InputTag("towersAboveThresholdTh7p3"))
hfNegTowersTh7p6 = hfNegTowers.clone(src=cms.InputTag("towersAboveThresholdTh7p6"))

# require at least one HF+ tower above threshold
hfPosFilter = cms.EDFilter("CandCountFilter",
    src = cms.InputTag("hfPosTowers"),
    minNumber = cms.uint32(1)
)

# require at least one HF- tower above threshold
hfNegFilter = cms.EDFilter("CandCountFilter",
    src = cms.InputTag("hfNegTowers"),
    minNumber = cms.uint32(1)
)

# require at least one HF+/HF- tower above threshold 4
hfPosFilterTh2 =hfPosFilter.clone(src="hfPosTowersTh2")
hfNegFilterTh2 =hfNegFilter.clone(src="hfNegTowersTh2")
hfPosFilterTh4 =hfPosFilter.clone(src="hfPosTowersTh4")
hfNegFilterTh4 =hfNegFilter.clone(src="hfNegTowersTh4")
hfPosFilterTh5 =hfPosFilter.clone(src="hfPosTowersTh5")
hfNegFilterTh5 =hfNegFilter.clone(src="hfNegTowersTh5")
hfPosFilterTh6 =hfPosFilter.clone(src="hfPosTowersTh6")
hfNegFilterTh6 =hfNegFilter.clone(src="hfNegTowersTh6")
hfPosFilterTh7 =hfPosFilter.clone(src="hfPosTowersTh7")
hfNegFilterTh7 =hfNegFilter.clone(src="hfNegTowersTh7")
hfPosFilterTh8 =hfPosFilter.clone(src="hfPosTowersTh8")
hfNegFilterTh8 =hfNegFilter.clone(src="hfNegTowersTh8")
hfPosFilterTh7p3 =hfPosFilter.clone(src="hfPosTowersTh7p3")
hfNegFilterTh7p6 =hfNegFilter.clone(src="hfNegTowersTh7p6")

# one HF tower above threshold on each side
hfCoincFilterTh3 = cms.Sequence(
    towersAboveThreshold *
    hfPosTowers *
    hfNegTowers *
    hfPosFilter *
    hfNegFilter)

hfCoincFilterTh2 = cms.Sequence(
    towersAboveThresholdTh2 *
    hfPosTowersTh2 *
    hfNegTowersTh2 *
    hfPosFilterTh2 *
    hfNegFilterTh2)

hfCoincFilterTh4 = cms.Sequence(
    towersAboveThresholdTh4 *
    hfPosTowersTh4 *
    hfNegTowersTh4 *
    hfPosFilterTh4 *
    hfNegFilterTh4)

hfCoincFilterTh5 = cms.Sequence(
    towersAboveThresholdTh5 *
    hfPosTowersTh5 *
    hfNegTowersTh5 *
    hfPosFilterTh5 *
    hfNegFilterTh5)

hfPosFilterTh3_seq = cms.Sequence(
    towersAboveThreshold *
    hfPosTowers *
    hfPosFilter)

hfNegFilterTh3_seq = cms.Sequence(
    towersAboveThreshold *
    hfNegTowers *
    hfNegFilter)

hfPosFilterTh4_seq = cms.Sequence(
    towersAboveThresholdTh4 *
    hfPosTowersTh4 *
    hfPosFilterTh4) 

hfNegFilterTh4_seq = cms.Sequence(
    towersAboveThresholdTh4 *
    hfNegTowersTh4 *
    hfNegFilterTh4)

hfPosFilterTh5_seq = cms.Sequence(
    towersAboveThresholdTh5 *
    hfPosTowersTh5 *
    hfPosFilterTh5)

hfNegFilterTh5_seq = cms.Sequence(
    towersAboveThresholdTh5 *
    hfNegTowersTh5 *
    hfNegFilterTh5)

hfPosFilterTh6_seq = cms.Sequence(
    towersAboveThresholdTh6 *
    hfPosTowersTh6 *
    hfPosFilterTh6)

hfNegFilterTh6_seq = cms.Sequence(
    towersAboveThresholdTh6 *
    hfNegTowersTh6 *
    hfNegFilterTh6)

hfPosFilterTh7_seq = cms.Sequence(
    towersAboveThresholdTh7 *
    hfPosTowersTh7 *
    hfPosFilterTh7)

hfNegFilterTh7_seq = cms.Sequence(
    towersAboveThresholdTh7 *
    hfNegTowersTh7 *
    hfNegFilterTh7)

hfPosFilterTh8_seq = cms.Sequence(
    towersAboveThresholdTh8 *
    hfPosTowersTh8 *
    hfPosFilterTh8)

hfNegFilterTh8_seq = cms.Sequence(
    towersAboveThresholdTh8 *
    hfNegTowersTh8 *
    hfNegFilterTh8)

hfPosFilterTh7p3_seq = cms.Sequence(
    towersAboveThresholdTh7p3 *
    hfPosTowersTh7p3 *
    hfPosFilterTh7p3)

hfNegFilterTh7p6_seq = cms.Sequence(
    towersAboveThresholdTh7p6 *
    hfNegTowersTh7p6 *
    hfNegFilterTh7p6)

# two HF towers above threshold on each side
hfPosFilter2 = hfPosFilter.clone(minNumber=cms.uint32(2))
hfNegFilter2 = hfNegFilter.clone(minNumber=cms.uint32(2))
hfPosFilter2Th2 = hfPosFilterTh2.clone(minNumber=cms.uint32(2))
hfNegFilter2Th2 = hfNegFilterTh2.clone(minNumber=cms.uint32(2))
hfPosFilter2Th4 = hfPosFilterTh4.clone(minNumber=cms.uint32(2))
hfNegFilter2Th4 = hfNegFilterTh4.clone(minNumber=cms.uint32(2))
hfPosFilter2Th5 = hfPosFilterTh5.clone(minNumber=cms.uint32(2))
hfNegFilter2Th5 = hfNegFilterTh5.clone(minNumber=cms.uint32(2))

hfCoincFilter2Th3 = cms.Sequence(
    towersAboveThreshold *
    hfPosTowers *
    hfNegTowers *
    hfPosFilter2 *
    hfNegFilter2)

hfCoincFilter2Th2 = cms.Sequence(
    towersAboveThresholdTh2 *
    hfPosTowersTh2 *
    hfNegTowersTh2 *
    hfPosFilter2Th2 *
    hfNegFilter2Th2)

hfCoincFilter2Th4 = cms.Sequence(
    towersAboveThresholdTh4 *
    hfPosTowersTh4 *
    hfNegTowersTh4 *
    hfPosFilter2Th4 *
    hfNegFilter2Th4)

hfCoincFilter2Th5 = cms.Sequence(
    towersAboveThresholdTh5 *
    hfPosTowersTh5 *
    hfNegTowersTh5 *
    hfPosFilter2Th5 *
    hfNegFilter2Th5)

#three HF towers above threshold on each side
hfPosFilter3 = hfPosFilter.clone(minNumber=cms.uint32(3))
hfNegFilter3 = hfNegFilter.clone(minNumber=cms.uint32(3))
hfPosFilter3Th2 = hfPosFilterTh2.clone(minNumber=cms.uint32(3))
hfNegFilter3Th2 = hfNegFilterTh2.clone(minNumber=cms.uint32(3))
hfPosFilter3Th4 = hfPosFilterTh4.clone(minNumber=cms.uint32(3))
hfNegFilter3Th4 = hfNegFilterTh4.clone(minNumber=cms.uint32(3))
hfPosFilter3Th5 = hfPosFilterTh5.clone(minNumber=cms.uint32(3))
hfNegFilter3Th5 = hfNegFilterTh5.clone(minNumber=cms.uint32(3))

hfCoincFilter3Th3 = cms.Sequence(
    towersAboveThreshold *
    hfPosTowers *
    hfNegTowers *
    hfPosFilter3 *
    hfNegFilter3)

hfCoincFilter3Th2 = cms.Sequence(
    towersAboveThresholdTh2 *
    hfPosTowersTh2 *
    hfNegTowersTh2 *
    hfPosFilter3Th2 *
    hfNegFilter3Th2)

hfCoincFilter3Th4 = cms.Sequence(
    towersAboveThresholdTh4 *
    hfPosTowersTh4 *
    hfNegTowersTh4 *
    hfPosFilter3Th4 *
    hfNegFilter3Th4)

hfCoincFilter3Th5 = cms.Sequence(
    towersAboveThresholdTh5 *
    hfPosTowersTh5 *
    hfNegTowersTh5 *
    hfPosFilter3Th5 *
    hfNegFilter3Th5)

#four HF towers above threshold on each side
hfPosFilter4 = hfPosFilter.clone(minNumber=cms.uint32(4))
hfNegFilter4 = hfNegFilter.clone(minNumber=cms.uint32(4))
hfPosFilter4Th2 = hfPosFilterTh2.clone(minNumber=cms.uint32(4))
hfNegFilter4Th2 = hfNegFilterTh2.clone(minNumber=cms.uint32(4))
hfPosFilter4Th4 = hfPosFilterTh4.clone(minNumber=cms.uint32(4))
hfNegFilter4Th4 = hfNegFilterTh4.clone(minNumber=cms.uint32(4))
hfPosFilter4Th5 = hfPosFilterTh5.clone(minNumber=cms.uint32(4))
hfNegFilter4Th5 = hfNegFilterTh5.clone(minNumber=cms.uint32(4))

hfCoincFilter4Th3 = cms.Sequence(
    towersAboveThreshold *
    hfPosTowers *
    hfNegTowers *
    hfPosFilter4 *
    hfNegFilter4)

hfCoincFilter4Th2 = cms.Sequence(
    towersAboveThresholdTh2 *
    hfPosTowersTh2 *
    hfNegTowersTh2 *
    hfPosFilter4Th2 *
    hfNegFilter4Th2)

hfCoincFilter4Th4 = cms.Sequence(
    towersAboveThresholdTh4 *
    hfPosTowersTh4 *
    hfNegTowersTh4 *
    hfPosFilter4Th4 *
    hfNegFilter4Th4)

hfCoincFilter4Th5 = cms.Sequence(
    towersAboveThresholdTh5 *
    hfPosTowersTh5 *
    hfNegTowersTh5 *
    hfPosFilter4Th5 *
    hfNegFilter4Th5)

#five hf towers above threshold on each side
hfPosFilter5 = hfPosFilter.clone(minNumber=cms.uint32(5))
hfNegFilter5 = hfNegFilter.clone(minNumber=cms.uint32(5))
hfPosFilter5Th2 = hfPosFilterTh2.clone(minNumber=cms.uint32(5))
hfNegFilter5Th2 = hfNegFilterTh2.clone(minNumber=cms.uint32(5))
hfPosFilter5Th4 = hfPosFilterTh4.clone(minNumber=cms.uint32(5))
hfNegFilter5Th4 = hfNegFilterTh4.clone(minNumber=cms.uint32(5))
hfPosFilter5Th5 = hfPosFilterTh5.clone(minNumber=cms.uint32(5))
hfNegFilter5Th5 = hfNegFilterTh5.clone(minNumber=cms.uint32(5))

hfCoincFilter5Th3 = cms.Sequence(
    towersAboveThreshold *
    hfPosTowers *
    hfNegTowers *
    hfPosFilter5 *
    hfNegFilter5)

hfCoincFilter5Th2 = cms.Sequence(
    towersAboveThresholdTh2 *
    hfPosTowersTh2 *
    hfNegTowersTh2 *
    hfPosFilter5Th2 *
    hfNegFilter5Th2)

hfCoincFilter5Th4 = cms.Sequence(
    towersAboveThresholdTh4 *
    hfPosTowersTh4 *
    hfNegTowersTh4 *
    hfPosFilter5Th4 *
    hfNegFilter5Th4)

hfCoincFilter5Th5 = cms.Sequence(
    towersAboveThresholdTh5 *
    hfPosTowersTh5 *
    hfNegTowersTh5 *
    hfPosFilter5Th5 *
    hfNegFilter5Th5)
