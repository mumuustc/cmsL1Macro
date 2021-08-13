#include "../common/headers.h"
#include "../common/function.C"
#include "../common/constant.h"

const Int_t    mFont = 42;
const Double_t mTinyNum = 1.e-3;

void checkMBTrig()
{
    gStyle->SetOptFit(1111);
    gStyle->SetLabelOffset(0.001,"z");
    //gStyle->SetEndErrorSize(4);

    TFile *f = TFile::Open("L1Eff.test.histo.root");

    TString dirName  = "mbTrigPlots";
    system(Form("mkdir -p %s", dirName.Data()));
    system(Form("rm -rf %s/*", dirName.Data()));

    TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);

    Int_t nColumns = 2;
    Int_t nRaws = 1;
    Int_t nPads = nColumns * nRaws;
    TCanvas* c2 = new TCanvas("c2", "c2", 1200, 450);
    c2->Divide(nColumns, nRaws);
    for(Int_t ipad=0; ipad<nPads; ipad++){
        c2->cd(ipad+1);
        setPad(0.12, 0.08, 0.08, 0.12);
    }

    nColumns = 2;
    nRaws = 2;
    nPads = nColumns * nRaws;
    TCanvas* c3 = new TCanvas("c3", "c3", 1200, 900);
    c3->Divide(nColumns, nRaws);
    for(Int_t ipad=0; ipad<nPads; ipad++){
        c3->cd(ipad+1);
        setPad(0.12, 0.08, 0.08, 0.12);
        gPad->Clear();
    }

    TH1D *hnEvts = (TH1D *)f->Get("hnEvts");
    hnEvts->Scale(1.e4/hnEvts->GetBinContent(1));

    TH1D *hCen_MB = (TH1D *)f->Get("hCen_MB");
    TH3D *hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen = (TH3D *)f->Get("hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen");
    TH2D *hMaxL1HFAdcPlusVsCen  = (TH2D *)f->Get("hMaxL1HFAdcPlusVsCen");
    TH2D *hMaxL1HFAdcMinusVsCen = (TH2D *)f->Get("hMaxL1HFAdcMinusVsCen");

    TH1D *hCen_HFAnd[nHiHFAdcThs];
    TH1D *hCen_HFAnd_AND_ZDCAnd[nLowHFAdcThs];
    TH1D *hCen_HFAnd_OR_ZDCAnd[nNeuThs];
    TH1D *hCen_MB_HFAnd[nHiHFAdcThs];
    TH1D *hCen_MB_HFAnd_AND_ZDCAnd[nLowHFAdcThs];
    TH1D *hCen_MB_HFAnd_OR_ZDCAnd[nNeuThs];
    for(Int_t idx=0; idx<nHiHFAdcThs; idx++){
        hCen_HFAnd[idx] = (TH1D *)f->Get(Form("hCen_HFAnd_AdcTh%d", mHiHFAdcTh[idx]));
        hCen_MB_HFAnd[idx] = (TH1D *)f->Get(Form("hCen_MB_HFAnd_AdcTh%d", mHiHFAdcTh[idx]));
    }
    for(Int_t idx=0; idx<nLowHFAdcThs; idx++){
        hCen_HFAnd_AND_ZDCAnd[idx] = (TH1D *)f->Get(Form("hCen_HFAnd_AND_ZDCAnd_AdcTh%d_2n", mLowHFAdcTh[idx]));
        hCen_MB_HFAnd_AND_ZDCAnd[idx] = (TH1D *)f->Get(Form("hCen_MB_HFAnd_AND_ZDCAnd_AdcTh%d_2n", mLowHFAdcTh[idx]));
    }
    for(Int_t idx=0; idx<nNeuThs; idx++){
        hCen_HFAnd_OR_ZDCAnd[idx] = (TH1D *)f->Get(Form("hCen_HFAnd_OR_ZDCAnd_AdcTh19_%dn", mNeuTh[idx]));
        hCen_MB_HFAnd_OR_ZDCAnd[idx] = (TH1D *)f->Get(Form("hCen_MB_HFAnd_OR_ZDCAnd_AdcTh19_%dn", mNeuTh[idx]));
    }

    c1->cd();
    setPad(0.12, 0.08, 0.08, 0.2);
    gPad->SetLogy(1);
    hnEvts->GetXaxis()->SetBinLabel(1, "Zero-bias");
    for(Int_t idx=0; idx<nHiHFAdcThs; idx++)  hnEvts->GetXaxis()->SetBinLabel(2+idx, Form("#color[2]{HFAndAdcTh%d}", mHiHFAdcTh[idx]));
    for(Int_t idx=0; idx<nLowHFAdcThs; idx++) hnEvts->GetXaxis()->SetBinLabel(2+nHiHFAdcThs+idx, Form("#color[4]{HFAndAdcTh%d & ZDCAnd2n}", mLowHFAdcTh[idx]));
    for(Int_t idx=0; idx<nNeuThs; idx++)      hnEvts->GetXaxis()->SetBinLabel(2+nHiHFAdcThs+nLowHFAdcThs+idx, Form("#color[6]{HFAndAdcTh19 || ZDCAnd%dn}", mNeuTh[idx]));
    hnEvts->GetXaxis()->SetBinLabel(2+nHiHFAdcThs+nLowHFAdcThs+nNeuThs, "#color[8]{Collision}");
    hnEvts->SetLineWidth(2);
    hnEvts->GetXaxis()->SetRangeUser(0, 30);
    hnEvts->GetXaxis()->LabelsOption("d");
    hnEvts->GetXaxis()->SetLabelSize(0.025);
    hnEvts->GetYaxis()->SetRangeUser(1, 1e5);
    hnEvts->GetYaxis()->SetTitle("Rate / ZB Rate #times 10^{4}");
    hnEvts->SetMarkerSize(1.6);
    hnEvts->SetMarkerColor(kGreen+3);
    hnEvts->Draw("histtext");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.05, 1);
    drawLatex(0.604, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.05, 1);
    c1->SaveAs(Form("%s/trigRate.pdf", dirName.Data()));

    c1->cd();
    setPad(0.12, 0.08, 0.08, 0.12);

    TH1D *hCen_MB_Extro = (TH1D *)hCen_MB->Clone("hCen_MB_Extro");
    Int_t inteBinLow = 1;
    Int_t inteBinHi  = hCen_MB_Extro->GetXaxis()->FindBin(160-mTinyNum);
    Double_t binCon = hCen_MB_Extro->Integral(inteBinLow, inteBinHi);
    binCon /= (inteBinHi - inteBinLow + 1);
    for(Int_t i=inteBinHi+1; i<=hCen_MB_Extro->GetNbinsX(); i++){
        hCen_MB_Extro->SetBinContent(i, binCon);
        hCen_MB_Extro->SetBinError(i, sqrt(binCon));
    }

    const    Int_t mBoxColor = kGreen - 9;
    const Double_t mColorAlpha = 0.2;

    c1->cd();
    gPad->SetLogy(0);
    setHisto(hCen_MB, 20, 0.6, 12, 12);
    setHisto(hCen_MB_Extro, 24, 0.6, kRed-4, kRed-4);
    hCen_MB->SetMinimum(0);
    hCen_MB->GetXaxis()->SetTitleOffset(1.0);
    hCen_MB->GetYaxis()->SetTitle("Entries");
    hCen_MB->Draw("histe");
    hCen_MB_Extro->Draw("psame");
    Double_t yCenter = hCen_MB->GetMaximum()/2.;
    Double_t yErr = yCenter;
    drawBox(180, yCenter, 20, yErr, 20, yErr, mBoxColor, 1, 1001, mColorAlpha);
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.05, 1);
    drawLatex(0.604, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.05, 1);
    drawLatex(0.15, 0.84, "Offline MB selection", mFont, 0.045, 1);
    c1->SaveAs(Form("%s/offlineMBCen.pdf", dirName.Data()));

    const Int_t nCenBins = 6;
    const Int_t mCenLow[nCenBins] = { 0, 30, 60, 70, 80,  90};
    const Int_t mCenHi[nCenBins]  = {30, 60, 70, 80, 90, 100};

    const Int_t    mMStyle[nCenBins] = {20, 24, 20, 24, 20, 24};
    const Double_t mMSize[nCenBins] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    const Int_t    mColor[nCenBins] = {kRed-4, kBlue-4, kMagenta-4, kAzure+8, kGreen+3, kOrange+1};

    TH1D *hMaxL1HFAdcPlus_CenBin[nCenBins];
    TH1D *hMaxL1HFAdcMinus_CenBin[nCenBins];
    TH2D *hMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin[nCenBins];
    for(Int_t icen=0; icen<nCenBins; icen++){
        Int_t cenBinLow = hMaxL1HFAdcPlusVsCen->GetXaxis()->FindBin(mCenLow[icen]*2+mTinyNum);
        Int_t cenBinHi  = hMaxL1HFAdcPlusVsCen->GetXaxis()->FindBin(mCenHi[icen]*2-mTinyNum);
        hMaxL1HFAdcPlus_CenBin[icen]  = (TH1D *)hMaxL1HFAdcPlusVsCen->ProjectionY(Form("hMaxL1HFAdcPlus_CenBin%d", icen), cenBinLow, cenBinHi);
        hMaxL1HFAdcMinus_CenBin[icen] = (TH1D *)hMaxL1HFAdcMinusVsCen->ProjectionY(Form("hMaxL1HFAdcMinus_CenBin%d", icen), cenBinLow, cenBinHi);

        cenBinLow = hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen->GetXaxis()->FindBin(mCenLow[icen]*2+mTinyNum);
        cenBinHi  = hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen->GetXaxis()->FindBin(mCenHi[icen]*2-mTinyNum);
        hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen->GetXaxis()->SetRange(cenBinLow, cenBinHi);
        hMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin[icen] = (TH2D *)hMaxL1HFAdcMinusVsMaxL1HFAdcPlusVsCen->Project3D(Form("ihMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin%d_zy", icen));
    }

    TLegend *leg = new TLegend(0.64, 0.5, 0.9, 0.86);
    leg->SetFillStyle(0);
    leg->SetFillColor(10);
    leg->SetTextFont(mFont);
    leg->SetTextSize(0.05);

    c2->cd(1);
    hMaxL1HFAdcPlusVsCen->Draw("col");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    drawLatex(0.15, 0.84, "Offline MB selection", mFont, 0.06, 1);
    c2->cd(2);
    for(Int_t icen=0; icen<nCenBins; icen++){
        setHisto(hMaxL1HFAdcPlus_CenBin[icen], mMStyle[icen], mMSize[icen], mColor[icen], mColor[icen], 1);
        if(icen==0){
            hMaxL1HFAdcPlus_CenBin[icen]->GetYaxis()->SetTitle("Entries");
            hMaxL1HFAdcPlus_CenBin[icen]->Draw("histe");
        }
        else{
            hMaxL1HFAdcPlus_CenBin[icen]->Draw("histesame");
        }
        leg->AddEntry(hMaxL1HFAdcPlus_CenBin[icen], Form("%d-%d%%", mCenLow[icen], mCenHi[icen]), "pzl");
    }
    leg->Draw("same");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    c2->SaveAs(Form("%s/maxL1HFAdcPlusVsCen.pdf", dirName.Data()));

    c2->cd(1);
    hMaxL1HFAdcMinusVsCen->Draw("col");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    drawLatex(0.15, 0.84, "Offline MB selection", mFont, 0.06, 1);
    c2->cd(2);
    for(Int_t icen=0; icen<nCenBins; icen++){
        setHisto(hMaxL1HFAdcMinus_CenBin[icen], mMStyle[icen], mMSize[icen], mColor[icen], mColor[icen], 1);
        if(icen==0){
            hMaxL1HFAdcMinus_CenBin[icen]->GetYaxis()->SetTitle("Entries");
            hMaxL1HFAdcMinus_CenBin[icen]->Draw("histe");
        }
        else{
            hMaxL1HFAdcMinus_CenBin[icen]->Draw("histesame");
        }
    }
    leg->Draw("same");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    c2->SaveAs(Form("%s/maxL1HFAdcMinusVsCen.pdf", dirName.Data()));

    const Int_t nHFAdcThs = 40; // Note the bin width of hMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin[icen] must be one in the following block
    TGraphErrors *grTrigEffVsHFAdcTh_CenBin[nCenBins];
    for(Int_t icen=0; icen<nCenBins; icen++){
        grTrigEffVsHFAdcTh_CenBin[icen] = new TGraphErrors(nHFAdcThs);
        grTrigEffVsHFAdcTh_CenBin[icen]->SetName(Form("grTrigEffVsHFAdcTh_CenBin%d", icen));

        Int_t nBinsX = hMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin[icen]->GetNbinsX();
        Int_t nBinsY = hMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin[icen]->GetNbinsY();
        for(Int_t idx=0; idx<nHFAdcThs; idx++){
            Double_t nDen, nDenErr, nNum, nNumErr, ratio, ratioErr;
            nDen  = hMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin[icen]->IntegralAndError(1, nBinsX, 1, nBinsY, nDenErr);
            nNum  = hMaxL1HFAdcMinusVsMaxL1HFAdcPlus_CenBin[icen]->IntegralAndError(idx+2, nBinsX, idx+2, nBinsY, nNumErr);

            ratio = nNum/nDen;
            ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nDenErr,2))/pow(nDen,2) );
            ratioErr = sqrt(ratioErr);

            grTrigEffVsHFAdcTh_CenBin[icen]->SetPoint(idx, idx+0.5, ratio);
            grTrigEffVsHFAdcTh_CenBin[icen]->SetPointError(idx, 0.5, ratioErr);
        }
    }

    TH2D *dHFEff = (TH2D *)histo("fHFEff", 200, 0, 40, 120, 0, 1.2, "L1 HF threshold (ADC)", "Trigger Efficiency");
    //dHFEff->GetXaxis()->SetTitleSize(0.055);
    dHFEff->GetXaxis()->SetTitleOffset(1.0);
    //dHFEff->GetXaxis()->SetLabelSize(0.045);
    //dHFEff->GetYaxis()->SetTitleSize(0.055);
    //dHFEff->GetYaxis()->SetTitleOffset(1);
    //dHFEff->GetYaxis()->SetLabelSize(0.045);
    //dHFEff->GetXaxis()->CenterTitle(kTRUE);
    //dHFEff->GetYaxis()->CenterTitle(kTRUE);
    //dHFEff->GetXaxis()->SetNdivisions(512);

    c1->cd();
    gPad->SetLogy(0);
    dHFEff->Draw("c");
    drawBox(12.5, 0.5, 0.5, 0.5, 0.5, 0.5, mBoxColor, 1, 1001, mColorAlpha);
    drawBox(15.5, 0.5, 0.5, 0.5, 0.5, 0.5, mBoxColor, 1, 1001, mColorAlpha);
    drawBox(19.5, 0.5, 0.5, 0.5, 0.5, 0.5, mBoxColor, 1, 1001, mColorAlpha);
    setLegend(leg, 0.15, 0.2, 0.35, 0.5, 0.045);
    for(Int_t icen=0; icen<nCenBins; icen++){
        setGraph(grTrigEffVsHFAdcTh_CenBin[icen], 25, 0.9, mColor[icen], mColor[icen], 2);
        grTrigEffVsHFAdcTh_CenBin[icen]->Draw("pzsame");
        leg->AddEntry(grTrigEffVsHFAdcTh_CenBin[icen], Form("%d-%d%%", mCenLow[icen], mCenHi[icen]), "pzl");
    }
    leg->Draw("same");
    drawLatex(0.12, 0.94, "Zero-bias Data", mFont, 0.05, 1);
    drawLatex(0.604, 0.94, "2018 PbPb 5.02 TeV", mFont, 0.05, 1);
    drawLatex(0.15, 0.84, "Offline MB selection", mFont, 0.045, 1);
    drawLatex(0.78, 0.84, "HF And", mFont, 0.045, 1);
    c1->SaveAs(Form("%s/HFTrigEffVsAdcTh.pdf", dirName.Data()));

    Int_t hfAndIdx0 = 4; // L1 ADC threshold is 15 for HFAnd Logic
    Int_t hfAndIdx1 = 8; // L1 ADC threshold is 19 for HFAnd Logic
    Int_t hfAndZdcIdx = 3; // L1 ADC threshold is 12 for HFAnd & ZDCAnd Logic
    Int_t hfOrZdcIdx = 2; // neutron threshold is 3 for HFAnd OR ZDCAnd Logic
    c2->cd(1);
    setHisto(hCen_MB, 20, 0.6, 12, 12);
    setHisto(hCen_MB_HFAnd[hfAndIdx0], 20, 0.6, kRed-4, kRed-4);
    setHisto(hCen_MB_HFAnd[hfAndIdx1], 20, 0.6, kGreen+3, kGreen+3);
    setHisto(hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx], 20, 0.6, kBlue-4, kBlue-4);
    setHisto(hCen_MB_HFAnd_OR_ZDCAnd[hfOrZdcIdx], 20, 0.6, kOrange+1, kOrange+1);
    hCen_MB->SetMinimum(0);
    hCen_MB->GetXaxis()->SetTitleOffset(1.0);
    hCen_MB->GetYaxis()->SetTitle("Entries");
    hCen_MB->Draw("histe");
    drawBox(180, yCenter, 20, yErr, 20, yErr, mBoxColor, 1, 1001, mColorAlpha);
    hCen_MB_HFAnd[hfAndIdx0]->Draw("histesame");
    hCen_MB_HFAnd[hfAndIdx1]->Draw("histesame");
    hCen_MB_HFAnd_OR_ZDCAnd[hfOrZdcIdx]->Draw("histesame");
    hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->Draw("histesame");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    setLegend(leg, 0.15, 0.61, 0.3, 0.88, 0.048);
    leg->AddEntry(hCen_MB, "OfflineMB", "pl");
    leg->AddEntry(hCen_MB_HFAnd[hfAndIdx0], "OfflineMB & HFAnd(15)", "pl");
    leg->AddEntry(hCen_MB_HFAnd[hfAndIdx1], "OfflineMB & HFAnd(19)", "pl");
    leg->AddEntry(hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx], "OfflineMB & HFAnd(12) & ZDCAnd(2n)", "pl");
    leg->AddEntry(hCen_MB_HFAnd_OR_ZDCAnd[hfOrZdcIdx], "OfflineMB & (HFAnd(19) || ZDCAnd(3n))", "pl");
    leg->DrawClone("same");
    c2->cd(2);
    gPad->SetLogy(0);
    gPad->Clear();
    TH1D *hHFTrigEffVsCen_AdcTh15 = (TH1D *)hCen_MB_HFAnd[hfAndIdx0]->Clone("hHFTrigEffVsCen_AdcTh15");
    TH1D *hHFTrigEffVsCen_AdcTh19 = (TH1D *)hCen_MB_HFAnd[hfAndIdx1]->Clone("hHFTrigEffVsCen_AdcTh19");
    TH1D *hHFANDZDCTrigEffVsCen_AdcTh12_2n = (TH1D *)hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->Clone("hHFANDZDCTrigEffVsCen_AdcTh12_2n");
    TH1D *hHFORZDCTrigEffVsCen_AdcTh19_3n = (TH1D *)hCen_MB_HFAnd_OR_ZDCAnd[hfOrZdcIdx]->Clone("hHFORZDCTrigEffVsCen_AdcTh19_3n");
    hHFTrigEffVsCen_AdcTh15->Divide(hHFTrigEffVsCen_AdcTh15, hCen_MB, 1, 1, "B");
    hHFTrigEffVsCen_AdcTh19->Divide(hHFTrigEffVsCen_AdcTh19, hCen_MB, 1, 1, "B");
    hHFANDZDCTrigEffVsCen_AdcTh12_2n->Divide(hHFANDZDCTrigEffVsCen_AdcTh12_2n, hCen_MB, 1, 1, "B");
    hHFORZDCTrigEffVsCen_AdcTh19_3n->Divide(hHFORZDCTrigEffVsCen_AdcTh19_3n, hCen_MB, 1, 1, "B");
    setHisto(hHFTrigEffVsCen_AdcTh15, 25, 1, kRed-4, kRed-4);
    setHisto(hHFTrigEffVsCen_AdcTh19, 25, 1, kGreen+3, kGreen+3);
    setHisto(hHFANDZDCTrigEffVsCen_AdcTh12_2n, 25, 1, kBlue-4, kBlue-4);
    setHisto(hHFORZDCTrigEffVsCen_AdcTh19_3n, 25, 1, kOrange+1, kOrange+1);
    hHFTrigEffVsCen_AdcTh15->GetXaxis()->SetRangeUser(120, 200);
    hHFTrigEffVsCen_AdcTh15->GetYaxis()->SetRangeUser(0, 1.2);
    hHFTrigEffVsCen_AdcTh15->GetYaxis()->SetTitle("Trigger Efficiency");
    hHFTrigEffVsCen_AdcTh15->Draw("p");
    drawBox(180, 0.6, 20, 0.6, 20, 0.6, mBoxColor, 1, 1001, mColorAlpha);
    setLegend(leg, 0.15, 0.18, 0.3, 0.40, 0.05);
    hHFTrigEffVsCen_AdcTh19->Draw("psame");
    hHFORZDCTrigEffVsCen_AdcTh19_3n->Draw("psame");
    hHFANDZDCTrigEffVsCen_AdcTh12_2n->Draw("psame");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    leg->AddEntry(hHFTrigEffVsCen_AdcTh15, "HFAnd(15)", "pl");
    leg->AddEntry(hHFTrigEffVsCen_AdcTh19, "HFAnd(19)", "pl");
    leg->AddEntry(hHFANDZDCTrigEffVsCen_AdcTh12_2n, "HFAnd(12) & ZDCAnd(2n)", "pl");
    leg->AddEntry(hHFORZDCTrigEffVsCen_AdcTh19_3n, "HFAnd(19) || ZDCAnd(3n)", "pl");
    leg->Draw("same");
    c2->SaveAs(Form("%s/TrigEffVsCen.pdf", dirName.Data()));

    TH1D *hTrueHFTrigEffVsCen_AdcTh15 = (TH1D *)hCen_MB_HFAnd[hfAndIdx0]->Clone("hTrueHFTrigEffVsCen_AdcTh15");
    TH1D *hTrueHFTrigEffVsCen_AdcTh19 = (TH1D *)hCen_MB_HFAnd[hfAndIdx1]->Clone("hTrueHFTrigEffVsCen_AdcTh19");
    TH1D *hTrueHFANDZDCTrigEffVsCen_AdcTh12_2n = (TH1D *)hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->Clone("hTrueHFANDZDCTrigEffVsCen_AdcTh12_2n");
    TH1D *hTrueHFORZDCTrigEffVsCen_AdcTh19_3n = (TH1D *)hCen_MB_HFAnd_OR_ZDCAnd[hfOrZdcIdx]->Clone("hTrueHFORZDCTrigEffVsCen_AdcTh19_3n");
    hTrueHFTrigEffVsCen_AdcTh15->Divide(hTrueHFTrigEffVsCen_AdcTh15, hCen_MB_Extro, 1, 1, "B");
    hTrueHFTrigEffVsCen_AdcTh19->Divide(hTrueHFTrigEffVsCen_AdcTh19, hCen_MB_Extro, 1, 1, "B");
    hTrueHFANDZDCTrigEffVsCen_AdcTh12_2n->Divide(hTrueHFANDZDCTrigEffVsCen_AdcTh12_2n, hCen_MB_Extro, 1, 1, "B");
    hTrueHFORZDCTrigEffVsCen_AdcTh19_3n->Divide(hTrueHFORZDCTrigEffVsCen_AdcTh19_3n, hCen_MB_Extro, 1, 1, "B");
    setHisto(hTrueHFTrigEffVsCen_AdcTh15, 25, 1, kRed-4, kRed-4);
    setHisto(hTrueHFTrigEffVsCen_AdcTh19, 25, 1, kGreen+3, kGreen+3);
    setHisto(hTrueHFANDZDCTrigEffVsCen_AdcTh12_2n, 25, 1, kBlue-4, kBlue-4);
    setHisto(hTrueHFORZDCTrigEffVsCen_AdcTh19_3n, 25, 1, kOrange+1, kOrange+1);
    c1->cd();
    hTrueHFTrigEffVsCen_AdcTh15->GetXaxis()->SetRangeUser(120, 200);
    hTrueHFTrigEffVsCen_AdcTh15->GetYaxis()->SetRangeUser(0, 1.2);
    hTrueHFTrigEffVsCen_AdcTh15->GetYaxis()->SetTitle("Trigger Efficiency");
    hTrueHFTrigEffVsCen_AdcTh15->Draw("p");
    drawBox(180, 0.6, 20, 0.6, 20, 0.6, mBoxColor, 1, 1001, mColorAlpha);
    setLegend(leg, 0.15, 0.18, 0.3, 0.40, 0.05);
    hTrueHFTrigEffVsCen_AdcTh19->Draw("psame");
    hTrueHFORZDCTrigEffVsCen_AdcTh19_3n->Draw("psame");
    hTrueHFANDZDCTrigEffVsCen_AdcTh12_2n->Draw("psame");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    leg->AddEntry(hTrueHFTrigEffVsCen_AdcTh15, "HFAnd(15)", "pl");
    leg->AddEntry(hTrueHFTrigEffVsCen_AdcTh19, "HFAnd(19)", "pl");
    leg->AddEntry(hTrueHFANDZDCTrigEffVsCen_AdcTh12_2n, "HFAnd(12) & ZDCAnd(2n)", "pl");
    leg->AddEntry(hTrueHFORZDCTrigEffVsCen_AdcTh19_3n, "HFAnd(19) || ZDCAnd(3n)", "pl");
    leg->Draw("same");
    drawLatex(0.15, 0.56, "Offline MB in 80-100% is extroplated", mFont, 0.04, 1);
    drawLatex(0.15, 0.51, "by using 0-80% data", mFont, 0.04, 1);
    c1->SaveAs(Form("%s/TrueTrigEffVsCen.pdf", dirName.Data()));

    hCen_HFAnd[hfAndIdx0]->GetYaxis()->SetRangeUser(1, 1.e4);
    hCen_HFAnd[hfAndIdx1]->GetYaxis()->SetRangeUser(1, 1.e4);
    hCen_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->GetYaxis()->SetRangeUser(1, 1.e4);

    c3->cd(1);
    gPad->SetLogy(1);
    setHisto(hCen_HFAnd[hfAndIdx0], 21, 0.8, 12, 12, 2);
    setHisto(hCen_MB_HFAnd[hfAndIdx0], 25, 1.0, kRed-4, kRed-4, 2);
    hCen_HFAnd[hfAndIdx0]->GetYaxis()->SetTitle("Entries");
    hCen_HFAnd[hfAndIdx0]->Draw("p");
    hCen_MB_HFAnd[hfAndIdx0]->Draw("psame");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    setLegend(leg, 0.15, 0.2, 0.36, 0.32, 0.06);
    leg->AddEntry(hCen_HFAnd[hfAndIdx0], "HFAnd(15)", "pl");
    leg->AddEntry(hCen_MB_HFAnd[hfAndIdx0], "HFAnd(15) & OfflineMB", "pl");
    leg->DrawClone("same");
    c3->cd(2);
    gPad->SetLogy(1);
    setHisto(hCen_HFAnd[hfAndIdx1], 21, 0.8, 12, 12, 2);
    setHisto(hCen_MB_HFAnd[hfAndIdx1], 25, 1.0, kGreen+3, kGreen+3, 2);
    hCen_HFAnd[hfAndIdx1]->GetYaxis()->SetTitle("Entries");
    hCen_HFAnd[hfAndIdx1]->SetMinimum(1);
    hCen_HFAnd[hfAndIdx1]->Draw("p");
    hCen_MB_HFAnd[hfAndIdx1]->Draw("psame");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    leg->Clear();
    leg->AddEntry(hCen_HFAnd[hfAndIdx1], "HFAnd(19)", "pl");
    leg->AddEntry(hCen_MB_HFAnd[hfAndIdx1], "HFAnd(19) & OfflineMB", "pl");
    leg->DrawClone("same");
    c3->cd(3);
    gPad->SetLogy(1);
    setHisto(hCen_HFAnd_AND_ZDCAnd[hfAndZdcIdx], 21, 0.8, 12, 12, 2);
    setHisto(hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx], 25, 1.0, kBlue-4, kBlue-4, 2);
    hCen_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->GetYaxis()->SetTitle("Entries");
    hCen_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->SetMinimum(1);
    hCen_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->Draw("p");
    hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->Draw("psame");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    leg->Clear();
    leg->AddEntry(hCen_HFAnd_AND_ZDCAnd[hfAndZdcIdx], "HFAnd(12) & ZDCAnd(2n)", "pl");
    leg->AddEntry(hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx], "HFAnd(12) & ZDCAnd(2n) & OfflineMB", "pl");
    leg->DrawClone("same");
    c3->cd(4);
    TH1D *hHFTrigPurityVsCen_AdcTh15 = (TH1D *)hCen_MB_HFAnd[hfAndIdx0]->Clone("hHFTrigPurityVsCen_AdcTh15");
    TH1D *hHFTrigPurityVsCen_AdcTh19 = (TH1D *)hCen_MB_HFAnd[hfAndIdx1]->Clone("hHFTrigPurityVsCen_AdcTh19");
    TH1D *hHFZDCTrigPurityVsCen_AdcTh12_2n = (TH1D *)hCen_MB_HFAnd_AND_ZDCAnd[hfAndZdcIdx]->Clone("hHFZDCTrigPurityVsCen_AdcTh12_2n");
    hHFTrigPurityVsCen_AdcTh15->Divide(hHFTrigPurityVsCen_AdcTh15, hCen_HFAnd[hfAndIdx0], 1, 1, "B");
    hHFTrigPurityVsCen_AdcTh19->Divide(hHFTrigPurityVsCen_AdcTh19, hCen_HFAnd[hfAndIdx1], 1, 1, "B");
    hHFZDCTrigPurityVsCen_AdcTh12_2n->Divide(hHFZDCTrigPurityVsCen_AdcTh12_2n, hCen_HFAnd_AND_ZDCAnd[hfAndZdcIdx], 1, 1, "B");
    setHisto(hHFTrigPurityVsCen_AdcTh15, 25, 1, kRed-4, kRed-4);
    setHisto(hHFTrigPurityVsCen_AdcTh19, 25, 1, kGreen+3, kGreen+3);
    setHisto(hHFZDCTrigPurityVsCen_AdcTh12_2n, 25, 1, kBlue-4, kBlue-4);
    hHFTrigPurityVsCen_AdcTh15->GetXaxis()->SetRangeUser(120, 200);
    hHFTrigPurityVsCen_AdcTh15->GetYaxis()->SetRangeUser(0, 1.2);
    hHFTrigPurityVsCen_AdcTh15->GetYaxis()->SetTitle("Trigger Purity");
    hHFTrigPurityVsCen_AdcTh15->Draw("p");
    hHFTrigPurityVsCen_AdcTh19->Draw("psame");
    hHFZDCTrigPurityVsCen_AdcTh12_2n->Draw("psame");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.06, 1);
    drawLatex(0.55, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.06, 1);
    setLegend(leg, 0.15, 0.2, 0.3, 0.4, 0.06);
    leg->AddEntry(hHFTrigPurityVsCen_AdcTh15, "HFAnd(15)", "pl");
    leg->AddEntry(hHFTrigPurityVsCen_AdcTh19, "HFAnd(19)", "pl");
    leg->AddEntry(hHFZDCTrigPurityVsCen_AdcTh12_2n, "HFAnd(12) & ZDCAnd(2n)", "pl");
    leg->DrawClone("same");
    c3->SaveAs(Form("%s/TrigPurityVsCen.pdf", dirName.Data()));

    TGraphErrors *grHFTrigEffVsRate[nHiHFAdcThs];
    TGraphErrors *grHFANDZDCTrigEffVsRate[nLowHFAdcThs];
    TGraphErrors *grHFORZDCTrigEffVsRate[nNeuThs];

    TGraphErrors *grTrueHFTrigEffVsRate[nHiHFAdcThs];
    TGraphErrors *grTrueHFANDZDCTrigEffVsRate[nLowHFAdcThs];
    TGraphErrors *grTrueHFORZDCTrigEffVsRate[nNeuThs];

    int periCenLow = 85;
    int periCenHi  = 95;
    int periCenBinLow = hCen_MB->GetXaxis()->FindBin(periCenLow*2 + mTinyNum);
    int periCenBinHi  = hCen_MB->GetXaxis()->FindBin(periCenHi*2 - mTinyNum);

    double nDen, nDenErr;
    nDen  = hCen_MB->IntegralAndError(periCenBinLow, periCenBinHi, nDenErr);

    double nTrueDen, nTrueDenErr;
    nTrueDen  = hCen_MB_Extro->IntegralAndError(periCenBinLow, periCenBinHi, nTrueDenErr);

    for(int idx=0; idx<nHiHFAdcThs; idx++){
        double rate = hnEvts->GetBinContent(2+idx);

        double nNum, nNumErr, ratio, ratioErr;
        nNum  = hCen_MB_HFAnd[idx]->IntegralAndError(periCenBinLow, periCenBinHi, nNumErr);

        ratio = nNum/nDen;
        ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nDenErr,2))/pow(nDen,2) );
        ratioErr = sqrt(ratioErr);

        grHFTrigEffVsRate[idx] = new TGraphErrors(1);
        grHFTrigEffVsRate[idx]->SetPoint(0, rate, ratio);
        grHFTrigEffVsRate[idx]->SetPointError(0, 0, ratioErr);

        ratio = nNum/nTrueDen;
        ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nTrueDenErr,2))/pow(nTrueDen,2) );
        ratioErr = sqrt(ratioErr);

        grTrueHFTrigEffVsRate[idx] = new TGraphErrors(1);
        grTrueHFTrigEffVsRate[idx]->SetPoint(0, rate, ratio);
        grTrueHFTrigEffVsRate[idx]->SetPointError(0, 0, ratioErr);
    }

    for(int idx=0; idx<nLowHFAdcThs; idx++){
        double rate = hnEvts->GetBinContent(2+nHiHFAdcThs+idx);

        double nNum, nNumErr, ratio, ratioErr;
        nNum  = hCen_MB_HFAnd_AND_ZDCAnd[idx]->IntegralAndError(periCenBinLow, periCenBinHi, nNumErr);

        ratio = nNum/nDen;
        ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nDenErr,2))/pow(nDen,2) );
        ratioErr = sqrt(ratioErr);

        grHFANDZDCTrigEffVsRate[idx] = new TGraphErrors(1);
        grHFANDZDCTrigEffVsRate[idx]->SetPoint(0, rate, ratio);
        grHFANDZDCTrigEffVsRate[idx]->SetPointError(0, 0, ratioErr);

        ratio = nNum/nTrueDen;
        ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nTrueDenErr,2))/pow(nTrueDen,2) );
        ratioErr = sqrt(ratioErr);

        grTrueHFANDZDCTrigEffVsRate[idx] = new TGraphErrors(1);
        grTrueHFANDZDCTrigEffVsRate[idx]->SetPoint(0, rate, ratio);
        grTrueHFANDZDCTrigEffVsRate[idx]->SetPointError(0, 0, ratioErr);
    }

    for(int idx=0; idx<nNeuThs; idx++){
        double rate = hnEvts->GetBinContent(2+nHiHFAdcThs+nLowHFAdcThs+idx);

        double nNum, nNumErr, ratio, ratioErr;
        nNum  = hCen_MB_HFAnd_OR_ZDCAnd[idx]->IntegralAndError(periCenBinLow, periCenBinHi, nNumErr);

        ratio = nNum/nDen;
        ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nDenErr,2))/pow(nDen,2) );
        ratioErr = sqrt(ratioErr);

        grHFORZDCTrigEffVsRate[idx] = new TGraphErrors(1);
        grHFORZDCTrigEffVsRate[idx]->SetPoint(0, rate, ratio);
        grHFORZDCTrigEffVsRate[idx]->SetPointError(0, 0, ratioErr);

        ratio = nNum/nTrueDen;
        ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nTrueDenErr,2))/pow(nTrueDen,2) );
        ratioErr = sqrt(ratioErr);

        grTrueHFORZDCTrigEffVsRate[idx] = new TGraphErrors(1);
        grTrueHFORZDCTrigEffVsRate[idx]->SetPoint(0, rate, ratio);
        grTrueHFORZDCTrigEffVsRate[idx]->SetPointError(0, 0, ratioErr);
    }

    TH2D *ddEffVsRate = (TH2D *)histo("ddEffVsRate", 200, 0, 100, 120, 0, 1.2, "Rate / ZB Rate #times 10^{4}", "Trigger Efficiency");
    ddEffVsRate->GetXaxis()->SetTitleSize(0.055);
    ddEffVsRate->GetXaxis()->SetTitleOffset(0.9);
    ddEffVsRate->GetXaxis()->SetLabelSize(0.045);
    ddEffVsRate->GetYaxis()->SetTitleSize(0.055);
    ddEffVsRate->GetYaxis()->SetTitleOffset(1);
    ddEffVsRate->GetYaxis()->SetLabelSize(0.045);
    ddEffVsRate->GetXaxis()->CenterTitle(kTRUE);
    ddEffVsRate->GetYaxis()->CenterTitle(kTRUE);
    ddEffVsRate->GetXaxis()->SetNdivisions(512);

    Int_t mHiHFAdcThIdxLow = 3;
    Int_t mLowHFAdcThIdxLow = 0;
    Int_t mNeuThIdx = 1;

    c1->cd();
    ddEffVsRate->GetXaxis()->SetRangeUser(15, 45);
    ddEffVsRate->Draw("c");
    for(Int_t idx=mHiHFAdcThIdxLow; idx<nHiHFAdcThs; idx++){
        setGraph(grHFTrigEffVsRate[idx], 24, 1.5, 2, 2, 2);
        grHFTrigEffVsRate[idx]->Draw("psame");
    }
    for(Int_t idx=mLowHFAdcThIdxLow; idx<nLowHFAdcThs; idx++){
        setGraph(grHFANDZDCTrigEffVsRate[idx], 20, 1.5, 4, 4, 2);
        grHFANDZDCTrigEffVsRate[idx]->Draw("psame");
    }
    for(Int_t idx=mNeuThIdx; idx<nNeuThs; idx++){
        setGraph(grHFORZDCTrigEffVsRate[idx], 25, 1.5, 6, 6, 2);
        grHFORZDCTrigEffVsRate[idx]->Draw("psame");
    }
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.05, 1);
    drawLatex(0.604, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.05, 1);
    drawLatex(0.15, 0.84, Form("Centrality: %d-%d%%", periCenLow, periCenHi), mFont, 0.05, 1);
    setLegend(leg, 0.36, 0.18, 0.64, 0.35, 0.04);
    TLegendEntry *legHFTrig       = leg->AddEntry(grHFTrigEffVsRate[nHiHFAdcThs-1], Form("HFAnd(%d-%d, step=1)", mHiHFAdcTh[mHiHFAdcThIdxLow], mHiHFAdcTh[nHiHFAdcThs-1]));
    TLegendEntry *legHFANDZDCTrig = leg->AddEntry(grHFANDZDCTrigEffVsRate[nLowHFAdcThs-1], Form("HFAnd(%d-%d, step=1) && ZDCAnd(2n)", mLowHFAdcTh[mLowHFAdcThIdxLow], mLowHFAdcTh[nLowHFAdcThs-1]));
    TLegendEntry *legHFORZDCTrig  = leg->AddEntry(grHFORZDCTrigEffVsRate[nNeuThs-1], Form("HFAnd(19) || ZDCAnd(%dn-%dn, step=1)", mNeuTh[mNeuThIdx], mNeuTh[nNeuThs-1]));
    leg->Draw("same");
    c1->SaveAs(Form("%s/TrigEffVsRate.pdf", dirName.Data()));

    c1->cd();
    ddEffVsRate->Draw("c");
    for(Int_t idx=mHiHFAdcThIdxLow; idx<nHiHFAdcThs; idx++){
        setGraph(grTrueHFTrigEffVsRate[idx], 24, 1.5, 2, 2, 2);
        grTrueHFTrigEffVsRate[idx]->Draw("psame");
    }
    for(Int_t idx=mLowHFAdcThIdxLow; idx<nLowHFAdcThs; idx++){
        setGraph(grTrueHFANDZDCTrigEffVsRate[idx], 20, 1.5, 4, 4, 2);
        grTrueHFANDZDCTrigEffVsRate[idx]->Draw("psame");
    }
    for(Int_t idx=mNeuThIdx; idx<nNeuThs; idx++){
        setGraph(grTrueHFORZDCTrigEffVsRate[idx], 25, 1.5, 6, 6, 2);
        grTrueHFORZDCTrigEffVsRate[idx]->Draw("psame");
    }
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.05, 1);
    drawLatex(0.604, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.05, 1);
    drawLatex(0.15, 0.84, Form("Centrality: %d-%d%%", periCenLow, periCenHi), mFont, 0.05, 1);
    leg->Draw("same");
    c1->SaveAs(Form("%s/TrueTrigEffVsRate.pdf", dirName.Data()));

    cout << "End of program !" << endl;
}
