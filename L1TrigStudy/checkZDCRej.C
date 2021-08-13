#include "../common/headers.h"
#include "../common/function.C"
#include "../common/constant.h"

const Int_t mFont = 42;

void checkZDCRej()
{
    gStyle->SetOptFit(1111);
    gStyle->SetLabelOffset(0.001,"z");
    //gStyle->SetEndErrorSize(4);

    TFile *f = TFile::Open("L1Eff.test.histo.root");

    TString dirName  = "zdcRejPlots";
    system(Form("mkdir -p %s", dirName.Data()));
    system(Form("rm -rf %s/*", dirName.Data()));

    TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 600, 400);

    Int_t nColumns = 2;
    Int_t nRaws = 2;
    Int_t nPads = nColumns * nRaws;
    TCanvas* c2 = new TCanvas("c2", "c2", 1200, 900);
    c2->Divide(nColumns, nRaws);
    for(Int_t ipad=0; ipad<nPads; ipad++){
        c2->cd(ipad+1);
        setPad(0.12, 0.08, 0.08, 0.12);
    }

    TH1D *hnEvts_checkZDCTh = (TH1D *)f->Get("hnEvts_checkZDCTh");
    hnEvts_checkZDCTh->Scale(8.e6/hnEvts_checkZDCTh->GetBinContent(1));

    c1->cd();
    setPad(0.12, 0.04, 0.08, 0.2);
    gPad->SetLogy(1);
    //for(Int_t iHF=0; iHF<nLowHFAdcThs; iHF++){
    //    for(Int_t iZDC=0; iZDC<nNeuThs; iZDC++){
    //        hnEvts_checkZDCTh->GetXaxis()->SetBinLabel(2+iHF*nNeuThs+iZDC, Form("AdcTh%d & %dn", mLowHFAdcTh[iHF], mNeuTh[iZDC]));
    //    }
    //}
    hnEvts_checkZDCTh->GetXaxis()->SetRangeUser(0, 43);
    hnEvts_checkZDCTh->GetYaxis()->SetRangeUser(1e3, 1e8);
    hnEvts_checkZDCTh->GetYaxis()->SetTitle("Trigger Rate (Hz)");
    hnEvts_checkZDCTh->SetMarkerSize(1.5);
    hnEvts_checkZDCTh->SetMarkerColor(kBlue-4);
    hnEvts_checkZDCTh->Draw("histtext");
    for(Int_t i=0; i<=nLowHFAdcThs; i++){
        drawLine(1+(nNeuThs+1)*i, 5e1, 1+(nNeuThs+1)*i, 1e6, 2, 2, 2);
    }
    drawLatex(0.50, 0.8, "Trigger setup: HFAnd & ZDCAnd", mFont, 0.05, 1);
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.05, 1);
    drawLatex(0.70, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.05, 1);
    c1->SaveAs(Form("%s/RateVsZDCTh.pdf", dirName.Data()));

    TLegend *leg = new TLegend(0.14, 0.64, 0.4, 0.86);
    leg->SetFillStyle(0);
    leg->SetFillColor(10);
    leg->SetTextFont(mFont);
    leg->SetTextSize(0.05);

    TLegend *leg1 = new TLegend(0.14, 0.15, 0.6, 0.4);
    leg1->SetFillStyle(0);
    leg1->SetFillColor(10);
    leg1->SetTextFont(mFont);
    leg1->SetTextSize(0.05);

    const    Int_t mBoxColor = kGreen - 9;
    const Double_t mColorAlpha = 0.2;

    TH1D *hCen_MB = (TH1D *)f->Get("hCen_MB");
    TH1D *hCen_MB_HFAnd = (TH1D *)f->Get("hCen_MB_HFAnd_AdcTh12");
    TH1D *hCen_MB_HFAnd_AND_ZDCAnd1n = (TH1D *)f->Get("hCen_MB_HFAnd_AND_ZDCAnd_AdcTh12_1n");
    TH1D *hCen_MB_HFAnd_AND_ZDCAnd2n = (TH1D *)f->Get("hCen_MB_HFAnd_AND_ZDCAnd_AdcTh12_2n");
    c2->cd(1);
    setHisto(hCen_MB, 20, 0.6, 12, 12);
    setHisto(hCen_MB_HFAnd, 20, 0.6, kRed-4, kRed-4);
    setHisto(hCen_MB_HFAnd_AND_ZDCAnd1n, 20, 0.6, kBlue-4, kBlue-4);
    setHisto(hCen_MB_HFAnd_AND_ZDCAnd2n, 20, 0.6, kOrange+1, kOrange+1);
    hCen_MB->SetMinimum(0);
    hCen_MB->GetXaxis()->SetTitleOffset(1.0);
    hCen_MB->GetYaxis()->SetTitle("Entries");
    hCen_MB->Draw("histe");
    hCen_MB_HFAnd->Draw("histesame");
    hCen_MB_HFAnd_AND_ZDCAnd1n->Draw("histesame");
    hCen_MB_HFAnd_AND_ZDCAnd2n->Draw("histesame");
    Double_t yCenter = hCen_MB->GetMaximum()/2.;
    Double_t yErr = yCenter;
    drawBox(180, yCenter, 20, yErr, 20, yErr, mBoxColor, 1, 1001, mColorAlpha);
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    setLegend(leg, 0.15, 0.61, 0.3, 0.88, 0.048);
    leg->AddEntry(hCen_MB, "OfflineMB", "pl");
    leg->AddEntry(hCen_MB_HFAnd, "OfflineMB & HFAnd(12)", "pl");
    leg->AddEntry(hCen_MB_HFAnd_AND_ZDCAnd1n, "OfflineMB & HFAnd(12) & ZDCAnd(1n)", "pl");
    leg->AddEntry(hCen_MB_HFAnd_AND_ZDCAnd2n, "OfflineMB & HFAnd(12) & ZDCAnd(2n)", "pl");
    leg->Draw("same");
    c2->cd(2);
    gPad->SetLogy(0);
    gPad->Clear();
    TH1D *hZDC1nEff = (TH1D *)hCen_MB_HFAnd_AND_ZDCAnd1n->Clone("hZDC1nEff");
    TH1D *hZDC2nEff = (TH1D *)hCen_MB_HFAnd_AND_ZDCAnd2n->Clone("hZDC2nEff");
    hZDC1nEff->Divide(hZDC1nEff, hCen_MB_HFAnd, 1, 1, "B");
    hZDC2nEff->Divide(hZDC2nEff, hCen_MB_HFAnd, 1, 1, "B");
    setHisto(hZDC1nEff, 25, 1, kBlue-4, kBlue-4);
    setHisto(hZDC2nEff, 25, 1, kOrange+1, kOrange+1);
    hZDC1nEff->GetXaxis()->SetRangeUser(120, 200);
    hZDC1nEff->GetYaxis()->SetRangeUser(0, 1.2);
    hZDC1nEff->GetYaxis()->SetTitle("ZDC Selection Efficiency");
    hZDC1nEff->Draw("p");
    hZDC2nEff->Draw("psame");
    drawBox(180, 0.6, 20, 0.6, 20, 0.6, mBoxColor, 1, 1001, mColorAlpha);
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    leg1->AddEntry(hZDC1nEff, "#frac{OfflineMB & HFAnd(12) & ZDCAnd(1n)}{OfflineMB & HFAnd(12)}", "pl");
    leg1->AddEntry(hZDC2nEff, "#frac{OfflineMB & HFAnd(12) & ZDCAnd(2n)}{OfflineMB & HFAnd(12)}", "pl");
    leg1->Draw("same");
    c2->cd(3);
    gPad->SetLogy(0);
    gPad->Clear();
    TH1D *hZDC2nEffOver1nEff = (TH1D *)hCen_MB_HFAnd_AND_ZDCAnd2n->Clone("hZDC2nEffOver1nEff");
    hZDC2nEffOver1nEff->Divide(hCen_MB_HFAnd_AND_ZDCAnd2n, hCen_MB_HFAnd_AND_ZDCAnd1n, 1, 1, "B");
    setHisto(hZDC2nEffOver1nEff, 20, 1, kBlack, kBlack);
    hZDC2nEffOver1nEff->GetXaxis()->SetRangeUser(120, 200);
    hZDC2nEffOver1nEff->GetYaxis()->SetRangeUser(0.7, 1.1);
    hZDC2nEffOver1nEff->GetYaxis()->SetTitle("Efficiency Ratio");
    hZDC2nEffOver1nEff->Draw("p");
    drawLine(120, 1, 200, 1, 2, 2, 2);
    hZDC2nEffOver1nEff->Draw("psame");
    drawBox(180, 0.9, 20, 0.2, 20, 0.2, mBoxColor, 1, 1001, mColorAlpha);
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    drawLatex(0.16, 0.25, "#frac{OfflineMB & HFAnd(12) & ZDCAnd(2n)}{OfflineMB & HFAnd(12) & ZDCAnd(1n)}", mFont, 0.05, 1);
    c2->SaveAs(Form("%s/ZDCEffVsCen.pdf", dirName.Data()));

    cout << "End of program !" << endl;
}
