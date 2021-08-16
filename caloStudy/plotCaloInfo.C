#include "../common/headers.h"
#include "../common/function.C"
#include "../common/constant.h"

const Bool_t  mStorePDF = kFALSE;

const Double_t mTinyNum = 1.e-6;
const Double_t mOffSet = 0.1;

const Int_t mMaxColors = 4;

const Int_t mFont = 42;

const Int_t    mMStyle[nSces] = {20, 24};
const Double_t mMSize[nSces] = {1.0, 1.0};
const Int_t    mColor[nSces] = {2, 1};

void plotCaloInfo(TString inputFileName="caloInfo.test.histo")
{
    gStyle->SetOptFit(1111);
    gStyle->SetLabelOffset(0.001,"z");

    TFile *f = TFile::Open(Form("%s.root", inputFileName.Data()));

    TString dirName  = "caloPlots";
    system(Form("mkdir -p %s", dirName.Data()));
    system(Form("rm -rf %s/*", dirName.Data()));

    TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
    setPad(0.12, 0.12, 0.07, 0.13);

    Int_t nColumns = 2;
    Int_t nRaws = 1;
    Int_t nPads = nColumns * nRaws;
    TCanvas* c2 = new TCanvas("c2", "c2", 1200, 450);
    c2->Divide(nColumns, nRaws);
    for(Int_t ipad=0; ipad<nPads; ipad++){
        c2->cd(ipad+1);
        setPad(0.12, 0.12, 0.08, 0.13);
    }

    nColumns = 2;
    nRaws = 2;
    nPads = nColumns * nRaws;
    TCanvas* c3 = new TCanvas("c3", "c3", 1200, 900);
    c3->Divide(nColumns, nRaws);
    for(Int_t ipad=0; ipad<nPads; ipad++){
        c3->cd(ipad+1);
        setPad(0.12, 0.12, 0.08, 0.13);
        gPad->Clear();
    }

    TH1D *hnEvts = (TH1D *)f->Get("hnEvts");
    TH1D *hCentrality_OfficialMB        = (TH1D *)f->Get("hCentrality_OfficialMB");
    TH1D *hCentrality_OfficialMB_ZDCAnd = (TH1D *)f->Get("hCentrality_OfficialMB_ZDCAnd");
    TH2D *hNtrkOfflineVsNtrkHP = (TH2D *)f->Get("hNtrkOfflineVsNtrkHP_Collision");

    TH2D *hIphiVsIeta_Emu[nSces];
    TH2D *hIphiVsIeta_Emu_ECal[nSces];
    TH2D *hIphiVsIeta_Emu_HCal[nSces];

    TH2D *hnTowersVsnEmuTowers = (TH2D *)f->Get("hnTowersVsnEmuTowers");
    TH2D *hMaxEtVsMaxEmuEt = (TH2D *)f->Get("hMaxEtVsMaxEmuEt");
    TH2D *hTotEtVsTotEmuEt = (TH2D *)f->Get("hTotEtVsTotEmuEt");

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
    for(Int_t i=0; i<nSces; i++){
        hIphiVsIeta_Emu[i] = (TH2D *)f->Get(Form("hIphiVsIeta_Emu_%s", sceName[i].Data()));
        hIphiVsIeta_Emu_ECal[i] = (TH2D *)f->Get(Form("hIphiVsIeta_Emu_ECal_%s", sceName[i].Data()));
        hIphiVsIeta_Emu_HCal[i] = (TH2D *)f->Get(Form("hIphiVsIeta_Emu_HCal_%s", sceName[i].Data()));

        hEmuEtVsTower[i] = (TProfile2D *)f->Get(Form("hEmuEtVsTower_%s", sceName[i].Data()));
        hEmuEtVsTowerIdx[i] = (TH2D *)f->Get(Form("hEmuEtVsTowerIdx_%s", sceName[i].Data()));
        hEmuEmEtVsTowerIdx[i] = (TH2D *)f->Get(Form("hEmuEmEtVsTowerIdx_%s", sceName[i].Data()));
        hEmuHadEtVsTowerIdx[i] = (TH2D *)f->Get(Form("hEmuHadEtVsTowerIdx_%s", sceName[i].Data()));
        hEmuEtVsTowerIdxVsCen[i] = (TH3D *)f->Get(Form("hEmuEtVsTowerIdxVsCen_%s", sceName[i].Data()));
        hEmuEmEtVsTowerIdxVsCen[i] = (TH3D *)f->Get(Form("hEmuEmEtVsTowerIdxVsCen_%s", sceName[i].Data()));
        hEmuHadEtVsTowerIdxVsCen[i] = (TH3D *)f->Get(Form("hEmuHadEtVsTowerIdxVsCen_%s", sceName[i].Data()));
        hMaxEmuEtVsCen[i] = (TH2D *)f->Get(Form("hMaxEmuEtVsCen_%s", sceName[i].Data()));
        hTotEmuEtVsCen[i] = (TH2D *)f->Get(Form("hTotEmuEtVsCen_%s", sceName[i].Data()));
        hTotEmuEmEtVsCen[i] = (TH2D *)f->Get(Form("hTotEmuEmEtVsCen_%s", sceName[i].Data()));
        hTotEmuHadEtVsCen[i] = (TH2D *)f->Get(Form("hTotEmuHadEtVsCen_%s", sceName[i].Data()));
        hTotEmuEmEtVsTotEmuHadEtVsCen[i] = (TH3D *)f->Get(Form("hTotEmuEmEtVsTotEmuHadEtVsCen_%s", sceName[i].Data()));
        hTotBarrelEmuEtVsCen[i] = (TH2D *)f->Get(Form("hTotBarrelEmuEtVsCen_%s", sceName[i].Data()));
        hTotBarrelEmuEmEtVsCen[i] = (TH2D *)f->Get(Form("hTotBarrelEmuEmEtVsCen_%s", sceName[i].Data()));
        hTotBarrelEmuHadEtVsCen[i] = (TH2D *)f->Get(Form("hTotBarrelEmuHadEtVsCen_%s", sceName[i].Data()));
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen[i] = (TH3D *)f->Get(Form("hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen_%s", sceName[i].Data()));
        hNTowersVsEmuEtThVsCen[i] = (TH3D *)f->Get(Form("hNTowersVsEmuEtThVsCen_%s", sceName[i].Data()));
        hNTowersVsEmuEmEtThVsCen[i] = (TH3D *)f->Get(Form("hNTowersVsEmuEmEtThVsCen_%s", sceName[i].Data()));
        hNTowersVsEmuHadEtThVsCen[i] = (TH3D *)f->Get(Form("hNTowersVsEmuHadEtThVsCen_%s", sceName[i].Data()));
    }

    for(Int_t i=0; i<nSces; i++){
        c3->cd(1);
        gPad->SetLogz(1);
        hIphiVsIeta_Emu[i]->GetXaxis()->SetRangeUser(-30, 30);
        hIphiVsIeta_Emu[i]->GetYaxis()->SetRangeUser(0.5, 72.5);
        hIphiVsIeta_Emu[i]->Draw("colz");
        drawLatex(0.32, 0.95, "ECal + HCal Towers", mFont, 0.06, 1);
        c3->cd(2);
        gPad->SetLogz(1);
        hIphiVsIeta_Emu_ECal[i]->GetXaxis()->SetRangeUser(-30, 30);
        hIphiVsIeta_Emu_ECal[i]->GetYaxis()->SetRangeUser(0.5, 72.5);
        hIphiVsIeta_Emu_ECal[i]->Draw("colz");
        drawLatex(0.38, 0.95, "ECal Towers", mFont, 0.06, 1);
        c3->cd(3);
        gPad->SetLogz(1);
        hIphiVsIeta_Emu_HCal[i]->GetXaxis()->SetRangeUser(-30, 30);
        hIphiVsIeta_Emu_HCal[i]->GetYaxis()->SetRangeUser(0.5, 72.5);
        hIphiVsIeta_Emu_HCal[i]->Draw("colz");
        drawLatex(0.38, 0.95, "HCal Towers", mFont, 0.06, 1);
        c3->cd(4);
        gPad->Clear();
        drawLatex(0.3, 0.5, Form("%s", sceName[i].Data()), mFont, 0.15, 1);
        c3->SaveAs(Form("%s/iEtaVsIphi_%s.png", dirName.Data(), sceName[i].Data()));
    }

    TLegend *leg = new TLegend(0.16, 0.16, 0.4, 0.30);
    leg->SetFillStyle(0);
    leg->SetFillColor(10);
    leg->SetTextFont(mFont);
    leg->SetTextSize(0.05);

    c3->cd(1);
    gPad->SetLogy(0);
    setHisto(hCentrality_OfficialMB, 20, 1., 1, 1, 2);
    setHisto(hCentrality_OfficialMB_ZDCAnd, 24, 1., 2, 2, 2);
    hCentrality_OfficialMB->RebinX();
    hCentrality_OfficialMB_ZDCAnd->RebinX();
    hCentrality_OfficialMB->Draw("hist");
    hCentrality_OfficialMB_ZDCAnd->Draw("histsame");
    leg->AddEntry(hCentrality_OfficialMB, "Conventional MB", "l");
    leg->AddEntry(hCentrality_OfficialMB_ZDCAnd, "Conventional MB & ZDCAnd1", "l");
    leg->DrawClone("same");
    drawLine(190, 0, 190, hCentrality_OfficialMB->GetMaximum(), 4, 2, 2);
    c3->cd(2);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hNtrkOfflineVsNtrkHP->Draw("col");
    c3->cd(3);
    gPad->SetLogy(1);
    TH1D *hNtrkHP = (TH1D *)hNtrkOfflineVsNtrkHP->ProjectionX("hNtrkHP");
    TH1D *hNtrkOffline = (TH1D *)hNtrkOfflineVsNtrkHP->ProjectionY("hNtrkOffline");
    setHisto(hNtrkHP, 20, 1., 1, 1, 2);
    setHisto(hNtrkOffline, 24, 1., 2, 2, 2);
    hNtrkHP->RebinX(10);
    hNtrkOffline->RebinX(10);
    hNtrkHP->Draw("hist");
    hNtrkOffline->Draw("histsame");
    leg->Clear();
    leg->AddEntry(hNtrkHP, "N_{trk}^{HP}", "l");
    leg->AddEntry(hNtrkOffline, "N_{trk}^{Offline}", "l");
    leg->Draw("same");
    c3->cd(4);
    gPad->Clear();
    c3->SaveAs(Form("%s/Collision_Info.png", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    setHisto(hCentrality_OfficialMB, 20, 1., 1, 1, 2);
    setHisto(hCentrality_OfficialMB_ZDCAnd, 24, 1., 2, 2, 2);
    hCentrality_OfficialMB->RebinX();
    hCentrality_OfficialMB_ZDCAnd->RebinX();
    hCentrality_OfficialMB->GetYaxis()->SetRangeUser(0, 1.5e3);
    hCentrality_OfficialMB->Draw("hist");
    hCentrality_OfficialMB_ZDCAnd->Draw("histsame");
    leg->Clear();
    leg->AddEntry(hCentrality_OfficialMB, "Conventional MB", "l");
    leg->AddEntry(hCentrality_OfficialMB_ZDCAnd, "Conventional MB & ZDCAnd1", "l");
    leg->DrawClone("same");
    drawLine(190, 0, 190, hCentrality_OfficialMB->GetMaximum(), 4, 2, 2);
    c1->SaveAs(Form("%s/CentralityFromZB_OfflineMB.pdf", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hnTowersVsnEmuTowers->Draw("colz");
    c3->cd(2);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hMaxEtVsMaxEmuEt->Draw("colz");
    c3->cd(3);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTotEtVsTotEmuEt->SetAxisRange(-400, 1.5e4, "X");
    hTotEtVsTotEmuEt->SetAxisRange(-400, 1.5e4, "Y");
    hTotEtVsTotEmuEt->Draw("colz");
    c3->SaveAs(Form("%s/DataVsEmulation.png", dirName.Data()));

    for(Int_t i=0; i<nSces; i++){
        c2->cd(i+1);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hEmuEtVsTowerIdx[i]->SetAxisRange(0, 60, "Y");
        hEmuEtVsTowerIdx[i]->Draw("colz");
        drawLatex(0.43, 0.95, Form("%s", sceName[i].Data()), mFont, 0.06, 1);
    }
    c2->SaveAs(Form("%s/caloEmuEtVsTowerIdx.png", dirName.Data()));

    for(Int_t i=0; i<nSces; i++){
        c2->cd(i+1);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hEmuEmEtVsTowerIdx[i]->SetAxisRange(0, 60, "Y");
        hEmuEmEtVsTowerIdx[i]->Draw("colz");
        drawLatex(0.43, 0.95, Form("%s", sceName[i].Data()), mFont, 0.06, 1);
    }
    c2->SaveAs(Form("%s/caloEmuEmEtVsTowerIdx.png", dirName.Data()));

    for(Int_t i=0; i<nSces; i++){
        c2->cd(i+1);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hEmuHadEtVsTowerIdx[i]->SetAxisRange(0, 60, "Y");
        hEmuHadEtVsTowerIdx[i]->Draw("colz");
        drawLatex(0.43, 0.95, Form("%s", sceName[i].Data()), mFont, 0.06, 1);
    }
    c2->SaveAs(Form("%s/caloEmuHadEtVsTowerIdx.png", dirName.Data()));

    const Int_t mMaxEtTh = 5;
    const Int_t mTotEtTh = 150;
    const Double_t mCMSHINRate = 1.e7;

    const Int_t nCenBins = 2;
    const Int_t cenLow[nCenBins] = {80, 90};
    const Int_t cenHi[nCenBins]  = {90, 100};

    c3->cd(1);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hMaxEmuEtVsCen[1]->GetZaxis()->SetRangeUser(1, 1e3);
    hMaxEmuEtVsCen[1]->GetYaxis()->SetRangeUser(0, 100);
    hMaxEmuEtVsCen[1]->Draw("");
    hMaxEmuEtVsCen[0]->Draw("colzsame");
    drawLine(0, mMaxEtTh, 200, mMaxEtTh, 4, 2, 2);
    c3->cd(2);
    gPad->SetLogy(1);
    hMaxEmuEtVsCen[0]->GetYaxis()->UnZoom();
    TH1D *hMaxEmuEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hMaxEmuEt[i] = hMaxEmuEtVsCen[i]->ProjectionY(Form("hMaxEmuEt_%s", sceName[i].Data()));
    }
    setHisto(hMaxEmuEt[0], 24, 1.0, 2, 2, 2);
    setHisto(hMaxEmuEt[1], 20, 1.0, 1, 1, 2);
    hMaxEmuEt[1]->GetYaxis()->SetTitle("Entries");
    hMaxEmuEt[1]->GetYaxis()->SetRangeUser(0.5, hMaxEmuEt[1]->GetMaximum()*10);
    hMaxEmuEt[1]->Draw("hist");
    hMaxEmuEt[0]->Draw("histsame");
    drawLine(mMaxEtTh, 0.5, mMaxEtTh, hMaxEmuEt[1]->GetMaximum(), 4, 2, 2);
    Int_t etBinLow = hMaxEmuEt[1]->GetXaxis()->FindBin(mMaxEtTh + mTinyNum);
    Int_t etBinHin = hMaxEmuEt[1]->GetNbinsX();
    Double_t noiseRate = hMaxEmuEt[1]->Integral(etBinLow, etBinHin) / hnEvts->GetBinContent(1) * mCMSHINRate;
    setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
    for(Int_t i=0; i<nSces; i++) leg->AddEntry(hMaxEmuEt[i], Form("%s", sceName[i].Data()), "l");
    leg->Draw("same");
    drawLatex(0.2, 0.6, Form("Noise rate: %1.1f kHz for E_{t}^{max} > %d", noiseRate/1000, mMaxEtTh), mFont, 0.06, 1);
    TH1D *hMaxEmuEt_Cen[nCenBins];
    for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
        c3->cd(icen-nCenBins+5);
        gPad->SetLogy(1);
        Int_t cenBinLow = hMaxEmuEtVsCen[0]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
        Int_t cenBinHi  = hMaxEmuEtVsCen[0]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
        hMaxEmuEt_Cen[icen] = (TH1D *)hMaxEmuEtVsCen[0]->ProjectionY(Form("hMaxEmuEt_Cen%d%d", cenLow[icen], cenHi[icen]), cenBinLow, cenBinHi);
        setHisto(hMaxEmuEt_Cen[icen], 20, 1.0, 2, 2, 2);
        hMaxEmuEt_Cen[icen]->GetYaxis()->SetTitle("Entries");
        hMaxEmuEt_Cen[icen]->Draw("hist");
        etBinLow = hMaxEmuEt_Cen[icen]->GetXaxis()->FindBin(mMaxEtTh + mTinyNum);
        etBinHin = hMaxEmuEt_Cen[icen]->GetNbinsX();
        Double_t eff = hMaxEmuEt_Cen[icen]->Integral(etBinLow, etBinHin) / hMaxEmuEt_Cen[icen]->Integral(0, -1);
        drawLine(mMaxEtTh, 0, mMaxEtTh, hMaxEmuEt_Cen[icen]->GetMaximum(), 4, 2, 2);
        drawLatex(0.3, 0.6, Form("Efficiency: %1.1f%% for E_{t}^{max} > %d", eff*100, mMaxEtTh), mFont, 0.06, 1);
        drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
    }
    c3->SaveAs(Form("%s/MaxEmuEt_NoiseVsCollision.png", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(1);
    gPad->SetLogz(1);
    hTotEmuEtVsCen[0]->Draw("colz");
    hTotEmuEtVsCen[1]->GetZaxis()->SetRangeUser(1, 1e3);
    hTotEmuEtVsCen[1]->Draw("same");
    hTotEmuEtVsCen[0]->Draw("same colz");
    drawLine(0, mTotEtTh, 200, mTotEtTh, 4, 2, 2);
    c3->cd(2);
    gPad->SetLogx(1);
    gPad->SetLogy(1);
    hTotEmuEtVsCen[0]->GetYaxis()->UnZoom();
    TH1D *hTotEmuEt[nSces];
    for(Int_t i=0; i<nSces; i++) hTotEmuEt[i] = hTotEmuEtVsCen[i]->ProjectionY(Form("hTotEmuEt_%s", sceName[i].Data()));
    setHisto(hTotEmuEt[0], 24, 1.0, 2, 2, 2);
    setHisto(hTotEmuEt[1], 20, 1.0, 1, 1, 2);
    hTotEmuEt[0]->GetYaxis()->SetTitle("Entries");
    hTotEmuEt[0]->GetYaxis()->SetRangeUser(0.5, hTotEmuEt[1]->GetMaximum()*10);
    hTotEmuEt[0]->Draw("hist");
    hTotEmuEt[1]->Draw("histsame");
    drawLine(mTotEtTh, 0, mTotEtTh, hTotEmuEt[1]->GetMaximum(), 4, 2, 2);
    etBinLow = hTotEmuEt[1]->GetXaxis()->FindBin(mTotEtTh + mTinyNum);
    etBinHin = hTotEmuEt[1]->GetNbinsX();
    noiseRate = hTotEmuEt[1]->Integral(etBinLow, etBinHin) / hnEvts->GetBinContent(1) * mCMSHINRate;
    setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
    for(Int_t i=0; i<nSces; i++) leg->AddEntry(hTotEmuEt[i], Form("%s", sceName[i].Data()), "l");
    leg->Draw("same");
    drawLatex(0.2, 0.6, Form("Noise rate: %1.1f kHz for E_{t}^{total} > %d", noiseRate/1000, mTotEtTh), mFont, 0.06, 1);
    TH1D *hTotEmuEt_Cen[nCenBins];
    for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
        c3->cd(icen-nCenBins+5);
        gPad->SetLogx(1);
        gPad->SetLogy(1);
        Int_t cenBinLow = hTotEmuEtVsCen[0]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
        Int_t cenBinHi  = hTotEmuEtVsCen[0]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
        hTotEmuEt_Cen[icen] = (TH1D *)hTotEmuEtVsCen[0]->ProjectionY(Form("hTotEmuEt_Cen%d%d", cenLow[icen], cenHi[icen]), cenBinLow, cenBinHi);
        setHisto(hTotEmuEt_Cen[icen], 20, 1.0, 2, 2, 2);
        hTotEmuEt_Cen[icen]->GetYaxis()->SetTitle("Entries");
        hTotEmuEt_Cen[icen]->Draw("hist");
        etBinLow = hTotEmuEt_Cen[icen]->GetXaxis()->FindBin(mTotEtTh + mTinyNum);
        etBinHin = hTotEmuEt_Cen[icen]->GetNbinsX();
        Double_t eff = hTotEmuEt_Cen[icen]->Integral(etBinLow, etBinHin) / hTotEmuEt_Cen[icen]->Integral(0, -1);
        drawLine(mTotEtTh, 0, mTotEtTh, hTotEmuEt_Cen[icen]->GetMaximum(), 4, 2, 2);
        drawLatex(0.3, 0.6, Form("Efficiency: %1.1f%% for E_{t}^{total} > %d", eff*100, mTotEtTh), mFont, 0.06, 1);
        drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
    }
    c3->SaveAs(Form("%s/TotEmuEt_NoiseVsCollision.png", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(1);
    gPad->SetLogz(1);
    hTotEmuEmEtVsCen[0]->Draw("colz");
    hTotEmuEmEtVsCen[1]->GetZaxis()->SetRangeUser(1, 1e3);
    hTotEmuEmEtVsCen[1]->Draw("same");
    hTotEmuEmEtVsCen[0]->Draw("same colz");
    c3->cd(2);
    gPad->SetLogx(1);
    gPad->SetLogy(1);
    hTotEmuEmEtVsCen[0]->GetYaxis()->UnZoom();
    TH1D *hTotEmuEmEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hTotEmuEmEt[i] = hTotEmuEmEtVsCen[i]->ProjectionY(Form("hTotEmuEmEt_%s", sceName[i].Data()));
        setHisto(hTotEmuEmEt[i], mMStyle[i], mMSize[i], mColor[i], mColor[i], 2);
    }
    hTotEmuEmEt[0]->GetYaxis()->SetTitle("Entries");
    hTotEmuEmEt[0]->GetYaxis()->SetRangeUser(0.5, hTotEmuEmEt[1]->GetMaximum()*10);
    hTotEmuEmEt[0]->Draw("hist");
    hTotEmuEmEt[1]->Draw("histsame");
    setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
    for(Int_t i=0; i<nSces; i++) leg->AddEntry(hTotEmuEmEt[i], Form("%s", sceName[i].Data()), "l");
    leg->Draw("same");
    TH1D *hTotEmuEmEt_Cen[nCenBins];
    for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
        c3->cd(icen-nCenBins+5);
        gPad->SetLogx(1);
        gPad->SetLogy(1);
        Int_t cenBinLow = hTotEmuEmEtVsCen[0]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
        Int_t cenBinHi  = hTotEmuEmEtVsCen[0]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
        hTotEmuEmEt_Cen[icen] = (TH1D *)hTotEmuEmEtVsCen[0]->ProjectionY(Form("hTotEmuEmEt_Cen%d%d", cenLow[icen], cenHi[icen]), cenBinLow, cenBinHi);
        setHisto(hTotEmuEmEt_Cen[icen], 20, 1.0, 2, 2, 2);
        hTotEmuEmEt_Cen[icen]->GetYaxis()->SetTitle("Entries");
        hTotEmuEmEt_Cen[icen]->SetMaximum(hTotEmuEmEt[1]->GetMaximum()*10);
        hTotEmuEmEt_Cen[icen]->Draw("hist");
        hTotEmuEmEt[1]->Draw("histsame");
        drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
    }
    c3->SaveAs(Form("%s/TotEmuEmEt_NoiseVsCollision.png", dirName.Data()));

    TH2D *hTotEmuEmEtVsTotEmuHadEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hTotEmuEmEtVsTotEmuHadEtVsCen[i]->GetXaxis()->SetRange(16, 20);
        hTotEmuEmEtVsTotEmuHadEt[i] = (TH2D *)hTotEmuEmEtVsTotEmuHadEtVsCen[i]->Project3D("zy");

        c2->cd(i+1);
        gPad->Clear();
        gPad->SetLogx(0);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hTotEmuEmEtVsTotEmuHadEt[i]->GetXaxis()->SetRangeUser(0, 200);
        hTotEmuEmEtVsTotEmuHadEt[i]->GetYaxis()->SetRangeUser(0, 200);
        hTotEmuEmEtVsTotEmuHadEt[i]->Draw("col");
        if(i==0) drawLatex(0.2, 0.8, "Centrality: 80-100%", mFont, 0.06, 1);
        if(i==1) drawLatex(0.2, 0.8, "Noise", mFont, 0.06, 1);
    }
    c2->SaveAs(Form("%s/TotEmuEmEtVsTotEmuHadEt.png", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(1);
    gPad->SetLogz(1);
    hTotEmuHadEtVsCen[0]->Draw("colz");
    hTotEmuHadEtVsCen[1]->GetZaxis()->SetRangeUser(1, 1e3);
    hTotEmuHadEtVsCen[1]->Draw("same");
    hTotEmuHadEtVsCen[0]->Draw("same colz");
    c3->cd(2);
    gPad->SetLogx(1);
    gPad->SetLogy(1);
    hTotEmuHadEtVsCen[0]->GetYaxis()->UnZoom();
    TH1D *hTotEmuHadEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hTotEmuHadEt[i] = hTotEmuHadEtVsCen[i]->ProjectionY(Form("hTotEmuHadEt_%s", sceName[i].Data()));
        setHisto(hTotEmuHadEt[i], mMStyle[i], mMSize[i], mColor[i], mColor[i], 2);
    }
    hTotEmuHadEt[0]->GetYaxis()->SetTitle("Entries");
    hTotEmuHadEt[0]->GetYaxis()->SetRangeUser(0.5, hTotEmuHadEt[1]->GetMaximum()*10);
    hTotEmuHadEt[0]->Draw("hist");
    hTotEmuHadEt[1]->Draw("histsame");
    setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
    for(Int_t i=0; i<nSces; i++) leg->AddEntry(hTotEmuHadEt[i], Form("%s", sceName[i].Data()), "l");
    leg->Draw("same");
    TH1D *hTotEmuHadEt_Cen[nCenBins];
    for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
        c3->cd(icen-nCenBins+5);
        gPad->SetLogx(1);
        gPad->SetLogy(1);
        Int_t cenBinLow = hTotEmuHadEtVsCen[0]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
        Int_t cenBinHi  = hTotEmuHadEtVsCen[0]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
        hTotEmuHadEt_Cen[icen] = (TH1D *)hTotEmuHadEtVsCen[0]->ProjectionY(Form("hTotEmuHadEt_Cen%d%d", cenLow[icen], cenHi[icen]), cenBinLow, cenBinHi);
        setHisto(hTotEmuHadEt_Cen[icen], 20, 1.0, 2, 2, 2);
        hTotEmuHadEt_Cen[icen]->GetYaxis()->SetTitle("Entries");
        hTotEmuHadEt_Cen[icen]->SetMaximum(hTotEmuHadEt[1]->GetMaximum()*10);
        hTotEmuHadEt_Cen[icen]->Draw("hist");
        hTotEmuHadEt[1]->Draw("histsame");
        drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
    }
    c3->SaveAs(Form("%s/TotEmuHadEt_NoiseVsCollision.png", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(1);
    gPad->SetLogz(1);
    hTotBarrelEmuEtVsCen[0]->Draw("colz");
    hTotBarrelEmuEtVsCen[1]->GetZaxis()->SetRangeUser(1, 1e3);
    hTotBarrelEmuEtVsCen[1]->Draw("same");
    hTotBarrelEmuEtVsCen[0]->Draw("same colz");
    c3->cd(2);
    gPad->SetLogx(1);
    gPad->SetLogy(1);
    hTotBarrelEmuEtVsCen[0]->GetYaxis()->UnZoom();
    TH1D *hTotBarrelEmuEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hTotBarrelEmuEt[i] = hTotBarrelEmuEtVsCen[i]->ProjectionY(Form("hTotBarrelEmuEt_%s", sceName[i].Data()));
        setHisto(hTotBarrelEmuEt[i], mMStyle[i], mMSize[i], mColor[i], mColor[i], 2);
    }
    hTotBarrelEmuEt[0]->GetYaxis()->SetTitle("Entries");
    hTotBarrelEmuEt[0]->GetYaxis()->SetRangeUser(0.5, hTotBarrelEmuEt[1]->GetMaximum()*10);
    hTotBarrelEmuEt[0]->Draw("hist");
    hTotBarrelEmuEt[1]->Draw("histsame");
    setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
    for(Int_t i=0; i<nSces; i++) leg->AddEntry(hTotBarrelEmuEt[i], Form("%s", sceName[i].Data()), "l");
    leg->Draw("same");
    TH1D *hTotBarrelEmuEt_Cen[nCenBins];
    for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
        c3->cd(icen-nCenBins+5);
        gPad->SetLogx(1);
        gPad->SetLogy(1);
        Int_t cenBinLow = hTotBarrelEmuEtVsCen[0]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
        Int_t cenBinHi  = hTotBarrelEmuEtVsCen[0]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
        hTotBarrelEmuEt_Cen[icen] = (TH1D *)hTotBarrelEmuEtVsCen[0]->ProjectionY(Form("hTotBarrelEmuEt_Cen%d%d", cenLow[icen], cenHi[icen]), cenBinLow, cenBinHi);
        setHisto(hTotBarrelEmuEt_Cen[icen], 20, 1.0, 2, 2, 2);
        hTotBarrelEmuEt_Cen[icen]->SetMaximum(hTotBarrelEmuEt[1]->GetMaximum()*10);
        hTotBarrelEmuEt_Cen[icen]->GetYaxis()->SetTitle("Entries");
        hTotBarrelEmuEt_Cen[icen]->Draw("hist");
        hTotBarrelEmuEt[1]->Draw("histsame");
        drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
    }
    c3->SaveAs(Form("%s/TotBarrelEmuEt_NoiseVsCollision.png", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(1);
    gPad->SetLogz(1);
    hTotBarrelEmuEmEtVsCen[0]->Draw("colz");
    hTotBarrelEmuEmEtVsCen[1]->GetZaxis()->SetRangeUser(1, 1e3);
    hTotBarrelEmuEmEtVsCen[1]->Draw("same");
    hTotBarrelEmuEmEtVsCen[0]->Draw("same colz");
    c3->cd(2);
    gPad->SetLogx(1);
    gPad->SetLogy(1);
    hTotBarrelEmuEmEtVsCen[0]->GetYaxis()->UnZoom();
    TH1D *hTotBarrelEmuEmEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hTotBarrelEmuEmEt[i] = hTotBarrelEmuEmEtVsCen[i]->ProjectionY(Form("hTotBarrelEmuEmEt_%s", sceName[i].Data()));
        setHisto(hTotBarrelEmuEmEt[i], mMStyle[i], mMSize[i], mColor[i], mColor[i], 2);
    }
    hTotBarrelEmuEmEt[0]->GetYaxis()->SetTitle("Entries");
    hTotBarrelEmuEmEt[0]->GetYaxis()->SetRangeUser(0.5, hTotBarrelEmuEmEt[1]->GetMaximum()*10);
    hTotBarrelEmuEmEt[0]->Draw("hist");
    hTotBarrelEmuEmEt[1]->Draw("histsame");
    etBinHin = hTotBarrelEmuEmEt[1]->GetNbinsX();
    noiseRate = hTotBarrelEmuEmEt[1]->Integral(etBinLow, etBinHin) / hnEvts->GetBinContent(1) * mCMSHINRate;
    setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
    for(Int_t i=0; i<nSces; i++) leg->AddEntry(hTotBarrelEmuEmEt[i], Form("%s", sceName[i].Data()), "l");
    leg->Draw("same");
    TH1D *hTotBarrelEmuEmEt_Cen[nCenBins];
    for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
        c3->cd(icen-nCenBins+5);
        gPad->SetLogx(1);
        gPad->SetLogy(1);
        Int_t cenBinLow = hTotBarrelEmuEmEtVsCen[0]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
        Int_t cenBinHi  = hTotBarrelEmuEmEtVsCen[0]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
        hTotBarrelEmuEmEt_Cen[icen] = (TH1D *)hTotBarrelEmuEmEtVsCen[0]->ProjectionY(Form("hTotBarrelEmuEmEt_Cen%d%d", cenLow[icen], cenHi[icen]), cenBinLow, cenBinHi);
        setHisto(hTotBarrelEmuEmEt_Cen[icen], 20, 1.0, 2, 2, 2);
        hTotBarrelEmuEmEt_Cen[icen]->GetYaxis()->SetTitle("Entries");
        hTotBarrelEmuEmEt_Cen[icen]->SetMaximum(hTotBarrelEmuEmEt[1]->GetMaximum()*10);
        hTotBarrelEmuEmEt_Cen[icen]->Draw("hist");
        hTotBarrelEmuEmEt[1]->Draw("histsame");
        drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
    }
    c3->SaveAs(Form("%s/TotBarrelEmuEmEt_NoiseVsCollision.png", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(1);
    gPad->SetLogz(1);
    hTotBarrelEmuHadEtVsCen[0]->Draw("colz");
    hTotBarrelEmuHadEtVsCen[1]->GetZaxis()->SetRangeUser(1, 1e3);
    hTotBarrelEmuHadEtVsCen[1]->Draw("same");
    hTotBarrelEmuHadEtVsCen[0]->Draw("same colz");
    c3->cd(2);
    gPad->SetLogx(1);
    gPad->SetLogy(1);
    hTotBarrelEmuHadEtVsCen[0]->GetYaxis()->UnZoom();
    TH1D *hTotBarrelEmuHadEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hTotBarrelEmuHadEt[i] = hTotBarrelEmuHadEtVsCen[i]->ProjectionY(Form("hTotBarrelEmuHadEt_%s", sceName[i].Data()));
        setHisto(hTotBarrelEmuHadEt[i], mMStyle[i], mMSize[i], mColor[i], mColor[i], 2);
    }
    hTotBarrelEmuHadEt[0]->GetYaxis()->SetTitle("Entries");
    hTotBarrelEmuHadEt[0]->GetYaxis()->SetRangeUser(0.5, hTotBarrelEmuHadEt[1]->GetMaximum()*10);
    hTotBarrelEmuHadEt[0]->Draw("hist");
    hTotBarrelEmuHadEt[1]->Draw("histsame");
    setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
    for(Int_t i=0; i<nSces; i++) leg->AddEntry(hTotBarrelEmuHadEt[i], Form("%s", sceName[i].Data()), "l");
    leg->Draw("same");
    TH1D *hTotBarrelEmuHadEt_Cen[nCenBins];
    for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
        c3->cd(icen-nCenBins+5);
        gPad->SetLogx(1);
        gPad->SetLogy(1);
        Int_t cenBinLow = hTotBarrelEmuHadEtVsCen[0]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
        Int_t cenBinHi  = hTotBarrelEmuHadEtVsCen[0]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
        hTotBarrelEmuHadEt_Cen[icen] = (TH1D *)hTotBarrelEmuHadEtVsCen[0]->ProjectionY(Form("hTotBarrelEmuHadEt_Cen%d%d", cenLow[icen], cenHi[icen]), cenBinLow, cenBinHi);
        setHisto(hTotBarrelEmuHadEt_Cen[icen], 20, 1.0, 2, 2, 2);
        hTotBarrelEmuHadEt_Cen[icen]->SetMaximum(hTotBarrelEmuHadEt[1]->GetMaximum()*10);
        hTotBarrelEmuHadEt_Cen[icen]->GetYaxis()->SetTitle("Entries");
        hTotBarrelEmuHadEt_Cen[icen]->Draw("hist");
        hTotBarrelEmuHadEt[1]->Draw("histsame");
        drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
    }
    c3->SaveAs(Form("%s/TotBarrelEmuHadEt_NoiseVsCollision.png", dirName.Data()));

    TH2D *hTotBarrelEmuEmEtVsTotBarrelEmuHadEt[nSces];
    for(Int_t i=0; i<nSces; i++){
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen[i]->GetXaxis()->SetRangeUser(160, 200);
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEt[i] = (TH2D *)hTotBarrelEmuEmEtVsTotBarrelEmuHadEtVsCen[i]->Project3D("zy");

        c2->cd(i+1);
        gPad->Clear();
        gPad->SetLogx(0);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEt[i]->GetXaxis()->SetRangeUser(0, 200);
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEt[i]->GetYaxis()->SetRangeUser(0, 200);
        hTotBarrelEmuEmEtVsTotBarrelEmuHadEt[i]->Draw("col");
        if(i==0) drawLatex(0.2, 0.8, "Centrality: 80-100%", mFont, 0.06, 1);
        if(i==1) drawLatex(0.2, 0.8, "Noise", mFont, 0.06, 1);
    }
    c2->SaveAs(Form("%s/TotBarrelEmuEmEtVsTotBarrelEmuHadEt.png", dirName.Data()));

    TH2D *hFrame = (TH2D *)histo("hFrame", 0.5, 5e3, 0.5, 1e8, "nTowers", "Entries");
    hFrame->GetXaxis()->SetLabelSize(0.05);
    hFrame->GetYaxis()->SetLabelSize(0.05);
    hFrame->GetXaxis()->SetLabelOffset(0.003);
    hFrame->GetYaxis()->SetLabelOffset(0.003);
    hFrame->GetXaxis()->SetTitleSize(0.06);
    hFrame->GetYaxis()->SetTitleSize(0.06);
    hFrame->GetXaxis()->SetTitleOffset(0.8);
    hFrame->GetYaxis()->SetTitleOffset(1.0);
    hFrame->GetXaxis()->SetNdivisions(505);
    hFrame->GetYaxis()->SetNdivisions(210);

    TH2D *hNEmuTowersVsCen_EtThBin[nSces][nEtThs];
    TH1D *hNEmuTowers_EtThBin[nSces][nEtThs];
    TH1D *hNEmuTowers_Collision_EtThBin_CenBin[nEtThs][nCenBins];
    for(Int_t ith=0; ith<nEtThs; ith++){
        for(Int_t i=0; i<nSces; i++){
            hNTowersVsEmuEtThVsCen[i]->GetYaxis()->SetRange(ith+1, ith+1);
            hNEmuTowersVsCen_EtThBin[i][ith] = (TH2D *)hNTowersVsEmuEtThVsCen[i]->Project3D(Form("hNEmuTowersVsCen_%s_EtThBin%d_zx", sceName[i].Data(), ith));

            hNTowersVsEmuEtThVsCen[i]->GetYaxis()->SetRange(ith+1, ith+1);
            hNEmuTowersVsCen_EtThBin[i][ith] = (TH2D *)hNTowersVsEmuEtThVsCen[i]->Project3D(Form("hNEmuTowersVsCen_%s_EtThBin%d_zx", sceName[i].Data(), ith));
        }

        c3->cd(1);
        gPad->SetLogy(1);
        gPad->SetLogz(1);
        hNEmuTowersVsCen_EtThBin[1][ith]->GetZaxis()->SetRangeUser(0.5, 5e2);
        hNEmuTowersVsCen_EtThBin[1][ith]->Draw("p");
        hNEmuTowersVsCen_EtThBin[0][ith]->Draw("same col");
        drawLatex(0.56, 0.8, Form("E_{t}^{tower} > %1.1f GeV", mEtTh[ith]*0.5), mFont, 0.06, 1);
        c3->cd(2);
        gPad->SetLogx(1);
        gPad->SetLogy(1);
        hFrame->Draw("c");
        setLegend(leg, 0.6, 0.7, 0.8, 0.84, 0.06);
        for(Int_t i=0; i<nSces; i++){
            hNEmuTowers_EtThBin[i][ith] = hNEmuTowersVsCen_EtThBin[i][ith]->ProjectionY(Form("hNEmuTowers_%s_EtThBin%d", sceName[i].Data(), ith));
            setHisto(hNEmuTowers_EtThBin[i][ith], mMStyle[i], mMSize[i], mColor[i], mColor[i], 2);
            hNEmuTowers_EtThBin[i][ith]->Draw("histsame");

            leg->AddEntry(hNEmuTowers_EtThBin[i][ith], Form("%s", sceName[i].Data()), "l");
        }
        leg->Draw("same");

        for(Int_t icen=nCenBins-2; icen<nCenBins; icen++){
            c3->cd(icen-nCenBins+5);
            gPad->SetLogx(1);
            gPad->SetLogy(1);
            Int_t cenBinLow = hNEmuTowersVsCen_EtThBin[0][ith]->GetXaxis()->FindBin(cenLow[icen]*2 + mTinyNum);
            Int_t cenBinHi  = hNEmuTowersVsCen_EtThBin[0][ith]->GetXaxis()->FindBin(cenHi[icen]*2 - mTinyNum);
            hNEmuTowers_Collision_EtThBin_CenBin[ith][icen] = (TH1D *)hNEmuTowersVsCen_EtThBin[0][ith]->ProjectionY(Form("hNEmuTowers_Collision_EtThBin%d_CenBin%d", ith, icen), cenBinLow, cenBinHi);
            setHisto(hNEmuTowers_Collision_EtThBin_CenBin[ith][icen], 20, 1.0, 2, 2, 2);
            hFrame->Draw("c");
            hNEmuTowers_EtThBin[1][ith]->Draw("histsame");
            hNEmuTowers_Collision_EtThBin_CenBin[ith][icen]->Draw("histsame");
            drawLatex(0.32, 0.94, Form("Centrality: %d-%d%%", cenLow[icen], cenHi[icen]), mFont, 0.06, 1);
        }
        c3->SaveAs(Form("%s/NTowersAboveTh%d_NoiseVsCollision.png", dirName.Data(), ith));
    }

    cout << "End of program !" << endl;
}
