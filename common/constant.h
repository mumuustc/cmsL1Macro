const Int_t nDirs = 2; // 0 - Plus; 1 - Minus;
const TString  mDir[nDirs] = {"Plus", "Minus"};

const Int_t nNeus = 4;
const Double_t mNeuZDCLow[nDirs][nNeus] = {
    {0, 4.2e3, 10.e3, 20.e3},
    {0, 6.0e3, 16.e3, 32.e3}
};
const Double_t mNeuZDCHi[nDirs][nNeus] = {
    {4.2e3, 10.e3, 20.e3, 5.e5},
    {6.0e3, 16.e3, 32.e3, 6.e5}
};

const Int_t nSces = 2;
TString sceName[nSces] = {"Collision", "Noise"};

const Int_t nEtThs = 5;
const Int_t mEtTh[nEtThs] = {1, 2, 3, 4, 5};

//const Int_t nHiHFAdcThs = 2;
//const Int_t mHiHFAdcTh[nHiHFAdcThs] = {15, 19};
//
//const Int_t nLowHFAdcThs = 3;
//const Int_t mLowHFAdcTh[nLowHFAdcThs] = {10, 12, 14};

const Int_t nHiHFAdcThs = 10;
const Int_t mHiHFAdcTh[nHiHFAdcThs] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

const Int_t nLowHFAdcThs = 10;
const Int_t mLowHFAdcTh[nLowHFAdcThs] = {9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

const Int_t nNeuThs = 3;
const Int_t mNeuTh[nNeuThs] = {1, 2, 3};
