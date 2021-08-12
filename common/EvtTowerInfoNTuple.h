//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May  5 16:44:26 2021 by ROOT version 6.22/08
// from TTree EvtTowerInfoNTuple/EvtTowerInfoNTuple
// found on file: output/EvtL1CaloInfo_327238_64.root
//////////////////////////////////////////////////////////

#ifndef EvtTowerInfoNTuple_h
#define EvtTowerInfoNTuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class EvtTowerInfoNTuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          mRunNb;
   UInt_t          mLSNb;
   UInt_t          mEventNb;
   Short_t         mCenBin;
   Float_t         mVx;
   Float_t         mVy;
   Float_t         mVz;
   Float_t         mZDCPlus;
   Float_t         mZDCMinus;
   Int_t           mMaxL1HFAdcPlus;
   Int_t           mMaxL1HFAdcMinus;
   Int_t           mNpixel;
   Int_t           mNtrkoffline;
   Int_t           mNtrkHP;
   Bool_t          mTrigHLT[12];
   Bool_t          mEvtSel[18];
   Short_t         nHCALTPs;
   Short_t         hcalTPieta[8000];   //[nHCALTPs]
   Short_t         hcalTPiphi[8000];   //[nHCALTPs]
   Short_t         hcalTPCaliphi[8000];   //[nHCALTPs]
   Short_t         hcalTPcompEt[8000];   //[nHCALTPs]
   Short_t         nECALTPs;
   Short_t         ecalTPieta[8000];   //[nECALTPs]
   Short_t         ecalTPiphi[8000];   //[nECALTPs]
   Short_t         ecalTPCaliphi[8000];   //[nECALTPs]
   Short_t         ecalTPcompEt[8000];   //[nECALTPs]
   Short_t         nTowers;
   Short_t         ieta[8000];   //[nTowers]
   Short_t         iphi[8000];   //[nTowers]
   Short_t         iet[8000];   //[nTowers]
   Short_t         iem[8000];   //[nTowers]
   Short_t         ihad[8000];   //[nTowers]
   Short_t         nHCALTPsEmu;
   Short_t         hcalTPietaEmu[8000];   //[nHCALTPsEmu]
   Short_t         hcalTPiphiEmu[8000];   //[nHCALTPsEmu]
   Short_t         hcalTPCaliphiEmu[8000];   //[nHCALTPsEmu]
   Short_t         hcalTPcompEtEmu[8000];   //[nHCALTPsEmu]
   Short_t         nECALTPsEmu;
   Short_t         ecalTPietaEmu[8000];   //[nECALTPsEmu]
   Short_t         ecalTPiphiEmu[8000];   //[nECALTPsEmu]
   Short_t         ecalTPCaliphiEmu[8000];   //[nECALTPsEmu]
   Short_t         ecalTPcompEtEmu[8000];   //[nECALTPsEmu]
   Short_t         nTowersEmu;
   Short_t         ietaEmu[8000];   //[nTowersEmu]
   Short_t         iphiEmu[8000];   //[nTowersEmu]
   Short_t         ietEmu[8000];   //[nTowersEmu]
   Short_t         iemEmu[8000];   //[nTowersEmu]
   Short_t         ihadEmu[8000];   //[nTowersEmu]

   // List of branches
   TBranch        *b_mRunNb;   //!
   TBranch        *b_mLSNb;   //!
   TBranch        *b_mEventNb;   //!
   TBranch        *b_mCenBin;   //!
   TBranch        *b_mVx;   //!
   TBranch        *b_mVy;   //!
   TBranch        *b_mVz;   //!
   TBranch        *b_mZDCPlus;   //!
   TBranch        *b_mZDCMinus;   //!
   TBranch        *b_mMaxL1HFAdcPlus;   //!
   TBranch        *b_mMaxL1HFAdcMinus;   //!
   TBranch        *b_mNpixel;   //!
   TBranch        *b_mNtrkoffline;   //!
   TBranch        *b_mNtrkHP;   //!
   TBranch        *b_mTrigHLT;   //!
   TBranch        *b_mEvtSel;   //!
   TBranch        *b_nHCALTPs;   //!
   TBranch        *b_hcalTPieta;   //!
   TBranch        *b_hcalTPiphi;   //!
   TBranch        *b_hcalTPCaliphi;   //!
   TBranch        *b_hcalTPcompEt;   //!
   TBranch        *b_nECALTPs;   //!
   TBranch        *b_ecalTPieta;   //!
   TBranch        *b_ecalTPiphi;   //!
   TBranch        *b_ecalTPCaliphi;   //!
   TBranch        *b_ecalTPcompEt;   //!
   TBranch        *b_nTowers;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_iet;   //!
   TBranch        *b_iem;   //!
   TBranch        *b_ihad;   //!
   TBranch        *b_nHCALTPsEmu;   //!
   TBranch        *b_hcalTPietaEmu;   //!
   TBranch        *b_hcalTPiphiEmu;   //!
   TBranch        *b_hcalTPCaliphiEmu;   //!
   TBranch        *b_hcalTPcompEtEmu;   //!
   TBranch        *b_nECALTPsEmu;   //!
   TBranch        *b_ecalTPietaEmu;   //!
   TBranch        *b_ecalTPiphiEmu;   //!
   TBranch        *b_ecalTPCaliphiEmu;   //!
   TBranch        *b_ecalTPcompEtEmu;   //!
   TBranch        *b_nTowersEmu;   //!
   TBranch        *b_ietaEmu;   //!
   TBranch        *b_iphiEmu;   //!
   TBranch        *b_ietEmu;   //!
   TBranch        *b_iemEmu;   //!
   TBranch        *b_ihadEmu;   //!

   EvtTowerInfoNTuple(TTree *tree=0);
   virtual ~EvtTowerInfoNTuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EvtTowerInfoNTuple_cxx
EvtTowerInfoNTuple::EvtTowerInfoNTuple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output/EvtL1CaloInfo_327238_64.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("output/EvtL1CaloInfo_327238_64.root");
      }
      f->GetObject("EvtTowerInfoNTuple",tree);

   }
   Init(tree);
}

EvtTowerInfoNTuple::~EvtTowerInfoNTuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EvtTowerInfoNTuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EvtTowerInfoNTuple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EvtTowerInfoNTuple::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("mRunNb", &mRunNb, &b_mRunNb);
   fChain->SetBranchAddress("mLSNb", &mLSNb, &b_mLSNb);
   fChain->SetBranchAddress("mEventNb", &mEventNb, &b_mEventNb);
   fChain->SetBranchAddress("mCenBin", &mCenBin, &b_mCenBin);
   fChain->SetBranchAddress("mVx", &mVx, &b_mVx);
   fChain->SetBranchAddress("mVy", &mVy, &b_mVy);
   fChain->SetBranchAddress("mVz", &mVz, &b_mVz);
   fChain->SetBranchAddress("mZDCPlus", &mZDCPlus, &b_mZDCPlus);
   fChain->SetBranchAddress("mZDCMinus", &mZDCMinus, &b_mZDCMinus);
   fChain->SetBranchAddress("mMaxL1HFAdcPlus", &mMaxL1HFAdcPlus, &b_mMaxL1HFAdcPlus);
   fChain->SetBranchAddress("mMaxL1HFAdcMinus", &mMaxL1HFAdcMinus, &b_mMaxL1HFAdcMinus);
   fChain->SetBranchAddress("mNpixel", &mNpixel, &b_mNpixel);
   fChain->SetBranchAddress("mNtrkoffline", &mNtrkoffline, &b_mNtrkoffline);
   fChain->SetBranchAddress("mNtrkHP", &mNtrkHP, &b_mNtrkHP);
   fChain->SetBranchAddress("mTrigHLT", mTrigHLT, &b_mTrigHLT);
   fChain->SetBranchAddress("mEvtSel", mEvtSel, &b_mEvtSel);
   fChain->SetBranchAddress("nHCALTPs", &nHCALTPs, &b_nHCALTPs);
   fChain->SetBranchAddress("hcalTPieta", hcalTPieta, &b_hcalTPieta);
   fChain->SetBranchAddress("hcalTPiphi", hcalTPiphi, &b_hcalTPiphi);
   fChain->SetBranchAddress("hcalTPCaliphi", hcalTPCaliphi, &b_hcalTPCaliphi);
   fChain->SetBranchAddress("hcalTPcompEt", hcalTPcompEt, &b_hcalTPcompEt);
   fChain->SetBranchAddress("nECALTPs", &nECALTPs, &b_nECALTPs);
   fChain->SetBranchAddress("ecalTPieta", ecalTPieta, &b_ecalTPieta);
   fChain->SetBranchAddress("ecalTPiphi", ecalTPiphi, &b_ecalTPiphi);
   fChain->SetBranchAddress("ecalTPCaliphi", ecalTPCaliphi, &b_ecalTPCaliphi);
   fChain->SetBranchAddress("ecalTPcompEt", ecalTPcompEt, &b_ecalTPcompEt);
   fChain->SetBranchAddress("nTowers", &nTowers, &b_nTowers);
   fChain->SetBranchAddress("ieta", ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", iphi, &b_iphi);
   fChain->SetBranchAddress("iet", iet, &b_iet);
   fChain->SetBranchAddress("iem", iem, &b_iem);
   fChain->SetBranchAddress("ihad", ihad, &b_ihad);
   fChain->SetBranchAddress("nHCALTPsEmu", &nHCALTPsEmu, &b_nHCALTPsEmu);
   fChain->SetBranchAddress("hcalTPietaEmu", hcalTPietaEmu, &b_hcalTPietaEmu);
   fChain->SetBranchAddress("hcalTPiphiEmu", hcalTPiphiEmu, &b_hcalTPiphiEmu);
   fChain->SetBranchAddress("hcalTPCaliphiEmu", hcalTPCaliphiEmu, &b_hcalTPCaliphiEmu);
   fChain->SetBranchAddress("hcalTPcompEtEmu", hcalTPcompEtEmu, &b_hcalTPcompEtEmu);
   fChain->SetBranchAddress("nECALTPsEmu", &nECALTPsEmu, &b_nECALTPsEmu);
   fChain->SetBranchAddress("ecalTPietaEmu", ecalTPietaEmu, &b_ecalTPietaEmu);
   fChain->SetBranchAddress("ecalTPiphiEmu", ecalTPiphiEmu, &b_ecalTPiphiEmu);
   fChain->SetBranchAddress("ecalTPCaliphiEmu", ecalTPCaliphiEmu, &b_ecalTPCaliphiEmu);
   fChain->SetBranchAddress("ecalTPcompEtEmu", ecalTPcompEtEmu, &b_ecalTPcompEtEmu);
   fChain->SetBranchAddress("nTowersEmu", &nTowersEmu, &b_nTowersEmu);
   fChain->SetBranchAddress("ietaEmu", ietaEmu, &b_ietaEmu);
   fChain->SetBranchAddress("iphiEmu", iphiEmu, &b_iphiEmu);
   fChain->SetBranchAddress("ietEmu", ietEmu, &b_ietEmu);
   fChain->SetBranchAddress("iemEmu", iemEmu, &b_iemEmu);
   fChain->SetBranchAddress("ihadEmu", ihadEmu, &b_ihadEmu);
   Notify();
}

Bool_t EvtTowerInfoNTuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EvtTowerInfoNTuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EvtTowerInfoNTuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EvtTowerInfoNTuple_cxx
