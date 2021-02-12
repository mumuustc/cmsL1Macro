#include "headers.h"

#include "L1AnalysisEventDataFormat.h"
#include "L1AnalysisCaloTPDataFormat.h"
#include "L1AnalysisL1CaloTowerDataFormat.h"

int makeCaloTree(TString L1File="/eos/cms/store/group/phys_heavyions/shuaiy/L1Emulator/L1Ntuple/L1NtupleRAWEMUCalo_ZeroBias_run326942/HIForward/L1NtupleRAWEMUCalo_ZeroBias_run326942/201217_001301/0000/L1Ntuple_1.root", TString HiForestFileList="ZBEvtInfo_RunByRunList/326942.list", TString outputFile="EvtL1CaloInfo_test")
{
    TFile *fL1Ntuple = TFile::Open(L1File);
    TTree *mL1EventTree = (TTree *)fL1Ntuple->Get("l1EventTree/L1EventTree");
    TTree *mL1CaloTowerTree = (TTree *)fL1Ntuple->Get("l1CaloTowerTree/L1CaloTowerTree");
    TTree *mL1CaloTowerEmuTree = (TTree *)fL1Ntuple->Get("l1CaloTowerEmuTree/L1CaloTowerTree");

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
    int          mNpixel, mNtrkoffline, mNtrkHP;
    Bool_t       mTrigHLT[nMaxTrigHLTBits];
    Bool_t       mEvtSel[nMaxEvtSelBits];
    int          nTowers;
    short        ieta[nMaxTowers], iphi[nMaxTowers], iet[nMaxTowers];
    int          nTowersEmu;
    short        ietaEmu[nMaxTowers], iphiEmu[nMaxTowers], ietEmu[nMaxTowers];

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
    EvtTowerInfoNTuple->Branch("mNpixel", &mNpixel, "mNpixel/I");
    EvtTowerInfoNTuple->Branch("mNtrkoffline", &mNtrkoffline, "mNtrkoffline/I");
    EvtTowerInfoNTuple->Branch("mNtrkHP", &mNtrkHP, "mNtrkHP/I");
    EvtTowerInfoNTuple->Branch("mTrigHLT", mTrigHLT, Form("mTrigHLT[%d]/O", nMaxTrigHLTBits));
    EvtTowerInfoNTuple->Branch("mEvtSel", mEvtSel, Form("mEvtSel[%d]/O", nMaxEvtSelBits));
    EvtTowerInfoNTuple->Branch("nTowers", &nTowers, "nTowers/I");
    EvtTowerInfoNTuple->Branch("ieta", ieta, "ieta[nTowers]/S");
    EvtTowerInfoNTuple->Branch("iphi", iphi, "iphi[nTowers]/S");
    EvtTowerInfoNTuple->Branch("iet", iet, "iet[nTowers]/S");
    EvtTowerInfoNTuple->Branch("nTowersEmu", &nTowersEmu, "nTowersEmu/I");
    EvtTowerInfoNTuple->Branch("ietaEmu", ietaEmu, "ietaEmu[nTowersEmu]/S");
    EvtTowerInfoNTuple->Branch("iphiEmu", iphiEmu, "iphiEmu[nTowersEmu]/S");
    EvtTowerInfoNTuple->Branch("ietEmu", ietEmu, "ietEmu[nTowersEmu]/S");

    //int nEvts = 150;
    int nEvts = mL1EventTree->GetEntries();
    cout<<"Total Entries: "<<nEvts<<endl;
    for(int ievt=0; ievt<nEvts; ievt++){
        //if(nEvts<100){
        //    cout << "begin " << ievt << "th entry...." << endl;
        //}
        //else if(ievt % (nEvts / 100) == 0){
        //    cout << "begin " << ievt << "th entry...." << endl;
        //}
        if(ievt%100 == 0) cout << "begin " << ievt << "th entry...." << endl;

        mL1EventTree->GetEntry(ievt);
        mL1CaloTowerTree->GetEntry(ievt);
        mL1CaloTowerEmuTree->GetEntry(ievt);

        memset(ieta, -1, sizeof(ieta));
        memset(iphi, -1, sizeof(iphi));
        memset(iet,  -1, sizeof(iet));
        memset(ietaEmu, -1, sizeof(ietaEmu));
        memset(iphiEmu, -1, sizeof(iphiEmu));
        memset(ietEmu,  -1, sizeof(ietEmu));

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

                nTowers = l1CaloTower->nTower;
                for(Int_t idx=0; idx<nTowers; idx++){
                    ieta[idx] = l1CaloTower->ieta[idx];
                    iphi[idx] = l1CaloTower->iphi[idx];
                    iet[idx]  = l1CaloTower->iet[idx];
                }

                nTowersEmu = l1CaloTowerEmu->nTower;
                for(Int_t idx=0; idx<nTowersEmu; idx++){
                    ietaEmu[idx] = l1CaloTowerEmu->ieta[idx];
                    iphiEmu[idx] = l1CaloTowerEmu->iphi[idx];
                    ietEmu[idx]  = l1CaloTowerEmu->iet[idx];
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
