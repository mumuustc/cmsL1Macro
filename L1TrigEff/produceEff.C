#include "../common/headers.h"
#include "../common/function.C"
#include "../common/constant.h"

//// if one just want to include EvtTowerInfoNTuple.h, one needs to change
//// "#ifdef EvtTowerInfoNTuple_cxx" to 
//// "#ifndef EvtTowerInfoNTuple_cxx"
//// "#define EvtTowerInfoNTuple_cxx"
//// And delete Loop() function
// #include "EvtTowerInfoNTuple.h"

#include "../common/EvtTowerInfoNTuple.C"

// ***** define histograms ******
// UPC trigger study
TH1D *hnEvts_0nXn;
TH1D *hHFAdcInXn_0nXn;
TH2D *hNtrkOfflineVsNtrkHP_0nXn;
TH2D *hHFAdcInXnVsNtrkHP_0nXn;
TH2D *hTotEndcapEtVsNtrkHP_0nXn;
TH2D *hTotEndcapEmEtVsNtrkHP_0nXn;
TH2D *hTotEndcapHadEtVsNtrkHP_0nXn;
TH2D *hMaxHadEtVsNtrkHP_0nXn;
TH2D *hMaxBarrelHadEtVsNtrkHP_0nXn;
TH2D *hMaxEndcapHadEtVsNtrkHP_0nXn;
TH2D *hTotEndcapHadEtInXnVsNtrkHP_0nXn;
TH2D *hMaxEndcapHadEtInXnVsNtrkHP_0nXn;
TH2D *hTotEndcapHadEtInXnVsHFAdcInXn_0nXn;
TH2D *hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn;
TH1D *hTotEt_0nXn;
TH1D *hTotEmEt_0nXn;
TH1D *hTotHadEt_0nXn;
TH1D *hMaxEt_0nXn;
TH1D *hMaxEmEt_0nXn;
TH1D *hMaxHadEt_0nXn;
TH1D *hTotBarrelEt_0nXn;
TH1D *hTotBarrelEmEt_0nXn;
TH1D *hTotBarrelHadEt_0nXn;
TH1D *hMaxBarrelEt_0nXn;
TH1D *hMaxBarrelEmEt_0nXn;
TH1D *hMaxBarrelHadEt_0nXn;
TH1D *hTotEndcapEt_0nXn;
TH1D *hTotEndcapEmEt_0nXn;
TH1D *hTotEndcapHadEt_0nXn;
TH1D *hMaxEndcapEt_0nXn;
TH1D *hMaxEndcapEmEt_0nXn;
TH1D *hMaxEndcapHadEt_0nXn;
TH1D *hTotEndcapEtInXn_0nXn;
TH1D *hTotEndcapEmEtInXn_0nXn;
TH1D *hTotEndcapHadEtInXn_0nXn;
TH1D *hMaxEndcapEtInXn_0nXn;
TH1D *hMaxEndcapEmEtInXn_0nXn;
TH1D *hMaxEndcapHadEtInXn_0nXn;

// MB trigger study
TH1D *hnEvts;
TH1D *hnEvts_checkZDCTh;
TH1D *hCen_HFAnd[nHiHFAdcThs];
TH1D *hCen_HFAnd_AND_ZDCAnd[nLowHFAdcThs];
TH1D *hCen_HFAnd_OR_ZDCAnd[nNeuThs];

TH1D *hCen_MB;
TH1D *hCen_MB_HFAnd[nHiHFAdcThs];
TH1D *hCen_MB_HFAnd_AND_ZDCAnd[nLowHFAdcThs];
TH1D *hCen_MB_HFAnd_AND_ZDCAnd_LowNeuTh[nLowHFAdcThs];
TH1D *hCen_MB_HFAnd_OR_ZDCAnd[nNeuThs];

TH3D *hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen;
TH2D *hMaxL1HFAdcPlusVsCen;
TH2D *hMaxL1HFAdcMinusVsCen;

TH2D *hZDCMinusVsZDCPlus_Peripheral;

void bookHistogram();
void writeHistogram(TString outputFileName);

int produceEff(TString caloTreeList="test.list", TString outputFileName="L1Eff.test.histo")
{
    TH1::SetDefaultSumw2(kTRUE);

    TChain *caloTreeChain = new TChain("EvtTowerInfoNTuple");

    Int_t ifile=0;
    char filename[512];
    ifstream *inputStream = new ifstream;
    inputStream->open(caloTreeList.Data());
    if (!(inputStream)) {
        printf("can not open list file\n");
        return 0;
    }
    for(;inputStream->good();){
        inputStream->getline(filename,512);
        if(inputStream->good()) {
            TFile *ftmp = new TFile(filename);
            if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())) {
                cout<<"something wrong"<<endl;
            } else {
                cout<<"read in "<<ifile<<"th file: "<<filename<<endl;
                caloTreeChain->Add(filename);
                ifile++;
            }
            delete ftmp;
        }
    }
    delete inputStream;

    bookHistogram();

    Long64_t nEvts = caloTreeChain->GetEntries();
    cout<<"Total Entries: "<<nEvts<<endl;

    EvtTowerInfoNTuple *event = new EvtTowerInfoNTuple(caloTreeChain);
    // event->Loop(); if you want to do so, write all the analysis code in the Loop function

    Bool_t HFAnd_AdcTh19 = 0;
    Bool_t HFAnd_HiTh[nHiHFAdcThs];
    Bool_t HFAnd_LowTh[nLowHFAdcThs];
    Bool_t ZDCAnd[nNeuThs];
    for(Long64_t ievt=0; ievt<nEvts; ievt++){
        if(nEvts<100){
            cout << "begin " << ievt << "th entry...." << endl;
        }
        else if(ievt % (nEvts / 10) == 0){
            cout << "begin " << ievt << "th entry...." << endl;
        }

        //evtSel[4-15]
        //[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
        //[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
        //[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
        //[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
        //[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
        //[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
        //[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
        //[16] is for Plus (Th = 7.3 GeV) & [17] is for Minus (Th = 7.6 GeV); 

        event->GetEntry(ievt);
        Short_t mCenBin      = event->mCenBin;
        Float_t mZDCPlus     = event->mZDCPlus;
        Float_t mZDCMinus    = event->mZDCMinus;
        Int_t   mNtrkoffline = event->mNtrkoffline;
        Int_t   mNtrkHP      = event->mNtrkHP;
        Bool_t  ZB_HLTBit    = event->mTrigHLT[9];
        Bool_t  colEvtSel                  = event->mEvtSel[0];
        Bool_t  hfCoincFilter2Th4          = event->mEvtSel[1];
        Bool_t  primaryVertexFilter        = event->mEvtSel[2];
        Bool_t  clusterCompatibilityFilter = event->mEvtSel[3];
        Bool_t  hfPosFilterTh5             = event->mEvtSel[8];
        Bool_t  hfNegFilterTh5             = event->mEvtSel[9];
        Bool_t  hfPosFilterTh7             = event->mEvtSel[12];
        Bool_t  hfNegFilterTh7             = event->mEvtSel[13];

        Int_t   mMaxL1HFAdcPlus  = event->mMaxL1HFAdcPlus;
        Int_t   mMaxL1HFAdcMinus = event->mMaxL1HFAdcMinus;

        if(!ZB_HLTBit) cout<<"What?! This sample has non-ZB data !!!"<<endl;

        Double_t maxEmuEt = 0, maxEmuEmEt = 0, maxEmuHadEt = 0;
        Double_t totEmuEt = 0, totEmuEmEt = 0, totEmuHadEt = 0;

        Double_t maxBarrelEmuEt = 0, maxBarrelEmuEmEt = 0, maxBarrelEmuHadEt = 0;
        Double_t totBarrelEmuEt = 0, totBarrelEmuEmEt = 0, totBarrelEmuHadEt = 0;

        Double_t maxEndcapEmuEt = 0, maxEndcapEmuEmEt = 0, maxEndcapEmuHadEt = 0;
        Double_t totEndcapEmuEt = 0, totEndcapEmuEmEt = 0, totEndcapEmuHadEt = 0;

        Double_t maxEndcapEmuEtPlus = 0, maxEndcapEmuEmEtPlus = 0, maxEndcapEmuHadEtPlus = 0;
        Double_t totEndcapEmuEtPlus = 0, totEndcapEmuEmEtPlus = 0, totEndcapEmuHadEtPlus = 0;

        Double_t maxEndcapEmuEtMinus = 0, maxEndcapEmuEmEtMinus = 0, maxEndcapEmuHadEtMinus = 0;
        Double_t totEndcapEmuEtMinus = 0, totEndcapEmuEmEtMinus = 0, totEndcapEmuHadEtMinus = 0;

        Int_t  nTowersEmu   = event->nTowersEmu;
        for(Int_t idx=0; idx<nTowersEmu; idx++){
            Short_t ietaEmu = event->ietaEmu[idx]; // Central Calorimeter [-28, 28]
            Short_t iphiEmu = event->iphiEmu[idx]; // [1, 72]
            Short_t ietEmu  = event->ietEmu[idx];
            Short_t iemEmu  = event->iemEmu[idx];
            Short_t ihadEmu = event->ihadEmu[idx];

            if(iphiEmu<0){
                cout<<"Should not generate negative iphi in emulation !!!"<<endl;
                cout<<event->mRunNb<<"   "<<event->mEventNb<<"   "<<event->mLSNb<<endl;
                cout<<endl;
            }

            if(ietaEmu<-28 || ietaEmu>28) continue; // skip HF information

            if(ietEmu>maxEmuEt)     maxEmuEt = ietEmu;
            if(iemEmu>maxEmuEmEt)   maxEmuEmEt = iemEmu;
            if(ihadEmu>maxEmuHadEt) maxEmuHadEt = ihadEmu;

            totEmuEt += ietEmu;
            totEmuEmEt += iemEmu;
            totEmuHadEt += ihadEmu;

            if(fabs(ietaEmu)<17){
                if(ietEmu>maxBarrelEmuEt)     maxBarrelEmuEt = ietEmu;
                if(iemEmu>maxBarrelEmuEmEt)   maxBarrelEmuEmEt = iemEmu;
                if(ihadEmu>maxBarrelEmuHadEt) maxBarrelEmuHadEt = ihadEmu;

                totBarrelEmuEt += ietEmu;
                totBarrelEmuEmEt += iemEmu;
                totBarrelEmuHadEt += ihadEmu;
            }
            else{
                if(ietEmu>maxEndcapEmuEt)     maxEndcapEmuEt = ietEmu;
                if(iemEmu>maxEndcapEmuEmEt)   maxEndcapEmuEmEt = iemEmu;
                if(ihadEmu>maxEndcapEmuHadEt) maxEndcapEmuHadEt = ihadEmu;

                totEndcapEmuEt += ietEmu;
                totEndcapEmuEmEt += iemEmu;
                totEndcapEmuHadEt += ihadEmu;

                if(ietaEmu>0){
                    if(ietEmu>maxEndcapEmuEtPlus)     maxEndcapEmuEtPlus = ietEmu;
                    if(iemEmu>maxEndcapEmuEmEtPlus)   maxEndcapEmuEmEtPlus = iemEmu;
                    if(ihadEmu>maxEndcapEmuHadEtPlus) maxEndcapEmuHadEtPlus = ihadEmu;

                    totEndcapEmuEtPlus += ietEmu;
                    totEndcapEmuEmEtPlus += iemEmu;
                    totEndcapEmuHadEtPlus += ihadEmu;
                }
                else{
                    if(ietEmu>maxEndcapEmuEtMinus)     maxEndcapEmuEtMinus = ietEmu;
                    if(iemEmu>maxEndcapEmuEmEtMinus)   maxEndcapEmuEmEtMinus = iemEmu;
                    if(ihadEmu>maxEndcapEmuHadEtMinus) maxEndcapEmuHadEtMinus = ihadEmu;

                    totEndcapEmuEtMinus += ietEmu;
                    totEndcapEmuEmEtMinus += iemEmu;
                    totEndcapEmuHadEtMinus += ihadEmu;
                }
            }
        }

        hnEvts_0nXn->Fill(0.5);

        Bool_t fireZDCPlus  = mZDCPlus > mNeuZDCHi[0][0];
        Bool_t fireZDCMinus = mZDCMinus > mNeuZDCHi[1][0];

        Bool_t evt0nXn = kFALSE;
        if( fireZDCPlus && !fireZDCMinus) evt0nXn = kTRUE;
        if( !fireZDCPlus && fireZDCMinus) evt0nXn = kTRUE;
        if(evt0nXn) hnEvts_0nXn->Fill(1.5);

        Bool_t evt0nXnAndHFVeto = kFALSE;
        if( fireZDCPlus && !fireZDCMinus && mMaxL1HFAdcMinus<15){
            evt0nXnAndHFVeto = kTRUE;

            hHFAdcInXn_0nXn->Fill(mMaxL1HFAdcPlus);
            hHFAdcInXnVsNtrkHP_0nXn->Fill(mNtrkHP, mMaxL1HFAdcPlus);
            hTotEndcapHadEtInXnVsNtrkHP_0nXn->Fill(mNtrkHP, totEndcapEmuHadEtPlus);
            hMaxEndcapHadEtInXnVsNtrkHP_0nXn->Fill(mNtrkHP, maxEndcapEmuHadEtPlus);

            hTotEndcapHadEtInXnVsHFAdcInXn_0nXn->Fill(mMaxL1HFAdcPlus, totEndcapEmuHadEtPlus);
            hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn->Fill(mMaxL1HFAdcPlus, maxEndcapEmuHadEtPlus);

            hTotEndcapEtInXn_0nXn->Fill(totEndcapEmuEtPlus);
            hTotEndcapEmEtInXn_0nXn->Fill(totEndcapEmuEmEtPlus);
            hTotEndcapHadEtInXn_0nXn->Fill(totEndcapEmuHadEtPlus);
            hMaxEndcapEtInXn_0nXn->Fill(maxEndcapEmuEtPlus);
            hMaxEndcapEmEtInXn_0nXn->Fill(maxEndcapEmuEmEtPlus);
            hMaxEndcapHadEtInXn_0nXn->Fill(maxEndcapEmuHadEtPlus);
        }
        if( !fireZDCPlus && fireZDCMinus && mMaxL1HFAdcPlus<15){
            evt0nXnAndHFVeto = kTRUE;

            hHFAdcInXn_0nXn->Fill(mMaxL1HFAdcMinus);
            hHFAdcInXnVsNtrkHP_0nXn->Fill(mNtrkHP, mMaxL1HFAdcMinus);
            hTotEndcapHadEtInXnVsNtrkHP_0nXn->Fill(mNtrkHP, totEndcapEmuHadEtMinus);
            hMaxEndcapHadEtInXnVsNtrkHP_0nXn->Fill(mNtrkHP, maxEndcapEmuHadEtMinus);

            hTotEndcapHadEtInXnVsHFAdcInXn_0nXn->Fill(mMaxL1HFAdcMinus, totEndcapEmuHadEtMinus);
            hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn->Fill(mMaxL1HFAdcMinus, maxEndcapEmuHadEtMinus);

            hTotEndcapEtInXn_0nXn->Fill(totEndcapEmuEtMinus);
            hTotEndcapEmEtInXn_0nXn->Fill(totEndcapEmuEmEtMinus);
            hTotEndcapHadEtInXn_0nXn->Fill(totEndcapEmuHadEtMinus);
            hMaxEndcapEtInXn_0nXn->Fill(maxEndcapEmuEtMinus);
            hMaxEndcapEmEtInXn_0nXn->Fill(maxEndcapEmuEmEtMinus);
            hMaxEndcapHadEtInXn_0nXn->Fill(maxEndcapEmuHadEtMinus);
        }

        if(evt0nXnAndHFVeto){
            hnEvts_0nXn->Fill(2.5);
            hNtrkOfflineVsNtrkHP_0nXn->Fill(mNtrkHP, mNtrkoffline);
            hTotEndcapEtVsNtrkHP_0nXn->Fill(mNtrkHP, totEndcapEmuEt);
            hTotEndcapEmEtVsNtrkHP_0nXn->Fill(mNtrkHP, totEndcapEmuEmEt);
            hTotEndcapHadEtVsNtrkHP_0nXn->Fill(mNtrkHP, totEndcapEmuHadEt);
            hMaxHadEtVsNtrkHP_0nXn->Fill(mNtrkHP, maxEmuHadEt);
            hMaxBarrelHadEtVsNtrkHP_0nXn->Fill(mNtrkHP, maxBarrelEmuHadEt);
            hMaxEndcapHadEtVsNtrkHP_0nXn->Fill(mNtrkHP, maxEndcapEmuHadEt);

            hTotEt_0nXn->Fill(totEmuEt);
            hTotEmEt_0nXn->Fill(totEmuEmEt);
            hTotHadEt_0nXn->Fill(totEmuHadEt);
            hMaxEt_0nXn->Fill(maxEmuEt);
            hMaxEmEt_0nXn->Fill(maxEmuEmEt);
            hMaxHadEt_0nXn->Fill(maxEmuHadEt);

            hTotBarrelEt_0nXn->Fill(totBarrelEmuEt);
            hTotBarrelEmEt_0nXn->Fill(totBarrelEmuEmEt);
            hTotBarrelHadEt_0nXn->Fill(totBarrelEmuHadEt);
            hMaxBarrelEt_0nXn->Fill(maxBarrelEmuEt);
            hMaxBarrelEmEt_0nXn->Fill(maxBarrelEmuEmEt);
            hMaxBarrelHadEt_0nXn->Fill(maxBarrelEmuHadEt);

            hTotEndcapEt_0nXn->Fill(totEndcapEmuEt);
            hTotEndcapEmEt_0nXn->Fill(totEndcapEmuEmEt);
            hTotEndcapHadEt_0nXn->Fill(totEndcapEmuHadEt);
            hMaxEndcapEt_0nXn->Fill(maxEndcapEmuEt);
            hMaxEndcapEmEt_0nXn->Fill(maxEndcapEmuEmEt);
            hMaxEndcapHadEt_0nXn->Fill(maxEndcapEmuHadEt);
        }

        hnEvts->Fill(0.5);
        hnEvts_checkZDCTh->Fill(0.5);

        memset(ZDCAnd, 0, sizeof(ZDCAnd));
        for(Int_t idx=0; idx<nNeuThs; idx++){
            ZDCAnd[idx] = (mZDCPlus>mNeuZDCHi[0][idx]) && (mZDCMinus>mNeuZDCHi[1][idx]);
        }

        HFAnd_AdcTh19 = (mMaxL1HFAdcPlus>19) && (mMaxL1HFAdcMinus>19);

        memset(HFAnd_HiTh, 0, sizeof(HFAnd_HiTh));
        for(Int_t idx=0; idx<nHiHFAdcThs; idx++){
            HFAnd_HiTh[idx] = (mMaxL1HFAdcPlus>mHiHFAdcTh[idx]) && (mMaxL1HFAdcMinus>mHiHFAdcTh[idx]);
            if(HFAnd_HiTh[idx]){
                hnEvts->Fill(1.5+idx);
                hCen_HFAnd[idx]->Fill(mCenBin);
            }
        }

        memset(HFAnd_LowTh, 0, sizeof(HFAnd_LowTh));
        for(Int_t idx=0; idx<nLowHFAdcThs; idx++){
            HFAnd_LowTh[idx] = (mMaxL1HFAdcPlus>mLowHFAdcTh[idx]) && (mMaxL1HFAdcMinus>mLowHFAdcTh[idx]);
            if(HFAnd_LowTh[idx] && ZDCAnd[1]){
                hnEvts->Fill(1.5+nHiHFAdcThs+idx);
                hCen_HFAnd_AND_ZDCAnd[idx]->Fill(mCenBin);
            }
        }

        for(Int_t idx=0; idx<nNeuThs; idx++){
            if(HFAnd_AdcTh19 || ZDCAnd[idx]){
                hnEvts->Fill(1.5+nHiHFAdcThs+nLowHFAdcThs+idx);
                hCen_HFAnd_OR_ZDCAnd[idx]->Fill(mCenBin);
            }
        }

        for(Int_t iHF=0; iHF<nLowHFAdcThs; iHF++){
            if(HFAnd_LowTh[iHF]) hnEvts_checkZDCTh->Fill(1.5+iHF*(nNeuThs+1));
            for(Int_t iZDC=0; iZDC<nNeuThs; iZDC++){
                if(HFAnd_LowTh[iHF] && ZDCAnd[iZDC]){
                    hnEvts_checkZDCTh->Fill(1.5+iHF*(nNeuThs+1)+iZDC+1);
                }
            }
        }

        if(colEvtSel){
            hnEvts->Fill(1.5+nHiHFAdcThs+nLowHFAdcThs+nNeuThs);
            hnEvts_checkZDCTh->Fill(1.5+nLowHFAdcThs*(nNeuThs+1));

            hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen->Fill(mCenBin, mMaxL1HFAdcPlus, mMaxL1HFAdcMinus);
            hMaxL1HFAdcPlusVsCen->Fill(mCenBin, mMaxL1HFAdcPlus);
            hMaxL1HFAdcMinusVsCen->Fill(mCenBin, mMaxL1HFAdcMinus);

            if(mCenBin>170) hZDCMinusVsZDCPlus_Peripheral->Fill(mZDCPlus, mZDCMinus);

            hCen_MB->Fill(mCenBin);

            for(Int_t idx=0; idx<nHiHFAdcThs; idx++){
                if(HFAnd_HiTh[idx])  hCen_MB_HFAnd[idx]->Fill(mCenBin);
            }

            for(Int_t idx=0; idx<nLowHFAdcThs; idx++){
                if(HFAnd_LowTh[idx] && ZDCAnd[0]) hCen_MB_HFAnd_AND_ZDCAnd_LowNeuTh[idx]->Fill(mCenBin);
                if(HFAnd_LowTh[idx] && ZDCAnd[1]) hCen_MB_HFAnd_AND_ZDCAnd[idx]->Fill(mCenBin);
            }

            for(Int_t idx=0; idx<nNeuThs; idx++){
                if(HFAnd_AdcTh19 || ZDCAnd[idx]) hCen_MB_HFAnd_OR_ZDCAnd[idx]->Fill(mCenBin);
            }
        }
    }

    writeHistogram(outputFileName);

    return 0;
}

void bookHistogram()
{
    const Int_t nCenBins = 100;
    const Int_t mHFAdcBins = 150;
    const Int_t nEtBins = 500;

    // event level
    hnEvts = new TH1D("hnEvts", "hnEvts;", 25, 0, 25);
    hnEvts->GetXaxis()->SetBinLabel(1, "Zero-bias");
    for(Int_t idx=0; idx<nHiHFAdcThs; idx++)  hnEvts->GetXaxis()->SetBinLabel(2+idx, Form("HFAndAdcTh%d", mHiHFAdcTh[idx]));
    for(Int_t idx=0; idx<nLowHFAdcThs; idx++) hnEvts->GetXaxis()->SetBinLabel(2+nHiHFAdcThs+idx, Form("HFAndAdcTh%d & ZDCAnd2n", mLowHFAdcTh[idx]));
    for(Int_t idx=0; idx<nNeuThs; idx++)      hnEvts->GetXaxis()->SetBinLabel(2+nHiHFAdcThs+nLowHFAdcThs+idx, Form("HFAndAdcTh19 || ZDCAnd%dn", mNeuTh[idx]));
    hnEvts->GetXaxis()->SetBinLabel(2+nHiHFAdcThs+nLowHFAdcThs+nNeuThs, "Collisions");
    hnEvts->GetXaxis()->LabelsOption("d");
    hnEvts->GetXaxis()->SetLabelSize(0.055);

    hnEvts_checkZDCTh = new TH1D("hnEvts_checkZDCTh", "hnEvts_checkZDCTh;", 45, 0, 45);
    hnEvts_checkZDCTh->GetXaxis()->SetBinLabel(1, "Zero-bias");
    for(Int_t iHF=0; iHF<nLowHFAdcThs; iHF++){
        hnEvts_checkZDCTh->GetXaxis()->SetBinLabel(2+iHF*(nNeuThs+1), Form("AdcTh%d", mLowHFAdcTh[iHF]));
        for(Int_t iZDC=0; iZDC<nNeuThs; iZDC++){
            hnEvts_checkZDCTh->GetXaxis()->SetBinLabel(2+iHF*(nNeuThs+1)+iZDC+1, Form("AdcTh%d & %dn", mLowHFAdcTh[iHF], mNeuTh[iZDC]));
        }
    }
    hnEvts_checkZDCTh->GetXaxis()->SetBinLabel(2+nLowHFAdcThs*(nNeuThs+1), "Collisions");
    hnEvts_checkZDCTh->GetXaxis()->LabelsOption("v");
    hnEvts_checkZDCTh->GetXaxis()->SetLabelSize(0.045);

    hnEvts_0nXn = new TH1D("hnEvts_0nXn", "hnEvts_0nXn;", 10, 0, 10);
    hnEvts_0nXn->GetXaxis()->SetBinLabel(1, "Zero-bias");
    hnEvts_0nXn->GetXaxis()->SetBinLabel(2, "0nXn");
    hnEvts_0nXn->GetXaxis()->SetBinLabel(3, "0nXn & HFveto");
    hnEvts_0nXn->GetXaxis()->LabelsOption("d");
    hnEvts_0nXn->GetXaxis()->SetLabelSize(0.055);

    hNtrkOfflineVsNtrkHP_0nXn = new TH2D("hNtrkOfflineVsNtrkHP_0nXn", "hNtrkOfflineVsNtrkHP_0nXn; N_{trk}^{HP}; N_{trk}^{Offline}", 100, 0, 100, 100, 0, 100);
    hHFAdcInXnVsNtrkHP_0nXn = new TH2D("hHFAdcInXnVsNtrkHP_0nXn", "hHFAdcInXnVsNtrkHP_0nXn; N_{trk}^{HP}; HF ADC in Xn", 100, 0, 100, 100, 0, 100);
    hTotEndcapEtVsNtrkHP_0nXn = new TH2D("hTotEndcapEtVsNtrkHP_0nXn", "hTotEndcapEtVsNtrkHP_0nXn; N_{trk}^{HP}; total endcap E_{T}", 100, 0, 100, nEtBins, 0, nEtBins);
    hTotEndcapEmEtVsNtrkHP_0nXn = new TH2D("hTotEndcapEmEtVsNtrkHP_0nXn", "hTotEndcapEmEtVsNtrkHP_0nXn; N_{trk}^{HP}; total endcap ECal E_{T}", 100, 0, 100, nEtBins, 0, nEtBins);
    hTotEndcapHadEtVsNtrkHP_0nXn = new TH2D("hTotEndcapHadEtVsNtrkHP_0nXn", "hTotEndcapHadEtVsNtrkHP_0nXn; N_{trk}^{HP}; total endcap HCal E_{T}", 100, 0, 100, nEtBins, 0, nEtBins);
    hMaxHadEtVsNtrkHP_0nXn = new TH2D("hMaxHadEtVsNtrkHP_0nXn", "hMaxHadEtVsNtrkHP_0nXn; N_{trk}^{HP}; maximum HCal E_{T}", 100, 0, 100, nEtBins, 0, nEtBins);
    hMaxBarrelHadEtVsNtrkHP_0nXn = new TH2D("hMaxBarrelHadEtVsNtrkHP_0nXn", "hMaxBarrelHadEtVsNtrkHP_0nXn; N_{trk}^{HP}; maximum barrel HCal E_{T}", 100, 0, 100, nEtBins, 0, nEtBins);
    hMaxEndcapHadEtVsNtrkHP_0nXn = new TH2D("hMaxEndcapHadEtVsNtrkHP_0nXn", "hMaxEndcapHadEtVsNtrkHP_0nXn; N_{trk}^{HP}; maximum endcap HCal E_{T}", 100, 0, 100, nEtBins, 0, nEtBins);
    hTotEndcapHadEtInXnVsNtrkHP_0nXn = new TH2D("hTotEndcapHadEtInXnVsNtrkHP_0nXn", "hTotEndcapHadEtInXnVsNtrkHP_0nXn; N_{trk}^{HP}; total endcap HCal E_{T} in Xn", 100, 0, 100, nEtBins, 0, nEtBins);
    hMaxEndcapHadEtInXnVsNtrkHP_0nXn = new TH2D("hMaxEndcapHadEtInXnVsNtrkHP_0nXn", "hMaxEndcapHadEtInXnVsNtrkHP_0nXn; N_{trk}^{HP}; maximum endcap HCal E_{T} in Xn", 100, 0, 100, nEtBins, 0, nEtBins);
    hTotEndcapHadEtInXnVsHFAdcInXn_0nXn = new TH2D("hTotEndcapHadEtInXnVsHFAdcInXn_0nXn", "hTotEndcapHadEtInXnVsHFAdcInXn_0nXn; HF ADC in Xn; total endcap HCal E_{T} in Xn", 100, 0, 100, nEtBins, 0, nEtBins);
    hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn = new TH2D("hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn", "hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn; HF ADC in Xn; maximum endcap HCal E_{T} in Xn", 100, 0, 100, nEtBins, 0, nEtBins);
    hHFAdcInXn_0nXn = new TH1D("hHFAdcInXn_0nXn", "hHFAdcInXn_0nXn; HF ADC in Xn", 100, 0, 100);
    hTotEt_0nXn = new TH1D("hTotEt_0nXn", "hTotEt_0nXn; total E_{T}", nEtBins, 0, nEtBins);
    hTotEmEt_0nXn = new TH1D("hTotEmEt_0nXn", "hTotEmEt_0nXn; total ECal E_{T}", nEtBins, 0, nEtBins);
    hTotHadEt_0nXn = new TH1D("hTotHadEt_0nXn", "hTotHadEt_0nXn; total HCal E_{T}", nEtBins, 0, nEtBins);
    hMaxEt_0nXn = new TH1D("hMaxEt_0nXn", "hMaxEt_0nXn; maximum E_{T}", nEtBins, 0, nEtBins);
    hMaxEmEt_0nXn = new TH1D("hMaxEmEt_0nXn", "hMaxEmEt_0nXn; maximum ECal E_{T}", nEtBins, 0, nEtBins);
    hMaxHadEt_0nXn = new TH1D("hMaxHadEt_0nXn", "hMaxHadEt_0nXn; maximum HCal E_{T}", nEtBins, 0, nEtBins);
    hTotBarrelEt_0nXn = new TH1D("hTotBarrelEt_0nXn", "hTotBarrelEt_0nXn; total barrel E_{T}", nEtBins, 0, nEtBins);
    hTotBarrelEmEt_0nXn = new TH1D("hTotBarrelEmEt_0nXn", "hTotBarrelEmEt_0nXn; total barrel ECal E_{T}", nEtBins, 0, nEtBins);
    hTotBarrelHadEt_0nXn = new TH1D("hTotBarrelHadEt_0nXn", "hTotBarrelHadEt_0nXn; total barrel HCal E_{T}", nEtBins, 0, nEtBins);
    hMaxBarrelEt_0nXn = new TH1D("hMaxBarrelEt_0nXn", "hMaxBarrelEt_0nXn; maximum barrel E_{T}", nEtBins, 0, nEtBins);
    hMaxBarrelEmEt_0nXn = new TH1D("hMaxBarrelEmEt_0nXn", "hMaxBarrelEmEt_0nXn; maximum barrel ECal E_{T}", nEtBins, 0, nEtBins);
    hMaxBarrelHadEt_0nXn = new TH1D("hMaxBarrelHadEt_0nXn", "hMaxBarrelHadEt_0nXn; maximum barrel HCal E_{T}", nEtBins, 0, nEtBins);
    hTotEndcapEt_0nXn = new TH1D("hTotEndcapEt_0nXn", "hTotEndcapEt_0nXn; total endcap E_{T}", nEtBins, 0, nEtBins);
    hTotEndcapEmEt_0nXn = new TH1D("hTotEndcapEmEt_0nXn", "hTotEndcapEmEt_0nXn; total endcap ECal E_{T}", nEtBins, 0, nEtBins);
    hTotEndcapHadEt_0nXn = new TH1D("hTotEndcapHadEt_0nXn", "hTotEndcapHadEt_0nXn; total endcap HCal E_{T}", nEtBins, 0, nEtBins);
    hMaxEndcapEt_0nXn = new TH1D("hMaxEndcapEt_0nXn", "hMaxEndcapEt_0nXn; maximum endcap E_{T}", nEtBins, 0, nEtBins);
    hMaxEndcapEmEt_0nXn = new TH1D("hMaxEndcapEmEt_0nXn", "hMaxEndcapEmEt_0nXn; maximum endcap ECal E_{T}", nEtBins, 0, nEtBins);
    hMaxEndcapHadEt_0nXn = new TH1D("hMaxEndcapHadEt_0nXn", "hMaxEndcapHadEt_0nXn; maximum endcap HCal E_{T}", nEtBins, 0, nEtBins);
    hTotEndcapEtInXn_0nXn = new TH1D("hTotEndcapEtInXn_0nXn", "hTotEndcapEtInXn_0nXn; total endcap E_{T} in Xn", nEtBins, 0, nEtBins);
    hTotEndcapEmEtInXn_0nXn = new TH1D("hTotEndcapEmEtInXn_0nXn", "hTotEndcapEmEtInXn_0nXn; total endcap ECal E_{T} in Xn", nEtBins, 0, nEtBins);
    hTotEndcapHadEtInXn_0nXn = new TH1D("hTotEndcapHadEtInXn_0nXn", "hTotEndcapHadEtInXn_0nXn; total endcap HCal E_{T} in Xn", nEtBins, 0, nEtBins);
    hMaxEndcapEtInXn_0nXn = new TH1D("hMaxEndcapEtInXn_0nXn", "hMaxEndcapEtInXn_0nXn; maximum endcap E_{T} in Xn", nEtBins, 0, nEtBins);
    hMaxEndcapEmEtInXn_0nXn = new TH1D("hMaxEndcapEmEtInXn_0nXn", "hMaxEndcapEmEtInXn_0nXn; maximum endcap ECal E_{T} in Xn", nEtBins, 0, nEtBins);
    hMaxEndcapHadEtInXn_0nXn = new TH1D("hMaxEndcapHadEtInXn_0nXn", "hMaxEndcapHadEtInXn_0nXn; maximum endcap HCal E_{T} in Xn", nEtBins, 0, nEtBins);

    hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen  = new TH3D("hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen", "hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen; Centrality Bin; Maximum HF^{+} ADC; Maximum HF^{-} ADC", 20, 0, 200, mHFAdcBins, 0, mHFAdcBins, mHFAdcBins, 0, mHFAdcBins);
    hMaxL1HFAdcPlusVsCen  = new TH2D("hMaxL1HFAdcPlusVsCen", "hMaxL1HFAdcPlusVsCen; Centrality Bin; Maximum HF^{+} ADC", nCenBins, 0, 200, mHFAdcBins, 0, mHFAdcBins);
    hMaxL1HFAdcMinusVsCen = new TH2D("hMaxL1HFAdcMinusVsCen", "hMaxL1HFAdcMinusVsCen; Centrality Bin; Maximum HF^{-} ADC", nCenBins, 0, 200, mHFAdcBins, 0, mHFAdcBins);

    hZDCMinusVsZDCPlus_Peripheral  = new TH2D("hZDCMinusVsZDCPlus_Peripheral", "hZDCMinusVsZDCPlus_Peripheral; ZDC^{Plus} (a.u.); ZDC^{Minus} (a.u.)", 1000, 0, 5.e5, 1000, 0, 5.e5);

    hCen_MB = new TH1D("hCen_MB", "hCen_MB; Centrality Bin", nCenBins, 0, 200);

    for(Int_t idx=0; idx<nHiHFAdcThs; idx++){
        hCen_HFAnd[idx]  = new TH1D(Form("hCen_HFAnd_AdcTh%d", mHiHFAdcTh[idx]), "hCen_HFAnd; Centrality Bin", nCenBins, 0, 200);
        hCen_MB_HFAnd[idx]  = new TH1D(Form("hCen_MB_HFAnd_AdcTh%d", mHiHFAdcTh[idx]), "hCen_MB_HFAnd; Centrality Bin", nCenBins, 0, 200);
    }

    for(Int_t idx=0; idx<nLowHFAdcThs; idx++){
        hCen_HFAnd_AND_ZDCAnd[idx]  = new TH1D(Form("hCen_HFAnd_AND_ZDCAnd_AdcTh%d_2n", mLowHFAdcTh[idx]), "hCen_HFAnd_AND_ZDCAnd; Centrality Bin", nCenBins, 0, 200);
        hCen_MB_HFAnd_AND_ZDCAnd[idx]  = new TH1D(Form("hCen_MB_HFAnd_AND_ZDCAnd_AdcTh%d_2n", mLowHFAdcTh[idx]), "hCen_MB_HFAnd_AND_ZDCAnd; Centrality Bin", nCenBins, 0, 200);
        hCen_MB_HFAnd_AND_ZDCAnd_LowNeuTh[idx]  = new TH1D(Form("hCen_MB_HFAnd_AND_ZDCAnd_AdcTh%d_1n", mLowHFAdcTh[idx]), "hCen_MB_HFAnd_AND_ZDCAnd_LowNeuTh; Centrality Bin", nCenBins, 0, 200);
    }

    for(Int_t idx=0; idx<nNeuThs; idx++){
        hCen_HFAnd_OR_ZDCAnd[idx]  = new TH1D(Form("hCen_HFAnd_OR_ZDCAnd_AdcTh19_%dn", mNeuTh[idx]), "hCen_HFAnd_OR_ZDCAnd; Centrality Bin", nCenBins, 0, 200);
        hCen_MB_HFAnd_OR_ZDCAnd[idx]  = new TH1D(Form("hCen_MB_HFAnd_OR_ZDCAnd_AdcTh19_%dn", mNeuTh[idx]), "hCen_MB_HFAnd_OR_ZDCAnd; Centrality Bin", nCenBins, 0, 200);
    }
}

void writeHistogram(TString outputFileName)
{
    TFile *fOut = new TFile(Form("%s.root", outputFileName.Data()), "recreate");

    fOut->cd();

    hnEvts->Write();
    hnEvts_checkZDCTh->Write();

    hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen->Write();
    hMaxL1HFAdcPlusVsCen->Write();
    hMaxL1HFAdcMinusVsCen->Write();

    hZDCMinusVsZDCPlus_Peripheral->Write();

    hCen_MB->Write();

    for(Int_t idx=0; idx<nHiHFAdcThs; idx++){
        hCen_HFAnd[idx]->Write();
        hCen_MB_HFAnd[idx]->Write();
    }

    for(Int_t idx=0; idx<nLowHFAdcThs; idx++){
        hCen_HFAnd_AND_ZDCAnd[idx]->Write();
        hCen_MB_HFAnd_AND_ZDCAnd[idx]->Write();
        hCen_MB_HFAnd_AND_ZDCAnd_LowNeuTh[idx]->Write();
    }

    for(Int_t idx=0; idx<nNeuThs; idx++){
        hCen_HFAnd_OR_ZDCAnd[idx]->Write();
        hCen_MB_HFAnd_OR_ZDCAnd[idx]->Write();
    }

    hnEvts_0nXn->Write();
    hHFAdcInXn_0nXn->Write();

    hNtrkOfflineVsNtrkHP_0nXn->Write();
    hHFAdcInXnVsNtrkHP_0nXn->Write();
    hTotEndcapEtVsNtrkHP_0nXn->Write();
    hTotEndcapEmEtVsNtrkHP_0nXn->Write();
    hTotEndcapHadEtVsNtrkHP_0nXn->Write();
    hMaxHadEtVsNtrkHP_0nXn->Write();
    hMaxBarrelHadEtVsNtrkHP_0nXn->Write();
    hMaxEndcapHadEtVsNtrkHP_0nXn->Write();
    hTotEndcapHadEtInXnVsNtrkHP_0nXn->Write();
    hMaxEndcapHadEtInXnVsNtrkHP_0nXn->Write();
    hTotEndcapHadEtInXnVsHFAdcInXn_0nXn->Write();
    hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn->Write();

    hTotEt_0nXn->Write();
    hTotEmEt_0nXn->Write();
    hTotHadEt_0nXn->Write();
    hMaxEt_0nXn->Write();
    hMaxEmEt_0nXn->Write();
    hMaxHadEt_0nXn->Write();
    hTotBarrelEt_0nXn->Write();
    hTotBarrelEmEt_0nXn->Write();
    hTotBarrelHadEt_0nXn->Write();
    hMaxBarrelEt_0nXn->Write();
    hMaxBarrelEmEt_0nXn->Write();
    hMaxBarrelHadEt_0nXn->Write();
    hTotEndcapEt_0nXn->Write();
    hTotEndcapEmEt_0nXn->Write();
    hTotEndcapHadEt_0nXn->Write();
    hMaxEndcapEt_0nXn->Write();
    hMaxEndcapEmEt_0nXn->Write();
    hMaxEndcapHadEt_0nXn->Write();
    hTotEndcapEtInXn_0nXn->Write();
    hTotEndcapEmEtInXn_0nXn->Write();
    hTotEndcapHadEtInXn_0nXn->Write();
    hMaxEndcapEtInXn_0nXn->Write();
    hMaxEndcapEmEtInXn_0nXn->Write();
    hMaxEndcapHadEtInXn_0nXn->Write();

    fOut->Close();
}
