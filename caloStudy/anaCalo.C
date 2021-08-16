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

const Int_t nHalfEtaBins = 28, nPhiBins = 72;

// ***** define histograms ******
TH1D *hnEvts;
TH1D *hCentrality_Cluster;
TH1D *hCentrality_Vtx;
TH1D *hCentrality_OfficialMB;
TH1D *hCentrality_OfficialMB_ZDCAnd;
TH1D *hCentrality_NewMB;
TH1D *hCentrality_NewMB_ZDCAnd;

TH2D *hNtrkOfflineVsNtrkHP_Collision;

// data
// majority of data are missing in current CMSSW version

// emulation
TH2D *hIphiVsIeta_Emu[nSces];
TH2D *hIphiVsIeta_Emu_ECal[nSces];
TH2D *hIphiVsIeta_Emu_HCal[nSces];

TProfile2D *hEmuEtVsTower[nSces];
TH2D *hEmuEtVsTowerIdx[nSces];
TH2D *hEmuEmEtVsTowerIdx[nSces];
TH2D *hEmuHadEtVsTowerIdx[nSces];
TH3D *hEmuEtVsTowerIdxVsCen[nSces];
TH3D *hEmuEmEtVsTowerIdxVsCen[nSces];
TH3D *hEmuHadEtVsTowerIdxVsCen[nSces];
TH2D *hMaxEmuEtVsCen[nSces];
TH2D *hTotEmuEtVsCen[nSces];
TH2D *hTotEmuEmEtVsCen[nSces];
TH2D *hTotEmuHadEtVsCen[nSces];
TH3D *hTotEmuEmEtVsTotEmuHadEtVsCen[nSces];
TH2D *hTotBarrelEmuEtVsCen[nSces];
TH2D *hTotBarrelEmuEmEtVsCen[nSces];
TH2D *hTotBarrelEmuHadEtVsCen[nSces];
TH3D *hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen[nSces];
TH3D *hNTowersVsEmuEtThVsCen[nSces];
TH3D *hNTowersVsEmuEmEtThVsCen[nSces];
TH3D *hNTowersVsEmuHadEtThVsCen[nSces];

TH2D *hnTowersVsnEmuTowers;
TH2D *hTotEtVsTotEmuEt;
TH2D *hMaxEtVsMaxEmuEt;

void bookHistogram();
void writeHistogram(TString outputFileName);

int anaCalo(TString caloTreeList="test.list", TString outputFileName="caloInfo.test.histo")
{
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

        if(!ZB_HLTBit) cout<<"What?! This sample has non-ZB data !!!"<<endl;

        Bool_t ZDCAnd1 = (mZDCPlus>mNeuZDCHi[0][0]) && (mZDCMinus>mNeuZDCHi[1][0]);
        Bool_t ZDCAnd2 = (mZDCPlus>mNeuZDCHi[0][1]) && (mZDCMinus>mNeuZDCHi[1][1]);
        Bool_t ZDCAnd3 = (mZDCPlus>mNeuZDCHi[0][2]) && (mZDCMinus>mNeuZDCHi[1][2]);

        hnEvts->Fill(0.5);
        if(hfPosFilterTh5 && hfNegFilterTh5){
            hnEvts->Fill(1.5);

            if(ZDCAnd1) hnEvts->Fill(2.5);
        }

        if(clusterCompatibilityFilter){
            hCentrality_Cluster->Fill(mCenBin);

            if(primaryVertexFilter) {
                hCentrality_Vtx->Fill(mCenBin);

                if(hfCoincFilter2Th4){
                    hCentrality_OfficialMB->Fill(mCenBin);
                    if(ZDCAnd1) hCentrality_OfficialMB_ZDCAnd->Fill(mCenBin);
                }

                if(hfPosFilterTh5 && hfNegFilterTh5){
                    hCentrality_NewMB->Fill(mCenBin);
                    if(ZDCAnd1) hCentrality_NewMB_ZDCAnd->Fill(mCenBin);
                }
            }
        }


        Bool_t isNoise = !primaryVertexFilter  && !hfCoincFilter2Th4 && !hfPosFilterTh7 && !hfPosFilterTh7 && mNtrkHP==0 && mNtrkoffline==0;
        Bool_t isCollision = colEvtSel && ZDCAnd1;

        Int_t sceIdx = -1;

        if(isCollision){
            sceIdx = 0;

            hnEvts->Fill(4.5);
            hNtrkOfflineVsNtrkHP_Collision->Fill(mNtrkHP, mNtrkoffline);
        }

        if(isNoise){
            sceIdx = 1;

            hnEvts->Fill(5.5);
        }

        if(sceIdx < 0) continue;

        // from data
        Double_t maxEt = 0;
        Double_t totEt = 0;
        Int_t  nTowers   = event->nTowers;
        for(Int_t idx=0; idx<nTowers; idx++){
            Short_t ieta = event->ieta[idx]; // Central Calorimeter [-28, 28]
            Short_t iphi = event->iphi[idx]; // [1, 72]
            Short_t iet  = event->iet[idx];
            Short_t iem  = event->iem[idx];
            Short_t ihad = event->ihad[idx];

            if(ieta<-nHalfEtaBins || ieta>nHalfEtaBins) continue; // skip HF information and weird iphi, iet events

            if(iet>maxEt){
                maxEt = iet;
            }
            totEt += iet;
        }

        // from emulation 
        Double_t maxEmuEt = 0;
        Double_t totEmuEt = 0, totEmuEmEt = 0, totEmuHadEt = 0;
        Double_t totBarrelEmuEt = 0, totBarrelEmuEmEt = 0, totBarrelEmuHadEt = 0; 
        Int_t  nTowersAboveEtTh[nEtThs], nTowersAboveEmEtTh[nEtThs], nTowersAboveHadEtTh[nEtThs];
        memset(nTowersAboveEtTh, 0, sizeof(nTowersAboveEtTh));
        memset(nTowersAboveEmEtTh, 0, sizeof(nTowersAboveEmEtTh));
        memset(nTowersAboveHadEtTh, 0, sizeof(nTowersAboveHadEtTh));

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

            if(ietaEmu<-nHalfEtaBins || ietaEmu>nHalfEtaBins) continue; // skip HF information

            for(Int_t ith=0; ith<nEtThs; ith++){
                if(ietEmu>mEtTh[ith])  nTowersAboveEtTh[ith]++;
                if(iemEmu>mEtTh[ith])  nTowersAboveEmEtTh[ith]++;
                if(ihadEmu>mEtTh[ith]) nTowersAboveHadEtTh[ith]++;
            }

            if(ietEmu>maxEmuEt){
                maxEmuEt = ietEmu;
            }

            totEmuEt += ietEmu;
            totEmuEmEt += iemEmu;
            totEmuHadEt += ihadEmu;
            if(fabs(ietaEmu)<17){
                totBarrelEmuEt += ietEmu;
                totBarrelEmuEmEt += iemEmu;
                totBarrelEmuHadEt += ihadEmu;
            }

            Int_t towerIdx = ietaEmu<0 ? ietaEmu*nPhiBins + iphiEmu - 1 : (ietaEmu-1)*nPhiBins + iphiEmu;

            hEmuEtVsTower[sceIdx]->Fill(ietaEmu, iphiEmu, ietEmu);
            hEmuEtVsTowerIdx[sceIdx]->Fill(towerIdx, ietEmu);
            hEmuEmEtVsTowerIdx[sceIdx]->Fill(towerIdx, iemEmu);
            hEmuHadEtVsTowerIdx[sceIdx]->Fill(towerIdx, ihadEmu);
            hEmuEtVsTowerIdxVsCen[sceIdx]->Fill(mCenBin, towerIdx, ietEmu);
            hEmuEmEtVsTowerIdxVsCen[sceIdx]->Fill(mCenBin, towerIdx, iemEmu);
            hEmuHadEtVsTowerIdxVsCen[sceIdx]->Fill(mCenBin, towerIdx, ihadEmu);

            if(ietEmu>0){
                hIphiVsIeta_Emu[sceIdx]->Fill(ietaEmu, iphiEmu);
            }
            if(iemEmu>0){
                hIphiVsIeta_Emu_ECal[sceIdx]->Fill(ietaEmu, iphiEmu);
            }
            if(ihadEmu>0){
                hIphiVsIeta_Emu_HCal[sceIdx]->Fill(ietaEmu, iphiEmu);
            }
        }

        hMaxEmuEtVsCen[sceIdx]->Fill(mCenBin, maxEmuEt);
        hTotEmuEtVsCen[sceIdx]->Fill(mCenBin, totEmuEt);
        hTotEmuEmEtVsCen[sceIdx]->Fill(mCenBin, totEmuEmEt);
        hTotEmuHadEtVsCen[sceIdx]->Fill(mCenBin, totEmuHadEt);
        hTotEmuEmEtVsTotEmuHadEtVsCen[sceIdx]->Fill(mCenBin, totEmuHadEt, totEmuEmEt);
        hTotBarrelEmuEtVsCen[sceIdx]->Fill(mCenBin, totBarrelEmuEt);
        hTotBarrelEmuEmEtVsCen[sceIdx]->Fill(mCenBin, totBarrelEmuEmEt);
        hTotBarrelEmuHadEtVsCen[sceIdx]->Fill(mCenBin, totBarrelEmuHadEt);
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen[sceIdx]->Fill(mCenBin, totBarrelEmuHadEt, totBarrelEmuEmEt);

        for(Int_t ith=0; ith<nEtThs; ith++){
            hNTowersVsEmuEtThVsCen[sceIdx]->Fill(mCenBin, ith+0.5, nTowersAboveEtTh[ith]);
            hNTowersVsEmuEmEtThVsCen[sceIdx]->Fill(mCenBin, ith+0.5, nTowersAboveEmEtTh[ith]);
            hNTowersVsEmuHadEtThVsCen[sceIdx]->Fill(mCenBin, ith+0.5, nTowersAboveHadEtTh[ith]);
        }

        hnTowersVsnEmuTowers->Fill(nTowersEmu, nTowers);
        hMaxEtVsMaxEmuEt->Fill(maxEmuEt, maxEt);
        hTotEtVsTotEmuEt->Fill(totEmuEt, totEt);
    }

    writeHistogram(outputFileName);

    //caloTreeChain->Delete();

    return 0;
}

void bookHistogram(){
    const Int_t    nMaxTowers = nHalfEtaBins * 2 * nPhiBins + 1;
    const Double_t mTowerIdxLow = -nHalfEtaBins*nPhiBins - 0.5; 
    const Double_t mTowerIdxHi  = nHalfEtaBins*nPhiBins + 0.5;
    const Int_t    nEtBins = 100;
    const Int_t    nCenBins = 20;
    const Int_t    nTotEtBins[nSces] = {1500, 500};
    const Double_t mMaxTotEt[nSces] = {15000, 500};

    // event level
    hnEvts = new TH1D("hnEvts", "hnEvts;", 10, 0, 10);
    hnEvts->GetXaxis()->SetBinLabel(1, "Zero-bias");
    hnEvts->GetXaxis()->SetBinLabel(2, "HFTh5And");
    hnEvts->GetXaxis()->SetBinLabel(3, "HFTh5And & ZDCAnd1");

    hnEvts->GetXaxis()->SetBinLabel(5, "Collision");
    hnEvts->GetXaxis()->SetBinLabel(6, "Noise");
    hnEvts->GetXaxis()->LabelsOption("d");
    hnEvts->GetXaxis()->SetLabelSize(0.055);

    hCentrality_Cluster = new TH1D("hCentrality_Cluster", "hCentrality_Cluster; Centrality", 200, 0, 200);
    hCentrality_Vtx     = new TH1D("hCentrality_Vtx", "hCentrality_Vtx; Centrality", 200, 0, 200);
    hCentrality_OfficialMB        = new TH1D("hCentrality_OfficialMB", "hCentrality_OfficialMB; Centrality", 200, 0, 200);
    hCentrality_OfficialMB_ZDCAnd = new TH1D("hCentrality_OfficialMB_ZDCAnd", "hCentrality_OfficialMB_ZDCAnd; Centrality", 200, 0, 200);
    hCentrality_NewMB             = new TH1D("hCentrality_NewMB", "hCentrality_NewMB; Centrality", 200, 0, 200);
    hCentrality_NewMB_ZDCAnd      = new TH1D("hCentrality_NewMB_ZDCAnd", "hCentrality_NewMB_ZDCAnd; Centrality", 200, 0, 200);

    hNtrkOfflineVsNtrkHP_Collision = new TH2D("hNtrkOfflineVsNtrkHP_Collision", "hNtrkOfflineVsNtrkHP_Collision; N_{trk}^{HP}; N_{trk}^{Offline}", 2000, 0, 4000, 2000, 0, 4000);

    for(Int_t i=0; i<nSces; i++){
        hIphiVsIeta_Emu[i]      = new TH2D(Form("hIphiVsIeta_Emu_%s", sceName[i].Data()), "hIphiVsIeta_Emu; ieta; iphi", 91, -45-0.5, 45+0.5, 145, -72-0.5, 72+0.5);
        hIphiVsIeta_Emu_ECal[i] = new TH2D(Form("hIphiVsIeta_Emu_ECal_%s", sceName[i].Data()), "hIphiVsIeta_Emu_ECal; ieta; iphi", 91, -45-0.5, 45+0.5, 145, -72-0.5, 72+0.5);
        hIphiVsIeta_Emu_HCal[i] = new TH2D(Form("hIphiVsIeta_Emu_HCal_%s", sceName[i].Data()), "hIphiVsIeta_Emu_HCal; ieta; iphi", 91, -45-0.5, 45+0.5, 145, -72-0.5, 72+0.5);

        hEmuEtVsTower[i]         = new TProfile2D(Form("hEmuEtVsTower_%s", sceName[i].Data()), "hEmuEtVsTower; ieta; iphi; #LTiet#GT", nHalfEtaBins*2+1, -nHalfEtaBins-0.5, nHalfEtaBins+0.5, nPhiBins, 0.5, nPhiBins+0.5);
        hEmuEtVsTowerIdx[i]      = new TH2D(Form("hEmuEtVsTowerIdx_%s", sceName[i].Data()), "hEmuEtVsTowerIdx; towerIdx; iet", nMaxTowers, mTowerIdxLow, mTowerIdxHi, nEtBins, 0, nEtBins);
        hEmuEmEtVsTowerIdx[i]    = new TH2D(Form("hEmuEmEtVsTowerIdx_%s", sceName[i].Data()), "hEmuEmEtVsTowerIdx; towerIdx; iet^{ECal}", nMaxTowers, mTowerIdxLow, mTowerIdxHi, nEtBins, 0, nEtBins);
        hEmuHadEtVsTowerIdx[i]   = new TH2D(Form("hEmuHadEtVsTowerIdx_%s", sceName[i].Data()), "hEmuHadEtVsTowerIdx; towerIdx; iet^{HCal}", nMaxTowers, mTowerIdxLow, mTowerIdxHi, nEtBins, 0, nEtBins);
        hEmuEtVsTowerIdxVsCen[i] = new TH3D(Form("hEmuEtVsTowerIdxVsCen_%s", sceName[i].Data()), "hEmuEtVsTowerIdxVsCen; Centrality; towerIdx, iet", nCenBins, 0, 200, nMaxTowers, mTowerIdxLow, mTowerIdxHi, nEtBins, 0, nEtBins);
        hEmuEmEtVsTowerIdxVsCen[i]  = new TH3D(Form("hEmuEmEtVsTowerIdxVsCen_%s", sceName[i].Data()), "hEmuEmEtVsTowerIdxVsCen; Centrality; towerIdx, iet^{ECal}", nCenBins, 0, 200, nMaxTowers, mTowerIdxLow, mTowerIdxHi, nEtBins, 0, nEtBins);
        hEmuHadEtVsTowerIdxVsCen[i] = new TH3D(Form("hEmuHadEtVsTowerIdxVsCen_%s", sceName[i].Data()), "hEmuHadEtVsTowerIdxVsCen; Centrality; towerIdx, iet^{HCal}", nCenBins, 0, 200, nMaxTowers, mTowerIdxLow, mTowerIdxHi, nEtBins, 0, nEtBins);
        hMaxEmuEtVsCen[i]    = new TH2D(Form("hMaxEmuEtVsCen_%s", sceName[i].Data()), "hMaxEmuEtVsCen; Centrality; maximum E_{t}^{Emu}", nCenBins, 0, 200, nEtBins, 0, nEtBins);
        hTotEmuEtVsCen[i]    = new TH2D(Form("hTotEmuEtVsCen_%s", sceName[i].Data()), "hTotEmuEtVsCen; Centrality; total E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i]);
        hTotEmuEmEtVsCen[i]  = new TH2D(Form("hTotEmuEmEtVsCen_%s", sceName[i].Data()), "hTotEmuEmEtVsCen; Centrality; total ECal E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i]);
        hTotEmuHadEtVsCen[i] = new TH2D(Form("hTotEmuHadEtVsCen_%s", sceName[i].Data()), "hTotEmuHadEtVsCen; Centrality; total HCal E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i]);
        hTotEmuEmEtVsTotEmuHadEtVsCen[i] = new TH3D(Form("hTotEmuEmEtVsTotEmuHadEtVsCen_%s", sceName[i].Data()), "hTotEmuEmEtVsTotEmuHadEtVsCen; Centrality; total HCal E_{t}^{Emu}; total ECal E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i], nTotEtBins[i], 0, mMaxTotEt[i]);
        hTotBarrelEmuEtVsCen[i]    = new TH2D(Form("hTotBarrelEmuEtVsCen_%s", sceName[i].Data()), "hTotBarrelEmuEtVsCen; Centrality; total barrel E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i]);
        hTotBarrelEmuEmEtVsCen[i]  = new TH2D(Form("hTotBarrelEmuEmEtVsCen_%s", sceName[i].Data()), "hTotBarrelEmuEmEtVsCen; Centrality; total barrel ECal E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i]);
        hTotBarrelEmuHadEtVsCen[i] = new TH2D(Form("hTotBarrelEmuHadEtVsCen_%s", sceName[i].Data()), "hTotBarrelEmuHadEtVsCen; Centrality; total barrel HCal E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i]);
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen[i] = new TH3D(Form("hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen_%s", sceName[i].Data()), "hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen; Centrality; total barrel HCal E_{t}^{Emu}; total barrel ECal E_{t}^{Emu}", nCenBins, 0, 200, nTotEtBins[i], 0, mMaxTotEt[i], nTotEtBins[i], 0, mMaxTotEt[i]);
        hNTowersVsEmuEtThVsCen[i]    = new TH3D(Form("hNTowersVsEmuEtThVsCen_%s", sceName[i].Data()), "hNTowersVsEmuEtThVsCen; Centrality; E_{t} Threshold; nTowers", nCenBins, 0, 200, nEtThs, 0, nEtThs, nMaxTowers, 0, nMaxTowers);
        hNTowersVsEmuEmEtThVsCen[i]  = new TH3D(Form("hNTowersVsEmuEmEtThVsCen_%s", sceName[i].Data()), "hNTowersVsEmuEmEtThVsCen; Centrality; ECal E_{t} Threshold; nECalTowers", nCenBins, 0, 200, nEtThs, 0, nEtThs, nMaxTowers, 0, nMaxTowers);
        hNTowersVsEmuHadEtThVsCen[i] = new TH3D(Form("hNTowersVsEmuHadEtThVsCen_%s", sceName[i].Data()), "hNTowersVsEmuHadEtThVsCen; Centrality; HCal E_{t} Threshold; nHCalTowers", nCenBins, 0, 200, nEtThs, 0, nEtThs, nMaxTowers, 0, nMaxTowers);
        for(Int_t ith=0; ith<nEtThs; ith++){
            hNTowersVsEmuEtThVsCen[i]->GetYaxis()->SetBinLabel(ith+1, Form("%1.1f GeV", mEtTh[ith]/2.));
            hNTowersVsEmuEmEtThVsCen[i]->GetYaxis()->SetBinLabel(ith+1, Form("%1.1f GeV", mEtTh[ith]/2.));
            hNTowersVsEmuHadEtThVsCen[i]->GetYaxis()->SetBinLabel(ith+1, Form("%1.1f GeV", mEtTh[ith]/2.));
        }
        hNTowersVsEmuEtThVsCen[i]->GetYaxis()->SetLabelSize(0.04);
        hNTowersVsEmuEmEtThVsCen[i]->GetYaxis()->SetLabelSize(0.04);
        hNTowersVsEmuHadEtThVsCen[i]->GetYaxis()->SetLabelSize(0.04);
    }

    hnTowersVsnEmuTowers  = new TH2D("hnTowersVsnEmuTowers", "hnTowersVsnEmuTowers; nTowers^{Emu}; nTowers", 2550, -100, 5000, 2550, -100, 5000);
    hMaxEtVsMaxEmuEt      = new TH2D("hMaxEtVsMaxEmuEt", "hMaxEtVsMaxEmuEt; maximum E_{t}^{Emu}; maximum E_{t}", nEtBins, 0, nEtBins, nEtBins, 0, nEtBins);
    hTotEtVsTotEmuEt  = new TH2D("hTotEtVsTotEmuEt", "hTotEtVsTotEmuEt; total E_{t}^{Emu}; total E_{t}", 1020, -400, 20000, 1020, -400, 20000);
}

void writeHistogram(TString outputFileName){

    TFile *fOut = new TFile(Form("%s.root", outputFileName.Data()), "recreate");

    fOut->cd();

    hnEvts->Write();
    hCentrality_Cluster->Write();
    hCentrality_Vtx->Write();
    hCentrality_OfficialMB->Write();
    hCentrality_OfficialMB_ZDCAnd->Write();
    hCentrality_NewMB->Write();
    hCentrality_NewMB_ZDCAnd->Write();

    hNtrkOfflineVsNtrkHP_Collision->Write();

    for(Int_t i=0; i<nSces; i++){
        hIphiVsIeta_Emu[i]->Write();
        hIphiVsIeta_Emu_ECal[i]->Write();
        hIphiVsIeta_Emu_HCal[i]->Write();

        hEmuEtVsTower[i]->Write();
        hEmuEtVsTowerIdx[i]->Write();
        hEmuEmEtVsTowerIdx[i]->Write();
        hEmuHadEtVsTowerIdx[i]->Write();
        hEmuEtVsTowerIdxVsCen[i]->Write();
        hEmuEmEtVsTowerIdxVsCen[i]->Write();
        hEmuHadEtVsTowerIdxVsCen[i]->Write();
        hMaxEmuEtVsCen[i]->Write();
        hTotEmuEtVsCen[i]->Write();
        hTotEmuEmEtVsCen[i]->Write();
        hTotEmuHadEtVsCen[i]->Write();
        hTotEmuEmEtVsTotEmuHadEtVsCen[i]->Write();
        hTotBarrelEmuEtVsCen[i]->Write();
        hTotBarrelEmuEmEtVsCen[i]->Write();
        hTotBarrelEmuHadEtVsCen[i]->Write();
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen[i]->Write();
        hNTowersVsEmuEtThVsCen[i]->Write();
        hNTowersVsEmuEmEtThVsCen[i]->Write();
        hNTowersVsEmuHadEtThVsCen[i]->Write();
    }

    hnTowersVsnEmuTowers->Write();
    hMaxEtVsMaxEmuEt->Write();
    hTotEtVsTotEmuEt->Write();

    fOut->Close();
}
