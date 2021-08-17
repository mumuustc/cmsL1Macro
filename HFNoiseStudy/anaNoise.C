#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include "math.h"
#include "sys/types.h"

using namespace std;

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include "hfConstant.h"
#include "L1AnalysisEventDataFormat.h"

int anaNoise(Int_t nEvts = 10, TString L1File="/eos/cms/store/group/phys_heavyions/shuaiy/L1Emulator/L1Ntuple/L1NtupleRAWEMUCalo_HFAdc_ZeroBias_run327211/HIForward/L1NtupleRAWEMUCalo_HFAdc_ZeroBias_run327211/210505_023909/0000/L1Ntuple_88.root", TString HiForestFileList="ZBEvtInfo_RunByRunList/327211.list", TString outputFile="L1HFHisto_test")
{
    TFile *fL1Ntuple = TFile::Open(L1File);
    TTree *mL1EventTree = (TTree *)fL1Ntuple->Get("l1EventTree/L1EventTree");
    TTree *mAdcTree = (TTree *)fL1Ntuple->Get("HFAdcana/adc");

    // grab L1 event information
    L1Analysis::L1AnalysisEventDataFormat *l1Event = new L1Analysis::L1AnalysisEventDataFormat();
    mL1EventTree->SetBranchAddress("Event", &l1Event);

    //for(Int_t i=0; i<25; i++){
    //    mL1EventTree->GetEntry(i);
    //    cout<<l1Event->run<<"   "<<l1Event->event<<"   "<<l1Event->lumi<<endl;
    //}
    //return -1;

    // grab L1 HF information
    std::vector<int> *ieta;
    std::vector<int> *iphi;
    std::vector<int> *depth;
    std::vector<int> *ampl;
    std::vector<float> *energy_ped;
    std::vector<float> *energy;

    ieta = 0;
    iphi = 0;
    depth = 0;
    ampl = 0;
    energy_ped = 0;
    energy = 0;

    mAdcTree->SetBranchStatus("*", 0);
    mAdcTree->SetBranchStatus("ieta",  1);
    mAdcTree->SetBranchStatus("iphi",  1);
    mAdcTree->SetBranchStatus("depth", 1);
    mAdcTree->SetBranchStatus("ampl",  1);
    mAdcTree->SetBranchStatus("energy",  1);
    mAdcTree->SetBranchStatus("energy_ped",  1);
    mAdcTree->SetBranchAddress("ieta",  &ieta);
    mAdcTree->SetBranchAddress("iphi",  &iphi);
    mAdcTree->SetBranchAddress("depth", &depth);
    mAdcTree->SetBranchAddress("ampl",  &ampl);
    mAdcTree->SetBranchAddress("energy_ped",  &energy_ped);
    mAdcTree->SetBranchAddress("energy",  &energy);

    //for(Int_t i=0; i<1; i++){
    //    mAdcTree->GetEntry(i);
    //    for(size_t idx=0; idx<ieta->size(); idx++){
    //        cout<<"idx="<<idx<<",   "<<(*ieta)[idx]<<"  "<<(*iphi)[idx]<<"  "<<(*depth)[idx]<<"  "<<(*ampl)[idx]<<"  "<<(*energy_ped)[idx]<<endl;
    //    }
    //}
    //return -1;

    // grab offline information
    TChain *mHiEvtChain  = new TChain("eventinfoana/EventInfoNtuple");

    Int_t ifile=0;
    char filename[512];
    ifstream *inputStream = new ifstream;
    inputStream->open(HiForestFileList.Data());
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
                mHiEvtChain->Add(filename);
                ifile++;
            }
            delete ftmp;
        }
    }
    delete inputStream;

    const int nMaxEvtSelBits = 18;
    const int nMaxTrigHLTBits = 12;

    unsigned int RunNb = 0;
    unsigned int LSNb = 0;
    unsigned int EventNb = 0;
    short        centrality = -1;
    float        bestvtxX, bestvtxY, bestvtxZ;
    float        ZDCPlus, ZDCMinus;
    int          Npixel, Ntrkoffline, NtrkHP;
    Bool_t       trigHLT[nMaxTrigHLTBits];
    Bool_t       evtSel[nMaxEvtSelBits];

    mHiEvtChain->SetBranchStatus("*", 0);
    mHiEvtChain->SetBranchStatus("RunNb", 1);
    mHiEvtChain->SetBranchStatus("LSNb", 1);
    mHiEvtChain->SetBranchStatus("EventNb", 1);
    mHiEvtChain->SetBranchStatus("centrality", 1);
    mHiEvtChain->SetBranchStatus("bestvtxX", 1);
    mHiEvtChain->SetBranchStatus("bestvtxY", 1);
    mHiEvtChain->SetBranchStatus("bestvtxZ", 1);
    mHiEvtChain->SetBranchStatus("Npixel", 1);
    mHiEvtChain->SetBranchStatus("ZDCPlus", 1);
    mHiEvtChain->SetBranchStatus("ZDCMinus", 1);
    mHiEvtChain->SetBranchStatus("Ntrkoffline", 1);
    mHiEvtChain->SetBranchStatus("NtrkHP", 1);
    mHiEvtChain->SetBranchStatus("trigHLT", 1);
    mHiEvtChain->SetBranchStatus("evtSel", 1);

    mHiEvtChain->SetBranchAddress("RunNb", &RunNb);
    mHiEvtChain->SetBranchAddress("LSNb", &LSNb);
    mHiEvtChain->SetBranchAddress("EventNb", &EventNb);
    mHiEvtChain->SetBranchAddress("centrality", &centrality);
    mHiEvtChain->SetBranchAddress("bestvtxX", &bestvtxX);
    mHiEvtChain->SetBranchAddress("bestvtxY", &bestvtxY);
    mHiEvtChain->SetBranchAddress("bestvtxZ", &bestvtxZ);
    mHiEvtChain->SetBranchAddress("ZDCPlus", &ZDCPlus);
    mHiEvtChain->SetBranchAddress("ZDCMinus", &ZDCMinus);
    mHiEvtChain->SetBranchAddress("Npixel", &Npixel);
    mHiEvtChain->SetBranchAddress("Ntrkoffline", &Ntrkoffline);
    mHiEvtChain->SetBranchAddress("NtrkHP", &NtrkHP);
    mHiEvtChain->SetBranchAddress("trigHLT", trigHLT);
    mHiEvtChain->SetBranchAddress("evtSel", evtSel);

    // define histograms
    const int    nAdcBins = 200;
    const int    nEBins   = 500;
    const double minE     = 0;
    const double maxE     = 200;

    TH2D *hHCalTPCompEtvsTower_Noise     = new TH2D("hHCalTPCompEtvsTower_Noise", "hHCalTPCompEtvsTower_Noise; towerIdx; hCalTPCompEt (GeV); Entries", nMaxTowers, -0.5, nMaxTowers-0.5, 200, 0, 100);
    TH2D *hHCalTPCompEtvsTower_Collision = new TH2D("hHCalTPCompEtvsTower_Collision", "hHCalTPCompEtvsTower_Collision; towerIdx; hCalTPCompEt (GeV); Entries", nMaxTowers, -0.5, nMaxTowers-0.5, 200, 0, 100);

    TH2D *hAdcvsTower_Noise[nMaxDepth];
    TH2D *hEpedvsTower_Noise[nMaxDepth];
    TH2D *hAdcvsTower_Collision[nMaxDepth];
    TH2D *hEpedvsTower_Collision[nMaxDepth];
    for(int depthIdx=0; depthIdx<nMaxDepth; depthIdx++){
        hAdcvsTower_Noise[depthIdx]  = new TH2D(Form("hAdcvsTower_Noise_depthIdx%d", depthIdx), Form("depth=%d; towerIdx; adc", depthIdx+1), nMaxTowers, -0.5, nMaxTowers-0.5, nAdcBins, 0, nAdcBins);
        hEpedvsTower_Noise[depthIdx] = new TH2D(Form("hEpedvsTower_Noise_depthIdx%d", depthIdx), Form("depth=%d; towerIdx; energy w/ pedestal (GeV)", depthIdx+1), nMaxTowers, -0.5, nMaxTowers-0.5, nEBins, minE, maxE);

        hAdcvsTower_Collision[depthIdx]  = new TH2D(Form("hAdcvsTower_Collision_depthIdx%d", depthIdx), Form("depth=%d; towerIdx; adc", depthIdx+1), nMaxTowers, -0.5, nMaxTowers-0.5, nAdcBins, 0, nAdcBins);
        hEpedvsTower_Collision[depthIdx] = new TH2D(Form("hEpedvsTower_Collision_depthIdx%d", depthIdx), Form("depth=%d; towerIdx; energy w/ pedestal (GeV)", depthIdx+1), nMaxTowers, -0.5, nMaxTowers-0.5, nEBins, minE, maxE);
    }

    TH2D *hAdcDep3vsDep1_Noise = new TH2D("hAdcDep3vsDep1_Noise", "hAdcDep3vsDep1_Noise; depth=1 adc; depth=3 adc", nAdcBins, 0, nAdcBins, nAdcBins, 0, nAdcBins);
    TH2D *hAdcDep4vsDep2_Noise = new TH2D("hAdcDep4vsDep2_Noise", "hAdcDep4vsDep2_Noise; depth=2 adc; depth=4 adc", nAdcBins, 0, nAdcBins, nAdcBins, 0, nAdcBins);
    TH2D *hEpedDep3vsDep1_Noise = new TH2D("hEpedDep3vsDep1_Noise", "hEpedDep3vsDep1_Noise; depth=1 energy w/ pedestal (GeV); depth=3 energy w/ pedestal (GeV)", nEBins, minE, maxE, nEBins, minE, maxE);
    TH2D *hEpedDep4vsDep2_Noise = new TH2D("hEpedDep4vsDep2_Noise", "hEpedDep4vsDep2_Noise; depth=2 energy w/ pedestal (GeV); depth=4 energy w/ pedestal (GeV)", nEBins, minE, maxE, nEBins, minE, maxE);

    TH2D *hAdcDep3vsDep1_Collision = new TH2D("hAdcDep3vsDep1_Collision", "hAdcDep3vsDep1_Collision; depth=1 adc; depth=3 adc", nAdcBins, 0, nAdcBins, nAdcBins, 0, nAdcBins);
    TH2D *hAdcDep4vsDep2_Collision = new TH2D("hAdcDep4vsDep2_Collision", "hAdcDep4vsDep2_Collision; depth=2 adc; depth=4 adc", nAdcBins, 0, nAdcBins, nAdcBins, 0, nAdcBins);
    TH2D *hEpedDep3vsDep1_Collision = new TH2D("hEpedDep3vsDep1_Collision", "hEpedDep3vsDep1_Collision; depth=1 energy w/ pedestal (GeV); depth=3 energy w/ pedestal (GeV)", nEBins, minE, maxE, nEBins, minE, maxE);
    TH2D *hEpedDep4vsDep2_Collision = new TH2D("hEpedDep4vsDep2_Collision", "hEpedDep4vsDep2_Collision; depth=2 energy w/ pedestal (GeV); depth=4 energy w/ pedestal (GeV)", nEBins, minE, maxE, nEBins, minE, maxE);

    TH1D *hAdcDep3Dep1Diff_Noise  = new TH1D("hAdcDep3Dep1Diff_Noise", "hAdcDep3Dep1Diff_Noise; adc (depth3 - depth1)", 2*nAdcBins, -nAdcBins, nAdcBins);
    TH1D *hAdcDep4Dep2Diff_Noise  = new TH1D("hAdcDep4Dep2Diff_Noise", "hAdcDep4Dep2Diff_Noise; adc (depth4 - depth2)", 2*nAdcBins, -nAdcBins, nAdcBins);
    TH1D *hEpedDep3Dep1Diff_Noise = new TH1D("hEpedDep3Dep1Diff_Noise", "hEpedDep3Dep1Diff_Noise; energy w/pedestal (depth3 - depth1)", 2*nEBins, -maxE, maxE);
    TH1D *hEpedDep4Dep2Diff_Noise = new TH1D("hEpedDep4Dep2Diff_Noise", "hEpedDep4Dep2Diff_Noise; energy w/pedestal (depth4 - depth2)", 2*nEBins, -maxE, maxE);

    TH1D *hAdcDep3Dep1Diff_Collision  = new TH1D("hAdcDep3Dep1Diff_Collision", "hAdcDep3Dep1Diff_Collision; adc (depth3 - depth1)", 2*nAdcBins, -nAdcBins, nAdcBins);
    TH1D *hAdcDep4Dep2Diff_Collision  = new TH1D("hAdcDep4Dep2Diff_Collision", "hAdcDep4Dep2Diff_Collision; adc (depth4 - depth2)", 2*nAdcBins, -nAdcBins, nAdcBins);
    TH1D *hEpedDep3Dep1Diff_Collision = new TH1D("hEpedDep3Dep1Diff_Collision", "hEpedDep3Dep1Diff_Collision; energy w/pedestal (depth3 - depth1)", 2*nEBins, -maxE, maxE);
    TH1D *hEpedDep4Dep2Diff_Collision = new TH1D("hEpedDep4Dep2Diff_Collision", "hEpedDep4Dep2Diff_Collision; energy w/pedestal (depth4 - depth2)", 2*nEBins, -maxE, maxE);

    TH2D *hFiberAdcvsTower_Noise[nMaxFiber];
    TH2D *hFiberEpedvsTower_Noise[nMaxFiber];
    TH2D *hFiberAdcvsTower_Collision[nMaxFiber];
    TH2D *hFiberEpedvsTower_Collision[nMaxFiber];
    for(int fiberIdx=0; fiberIdx<nMaxFiber; fiberIdx++){
        hFiberAdcvsTower_Noise[fiberIdx]  = new TH2D(Form("hFiberAdcvsTower_Noise_fiberIdx%d", fiberIdx), Form("%s; towerIdx; adc", fiberName[fiberIdx].Data()), nMaxTowers, -0.5, nMaxTowers-0.5, nAdcBins, 0, nAdcBins);
        hFiberEpedvsTower_Noise[fiberIdx] = new TH2D(Form("hFiberEpedvsTower_Noise_fiberIdx%d", fiberIdx), Form("%s; towerIdx; energy w/ pedestal (GeV)", fiberName[fiberIdx].Data()), nMaxTowers, -0.5, nMaxTowers-0.5, nEBins, minE, maxE);

        hFiberAdcvsTower_Collision[fiberIdx]  = new TH2D(Form("hFiberAdcvsTower_Collision_fiberIdx%d", fiberIdx), Form("%s; towerIdx; adc", fiberName[fiberIdx].Data()), nMaxTowers, -0.5, nMaxTowers-0.5, nAdcBins, 0, nAdcBins);
        hFiberEpedvsTower_Collision[fiberIdx] = new TH2D(Form("hFiberEpedvsTower_Collision_fiberIdx%d", fiberIdx), Form("%s; towerIdx; energy w/ pedestal (GeV)", fiberName[fiberIdx].Data()), nMaxTowers, -0.5, nMaxTowers-0.5, nEBins, minE, maxE);
    }
    TH2D *hAdcShortvsLong_Noise  = new TH2D("hAdcShortvsLong_Noise", "hAdcShortvsLong_Noise; long fiber adc; short fiber adc", nAdcBins, 0, nAdcBins, nAdcBins, 0, nAdcBins);
    TH2D *hEpedShortvsLong_Noise = new TH2D("hEpedShortvsLong_Noise", "hEpedShortvsLong_Noise; long fiber energy w/ pedestal (GeV); short fiber energy w/ pedestal (GeV)", nEBins, minE, maxE, nEBins, minE, maxE);
    TH2D *hAdcShortvsLong_Collision  = new TH2D("hAdcShortvsLong_Collision", "hAdcShortvsLong_Collision; long fiber adc; short fiber adc", nAdcBins, 0, nAdcBins, nAdcBins, 0, nAdcBins);
    TH2D *hEpedShortvsLong_Collision = new TH2D("hEpedShortvsLong_Collision", "hEpedShortvsLong_Collision; long fiber energy w/ pedestal (GeV); short fiber energy w/ pedestal (GeV)", nEBins, minE, maxE, nEBins, minE, maxE);

    TH1D *hAdcLongShortDiff_Noise  = new TH1D("hAdcLongShortDiff_Noise", "hAdcLongShortDiff_Noise; adc (long fiber - short fiber)", 2*nAdcBins, -nAdcBins, nAdcBins);
    TH1D *hEpedLongShortDiff_Noise = new TH1D("hEpedLongShortDiff_Noise", "hEpedLongShortDiff_Noise; energy w/pedestal (long fiber - short fiber)", 2*nEBins, -maxE, maxE);
    TH1D *hAdcLongShortDiff_Collision  = new TH1D("hAdcLongShortDiff_Collision", "hAdcLongShortDiff_Collision; adc (long fiber - short fiber)", 2*nAdcBins, -nAdcBins, nAdcBins);
    TH1D *hEpedLongShortDiff_Collision = new TH1D("hEpedLongShortDiff_Collision", "hEpedLongShortDiff_Collision; energy w/pedestal (long fiber - short fiber)", 2*nEBins, -maxE, maxE);

    // these histograms are only for tower has at least an adc number above threshold
    TH2D *hTrigAdcLongShortDiffvsCen_Noise  = new TH2D("hTrigAdcLongShortDiffvsCen_Noise", "hTrigAdcLongShortDiffvsCen_Noise; centrality; adc (long fiber - short fiber)", 20, 0, 200, 2*nAdcBins, -nAdcBins, nAdcBins);
    TH2D *hTrigEpedLongShortDiffvsCen_Noise = new TH2D("hTrigEpedLongShortDiffvsCen_Noise", "hTrigEpedLongShortDiffvsCen_Noise; centrality;  energy w/pedestal (long fiber - short fiber)", 20, 0, 200, 2*nEBins, -maxE, maxE);
    TH2D *hTrigAdcLongShortDiffvsCen_Collision  = new TH2D("hTrigAdcLongShortDiffvsCen_Collision", "hTrigAdcLongShortDiffvsCen_Collision; centrality; adc (long fiber - short fiber)", 20, 0, 200, 2*nAdcBins, -nAdcBins, nAdcBins);
    TH2D *hTrigEpedLongShortDiffvsCen_Collision = new TH2D("hTrigEpedLongShortDiffvsCen_Collision", "hTrigEpedLongShortDiffvsCen_Collision; centrality; energy w/pedestal (long fiber - short fiber)", 20, 0, 200, 2*nEBins, -maxE, maxE);

    // L1 & offline information match
    int    adc[nMaxEta][nMaxPhi][nMaxDepth];
    double e_ped[nMaxEta][nMaxPhi][nMaxDepth];
    double e[nMaxEta][nMaxPhi][nMaxDepth];
    bool   trigTower[nMaxEta][nMaxPhi];

    int    fiberAdc[nMaxEta][nMaxPhi][nMaxFiber];
    double fiberE_ped[nMaxEta][nMaxPhi][nMaxFiber];

    int    nCollisions=0;

    if(nEvts<0){
        nEvts = mL1EventTree->GetEntries();
    }
    cout<<"Total Entries: "<<nEvts<<endl;
    for(int ievt=0; ievt<nEvts; ievt++){
        if(nEvts<100){
            cout << "begin " << ievt << "th entry...." << endl;
        }
        else if(ievt % (nEvts / 100) == 0){
            cout << "begin " << ievt << "th entry...." << endl;
        }
        //if(ievt%100 == 0) cout << "begin " << ievt << "th entry...." << endl;

        mL1EventTree->GetEntry(ievt);
        mAdcTree->GetEntry(ievt);

        Bool_t isNoise = kFALSE;
        Bool_t isCollision = kFALSE;
        Bool_t evtMatch = kFALSE;
        Int_t  cenBin = -1;
        for(Int_t jevt=0; jevt<mHiEvtChain->GetEntries(); jevt++){
            mHiEvtChain->GetEntry(jevt);

            Bool_t  colEvtSel                  = evtSel[0];
            Bool_t  hfCoincFilter2Th4          = evtSel[1];
            Bool_t  primaryVertexFilter        = evtSel[2];
            Bool_t  clusterCompatibilityFilter = evtSel[3];
            Bool_t  hfPosFilterTh5             = evtSel[8];
            Bool_t  hfNegFilterTh5             = evtSel[9];
            Bool_t  hfPosFilterTh7             = evtSel[12];
            Bool_t  hfNegFilterTh7             = evtSel[13];

            if(l1Event->run == RunNb && l1Event->event == EventNb && l1Event->lumi == LSNb){
                //isNoise = !(pprimaryVertexFilter || phfCoincFilter2Th4); 
                isNoise     = !colEvtSel; 
                isCollision = colEvtSel;
                cenBin      = centrality;

                evtMatch = kTRUE;

                break;
            }
        }

        if(!evtMatch){
            cout<<"Cannot find offline event matched with L1 event: "<<endl;
            cout<<"runId: "<<l1Event->run<<"     eventId: "<<l1Event->event<<"     lumiId: "<<l1Event->lumi<<endl;

            continue;
        }
        if(isCollision) nCollisions++;

        memset(adc, -1, sizeof(adc));
        memset(e, -1, sizeof(e));
        memset(e_ped, -1, sizeof(e_ped));
        memset(trigTower, 0, sizeof(trigTower));
        memset(fiberAdc, 0, sizeof(fiberAdc));
        memset(fiberE_ped, 0, sizeof(fiberE_ped));

        for(size_t idx=0; idx<ieta->size(); idx++){
            //cout<<"idx="<<idx<<",   "<<(*ieta)[idx]<<"  "<<(*iphi)[idx]<<"  "<<(*depth)[idx]<<"  "<<(*ampl)[idx]<<"  "<<(*energy_ped)[idx]<<endl;

            int etaIdx   = (*ieta)[idx]>0 ? (*ieta)[idx]-16 : (*ieta)[idx]+41;
            int phiIdx   = (*iphi)[idx]/2;
            int depthIdx = (*depth)[idx] - 1;
            int fiberIdx = ((*depth)[idx]-1) % 2 ; // 0 - long fiber; 1 - short fiber;

            adc[etaIdx][phiIdx][depthIdx]       = (*ampl)[idx];
            fiberAdc[etaIdx][phiIdx][fiberIdx] += (*ampl)[idx];

            e_ped[etaIdx][phiIdx][depthIdx]       = (*energy_ped)[idx];
            fiberE_ped[etaIdx][phiIdx][fiberIdx] += (*energy_ped)[idx];

            if(adc[etaIdx][phiIdx][depthIdx]>15) trigTower[etaIdx][phiIdx] = kTRUE;
        }

        for(int etaIdx=0; etaIdx<nMaxEta; etaIdx++){
            for(int phiIdx=0; phiIdx<nMaxPhi; phiIdx++){
                int towerIdx = etaIdx*nMaxPhi + phiIdx;

                for(int depthIdx=0; depthIdx<nMaxDepth; depthIdx++){
                    if(isNoise){
                        hAdcvsTower_Noise[depthIdx]->Fill(towerIdx, adc[etaIdx][phiIdx][depthIdx]);
                        hEpedvsTower_Noise[depthIdx]->Fill(towerIdx, e_ped[etaIdx][phiIdx][depthIdx]);
                    }

                    if(isCollision){
                        hAdcvsTower_Collision[depthIdx]->Fill(towerIdx, adc[etaIdx][phiIdx][depthIdx]);
                        hEpedvsTower_Collision[depthIdx]->Fill(towerIdx, e_ped[etaIdx][phiIdx][depthIdx]);
                    }
                }

                for(int fiberIdx=0; fiberIdx<nMaxFiber; fiberIdx++){
                    if(adc[etaIdx][phiIdx][0]<0){
                        fiberAdc[etaIdx][phiIdx][fiberIdx] = -1;
                        fiberE_ped[etaIdx][phiIdx][fiberIdx] = -1;
                    }

                    if(isNoise){
                        hFiberAdcvsTower_Noise[fiberIdx]->Fill(towerIdx, fiberAdc[etaIdx][phiIdx][fiberIdx]);
                        hFiberEpedvsTower_Noise[fiberIdx]->Fill(towerIdx, fiberE_ped[etaIdx][phiIdx][fiberIdx]);
                    }
                    if(isCollision){
                        hFiberAdcvsTower_Collision[fiberIdx]->Fill(towerIdx, fiberAdc[etaIdx][phiIdx][fiberIdx]);
                        hFiberEpedvsTower_Collision[fiberIdx]->Fill(towerIdx, fiberE_ped[etaIdx][phiIdx][fiberIdx]);
                    }
                }

                if(isNoise){
                    hAdcDep3vsDep1_Noise->Fill(adc[etaIdx][phiIdx][0], adc[etaIdx][phiIdx][2]);
                    hAdcDep4vsDep2_Noise->Fill(adc[etaIdx][phiIdx][1], adc[etaIdx][phiIdx][3]);
                    hEpedDep3vsDep1_Noise->Fill(e_ped[etaIdx][phiIdx][0], e_ped[etaIdx][phiIdx][2]);
                    hEpedDep4vsDep2_Noise->Fill(e_ped[etaIdx][phiIdx][1], e_ped[etaIdx][phiIdx][3]);

                    hAdcShortvsLong_Noise->Fill(fiberAdc[etaIdx][phiIdx][0], fiberAdc[etaIdx][phiIdx][1]);
                    hEpedShortvsLong_Noise->Fill(fiberE_ped[etaIdx][phiIdx][0], fiberE_ped[etaIdx][phiIdx][1]);

                    hAdcDep3Dep1Diff_Noise->Fill(adc[etaIdx][phiIdx][2] - adc[etaIdx][phiIdx][0]);
                    hAdcDep4Dep2Diff_Noise->Fill(adc[etaIdx][phiIdx][3] - adc[etaIdx][phiIdx][1]);
                    hEpedDep3Dep1Diff_Noise->Fill(e_ped[etaIdx][phiIdx][2] - e_ped[etaIdx][phiIdx][0]);
                    hEpedDep4Dep2Diff_Noise->Fill(e_ped[etaIdx][phiIdx][3] - e_ped[etaIdx][phiIdx][1]);

                    hAdcLongShortDiff_Noise->Fill(fiberAdc[etaIdx][phiIdx][0] - fiberAdc[etaIdx][phiIdx][1]);
                    hEpedLongShortDiff_Noise->Fill(fiberE_ped[etaIdx][phiIdx][0] - fiberE_ped[etaIdx][phiIdx][1]);

                    if(trigTower[etaIdx][phiIdx]){
                        hTrigAdcLongShortDiffvsCen_Noise->Fill(cenBin, fiberAdc[etaIdx][phiIdx][0] - fiberAdc[etaIdx][phiIdx][1]);
                        hTrigEpedLongShortDiffvsCen_Noise->Fill(cenBin, fiberE_ped[etaIdx][phiIdx][0] - fiberE_ped[etaIdx][phiIdx][1]);
                    }
                }

                if(isCollision){
                    hAdcDep3vsDep1_Collision->Fill(adc[etaIdx][phiIdx][0], adc[etaIdx][phiIdx][2]);
                    hAdcDep4vsDep2_Collision->Fill(adc[etaIdx][phiIdx][1], adc[etaIdx][phiIdx][3]);
                    hEpedDep3vsDep1_Collision->Fill(e_ped[etaIdx][phiIdx][0], e_ped[etaIdx][phiIdx][2]);
                    hEpedDep4vsDep2_Collision->Fill(e_ped[etaIdx][phiIdx][1], e_ped[etaIdx][phiIdx][3]);

                    hAdcShortvsLong_Collision->Fill(fiberAdc[etaIdx][phiIdx][0], fiberAdc[etaIdx][phiIdx][1]);
                    hEpedShortvsLong_Collision->Fill(fiberE_ped[etaIdx][phiIdx][0], fiberE_ped[etaIdx][phiIdx][1]);

                    hAdcDep3Dep1Diff_Collision->Fill(adc[etaIdx][phiIdx][2] - adc[etaIdx][phiIdx][0]);
                    hAdcDep4Dep2Diff_Collision->Fill(adc[etaIdx][phiIdx][3] - adc[etaIdx][phiIdx][1]);
                    hEpedDep3Dep1Diff_Collision->Fill(e_ped[etaIdx][phiIdx][2] - e_ped[etaIdx][phiIdx][0]);
                    hEpedDep4Dep2Diff_Collision->Fill(e_ped[etaIdx][phiIdx][3] - e_ped[etaIdx][phiIdx][1]);

                    hAdcLongShortDiff_Collision->Fill(fiberAdc[etaIdx][phiIdx][0] - fiberAdc[etaIdx][phiIdx][1]);
                    hEpedLongShortDiff_Collision->Fill(fiberE_ped[etaIdx][phiIdx][0] - fiberE_ped[etaIdx][phiIdx][1]);

                    if(trigTower[etaIdx][phiIdx]){
                        hTrigAdcLongShortDiffvsCen_Collision->Fill(cenBin, fiberAdc[etaIdx][phiIdx][0] - fiberAdc[etaIdx][phiIdx][1]);
                        hTrigEpedLongShortDiffvsCen_Collision->Fill(cenBin, fiberE_ped[etaIdx][phiIdx][0] - fiberE_ped[etaIdx][phiIdx][1]);
                    }
                }
            }
        }
    }

    cout<<"Total number of collision events: "<<nCollisions<<endl;

    TFile *fOut = new TFile(Form("%s.root", outputFile.Data()), "recreate");
    fOut->cd();
    for(int depthIdx=0; depthIdx<nMaxDepth; depthIdx++){
        hAdcvsTower_Noise[depthIdx]->Write();
        hEpedvsTower_Noise[depthIdx]->Write();
        hAdcvsTower_Collision[depthIdx]->Write();
        hEpedvsTower_Collision[depthIdx]->Write();
    }

    for(int fiberIdx=0; fiberIdx<nMaxFiber; fiberIdx++){
        hFiberAdcvsTower_Noise[fiberIdx]->Write();
        hFiberEpedvsTower_Noise[fiberIdx]->Write();
        hFiberAdcvsTower_Collision[fiberIdx]->Write();
        hFiberEpedvsTower_Collision[fiberIdx]->Write();
    }

    hHCalTPCompEtvsTower_Noise->Write();
    hHCalTPCompEtvsTower_Collision->Write();

    hAdcDep3vsDep1_Noise->Write();
    hAdcDep4vsDep2_Noise->Write();
    hEpedDep3vsDep1_Noise->Write();
    hEpedDep4vsDep2_Noise->Write();
    hAdcShortvsLong_Noise->Write();
    hEpedShortvsLong_Noise->Write();

    hAdcDep3Dep1Diff_Noise->Write();
    hAdcDep4Dep2Diff_Noise->Write();
    hEpedDep3Dep1Diff_Noise->Write();
    hEpedDep4Dep2Diff_Noise->Write();
    hAdcLongShortDiff_Noise->Write();
    hEpedLongShortDiff_Noise->Write();
    hTrigAdcLongShortDiffvsCen_Noise->Write();
    hTrigEpedLongShortDiffvsCen_Noise->Write();

    hAdcDep3vsDep1_Collision->Write();
    hAdcDep4vsDep2_Collision->Write();
    hEpedDep3vsDep1_Collision->Write();
    hEpedDep4vsDep2_Collision->Write();
    hAdcShortvsLong_Collision->Write();
    hEpedShortvsLong_Collision->Write();
    hTrigAdcLongShortDiffvsCen_Collision->Write();
    hTrigEpedLongShortDiffvsCen_Collision->Write();

    hAdcDep3Dep1Diff_Collision->Write();
    hAdcDep4Dep2Diff_Collision->Write();
    hEpedDep3Dep1Diff_Collision->Write();
    hEpedDep4Dep2Diff_Collision->Write();
    hAdcLongShortDiff_Collision->Write();
    hEpedLongShortDiff_Collision->Write();

    fOut->Close();

    return 0;
}
