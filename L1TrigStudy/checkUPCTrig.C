#include "../common/headers.h"
#include "../common/function.C"
#include "../common/constant.h"

const Int_t mFont = 42;

void checkUPCTrig()
{
    gStyle->SetOptFit(1111);
    gStyle->SetLabelOffset(0.001,"z");
    //gStyle->SetEndErrorSize(4);

    TFile *f = TFile::Open("L1Eff.test.histo.root");

    TString dirName  = "upcTrigPlots";
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

    const Int_t nSces = 6;
    TString sceName[nSces] = {"Tot", "Max", "TotBarrel", "MaxBarrel", "TotEndcap", "MaxEndcap"};

    TH1D *hEt_0nXn[nSces];
    TH1D *hEmEt_0nXn[nSces];
    TH1D *hHadEt_0nXn[nSces];
    for(Int_t idx=0; idx<nSces; idx++){
        hEt_0nXn[idx] = (TH1D *)f->Get(Form("h%sEt_0nXn", sceName[idx].Data()));
        hEmEt_0nXn[idx] = (TH1D *)f->Get(Form("h%sEmEt_0nXn", sceName[idx].Data()));
        hHadEt_0nXn[idx] = (TH1D *)f->Get(Form("h%sHadEt_0nXn", sceName[idx].Data()));
    }
    TH1D *hnEvts_0nXn = (TH1D *)f->Get("hnEvts_0nXn");
    TH1D *hHFAdcInXn_0nXn = (TH1D *)f->Get("hHFAdcInXn_0nXn");
    TH2D *hNtrkOfflineVsNtrkHP_0nXn = (TH2D *)f->Get("hNtrkOfflineVsNtrkHP_0nXn");
    TH1D *hTotEndcapEtInXn_0nXn = (TH1D *)f->Get("hTotEndcapEtInXn_0nXn");
    TH1D *hTotEndcapEmEtInXn_0nXn = (TH1D *)f->Get("hTotEndcapEmEtInXn_0nXn");
    TH1D *hTotEndcapHadEtInXn_0nXn = (TH1D *)f->Get("hTotEndcapHadEtInXn_0nXn");
    TH1D *hMaxEndcapEtInXn_0nXn = (TH1D *)f->Get("hMaxEndcapEtInXn_0nXn");
    TH1D *hMaxEndcapEmEtInXn_0nXn = (TH1D *)f->Get("hMaxEndcapEmEtInXn_0nXn");
    TH1D *hMaxEndcapHadEtInXn_0nXn = (TH1D *)f->Get("hMaxEndcapHadEtInXn_0nXn");

    TH2D *hHFAdcInXnVsNtrkHP_0nXn = (TH2D *)f->Get("hHFAdcInXnVsNtrkHP_0nXn");
    TH2D *hMaxHadEtVsNtrkHP_0nXn = (TH2D *)f->Get("hMaxHadEtVsNtrkHP_0nXn");
    TH2D *hMaxBarrelHadEtVsNtrkHP_0nXn = (TH2D *)f->Get("hMaxBarrelHadEtVsNtrkHP_0nXn");
    TH2D *hTotEndcapEtVsNtrkHP_0nXn = (TH2D *)f->Get("hTotEndcapEtVsNtrkHP_0nXn");
    TH2D *hTotEndcapEmEtVsNtrkHP_0nXn = (TH2D *)f->Get("hTotEndcapEmEtVsNtrkHP_0nXn");
    TH2D *hTotEndcapHadEtVsNtrkHP_0nXn = (TH2D *)f->Get("hTotEndcapHadEtVsNtrkHP_0nXn");
    TH2D *hMaxEndcapHadEtVsNtrkHP_0nXn = (TH2D *)f->Get("hMaxEndcapHadEtVsNtrkHP_0nXn");
    TH2D *hTotEndcapHadEtInXnVsNtrkHP_0nXn = (TH2D *)f->Get("hTotEndcapHadEtInXnVsNtrkHP_0nXn");
    TH2D *hMaxEndcapHadEtInXnVsNtrkHP_0nXn = (TH2D *)f->Get("hMaxEndcapHadEtInXnVsNtrkHP_0nXn");
    TH2D *hTotEndcapHadEtInXnVsHFAdcInXn_0nXn = (TH2D *)f->Get("hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn");
    TH2D *hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn = (TH2D *)f->Get("hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn");

    c1->cd();
    hnEvts_0nXn->Scale(1.e7/hnEvts_0nXn->GetBinContent(1));
    setPad(0.12, 0.08, 0.08, 0.16);
    gPad->SetLogy(1);
    hnEvts_0nXn->GetXaxis()->SetRangeUser(0, 5);
    hnEvts_0nXn->GetYaxis()->SetTitle("Rate / ZB Rate #times 10^{7}");
    hnEvts_0nXn->SetMarkerSize(1.6);
    hnEvts_0nXn->Draw("histtext");
    drawLatex(0.12, 0.945, "Zero-bias Data", mFont, 0.05, 1);
    drawLatex(0.604, 0.945, "2018 PbPb 5.02 TeV", mFont, 0.05, 1);
    c1->SaveAs(Form("%s/upcTrigRate.pdf", dirName.Data()));

    c1->cd();
    setPad(0.12, 0.08, 0.08, 0.12);
    gPad->SetLogy(1);
    hHFAdcInXn_0nXn->Draw("p");
    c1->SaveAs(Form("%s/maxHFAdcInXn.pdf", dirName.Data()));
    c1->SaveAs(Form("%s/maxHFAdcInXn.png", dirName.Data()));

    Int_t mMaxNtrkHP = 30;

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hHFAdcInXnVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hHFAdcInXnVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/maxHFAdcInXnVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hMaxHadEtVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hMaxHadEtVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 50);
    hMaxHadEtVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/maxHadEtVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hMaxBarrelHadEtVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hMaxBarrelHadEtVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hMaxBarrelHadEtVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/maxBarrelHadEtVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTotEndcapEtVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hTotEndcapEtVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hTotEndcapEtVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/totEndcapEtVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTotEndcapEmEtVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hTotEndcapEmEtVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hTotEndcapEmEtVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/totEndcapEmEtVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTotEndcapHadEtVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hTotEndcapHadEtVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hTotEndcapHadEtVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/totEndcapHadEtVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hMaxEndcapHadEtVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hMaxEndcapHadEtVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hMaxEndcapHadEtVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/maxEndcapHadEtVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTotEndcapHadEtInXnVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hTotEndcapHadEtInXnVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hTotEndcapHadEtInXnVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/totEndcapHadEtInXnVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hMaxEndcapHadEtInXnVsNtrkHP_0nXn->GetXaxis()->SetRangeUser(0, mMaxNtrkHP);
    hMaxEndcapHadEtInXnVsNtrkHP_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hMaxEndcapHadEtInXnVsNtrkHP_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/maxEndcapHadEtInXnVsNtrkHP.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hTotEndcapHadEtInXnVsHFAdcInXn_0nXn->GetXaxis()->SetRangeUser(0, 100);
    hTotEndcapHadEtInXnVsHFAdcInXn_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hTotEndcapHadEtInXnVsHFAdcInXn_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/totEndcapHadEtInXnVsHFAdcInXn.pdf", dirName.Data()));

    c1->cd();
    gPad->SetLogy(0);
    gPad->SetLogz(1);
    hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn->GetXaxis()->SetRangeUser(0, 100);
    hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn->GetYaxis()->SetRangeUser(0, 30);
    hMaxEndcapHadEtInXnVsHFAdcInXn_0nXn->Draw("colz");
    c1->SaveAs(Form("%s/maxEndcapHadEtInXnVsHFAdcInXn.pdf", dirName.Data()));

    Int_t mMaxEt[nSces]    = {300, 60, 300, 50, 50, 30};
    Int_t mMaxEmEt[nSces]  = {300, 60, 300, 50, 50, 30};
    Int_t mMaxHadEt[nSces] = {300, 60, 300, 50, 50, 30};
    for(Int_t isce=0; isce<nSces; isce++){
        c3->cd(1);
        gPad->SetLogy(1);
        setHisto(hEt_0nXn[isce], 20, 1.2, 1, 1, 2);
        hEt_0nXn[isce]->GetXaxis()->SetRangeUser(0, mMaxEt[isce]);
        hEt_0nXn[isce]->GetXaxis()->SetTitleOffset(0.95);
        hEt_0nXn[isce]->GetYaxis()->SetTitle("Entries");
        hEt_0nXn[isce]->Draw("p");
        drawLatex(0.39, 0.945, "ECal + HCal", mFont, 0.07, 1);

        c3->cd(2);
        gPad->SetLogy(1);
        setHisto(hEmEt_0nXn[isce], 20, 1.2, 2, 2, 2);
        hEmEt_0nXn[isce]->GetXaxis()->SetRangeUser(0, mMaxEmEt[isce]);
        hEmEt_0nXn[isce]->GetXaxis()->SetTitleOffset(0.95);
        hEmEt_0nXn[isce]->GetYaxis()->SetTitle("Entries");
        hEmEt_0nXn[isce]->Draw("p");
        drawLatex(0.47, 0.945, "ECal", mFont, 0.07, 2);

        c3->cd(3);
        gPad->SetLogy(1);
        setHisto(hHadEt_0nXn[isce], 20, 1.2, 4, 4, 2);
        hHadEt_0nXn[isce]->GetXaxis()->SetRangeUser(0, mMaxHadEt[isce]);
        hHadEt_0nXn[isce]->GetXaxis()->SetTitleOffset(0.95);
        hHadEt_0nXn[isce]->GetYaxis()->SetTitle("Entries");
        hHadEt_0nXn[isce]->Draw("p");
        drawLatex(0.47, 0.945, "HCal", mFont, 0.07, 4);

        c3->cd(4);
        gPad->Clear();
        if(sceName[isce].Contains("Tot")){
            if(sceName[isce].Contains("Barrel")){
                drawLatex(0.41, 0.6, "Barrel", mFont, 0.1, 1);
            }
            else if(sceName[isce].Contains("Endcap")){
                drawLatex(0.40, 0.6, "Endcap", mFont, 0.1, 1);
            }
            else{
                drawLatex(0.28, 0.6, "Barrel + Endcap", mFont, 0.1, 1);
            }

            drawLatex(0.40, 0.4, "Total E_{T}", mFont, 0.1, 1);
        }
        if(sceName[isce].Contains("Max")){
            if(sceName[isce].Contains("Barrel")){
                drawLatex(0.44, 0.6, "Barrel", mFont, 0.1, 1);
            }
            else if(sceName[isce].Contains("Endcap")){
                drawLatex(0.42, 0.6, "Endcap", mFont, 0.1, 1);
            }
            else{
                drawLatex(0.30, 0.6, "Barrel + Endcap", mFont, 0.1, 1);
            }

            drawLatex(0.36, 0.4, "Maximum E_{T}", mFont, 0.1, 1);
        }

        c3->SaveAs(Form("%s/%sEt.pdf", dirName.Data(), sceName[isce].Data()));
        c3->SaveAs(Form("%s/%sEt.png", dirName.Data(), sceName[isce].Data()));
    }

    c3->cd(1);
    gPad->SetLogy(1);
    setHisto(hTotEndcapEtInXn_0nXn, 20, 1.2, 1, 1, 2);
    hTotEndcapEtInXn_0nXn->GetXaxis()->SetRangeUser(0, 50);
    hTotEndcapEtInXn_0nXn->GetXaxis()->SetTitleOffset(0.95);
    hTotEndcapEtInXn_0nXn->GetYaxis()->SetTitle("Entries");
    hTotEndcapEtInXn_0nXn->Draw("p");
    drawLatex(0.41, 0.945, "ECal + HCal", mFont, 0.07, 1);
    c3->cd(2);
    gPad->SetLogy(1);
    setHisto(hTotEndcapEmEtInXn_0nXn, 20, 1.2, 2, 2, 2);
    hTotEndcapEmEtInXn_0nXn->GetXaxis()->SetRangeUser(0, 50);
    hTotEndcapEmEtInXn_0nXn->GetXaxis()->SetTitleOffset(0.95);
    hTotEndcapEmEtInXn_0nXn->GetYaxis()->SetTitle("Entries");
    hTotEndcapEmEtInXn_0nXn->Draw("p");
    drawLatex(0.47, 0.945, "ECal", mFont, 0.07, 2);
    c3->cd(3);
    gPad->SetLogy(1);
    setHisto(hTotEndcapHadEtInXn_0nXn, 20, 1.2, 4, 4, 2);
    hTotEndcapHadEtInXn_0nXn->GetXaxis()->SetRangeUser(0, 50);
    hTotEndcapHadEtInXn_0nXn->GetXaxis()->SetTitleOffset(0.95);
    hTotEndcapHadEtInXn_0nXn->GetYaxis()->SetTitle("Entries");
    hTotEndcapHadEtInXn_0nXn->Draw("p");
    drawLatex(0.47, 0.945, "HCal", mFont, 0.07, 4);
    c3->cd(4);
    gPad->Clear();
    drawLatex(0.4, 0.6, "Endcap", mFont, 0.1, 1);
    drawLatex(0.3, 0.4, "Total E_{T} in Xn", mFont, 0.1, 1);
    c3->SaveAs(Form("%s/TotEndcapEtInXn.pdf", dirName.Data()));
    c3->SaveAs(Form("%s/TotEndcapEtInXn.png", dirName.Data()));

    c3->cd(1);
    gPad->SetLogy(1);
    setHisto(hMaxEndcapEtInXn_0nXn, 20, 1.2, 1, 1, 2);
    hMaxEndcapEtInXn_0nXn->GetXaxis()->SetRangeUser(0, 30);
    hMaxEndcapEtInXn_0nXn->GetXaxis()->SetTitleOffset(0.95);
    hMaxEndcapEtInXn_0nXn->GetYaxis()->SetTitle("Entries");
    hMaxEndcapEtInXn_0nXn->Draw("p");
    drawLatex(0.41, 0.945, "ECal + HCal", mFont, 0.07, 1);
    c3->cd(2);
    gPad->SetLogy(1);
    setHisto(hMaxEndcapEmEtInXn_0nXn, 20, 1.2, 2, 2, 2);
    hMaxEndcapEmEtInXn_0nXn->GetXaxis()->SetRangeUser(0, 30);
    hMaxEndcapEmEtInXn_0nXn->GetXaxis()->SetTitleOffset(0.95);
    hMaxEndcapEmEtInXn_0nXn->GetYaxis()->SetTitle("Entries");
    hMaxEndcapEmEtInXn_0nXn->Draw("p");
    drawLatex(0.47, 0.945, "ECal", mFont, 0.07, 2);
    c3->cd(3);
    gPad->SetLogy(1);
    setHisto(hMaxEndcapHadEtInXn_0nXn, 20, 1.2, 4, 4, 2);
    hMaxEndcapHadEtInXn_0nXn->GetXaxis()->SetRangeUser(0, 30);
    hMaxEndcapHadEtInXn_0nXn->GetXaxis()->SetTitleOffset(0.95);
    hMaxEndcapHadEtInXn_0nXn->GetYaxis()->SetTitle("Entries");
    hMaxEndcapHadEtInXn_0nXn->Draw("p");
    drawLatex(0.47, 0.945, "HCal", mFont, 0.07, 4);
    c3->cd(4);
    gPad->Clear();
    drawLatex(0.4, 0.6, "Endcap", mFont, 0.1, 1);
    drawLatex(0.28, 0.4, "Maximum E_{T} in Xn", mFont, 0.1, 1);
    c3->SaveAs(Form("%s/MaxEndcapEtInXn.pdf", dirName.Data()));
    c3->SaveAs(Form("%s/MaxEndcapEtInXn.png", dirName.Data()));

    cout << "End of program !" << endl;
}
