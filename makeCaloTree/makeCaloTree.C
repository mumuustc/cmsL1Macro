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

#include "L1AnalysisEventDataFormat.h"
#include "L1AnalysisCaloTPDataFormat.h"
#include "L1AnalysisL1CaloTowerDataFormat.h"

int makeCaloTree(Int_t nEvts = 10, TString L1File="/eos/cms/store/group/phys_heavyions/shuaiy/L1Emulator/L1Ntuple/L1NtupleRAWEMUCalo_HFAdc_ZeroBias_run327211/HIForward/L1NtupleRAWEMUCalo_HFAdc_ZeroBias_run327211/210505_023909/0000/L1Ntuple_88.root", TString HiForestFileList="ZBEvtInfo_RunByRunList/327211.list", TString outputFile="EvtL1CaloInfo_test")
{
    TFile *fL1Ntuple = TFile::Open(L1File);
    TTree *mL1EventTree = (TTree *)fL1Ntuple->Get("l1EventTree/L1EventTree");
    TTree *mL1CaloTowerTree = (TTree *)fL1Ntuple->Get("l1CaloTowerTree/L1CaloTowerTree");
    TTree *mL1CaloTowerEmuTree = (TTree *)fL1Ntuple->Get("l1CaloTowerEmuTree/L1CaloTowerTree");
    TTree *mAdcTree = (TTree *)fL1Ntuple->Get("HFAdcana/adc");

    L1Analysis::L1AnalysisEventDataFormat *l1Event = new L1Analysis::L1AnalysisEventDataFormat();
    mL1EventTree->SetBranchAddress("Event", &l1Event);

    L1Analysis::L1AnalysisCaloTPDataFormat *l1CaloTP = new L1Analysis::L1AnalysisCaloTPDataFormat();
    mL1CaloTowerTree->SetBranchAddress("CaloTP", &l1CaloTP);

    L1Analysis::L1AnalysisL1CaloTowerDataFormat *l1CaloTower = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();
    mL1CaloTowerTree->SetBranchAddress("L1CaloTower", &l1CaloTower);

    L1Analysis::L1AnalysisCaloTPDataFormat *l1CaloTPEmu = new L1Analysis::L1AnalysisCaloTPDataFormat();
    mL1CaloTowerEmuTree->SetBranchAddress("CaloTP", &l1CaloTPEmu);

    L1Analysis::L1AnalysisL1CaloTowerDataFormat *l1CaloTowerEmu = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();
    mL1CaloTowerEmuTree->SetBranchAddress("L1CaloTower", &l1CaloTowerEmu);


    //for(Int_t i=0; i<25; i++){
    //    mL1EventTree->GetEntry(i);
    //    cout<<l1Event->run<<"   "<<l1Event->event<<"   "<<l1Event->lumi<<endl;
    //}
    //return -1;

    //for(Int_t i=0; i<20000; i++){
    //    mL1CaloTowerTree->GetEntry(i);
    //    if(l1CaloTP->nHCALTP>0)   cout<<l1CaloTP->nHCALTP<<"   "<<l1CaloTP->hcalTPieta.size()<<endl;
    //    if(l1CaloTower->nTower>0) cout<<l1CaloTower->nTower<<"   "<<l1CaloTower->ieta.size()<<endl;
    //}
    //return -1;

    std::vector<int> *ieta_HF;
    std::vector<int> *iphi_HF;
    std::vector<int> *depth_HF;
    std::vector<int> *ampl_HF;
    std::vector<float> *energy_ped_HF;
    std::vector<float> *energy_HF;

    ieta_HF = 0;
    iphi_HF = 0;
    depth_HF = 0;
    ampl_HF = 0;
    energy_ped_HF = 0;
    energy_HF = 0;

    mAdcTree->SetBranchStatus("*", 0);
    mAdcTree->SetBranchStatus("ieta",  1);
    mAdcTree->SetBranchStatus("iphi",  1);
    mAdcTree->SetBranchStatus("depth", 1);
    mAdcTree->SetBranchStatus("ampl",  1);
    mAdcTree->SetBranchStatus("energy",  1);
    mAdcTree->SetBranchStatus("energy_ped",  1);
    mAdcTree->SetBranchAddress("ieta",  &ieta_HF);
    mAdcTree->SetBranchAddress("iphi",  &iphi_HF);
    mAdcTree->SetBranchAddress("depth", &depth_HF);
    mAdcTree->SetBranchAddress("ampl",  &ampl_HF);
    mAdcTree->SetBranchAddress("energy_ped",  &energy_ped_HF);
    mAdcTree->SetBranchAddress("energy",  &energy_HF);

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
    const int nMaxTowers = 8000;

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

    //for(Int_t i=0; i<25; i++){
    //    mHiEvtChain->GetEntry(i);
    //    cout<<"Evt: "<<i<<"   "<<RunNb<<"   "<<EventNb<<"   "<<LSNb<<"   "<<centrality<<"   "<<trigHLT[9]<<"   "<<evtSel[4]<<endl;
    //}

    ////return -1;

    unsigned int mRunNb = 0;
    unsigned int mLSNb = 0;
    unsigned int mEventNb = 0;
    short        mCenBin = -1;
    float        mVx, mVy, mVz;
    float        mZDCPlus, mZDCMinus;
    int          mMaxL1HFAdcPlus, mMaxL1HFAdcMinus;
    int          mNpixel, mNtrkoffline, mNtrkHP;
    Bool_t       mTrigHLT[nMaxTrigHLTBits];
    Bool_t       mEvtSel[nMaxEvtSelBits];
    short        nHCALTPs;
    short        hcalTPieta[nMaxTowers], hcalTPiphi[nMaxTowers], hcalTPCaliphi[nMaxTowers], hcalTPcompEt[nMaxTowers];
    short        nECALTPs;
    short        ecalTPieta[nMaxTowers], ecalTPiphi[nMaxTowers], ecalTPCaliphi[nMaxTowers], ecalTPcompEt[nMaxTowers];
    short        nTowers;
    short        ieta[nMaxTowers], iphi[nMaxTowers], iet[nMaxTowers], iem[nMaxTowers], ihad[nMaxTowers];
    short        nHCALTPsEmu;
    short        hcalTPietaEmu[nMaxTowers], hcalTPiphiEmu[nMaxTowers], hcalTPCaliphiEmu[nMaxTowers], hcalTPcompEtEmu[nMaxTowers];
    short        nECALTPsEmu;
    short        ecalTPietaEmu[nMaxTowers], ecalTPiphiEmu[nMaxTowers], ecalTPCaliphiEmu[nMaxTowers], ecalTPcompEtEmu[nMaxTowers];
    short        nTowersEmu;
    short        ietaEmu[nMaxTowers], iphiEmu[nMaxTowers], ietEmu[nMaxTowers], iemEmu[nMaxTowers], ihadEmu[nMaxTowers];

    TTree *EvtTowerInfoNTuple = new TTree("EvtTowerInfoNTuple", "EvtTowerInfoNTuple");
    EvtTowerInfoNTuple->Branch("mRunNb", &mRunNb, "mRunNb/i");
    EvtTowerInfoNTuple->Branch("mLSNb", &mLSNb, "mLSNb/i");
    EvtTowerInfoNTuple->Branch("mEventNb", &mEventNb, "mEventNb/i");
    EvtTowerInfoNTuple->Branch("mCenBin", &mCenBin, "mCenBin/S");
    EvtTowerInfoNTuple->Branch("mVx", &mVx, "mVx/F");
    EvtTowerInfoNTuple->Branch("mVy", &mVy, "mVy/F");
    EvtTowerInfoNTuple->Branch("mVz", &mVz, "mVz/F");
    EvtTowerInfoNTuple->Branch("mZDCPlus", &mZDCPlus, "mZDCPlus/F");
    EvtTowerInfoNTuple->Branch("mZDCMinus", &mZDCMinus, "mZDCMinus/F");
    EvtTowerInfoNTuple->Branch("mMaxL1HFAdcPlus", &mMaxL1HFAdcPlus, "mMaxL1HFAdcPlus/I");
    EvtTowerInfoNTuple->Branch("mMaxL1HFAdcMinus", &mMaxL1HFAdcMinus, "mMaxL1HFAdcMinus/I");
    EvtTowerInfoNTuple->Branch("mNpixel", &mNpixel, "mNpixel/I");
    EvtTowerInfoNTuple->Branch("mNtrkoffline", &mNtrkoffline, "mNtrkoffline/I");
    EvtTowerInfoNTuple->Branch("mNtrkHP", &mNtrkHP, "mNtrkHP/I");
    EvtTowerInfoNTuple->Branch("mTrigHLT", mTrigHLT, Form("mTrigHLT[%d]/O", nMaxTrigHLTBits));
    EvtTowerInfoNTuple->Branch("mEvtSel", mEvtSel, Form("mEvtSel[%d]/O", nMaxEvtSelBits));
    EvtTowerInfoNTuple->Branch("nHCALTPs", &nHCALTPs, "nHCALTPs/S");
    EvtTowerInfoNTuple->Branch("hcalTPieta", hcalTPieta, "hcalTPieta[nHCALTPs]/S");
    EvtTowerInfoNTuple->Branch("hcalTPiphi", hcalTPiphi, "hcalTPiphi[nHCALTPs]/S");
    EvtTowerInfoNTuple->Branch("hcalTPCaliphi", hcalTPCaliphi, "hcalTPCaliphi[nHCALTPs]/S");
    EvtTowerInfoNTuple->Branch("hcalTPcompEt", hcalTPcompEt, "hcalTPcompEt[nHCALTPs]/S");
    EvtTowerInfoNTuple->Branch("nECALTPs", &nECALTPs, "nECALTPs/S");
    EvtTowerInfoNTuple->Branch("ecalTPieta", ecalTPieta, "ecalTPieta[nECALTPs]/S");
    EvtTowerInfoNTuple->Branch("ecalTPiphi", ecalTPiphi, "ecalTPiphi[nECALTPs]/S");
    EvtTowerInfoNTuple->Branch("ecalTPCaliphi", ecalTPCaliphi, "ecalTPCaliphi[nECALTPs]/S");
    EvtTowerInfoNTuple->Branch("ecalTPcompEt", ecalTPcompEt, "ecalTPcompEt[nECALTPs]/S");
    EvtTowerInfoNTuple->Branch("nTowers", &nTowers, "nTowers/S");
    EvtTowerInfoNTuple->Branch("ieta", ieta, "ieta[nTowers]/S");
    EvtTowerInfoNTuple->Branch("iphi", iphi, "iphi[nTowers]/S");
    EvtTowerInfoNTuple->Branch("iet", iet, "iet[nTowers]/S");
    EvtTowerInfoNTuple->Branch("iem", iem, "iem[nTowers]/S");
    EvtTowerInfoNTuple->Branch("ihad", ihad, "ihad[nTowers]/S");
    EvtTowerInfoNTuple->Branch("nHCALTPsEmu", &nHCALTPsEmu, "nHCALTPsEmu/S");
    EvtTowerInfoNTuple->Branch("hcalTPietaEmu", hcalTPietaEmu, "hcalTPietaEmu[nHCALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("hcalTPiphiEmu", hcalTPiphiEmu, "hcalTPiphiEmu[nHCALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("hcalTPCaliphiEmu", hcalTPCaliphiEmu, "hcalTPCaliphiEmu[nHCALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("hcalTPcompEtEmu", hcalTPcompEtEmu, "hcalTPcompEtEmu[nHCALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("nECALTPsEmu", &nECALTPsEmu, "nECALTPsEmu/S");
    EvtTowerInfoNTuple->Branch("ecalTPietaEmu", ecalTPietaEmu, "ecalTPietaEmu[nECALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("ecalTPiphiEmu", ecalTPiphiEmu, "ecalTPiphiEmu[nECALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("ecalTPCaliphiEmu", ecalTPCaliphiEmu, "ecalTPCaliphiEmu[nECALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("ecalTPcompEtEmu", ecalTPcompEtEmu, "ecalTPcompEtEmu[nECALTPsEmu]/S");
    EvtTowerInfoNTuple->Branch("nTowersEmu", &nTowersEmu, "nTowersEmu/S");
    EvtTowerInfoNTuple->Branch("ietaEmu", ietaEmu, "ietaEmu[nTowersEmu]/S");
    EvtTowerInfoNTuple->Branch("iphiEmu", iphiEmu, "iphiEmu[nTowersEmu]/S");
    EvtTowerInfoNTuple->Branch("ietEmu", ietEmu, "ietEmu[nTowersEmu]/S");
    EvtTowerInfoNTuple->Branch("iemEmu", iemEmu, "iemEmu[nTowersEmu]/S");
    EvtTowerInfoNTuple->Branch("ihadEmu", ihadEmu, "ihadEmu[nTowersEmu]/S");

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
        mL1CaloTowerTree->GetEntry(ievt);
        mL1CaloTowerEmuTree->GetEntry(ievt);
        mAdcTree->GetEntry(ievt);

        memset(hcalTPieta, -1, sizeof(hcalTPieta));
        memset(hcalTPiphi, -1, sizeof(hcalTPiphi));
        memset(hcalTPCaliphi, -1, sizeof(hcalTPCaliphi));
        memset(hcalTPcompEt, -1, sizeof(hcalTPcompEt));
        memset(ecalTPieta, -1, sizeof(ecalTPieta));
        memset(ecalTPiphi, -1, sizeof(ecalTPiphi));
        memset(ecalTPCaliphi, -1, sizeof(ecalTPCaliphi));
        memset(ecalTPcompEt, -1, sizeof(ecalTPcompEt));
        memset(ieta, -1, sizeof(ieta));
        memset(iphi, -1, sizeof(iphi));
        memset(iet,  -1, sizeof(iet));
        memset(iem,  -1, sizeof(iem));
        memset(ihad,  -1, sizeof(ihad));
        memset(hcalTPietaEmu, -1, sizeof(hcalTPietaEmu));
        memset(hcalTPiphiEmu, -1, sizeof(hcalTPiphiEmu));
        memset(hcalTPCaliphiEmu, -1, sizeof(hcalTPCaliphiEmu));
        memset(hcalTPcompEtEmu, -1, sizeof(hcalTPcompEtEmu));
        memset(ecalTPietaEmu, -1, sizeof(ecalTPietaEmu));
        memset(ecalTPiphiEmu, -1, sizeof(ecalTPiphiEmu));
        memset(ecalTPCaliphiEmu, -1, sizeof(ecalTPCaliphiEmu));
        memset(ecalTPcompEtEmu, -1, sizeof(ecalTPcompEtEmu));
        memset(ietaEmu, -1, sizeof(ietaEmu));
        memset(iphiEmu, -1, sizeof(iphiEmu));
        memset(ietEmu,  -1, sizeof(ietEmu));
        memset(iemEmu,  -1, sizeof(iemEmu));
        memset(ihadEmu,  -1, sizeof(ihadEmu));

        Bool_t isMatch = kFALSE;
        for(Int_t jevt=0; jevt<mHiEvtChain->GetEntries(); jevt++){
            mHiEvtChain->GetEntry(jevt);

            if(l1Event->run == RunNb && l1Event->event == EventNb && l1Event->lumi == LSNb){
                isMatch = kTRUE;

                mRunNb   = RunNb;
                mLSNb    = LSNb;
                mEventNb = EventNb;
                mCenBin  = centrality;
                mVx      = bestvtxX;
                mVy      = bestvtxY;
                mVz      = bestvtxZ;
                mZDCPlus = ZDCPlus;
                mZDCMinus = ZDCMinus;
                mNpixel   = Npixel;
                mNtrkoffline = Ntrkoffline;
                mNtrkHP      = NtrkHP;

                for(Int_t itrig=0; itrig<nMaxTrigHLTBits; itrig++) 
                    mTrigHLT[itrig] = trigHLT[itrig];
                for(Int_t ifilter=0; ifilter<nMaxEvtSelBits; ifilter++) 
                    mEvtSel[ifilter] = evtSel[ifilter];

                mMaxL1HFAdcPlus  = -999;
                mMaxL1HFAdcMinus = -999;
                for(size_t idx=0; idx<ieta_HF->size(); idx++){
                    //cout<<"idx="<<idx<<":   "<<(*ieta_HF)[idx]<<"  "<<(*iphi_HF)[idx]<<"  "<<(*depth_HF)[idx]<<"  "<<(*ampl_HF)[idx]<<"  "<<(*energy_ped_HF)[idx]<<endl;

                    if((*ieta_HF)[idx]>0 && mMaxL1HFAdcPlus<(*ampl_HF)[idx]){
                        mMaxL1HFAdcPlus = (*ampl_HF)[idx];
                    }

                    if((*ieta_HF)[idx]<0 && mMaxL1HFAdcMinus<(*ampl_HF)[idx]){
                        mMaxL1HFAdcMinus = (*ampl_HF)[idx];
                    }
                }
                //cout<<"mMaxL1HFAdcPlus: "<<mMaxL1HFAdcPlus<<"      mMaxL1HFAdcMinus: "<<mMaxL1HFAdcMinus<<endl;

                nHCALTPs = l1CaloTP->nHCALTP;
                for(Int_t idx=0; idx<nHCALTPs; idx++){
                    hcalTPieta[idx]    = l1CaloTP->hcalTPieta[idx];
                    hcalTPiphi[idx]    = l1CaloTP->hcalTPiphi[idx];
                    hcalTPCaliphi[idx] = l1CaloTP->hcalTPCaliphi[idx];
                    hcalTPcompEt[idx]  = l1CaloTP->hcalTPcompEt[idx];
                }

                nECALTPs = l1CaloTP->nECALTP;
                for(Int_t idx=0; idx<nECALTPs; idx++){
                    ecalTPieta[idx]    = l1CaloTP->ecalTPieta[idx];
                    ecalTPiphi[idx]    = l1CaloTP->ecalTPiphi[idx];
                    ecalTPCaliphi[idx] = l1CaloTP->ecalTPCaliphi[idx];
                    ecalTPcompEt[idx]  = l1CaloTP->ecalTPcompEt[idx];
                }

                nTowers = l1CaloTower->nTower;
                for(Int_t idx=0; idx<nTowers; idx++){
                    ieta[idx] = l1CaloTower->ieta[idx];
                    iphi[idx] = l1CaloTower->iphi[idx];
                    iet[idx]  = l1CaloTower->iet[idx];
                    iem[idx]  = l1CaloTower->iem[idx];
                    ihad[idx] = l1CaloTower->ihad[idx];
                }

                nHCALTPsEmu = l1CaloTPEmu->nHCALTP;
                for(Int_t idx=0; idx<nHCALTPsEmu; idx++){
                    hcalTPietaEmu[idx]    = l1CaloTPEmu->hcalTPieta[idx];
                    hcalTPiphiEmu[idx]    = l1CaloTPEmu->hcalTPiphi[idx];
                    hcalTPCaliphiEmu[idx] = l1CaloTPEmu->hcalTPCaliphi[idx];
                    hcalTPcompEtEmu[idx]  = l1CaloTPEmu->hcalTPcompEt[idx];
                }

                nECALTPsEmu = l1CaloTPEmu->nECALTP;
                for(Int_t idx=0; idx<nECALTPsEmu; idx++){
                    ecalTPietaEmu[idx]    = l1CaloTPEmu->ecalTPieta[idx];
                    ecalTPiphiEmu[idx]    = l1CaloTPEmu->ecalTPiphi[idx];
                    ecalTPCaliphiEmu[idx] = l1CaloTPEmu->ecalTPCaliphi[idx];
                    ecalTPcompEtEmu[idx]  = l1CaloTPEmu->ecalTPcompEt[idx];
                }

                nTowersEmu = l1CaloTowerEmu->nTower;
                for(Int_t idx=0; idx<nTowersEmu; idx++){
                    ietaEmu[idx] = l1CaloTowerEmu->ieta[idx];
                    iphiEmu[idx] = l1CaloTowerEmu->iphi[idx];
                    ietEmu[idx]  = l1CaloTowerEmu->iet[idx];
                    iemEmu[idx]  = l1CaloTowerEmu->iem[idx];
                    ihadEmu[idx] = l1CaloTowerEmu->ihad[idx];
                }

                EvtTowerInfoNTuple->Fill();

                break;
            }
        }

        if(!isMatch) cout<<"mL1EventTree - Entry ID:"<<ievt<<"   runNum:"<<l1Event->run<<"   evtNb:"<<l1Event->event<<"   lSNb:"<<l1Event->lumi<<"  does not match ZB event info !!!"<<endl;

    }

    TFile *fOut = new TFile(Form("%s.root", outputFile.Data()), "recreate");
    fOut->cd();
    EvtTowerInfoNTuple->Write();
    fOut->Close();

    return 0;
}
