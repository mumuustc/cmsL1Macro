#include "../common/headers.h"
#include "../common/function.C"
#include "hfConstant.h"

const Double_t mTinyNum = 1.e-6;

Int_t    mGenMarker = 20;
Int_t    mGenColor = 1;
Int_t    mGenWidth = 2;
Double_t mGenSize = 0.8;

Int_t  mColor[nMaxDepth] = {1, 2, 4, 6};

const Int_t mFont = 42;

void plotNoise(TString filename="L1ZeroBias")
{
    double minAdc=0, maxAdc=0, minFiberAdc=0, maxFiberAdc=0;
    double minNoiseAdc=0, maxNoiseAdc=30;
    double minCollisionAdc=0, maxCollisionAdc=100;
    double minTPEt=0, maxTPEt=30;
    if(filename.EqualTo("L1EmptyBX")){
        minAdc = 0;
        maxAdc = 30;
        minFiberAdc = 0;
        maxFiberAdc = 50;
    }
    else if(filename.EqualTo("L1ZeroBias")){
        minAdc = 0;
        maxAdc = 100;
        minFiberAdc = 0;
        maxFiberAdc = 200;

        minTPEt = 0;
        maxTPEt = 30;
    }
    else{
        cout<<"filename is wrong !!!"<<endl;
        return;
    }

    gStyle->SetOptFit(1111);

    //TFile *f = TFile::Open(Form("%s.root", filename.Data()));
    TFile *f = TFile::Open("HFNoise_lxplus.root");

    TH2D *hAdcvsTower_Noise[nMaxDepth];
    TH1D *hHFPlusAdc_Noise[nMaxDepth];
    TH1D *hHFMinusAdc_Noise[nMaxDepth];
    TH2D *hFiberAdcvsTower_Noise[nMaxFiber];
    TH1D *hHFPlusFiberAdc_Noise[nMaxDepth];
    TH1D *hHFMinusFiberAdc_Noise[nMaxDepth];

    TH2D *hAdcvsTower_Collision[nMaxDepth];
    TH1D *hHFPlusAdc_Collision[nMaxDepth];
    TH1D *hHFMinusAdc_Collision[nMaxDepth];
    TH2D *hFiberAdcvsTower_Collision[nMaxFiber];
    TH1D *hHFPlusFiberAdc_Collision[nMaxDepth];
    TH1D *hHFMinusFiberAdc_Collision[nMaxDepth];

    for(int idep=0; idep<nMaxDepth; idep++){
        hAdcvsTower_Noise[idep] = (TH2D *)f->Get(Form("hAdcvsTower_Noise_depthIdx%d", idep));
        hHFPlusAdc_Noise[idep]  = (TH1D *)hAdcvsTower_Noise[idep]->ProjectionY(Form("hHFPlusAdc_Noise_depthIdx%d", idep), 1, nMaxTowers/2);
        hHFMinusAdc_Noise[idep] = (TH1D *)hAdcvsTower_Noise[idep]->ProjectionY(Form("hHFMinusAdc_Noise_depthIdx%d", idep), nMaxTowers/2+1, nMaxTowers);

        hAdcvsTower_Collision[idep] = (TH2D *)f->Get(Form("hAdcvsTower_Collision_depthIdx%d", idep));
        hHFPlusAdc_Collision[idep]  = (TH1D *)hAdcvsTower_Collision[idep]->ProjectionY(Form("hHFPlusAdc_Collision_depthIdx%d", idep), 1, nMaxTowers/2);
        hHFMinusAdc_Collision[idep] = (TH1D *)hAdcvsTower_Collision[idep]->ProjectionY(Form("hHFMinusAdc_Collision_depthIdx%d", idep), nMaxTowers/2+1, nMaxTowers);
    }
    for(int ifib=0; ifib<nMaxFiber; ifib++){
        hFiberAdcvsTower_Noise[ifib] = (TH2D *)f->Get(Form("hFiberAdcvsTower_Noise_fiberIdx%d", ifib));
        hHFPlusFiberAdc_Noise[ifib]  = (TH1D *)hFiberAdcvsTower_Noise[ifib]->ProjectionY(Form("hHFPlusFiberAdc_Noise_fiberIdx%d", ifib), 1, nMaxTowers/2);
        hHFMinusFiberAdc_Noise[ifib] = (TH1D *)hFiberAdcvsTower_Noise[ifib]->ProjectionY(Form("hHFMinusFiberAdc_Noise_fiberIdx%d", ifib), nMaxTowers/2+1, nMaxTowers);

        hFiberAdcvsTower_Collision[ifib] = (TH2D *)f->Get(Form("hFiberAdcvsTower_Collision_fiberIdx%d", ifib));
        hHFPlusFiberAdc_Collision[ifib]  = (TH1D *)hFiberAdcvsTower_Collision[ifib]->ProjectionY(Form("hHFPlusFiberAdc_Collision_fiberIdx%d", ifib), 1, nMaxTowers/2);
        hHFMinusFiberAdc_Collision[ifib] = (TH1D *)hFiberAdcvsTower_Collision[ifib]->ProjectionY(Form("hHFMinusFiberAdc_Collision_fiberIdx%d", ifib), nMaxTowers/2+1, nMaxTowers);
    }

    TString dir = Form("noisePlots_%s", filename.Data());
    system(Form("mkdir -p %s", dir.Data()));
    system(Form("rm -rf %s/*", dir.Data()));

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);

    TCanvas* c2 = new TCanvas("c2", "c2", 0, 0, 800, 600);
    c2->Divide(2, 2);

    TCanvas* c3 = new TCanvas("c3", "c3", 800, 300);
    c3->Divide(2, 1);

    for(int idep=0; idep<nMaxDepth; idep++){
        c2->cd(idep+1);
        setPad(0.1, 0.12, 0.08, 0.12);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hAdcvsTower_Noise[idep]->GetYaxis()->SetRangeUser(minAdc, maxAdc);
        hAdcvsTower_Noise[idep]->GetYaxis()->SetTitleOffset(0.9);
        hAdcvsTower_Noise[idep]->GetXaxis()->SetTitleFont(mFont);
        hAdcvsTower_Noise[idep]->GetXaxis()->SetLabelFont(mFont);
        hAdcvsTower_Noise[idep]->Draw("colz");
        drawLine(-0.5, HFMinusAdcTh, nMaxTowers/2-0.5, HFMinusAdcTh, 2, 2, 2);
        drawLine(nMaxTowers/2-0.5, HFPlusAdcTh, nMaxTowers-0.5, HFPlusAdcTh, 2, 2, 2);
        drawLine(nMaxTowers/2-0.5, minAdc, nMaxTowers/2-0.5, maxAdc, 1, 2, 2);
        drawLatex(0.30, 0.95, Form("depth == %d (Noise)", idep+1), mFont, 0.07, 1);
        drawLatex(0.27, 0.8, "HF^{-}", mFont, 0.08, 1);
        drawLatex(0.66, 0.8, "HF^{+}", mFont, 0.08, 1);
    }
    c2->SaveAs(Form("%s/adcvsTower_Noise.pdf", dir.Data()));
    c2->SaveAs(Form("%s/adcvsTower_Noise.png", dir.Data()));

    for(int idep=0; idep<nMaxDepth; idep++){
        c2->cd(idep+1);
        setPad(0.1, 0.12, 0.08, 0.12);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hAdcvsTower_Collision[idep]->GetYaxis()->SetRangeUser(minAdc, maxAdc);
        hAdcvsTower_Collision[idep]->GetYaxis()->SetTitleOffset(0.9);
        hAdcvsTower_Collision[idep]->GetXaxis()->SetTitleFont(mFont);
        hAdcvsTower_Collision[idep]->GetXaxis()->SetLabelFont(mFont);
        hAdcvsTower_Collision[idep]->Draw("colz");
        drawLine(-0.5, HFMinusAdcTh, nMaxTowers/2-0.5, HFMinusAdcTh, 2, 2, 2);
        drawLine(nMaxTowers/2-0.5, HFPlusAdcTh, nMaxTowers-0.5, HFPlusAdcTh, 2, 2, 2);
        drawLine(nMaxTowers/2-0.5, minAdc, nMaxTowers/2-0.5, maxAdc, 1, 2, 2);
        drawLatex(0.30, 0.95, Form("depth == %d (Collision)", idep+1), mFont, 0.07, 1);
        drawLatex(0.27, 0.8, "HF^{-}", mFont, 0.08, 1);
        drawLatex(0.66, 0.8, "HF^{+}", mFont, 0.08, 1);
    }
    c2->SaveAs(Form("%s/adcvsTower_Collision.pdf", dir.Data()));
    c2->SaveAs(Form("%s/adcvsTower_Collision.png", dir.Data()));

    TLegend* leg = new TLegend(0.6, 0.68, 0.84, 0.88);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.05);

    c1->cd();
    gPad->SetLogy(1);
    for(int idep=0; idep<nMaxDepth; idep++){
        setHisto(hHFPlusAdc_Noise[idep], 20, 0.5, mColor[idep], mColor[idep], 2);
        hHFPlusAdc_Noise[idep]->GetXaxis()->SetRangeUser(minAdc, maxAdc);
        hHFPlusAdc_Noise[idep]->GetXaxis()->SetTitleFont(mFont);
        hHFPlusAdc_Noise[idep]->GetXaxis()->SetLabelFont(mFont);
        hHFPlusAdc_Noise[idep]->SetMinimum(2e-1);
        hHFPlusAdc_Noise[idep]->GetYaxis()->SetTitle("Entries");
        if(idep==0) hHFPlusAdc_Noise[idep]->Draw("hist");
        else        hHFPlusAdc_Noise[idep]->Draw("histsame");

        Int_t binLow = hHFPlusAdc_Noise[idep]->GetXaxis()->FindBin(HFPlusAdcTh + mTinyNum);
        Int_t binHi  = hHFPlusAdc_Noise[idep]->GetNbinsX()+1;
        Double_t ratio = hHFPlusAdc_Noise[idep]->Integral(binLow, binHi)*1./hHFPlusAdc_Noise[idep]->Integral(0, -1);
        //leg->AddEntry(hHFPlusAdc_Noise[idep], Form("depth == %d (%1.4f%%)", idep+1, ratio*100), "l");
        leg->AddEntry(hHFPlusAdc_Noise[idep], Form("depth == %d", idep+1), "l");
    }
    leg->Draw("same");
    drawLine(HFPlusAdcTh, 2e-1, HFPlusAdcTh, hHFPlusAdc_Noise[0]->GetMaximum()*1e-3, 1, 2, 2);
    drawLatex(0.4, 0.94, "HF^{+} (Noise)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFPlusAdc_Noise.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFPlusAdc_Noise.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(1);
    leg->Clear();
    for(int idep=0; idep<nMaxDepth; idep++){
        setHisto(hHFMinusAdc_Noise[idep], 20, 0.5, mColor[idep], mColor[idep], 2);
        hHFMinusAdc_Noise[idep]->GetXaxis()->SetRangeUser(minAdc, maxAdc);
        hHFMinusAdc_Noise[idep]->GetXaxis()->SetTitleFont(mFont);
        hHFMinusAdc_Noise[idep]->GetXaxis()->SetLabelFont(mFont);
        hHFMinusAdc_Noise[idep]->SetMinimum(2e-1);
        hHFMinusAdc_Noise[idep]->GetYaxis()->SetTitle("Entries");
        if(idep==0) hHFMinusAdc_Noise[idep]->Draw("hist");
        else        hHFMinusAdc_Noise[idep]->Draw("histsame");

        Int_t binLow = hHFMinusAdc_Noise[idep]->GetXaxis()->FindBin(HFMinusAdcTh + mTinyNum);
        Int_t binHi  = hHFMinusAdc_Noise[idep]->GetNbinsX()+1;
        Double_t ratio = hHFMinusAdc_Noise[idep]->Integral(binLow, binHi)*1./hHFMinusAdc_Noise[idep]->Integral(0, -1);
        //leg->AddEntry(hHFMinusAdc_Noise[idep], Form("depth == %d (%1.4f%%)", idep+1, ratio*100), "l");
        leg->AddEntry(hHFMinusAdc_Noise[idep], Form("depth == %d", idep+1), "l");
    }
    leg->Draw("same");
    drawLine(HFMinusAdcTh, 2e-1, HFMinusAdcTh, hHFMinusAdc_Noise[0]->GetMaximum()*1e-3, 1, 2, 2);
    drawLatex(0.4, 0.94, "HF^{-} (Noise)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFMinusAdc_Noise.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFMinusAdc_Noise.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(1);
    leg->Clear();
    for(int idep=0; idep<nMaxDepth; idep++){
        setHisto(hHFPlusAdc_Collision[idep], 20, 0.5, mColor[idep], mColor[idep], 2);
        hHFPlusAdc_Collision[idep]->GetXaxis()->SetRangeUser(minAdc, maxAdc);
        hHFPlusAdc_Collision[idep]->GetXaxis()->SetTitleFont(mFont);
        hHFPlusAdc_Collision[idep]->GetXaxis()->SetLabelFont(mFont);
        hHFPlusAdc_Collision[idep]->SetMinimum(2e-1);
        hHFPlusAdc_Collision[idep]->GetYaxis()->SetTitle("Entries");
        if(idep==0) hHFPlusAdc_Collision[idep]->Draw("hist");
        else        hHFPlusAdc_Collision[idep]->Draw("histsame");

        Int_t binLow = hHFPlusAdc_Collision[idep]->GetXaxis()->FindBin(HFPlusAdcTh + mTinyNum);
        Int_t binHi  = hHFPlusAdc_Collision[idep]->GetNbinsX()+1;
        Double_t ratio = hHFPlusAdc_Collision[idep]->Integral(binLow, binHi)*1./hHFPlusAdc_Collision[idep]->Integral(0, -1);
        //leg->AddEntry(hHFPlusAdc_Collision[idep], Form("depth == %d (%1.4f%%)", idep+1, ratio*100), "l");
        leg->AddEntry(hHFPlusAdc_Collision[idep], Form("depth == %d", idep+1), "l");
    }
    leg->Draw("same");
    drawLine(HFPlusAdcTh, 2e-1, HFPlusAdcTh, hHFPlusAdc_Collision[0]->GetMaximum()*0.5, 1, 2, 2);
    drawLatex(0.4, 0.94, "HF^{+} (Collision)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFPlusAdc_Collision.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFPlusAdc_Collision.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(1);
    leg->Clear();
    for(int idep=0; idep<nMaxDepth; idep++){
        setHisto(hHFMinusAdc_Collision[idep], 20, 0.5, mColor[idep], mColor[idep], 2);
        hHFMinusAdc_Collision[idep]->GetXaxis()->SetRangeUser(minAdc, maxAdc);
        hHFMinusAdc_Collision[idep]->GetXaxis()->SetTitleFont(mFont);
        hHFMinusAdc_Collision[idep]->GetXaxis()->SetLabelFont(mFont);
        hHFMinusAdc_Collision[idep]->SetMinimum(2e-1);
        hHFMinusAdc_Collision[idep]->GetYaxis()->SetTitle("Entries");
        if(idep==0) hHFMinusAdc_Collision[idep]->Draw("hist");
        else        hHFMinusAdc_Collision[idep]->Draw("histsame");

        Int_t binLow = hHFMinusAdc_Collision[idep]->GetXaxis()->FindBin(HFMinusAdcTh + mTinyNum);
        Int_t binHi  = hHFMinusAdc_Collision[idep]->GetNbinsX()+1;
        Double_t ratio = hHFMinusAdc_Collision[idep]->Integral(binLow, binHi)*1./hHFMinusAdc_Collision[idep]->Integral(0, -1);
        //leg->AddEntry(hHFMinusAdc_Collision[idep], Form("depth == %d (%1.4f%%)", idep+1, ratio*100), "l");
        leg->AddEntry(hHFMinusAdc_Collision[idep], Form("depth == %d", idep+1), "l");
    }
    leg->Draw("same");
    drawLine(HFMinusAdcTh, 2e-1, HFMinusAdcTh, hHFMinusAdc_Collision[0]->GetMaximum()*0.5, 1, 2, 2);
    drawLatex(0.4, 0.94, "HF^{-} (Collision)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFMinusAdc_Collision.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFMinusAdc_Collision.png", dir.Data()));

    setLegend(leg, 0.6, 0.74, 0.84, 0.84, 0.05);

    c1->cd();
    gPad->SetLogy(1);
    for(int ifib=0; ifib<nMaxFiber; ifib++){
        setHisto(hHFPlusFiberAdc_Noise[ifib], 20, 0.5, mColor[ifib], mColor[ifib], 2);
        hHFPlusFiberAdc_Noise[ifib]->GetXaxis()->SetRangeUser(minFiberAdc, maxFiberAdc);
        hHFPlusFiberAdc_Noise[ifib]->GetXaxis()->SetLabelFont(mFont);
        hHFPlusFiberAdc_Noise[ifib]->GetXaxis()->SetTitleFont(mFont);
        hHFPlusFiberAdc_Noise[ifib]->SetMinimum(2e-1);
        hHFPlusFiberAdc_Noise[ifib]->GetYaxis()->SetTitle("Entries");
        if(ifib==0) hHFPlusFiberAdc_Noise[ifib]->Draw("hist");
        else        hHFPlusFiberAdc_Noise[ifib]->Draw("histsame");

        leg->AddEntry(hHFPlusFiberAdc_Noise[ifib], Form("%s", fiberName[ifib].Data()), "l");
    }
    leg->Draw("same");
    drawLatex(0.40, 0.94, "HF^{+} (Noise)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFPlusFiberAdc_Noise.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFPlusFiberAdc_Noise.png", dir.Data()));

    for(int ifib=0; ifib<nMaxFiber; ifib++){
        c3->cd(ifib+1);
        setPad(0.1, 0.12, 0.08, 0.12);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hFiberAdcvsTower_Noise[ifib]->GetXaxis()->SetLabelFont(mFont);
        hFiberAdcvsTower_Noise[ifib]->GetXaxis()->SetTitleFont(mFont);
        hFiberAdcvsTower_Noise[ifib]->GetYaxis()->SetRangeUser(minFiberAdc, maxFiberAdc);
        hFiberAdcvsTower_Noise[ifib]->GetYaxis()->SetTitleOffset(0.9);
        hFiberAdcvsTower_Noise[ifib]->Draw("colz");
        drawLine(nMaxTowers/2-0.5, minFiberAdc, nMaxTowers/2-0.5, maxFiberAdc, 1, 2, 2);
        drawLatex(0.36, 0.95, Form("%s (Noise)", fiberName[ifib].Data()), mFont, 0.07, 1);
        drawLatex(0.27, 0.8, "HF^{-}", mFont, 0.08, 1);
        drawLatex(0.66, 0.8, "HF^{+}", mFont, 0.08, 1);
    }
    c3->SaveAs(Form("%s/fiberAdcvsTower_Noise.pdf", dir.Data()));
    c3->SaveAs(Form("%s/fiberAdcvsTower_Noise.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(1);
    for(int ifib=0; ifib<nMaxFiber; ifib++){
        setHisto(hHFMinusFiberAdc_Noise[ifib], 20, 0.5, mColor[ifib], mColor[ifib], 2);
        hHFMinusFiberAdc_Noise[ifib]->GetXaxis()->SetRangeUser(minFiberAdc, maxFiberAdc);
        hHFMinusFiberAdc_Noise[ifib]->GetXaxis()->SetLabelFont(mFont);
        hHFMinusFiberAdc_Noise[ifib]->GetXaxis()->SetTitleFont(mFont);
        hHFMinusFiberAdc_Noise[ifib]->SetMinimum(2e-1);
        hHFMinusFiberAdc_Noise[ifib]->GetYaxis()->SetTitle("Entries");
        if(ifib==0) hHFMinusFiberAdc_Noise[ifib]->Draw("hist");
        else        hHFMinusFiberAdc_Noise[ifib]->Draw("histsame");
    }
    leg->Draw("same");
    drawLatex(0.40, 0.94, "HF^{-} (Noise)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFMinusFiberAdc_Noise.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFMinusFiberAdc_Noise.png", dir.Data()));

    for(int ifib=0; ifib<nMaxFiber; ifib++){
        c3->cd(ifib+1);
        setPad(0.1, 0.12, 0.08, 0.12);
        gPad->SetLogy(0);
        gPad->SetLogz(1);
        hFiberAdcvsTower_Collision[ifib]->GetXaxis()->SetLabelFont(mFont);
        hFiberAdcvsTower_Collision[ifib]->GetXaxis()->SetTitleFont(mFont);
        hFiberAdcvsTower_Collision[ifib]->GetYaxis()->SetRangeUser(minFiberAdc, maxFiberAdc);
        hFiberAdcvsTower_Collision[ifib]->GetYaxis()->SetTitleOffset(0.9);
        hFiberAdcvsTower_Collision[ifib]->Draw("colz");
        drawLine(nMaxTowers/2-0.5, minFiberAdc, nMaxTowers/2-0.5, maxFiberAdc, 1, 2, 2);
        drawLatex(0.36, 0.95, Form("%s (Collision)", fiberName[ifib].Data()), mFont, 0.07, 1);
        drawLatex(0.27, 0.8, "HF^{-}", mFont, 0.08, 1);
        drawLatex(0.66, 0.8, "HF^{+}", mFont, 0.08, 1);
    }
    c3->SaveAs(Form("%s/fiberAdcvsTower_Collision.pdf", dir.Data()));
    c3->SaveAs(Form("%s/fiberAdcvsTower_Collision.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(1);
    for(int ifib=0; ifib<nMaxFiber; ifib++){
        setHisto(hHFPlusFiberAdc_Collision[ifib], 20, 0.5, mColor[ifib], mColor[ifib], 2);
        hHFPlusFiberAdc_Collision[ifib]->GetXaxis()->SetRangeUser(minFiberAdc, maxFiberAdc);
        hHFPlusFiberAdc_Collision[ifib]->GetXaxis()->SetLabelFont(mFont);
        hHFPlusFiberAdc_Collision[ifib]->GetXaxis()->SetTitleFont(mFont);
        hHFPlusFiberAdc_Collision[ifib]->SetMinimum(2e-1);
        hHFPlusFiberAdc_Collision[ifib]->GetYaxis()->SetTitle("Entries");
        if(ifib==0) hHFPlusFiberAdc_Collision[ifib]->Draw("hist");
        else        hHFPlusFiberAdc_Collision[ifib]->Draw("histsame");
    }
    leg->Draw("same");
    drawLatex(0.40, 0.94, "HF^{+} (Collision)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFPlusFiberAdc_Collision.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFPlusFiberAdc_Collision.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(1);
    for(int ifib=0; ifib<nMaxFiber; ifib++){
        setHisto(hHFMinusFiberAdc_Collision[ifib], 20, 0.5, mColor[ifib], mColor[ifib], 2);
        hHFMinusFiberAdc_Collision[ifib]->GetXaxis()->SetRangeUser(minFiberAdc, maxFiberAdc);
        hHFMinusFiberAdc_Collision[ifib]->GetXaxis()->SetLabelFont(mFont);
        hHFMinusFiberAdc_Collision[ifib]->GetXaxis()->SetTitleFont(mFont);
        hHFMinusFiberAdc_Collision[ifib]->SetMinimum(2e-1);
        hHFMinusFiberAdc_Collision[ifib]->GetYaxis()->SetTitle("Entries");
        if(ifib==0) hHFMinusFiberAdc_Collision[ifib]->Draw("hist");
        else        hHFMinusFiberAdc_Collision[ifib]->Draw("histsame");
    }
    leg->Draw("same");
    drawLatex(0.40, 0.94, "HF^{-} (Collision)", mFont, 0.06, 1);
    c1->SaveAs(Form("%s/HFMinusFiberAdc_Collision.pdf", dir.Data()));
    c1->SaveAs(Form("%s/HFMinusFiberAdc_Collision.png", dir.Data()));

    TH2D *hHCalTPCompEtvsTower_Noise     = (TH2D *)f->Get("hHCalTPCompEtvsTower_Noise");
    TH2D *hHCalTPCompEtvsTower_Collision = (TH2D *)f->Get("hHCalTPCompEtvsTower_Collision");

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hHCalTPCompEtvsTower_Noise->GetYaxis()->SetRangeUser(minTPEt, maxTPEt);
    hHCalTPCompEtvsTower_Noise->GetYaxis()->SetTitleOffset(0.9);
    hHCalTPCompEtvsTower_Noise->GetXaxis()->SetTitleFont(mFont);
    hHCalTPCompEtvsTower_Noise->GetXaxis()->SetLabelFont(mFont);
    hHCalTPCompEtvsTower_Noise->GetYaxis()->SetTitleFont(mFont);
    hHCalTPCompEtvsTower_Noise->GetYaxis()->SetLabelFont(mFont);
    hHCalTPCompEtvsTower_Noise->Draw("colz");
    drawLine(nMaxTowers/2-0.5, minTPEt, nMaxTowers/2-0.5, maxTPEt, 1, 2, 2);
    drawLatex(0.27, 0.8, "HF^{-}", mFont, 0.08, 1);
    drawLatex(0.66, 0.8, "HF^{+}", mFont, 0.08, 1);
    drawLatex(0.42, 0.94, "Noise", mFont, 0.07, 1);
    c1->SaveAs(Form("%s/hCalTPCompEtvsTower_Noise.pdf", dir.Data()));
    c1->SaveAs(Form("%s/hCalTPCompEtvsTower_Noise.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetRangeUser(minTPEt, maxTPEt);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetTitleOffset(0.9);
    hHCalTPCompEtvsTower_Collision->GetXaxis()->SetTitleFont(mFont);
    hHCalTPCompEtvsTower_Collision->GetXaxis()->SetLabelFont(mFont);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetTitleFont(mFont);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetLabelFont(mFont);
    hHCalTPCompEtvsTower_Collision->Draw("colz");
    drawLine(nMaxTowers/2-0.5, minTPEt, nMaxTowers/2-0.5, maxTPEt, 1, 2, 2);
    drawLatex(0.27, 0.8, "HF^{-}", mFont, 0.08, 1);
    drawLatex(0.66, 0.8, "HF^{+}", mFont, 0.08, 1);
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c1->SaveAs(Form("%s/hCalTPCompEtvsTower_Collision.pdf", dir.Data()));
    c1->SaveAs(Form("%s/hCalTPCompEtvsTower_Collision.png", dir.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetRangeUser(minTPEt, maxTPEt);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetTitleOffset(0.9);
    hHCalTPCompEtvsTower_Collision->GetXaxis()->SetTitleFont(mFont);
    hHCalTPCompEtvsTower_Collision->GetXaxis()->SetLabelFont(mFont);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetTitleFont(mFont);
    hHCalTPCompEtvsTower_Collision->GetYaxis()->SetLabelFont(mFont);
    hHCalTPCompEtvsTower_Collision->Draw("colz");
    drawLine(nMaxTowers/2-0.5, minTPEt, nMaxTowers/2-0.5, maxTPEt, 1, 2, 2);
    drawLatex(0.27, 0.8, "HF^{-}", mFont, 0.08, 1);
    drawLatex(0.66, 0.8, "HF^{+}", mFont, 0.08, 1);
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c1->SaveAs(Form("%s/hCalTPCompEtvsTower_Collision.pdf", dir.Data()));
    c1->SaveAs(Form("%s/hCalTPCompEtvsTower_Collision.png", dir.Data()));

    TH2D *hAdcDep3vsDep1_Noise    = (TH2D *)f->Get("hAdcDep3vsDep1_Noise");
    TH2D *hAdcDep4vsDep2_Noise    = (TH2D *)f->Get("hAdcDep4vsDep2_Noise");
    TH2D *hAdcShortvsLong_Noise   = (TH2D *)f->Get("hAdcShortvsLong_Noise");
    TH1D *hAdcDep3Dep1Diff_Noise  = (TH1D *)f->Get("hAdcDep3Dep1Diff_Noise");
    TH1D *hAdcDep4Dep2Diff_Noise  = (TH1D *)f->Get("hAdcDep4Dep2Diff_Noise");
    TH1D *hAdcLongShortDiff_Noise = (TH1D *)f->Get("hAdcLongShortDiff_Noise");
    TH2D *hAdcDep3vsDep1_Collision    = (TH2D *)f->Get("hAdcDep3vsDep1_Collision");
    TH2D *hAdcDep4vsDep2_Collision    = (TH2D *)f->Get("hAdcDep4vsDep2_Collision");
    TH2D *hAdcShortvsLong_Collision   = (TH2D *)f->Get("hAdcShortvsLong_Collision");
    TH1D *hAdcDep3Dep1Diff_Collision  = (TH1D *)f->Get("hAdcDep3Dep1Diff_Collision");
    TH1D *hAdcDep4Dep2Diff_Collision  = (TH1D *)f->Get("hAdcDep4Dep2Diff_Collision");
    TH1D *hAdcLongShortDiff_Collision = (TH1D *)f->Get("hAdcLongShortDiff_Collision");

    c2->cd(1);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hAdcDep3vsDep1_Noise->GetXaxis()->SetRangeUser(minNoiseAdc, maxNoiseAdc);
    hAdcDep3vsDep1_Noise->GetYaxis()->SetRangeUser(minNoiseAdc, maxNoiseAdc);
    hAdcDep3vsDep1_Noise->GetXaxis()->SetTitleFont(mFont);
    hAdcDep3vsDep1_Noise->GetXaxis()->SetLabelFont(mFont);
    hAdcDep3vsDep1_Noise->GetYaxis()->SetTitleFont(mFont);
    hAdcDep3vsDep1_Noise->GetYaxis()->SetLabelFont(mFont);
    hAdcDep3vsDep1_Noise->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep3vsDep1_Noise->Draw("colz");
    drawLine(minNoiseAdc, minNoiseAdc, maxNoiseAdc, maxNoiseAdc, 2, 2, 2);
    drawLatex(0.42, 0.94, "Noise", mFont, 0.07, 1);
    c2->cd(2);
    gPad->SetLogy(1);
    hAdcDep3Dep1Diff_Noise->GetXaxis()->SetRangeUser(-maxNoiseAdc, maxNoiseAdc);
    hAdcDep3Dep1Diff_Noise->GetXaxis()->SetTitleFont(mFont);
    hAdcDep3Dep1Diff_Noise->GetXaxis()->SetLabelFont(mFont);
    hAdcDep3Dep1Diff_Noise->GetYaxis()->SetTitleFont(mFont);
    hAdcDep3Dep1Diff_Noise->GetYaxis()->SetLabelFont(mFont);
    hAdcDep3Dep1Diff_Noise->GetYaxis()->SetTitle("Entries");
    hAdcDep3Dep1Diff_Noise->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep3Dep1Diff_Noise->Draw("hist");
    drawLatex(0.42, 0.94, "Noise", mFont, 0.07, 1);
    c2->cd(3);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hAdcDep4vsDep2_Noise->GetXaxis()->SetRangeUser(minNoiseAdc, maxNoiseAdc);
    hAdcDep4vsDep2_Noise->GetYaxis()->SetRangeUser(minNoiseAdc, maxNoiseAdc);
    hAdcDep4vsDep2_Noise->GetXaxis()->SetTitleFont(mFont);
    hAdcDep4vsDep2_Noise->GetXaxis()->SetLabelFont(mFont);
    hAdcDep4vsDep2_Noise->GetYaxis()->SetTitleFont(mFont);
    hAdcDep4vsDep2_Noise->GetYaxis()->SetLabelFont(mFont);
    hAdcDep4vsDep2_Noise->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep4vsDep2_Noise->Draw("colz");
    drawLine(minNoiseAdc, minNoiseAdc, maxNoiseAdc, maxNoiseAdc, 2, 2, 2);
    drawLatex(0.42, 0.94, "Noise", mFont, 0.07, 1);
    c2->cd(4);
    gPad->SetLogy(1);
    hAdcDep4Dep2Diff_Noise->GetXaxis()->SetRangeUser(-maxNoiseAdc, maxNoiseAdc);
    hAdcDep4Dep2Diff_Noise->GetXaxis()->SetTitleFont(mFont);
    hAdcDep4Dep2Diff_Noise->GetXaxis()->SetLabelFont(mFont);
    hAdcDep4Dep2Diff_Noise->GetYaxis()->SetTitleFont(mFont);
    hAdcDep4Dep2Diff_Noise->GetYaxis()->SetLabelFont(mFont);
    hAdcDep4Dep2Diff_Noise->GetYaxis()->SetTitle("Entries");
    hAdcDep4Dep2Diff_Noise->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep4Dep2Diff_Noise->Draw("hist");
    drawLatex(0.42, 0.94, "Noise", mFont, 0.07, 1);
    c2->SaveAs(Form("%s/AdcCorr_Noise.pdf", dir.Data()));
    c2->SaveAs(Form("%s/AdcCorr_Noise.png", dir.Data()));

    c2->cd(1);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hAdcDep3vsDep1_Collision->GetXaxis()->SetRangeUser(minCollisionAdc, maxCollisionAdc);
    hAdcDep3vsDep1_Collision->GetYaxis()->SetRangeUser(minCollisionAdc, maxCollisionAdc);
    hAdcDep3vsDep1_Collision->GetXaxis()->SetTitleFont(mFont);
    hAdcDep3vsDep1_Collision->GetXaxis()->SetLabelFont(mFont);
    hAdcDep3vsDep1_Collision->GetYaxis()->SetTitleFont(mFont);
    hAdcDep3vsDep1_Collision->GetYaxis()->SetLabelFont(mFont);
    hAdcDep3vsDep1_Collision->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep3vsDep1_Collision->Draw("colz");
    drawLine(minCollisionAdc, minCollisionAdc, maxCollisionAdc, maxCollisionAdc, 2, 2, 2);
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c2->cd(2);
    gPad->SetLogy(1);
    hAdcDep3Dep1Diff_Collision->GetXaxis()->SetRangeUser(-60, 60);
    hAdcDep3Dep1Diff_Collision->GetXaxis()->SetTitleFont(mFont);
    hAdcDep3Dep1Diff_Collision->GetXaxis()->SetLabelFont(mFont);
    hAdcDep3Dep1Diff_Collision->GetYaxis()->SetTitleFont(mFont);
    hAdcDep3Dep1Diff_Collision->GetYaxis()->SetLabelFont(mFont);
    hAdcDep3Dep1Diff_Collision->GetYaxis()->SetTitle("Entries");
    hAdcDep3Dep1Diff_Collision->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep3Dep1Diff_Collision->Draw("hist");
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c2->cd(3);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hAdcDep4vsDep2_Collision->GetXaxis()->SetRangeUser(minCollisionAdc, maxCollisionAdc);
    hAdcDep4vsDep2_Collision->GetYaxis()->SetRangeUser(minCollisionAdc, maxCollisionAdc);
    hAdcDep4vsDep2_Collision->GetXaxis()->SetTitleFont(mFont);
    hAdcDep4vsDep2_Collision->GetXaxis()->SetLabelFont(mFont);
    hAdcDep4vsDep2_Collision->GetYaxis()->SetTitleFont(mFont);
    hAdcDep4vsDep2_Collision->GetYaxis()->SetLabelFont(mFont);
    hAdcDep4vsDep2_Collision->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep4vsDep2_Collision->Draw("colz");
    drawLine(minCollisionAdc, minCollisionAdc, maxCollisionAdc, maxCollisionAdc, 2, 2, 2);
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c2->cd(4);
    gPad->SetLogy(1);
    hAdcDep4Dep2Diff_Collision->GetXaxis()->SetRangeUser(-60, 60);
    hAdcDep4Dep2Diff_Collision->GetXaxis()->SetTitleFont(mFont);
    hAdcDep4Dep2Diff_Collision->GetXaxis()->SetLabelFont(mFont);
    hAdcDep4Dep2Diff_Collision->GetYaxis()->SetTitleFont(mFont);
    hAdcDep4Dep2Diff_Collision->GetYaxis()->SetLabelFont(mFont);
    hAdcDep4Dep2Diff_Collision->GetYaxis()->SetTitle("Entries");
    hAdcDep4Dep2Diff_Collision->GetYaxis()->SetTitleOffset(0.9);
    hAdcDep4Dep2Diff_Collision->Draw("hist");
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c2->SaveAs(Form("%s/AdcCorr_Collision.pdf", dir.Data()));
    c2->SaveAs(Form("%s/AdcCorr_Collision.png", dir.Data()));

    c2->cd(1);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hAdcShortvsLong_Noise->GetXaxis()->SetRangeUser(0, 150);
    hAdcShortvsLong_Noise->GetYaxis()->SetRangeUser(0, 150);
    hAdcShortvsLong_Noise->GetXaxis()->SetTitleFont(mFont);
    hAdcShortvsLong_Noise->GetXaxis()->SetLabelFont(mFont);
    hAdcShortvsLong_Noise->GetYaxis()->SetTitleFont(mFont);
    hAdcShortvsLong_Noise->GetYaxis()->SetLabelFont(mFont);
    hAdcShortvsLong_Noise->GetYaxis()->SetTitleOffset(0.9);
    hAdcShortvsLong_Noise->Draw("colz");
    drawLine(0, 0, 150, 150, 2, 2, 2);
    drawLatex(0.42, 0.94, "Noise", mFont, 0.07, 1);
    c2->cd(2);
    gPad->SetLogy(1);
    hAdcLongShortDiff_Noise->GetXaxis()->SetRangeUser(-100, 100);
    hAdcLongShortDiff_Noise->GetXaxis()->SetTitleFont(mFont);
    hAdcLongShortDiff_Noise->GetXaxis()->SetLabelFont(mFont);
    hAdcLongShortDiff_Noise->GetYaxis()->SetTitleFont(mFont);
    hAdcLongShortDiff_Noise->GetYaxis()->SetLabelFont(mFont);
    hAdcLongShortDiff_Noise->GetYaxis()->SetTitle("Entries");
    hAdcLongShortDiff_Noise->GetYaxis()->SetTitleOffset(0.9);
    hAdcLongShortDiff_Noise->Draw("hist");
    drawLatex(0.42, 0.94, "Noise", mFont, 0.07, 1);
    c2->cd(3);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hAdcShortvsLong_Collision->GetXaxis()->SetRangeUser(minCollisionAdc, maxCollisionAdc+50);
    hAdcShortvsLong_Collision->GetYaxis()->SetRangeUser(minCollisionAdc, maxCollisionAdc+50);
    hAdcShortvsLong_Collision->GetXaxis()->SetTitleFont(mFont);
    hAdcShortvsLong_Collision->GetXaxis()->SetLabelFont(mFont);
    hAdcShortvsLong_Collision->GetYaxis()->SetTitleFont(mFont);
    hAdcShortvsLong_Collision->GetYaxis()->SetLabelFont(mFont);
    hAdcShortvsLong_Collision->GetYaxis()->SetTitleOffset(0.9);
    hAdcShortvsLong_Collision->Draw("colz");
    drawLine(minCollisionAdc, minCollisionAdc, maxCollisionAdc+50, maxCollisionAdc+50, 2, 2, 2);
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c2->cd(4);
    gPad->SetLogy(1);
    hAdcLongShortDiff_Collision->GetXaxis()->SetRangeUser(-100, 100);
    hAdcLongShortDiff_Collision->GetXaxis()->SetTitleFont(mFont);
    hAdcLongShortDiff_Collision->GetXaxis()->SetLabelFont(mFont);
    hAdcLongShortDiff_Collision->GetYaxis()->SetTitleFont(mFont);
    hAdcLongShortDiff_Collision->GetYaxis()->SetLabelFont(mFont);
    hAdcLongShortDiff_Collision->GetYaxis()->SetTitle("Entries");
    hAdcLongShortDiff_Collision->GetYaxis()->SetTitleOffset(0.9);
    hAdcLongShortDiff_Collision->Draw("hist");
    drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c2->SaveAs(Form("%s/fiberAdcCorr_LongvsShort.pdf", dir.Data()));
    c2->SaveAs(Form("%s/fiberAdcCorr_LongvsShort.png", dir.Data()));

    TH2D *hTrigAdcLongShortDiffvsCen_Noise = (TH2D *)f->Get("hTrigAdcLongShortDiffvsCen_Noise");
    TH2D *hTrigAdcLongShortDiffvsCen_Collision = (TH2D *)f->Get("hTrigAdcLongShortDiffvsCen_Collision");

    c2->cd(1);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTrigAdcLongShortDiffvsCen_Noise->GetYaxis()->SetRangeUser(-100, 100);
    hTrigAdcLongShortDiffvsCen_Noise->GetXaxis()->SetTitleFont(mFont);
    hTrigAdcLongShortDiffvsCen_Noise->GetXaxis()->SetLabelFont(mFont);
    hTrigAdcLongShortDiffvsCen_Noise->GetYaxis()->SetTitleFont(mFont);
    hTrigAdcLongShortDiffvsCen_Noise->GetYaxis()->SetLabelFont(mFont);
    hTrigAdcLongShortDiffvsCen_Noise->GetYaxis()->SetTitleOffset(0.9);
    hTrigAdcLongShortDiffvsCen_Noise->Draw("colz");
    drawLatex(0.35, 0.94, "Noise (Trigger)", mFont, 0.07, 1);
    c2->cd(2);
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTrigAdcLongShortDiffvsCen_Collision->GetYaxis()->SetRangeUser(-100, 100);
    hTrigAdcLongShortDiffvsCen_Collision->GetXaxis()->SetTitleFont(mFont);
    hTrigAdcLongShortDiffvsCen_Collision->GetXaxis()->SetLabelFont(mFont);
    hTrigAdcLongShortDiffvsCen_Collision->GetYaxis()->SetTitleFont(mFont);
    hTrigAdcLongShortDiffvsCen_Collision->GetYaxis()->SetLabelFont(mFont);
    hTrigAdcLongShortDiffvsCen_Collision->GetYaxis()->SetTitleOffset(0.9);
    hTrigAdcLongShortDiffvsCen_Collision->Draw("colz");
    TGraphErrors *grMeanTrigAdcLongShortDiffvsCen = new TGraphErrors(1);
    Int_t ipt=0;
    for(Int_t icen=0; icen<hTrigAdcLongShortDiffvsCen_Collision->GetNbinsX(); icen++){
        TH1D *hTemp = hTrigAdcLongShortDiffvsCen_Collision->ProjectionY(Form("htemp_%d", icen), icen+1, icen+1);
        if(hTemp->Integral()>0){
            grMeanTrigAdcLongShortDiffvsCen->SetPoint(ipt, hTrigAdcLongShortDiffvsCen_Collision->GetXaxis()->GetBinCenter(icen+1), hTemp->GetMean());
            grMeanTrigAdcLongShortDiffvsCen->SetPointError(ipt, 0, hTemp->GetMeanError());
            ipt++;
        }
    }
    setGraph(grMeanTrigAdcLongShortDiffvsCen, 20, 0.8, 1, 1, 2);
    grMeanTrigAdcLongShortDiffvsCen->Draw("pzsame");
    drawLatex(0.35, 0.94, "Collision (Trigger)", mFont, 0.07, 1);
    c2->cd(3);
    gPad->SetLogy(1);
    gPad->SetLogz(1);
    gPad->Clear();
    Int_t cenBinLow = hTrigAdcLongShortDiffvsCen_Collision->GetXaxis()->FindBin(180+1.e-3);
    Int_t cenBinHi  = hTrigAdcLongShortDiffvsCen_Collision->GetXaxis()->FindBin(180+1.e-3);
    TH1D *hTrigAdcLongShortDiff_Noise = (TH1D *)hTrigAdcLongShortDiffvsCen_Noise->ProjectionY("hTrigAdcLongShortDiff_Noise");
    TH1D *hTrigAdcLongShortDiff_Collision_Cen90100 = (TH1D *)hTrigAdcLongShortDiffvsCen_Collision->ProjectionY("hTrigAdcLongShortDiff_Collision_Cen90100", cenBinLow, cenBinHi);
    setHisto(hTrigAdcLongShortDiff_Noise, 24, 1, 4, 4, 2);
    setHisto(hTrigAdcLongShortDiff_Collision_Cen90100, 20, 1, 1, 1, 2);
    hTrigAdcLongShortDiff_Noise->GetXaxis()->SetRangeUser(-60, 60);
    hTrigAdcLongShortDiff_Noise->GetXaxis()->SetTitleFont(mFont);
    hTrigAdcLongShortDiff_Noise->GetXaxis()->SetLabelFont(mFont);
    hTrigAdcLongShortDiff_Noise->GetYaxis()->SetTitleFont(mFont);
    hTrigAdcLongShortDiff_Noise->GetYaxis()->SetLabelFont(mFont);
    hTrigAdcLongShortDiff_Noise->GetYaxis()->SetTitleOffset(0.9);
    hTrigAdcLongShortDiff_Noise->Draw("hist");
    hTrigAdcLongShortDiff_Collision_Cen90100->Draw("histsame");
    drawLatex(0.30, 0.94, "Centrality: 90-100%", mFont, 0.07, 1);
    setLegend(leg,0.13,0.72,0.30,0.84,0.06);
    leg->AddEntry(hTrigAdcLongShortDiff_Noise, "Noise", "l");
    leg->AddEntry(hTrigAdcLongShortDiff_Collision_Cen90100, "Collision", "l");
    leg->Draw("same");
    c2->cd(4);
    gPad->SetLogy(1);
    gPad->Clear();
    //hAdcLongShortDiff_Collision->GetXaxis()->SetRangeUser(-60, 100);
    //hAdcLongShortDiff_Collision->GetXaxis()->SetTitleFont(mFont);
    //hAdcLongShortDiff_Collision->GetXaxis()->SetLabelFont(mFont);
    //hAdcLongShortDiff_Collision->GetYaxis()->SetTitleFont(mFont);
    //hAdcLongShortDiff_Collision->GetYaxis()->SetLabelFont(mFont);
    //hAdcLongShortDiff_Collision->GetYaxis()->SetTitle("Entries");
    //hAdcLongShortDiff_Collision->GetYaxis()->SetTitleOffset(0.9);
    //hAdcLongShortDiff_Collision->Draw("hist");
    //drawLatex(0.42, 0.94, "Collision", mFont, 0.07, 1);
    c2->SaveAs(Form("%s/trigFiberAdcCorr_LongvsShort.pdf", dir.Data()));
    c2->SaveAs(Form("%s/trigFiberAdcCorr_LongvsShort.png", dir.Data()));

    cout << "End of program !" << endl;
}
