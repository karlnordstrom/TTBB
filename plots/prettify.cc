#include <Math/Vector4D.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVector2.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TChain.h>
#include <TStyle.h>

#include <cmath>
#include <vector>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <string.h>

void prettify() {

//Draw Text
    TLatex *t = new TLatex();
    t->SetNDC();

    TFile *f= new TFile("/afs/phas.gla.ac.uk/user/k/knordstrom/output/fulldata.root");

    TCanvas *c1 = new TCanvas("canvas", "canvas", -1);

    TH2F *gen1=(TH2F*) gDirectory->Get("pT_truth_vs_reco");

    gen1->SetStats(0);
    gen1->SetTitle("Truth jet p_{T} vs reconstructed jet p_{T} for matched jet pairs");

    gen1->GetXaxis()->SetTitle("p_{T}^{truth} [GeV]");
    gen1->GetYaxis()->SetTitle("p_{T}^{reco} [GeV]");
    gen1->GetYaxis()->SetTitleOffset(1.);

    gen1->SetMarkerColor(kRed);
    gen1->SetLineColor(kRed);
    gen1->SetMarkerStyle(7);

    Double_t correlation = gen1->GetCorrelationFactor();

    gen1->SetMarkerColor(kWhite);
    gen1->SetLineColor(kWhite);

    gen1->Draw("COLZ");

    char name[100];
    char number[10];
    sprintf(number, "%.3f", (double)correlation);
    strcpy (name, "Correlation factor: ");
    strcat (name, number);

    TText *text = new TText();
    text->SetNDC();
    text->SetTextSize(0.04);
    text->DrawText(0.45,0.25, name);

    c1->SaveAs("pt_truth_vs_reco.ps");

//    TLegend *mleg=new TLegend(0.20,0.78,0.45,0.93);
//
//
//    mleg->SetTextSize(0.04);
//    mleg->SetFillColor(kWhite);                       // background color
//    mleg->SetBorderSize(0);                          // get rid of borders


    TH2F *gen2=(TH2F*) gDirectory->Get("unmatched_dpt_vs_dr");
    gen2->SetStats(0);
    gen2->SetTitle("dR vs dp_{T} to closest in dR for unmatched, matchable truth jets");

    gen2->GetXaxis()->SetTitle("dR");
    gen2->GetYaxis()->SetTitle("|dp_{T}| [GeV]");

    gen2->SetMarkerColor(kRed);
    gen2->SetLineColor(kRed);
    gen2->SetMarkerStyle(7);

    gen2->SetMarkerColor(kWhite);
    gen2->SetLineColor(kWhite);

    gen2->Draw("COLZ");

    c1->SaveAs("unmatched_dr_vs_dpt.ps");

    TH2F *gen2=(TH2F*) gDirectory->Get("dpt_vs_dr");
    gen2->SetStats(0);
    gen2->SetTitle("dR vs dp_{T} to closest in dR for all matchable truth jets");

    gen2->GetXaxis()->SetTitle("dR");
    gen2->GetYaxis()->SetTitle("|dp_{T}| [GeV]");

    gen2->SetMarkerColor(kRed);
    gen2->SetLineColor(kRed);
    gen2->SetMarkerStyle(7);

    gen2->SetMarkerColor(kWhite);
    gen2->SetLineColor(kWhite);

    gen2->Draw("COLZ");

    c1->SaveAs("dr_vs_dpt.ps");


    TH1F *gen3=(TH1F*) gDirectory->Get("n_matches_reco");
    gen3->SetStats(1);
    gen3->SetTitle("Number of truth jets matched to each reconstructed jet");

    gen3->GetXaxis()->SetTitle("Number of matches");
    gen3->GetYaxis()->SetTitle(" Number of jets ");
    gen3->GetYaxis()->SetTitleOffset(1.5);

    gen3->Draw();

    c1->SaveAs("n_matches_reco.ps");

    TH1F *gen4=(TH1F*) gDirectory->Get("n_matches_truth");
    gen4->SetStats(1);
    gen4->SetTitle("Number of reconstructed jets matched to each truth jet before cuts on truth jets");

    gen4->GetXaxis()->SetTitle("Number of matches");
    gen4->GetYaxis()->SetTitle(" Number of jets ");
    gen4->GetYaxis()->SetTitleOffset(1.5);

    gen4->Draw();

    c1->SaveAs("n_matches_truth.ps");

    TH1F *gen4=(TH1F*) gDirectory->Get("n_matches_truth_real");
    gen4->SetStats(1);
    gen4->SetTitle("Number of reconstructed jets matched to each truth jet that passes cuts");

    gen4->GetXaxis()->SetTitle("Number of matches");
    gen4->GetYaxis()->SetTitle(" Number of jets ");
    gen4->GetYaxis()->SetTitleOffset(1.5);

    gen4->Draw();

    c1->SaveAs("n_matches_truth_real.ps");

    TH1F *gen4=(TH1F*) gDirectory->Get("pT_unmatched_truth");
    gen4->SetStats(0);
    gen4->SetTitle("p_{T} of unmatched jets");

    gen4->GetXaxis()->SetTitle(" p_{T} [GeV]");
    gen4->GetYaxis()->SetTitle(" Relative Occurence ");
    gen4->GetYaxis()->SetTitleOffset(1.);
    TH1F *gen5=(TH1F*) gDirectory->Get("pT_unmatched_reco");
    gen5->SetStats(0);
    gen5->SetLineColor(kRed);

    gen4->GetXaxis()->SetRangeUser(0,100);
    gen5->GetXaxis()->SetRangeUser(0,100);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen4->Draw();
    gen5->Draw("same");

    leg_hist = new TLegend(0.5,0.6,0.79,0.79);
    leg_hist->AddEntry(gen4,"Truth","l");
    leg_hist->AddEntry(gen5,"Reconstructed","l");
    leg_hist->Draw();

    c1->SaveAs("pt_unmatched.ps");

    TH1F *gen6=(TH1F*) gDirectory->Get("dpT_jet_ave_new");
    TH1F *gen7=(TH1F*) gDirectory->Get("dpT_jet_ave_entries");
    gen6->Sumw2();
    gen7->Sumw2();

    gen6->Divide(gen7);
    gen6->Sumw2();
    gen6->SetStats(0);
    gen6->SetTitle("Average |dp_{T}| as a function of p_{T}^{truth} for matched jet pairs");

    gen6->GetXaxis()->SetTitle("p_{T}^{truth} [GeV]");
    gen6->GetYaxis()->SetTitle("<|dp_{T}|> [GeV]");
    gen6->GetYaxis()->SetTitleOffset(1.5);

    gen6->Draw();

    TF1 *sqroot= new TF1("sqroot", "sqrt(0.35*x) + 1.7", 0, 250);
    sqroot->Draw("same");

    leg_hist = new TLegend(0.5,0.13,0.79,0.33);
    leg_hist->AddEntry(gen6,"Average |dp_{T}|","l");
    leg_hist->AddEntry(gen5,"0.6 #times #sqrt{p_{T}^{truth}} + 1.7","l");
    leg_hist->Draw();

    c1->SaveAs("average_dpt.ps");

    TH1F *gen4=(TH1F*) gDirectory->Get("dR_jet");
    gen4->SetStats(0);

    TH1F *gen5=(TH1F*) gDirectory->Get("dR_jet_closest");
    gen5->SetStats(0);
    gen5->SetTitle("dR of matched and unmatched, matchable truth jets to closest in dR");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(0.);
    Int_t xmax = gen5->GetXaxis()->FindBin(1.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.2);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" dR ");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    gen5->Draw();
    gen4->Draw("same");

    leg_hist = new TLegend(0.5,0.75,0.79,0.89);
    leg_hist->AddEntry(gen4,"Matched","l");
    leg_hist->AddEntry(gen5,"Unmatched","l");
    leg_hist->Draw();

    c1->SaveAs("dr_matched_vs_unmatched.ps");

    TH1F *gen6=(TH1F*) gDirectory->Get("dpT_jet");
    gen6->SetStats(1);
    gen6->SetTitle("dp_{T} for matched jet pairs");

    gen6->GetXaxis()->SetTitle("dp_{T} [GeV]");
    gen6->GetYaxis()->SetTitle("Number of jets");
    gen6->GetYaxis()->SetTitleOffset(1.5);
    gen6->Sumw2();

    gen6->Fit("gaus");
    TF1 *fit = gen6->GetFunction("gaus");
    Double_t p0 = fit->GetParameter(0);
    Double_t p1 = fit->GetParameter(1);
    Double_t p2 = 2 * fit->GetParameter(2);

    char name[250];

    char number[10];
    sprintf(number, "%d", (double)p0);
    strcpy (name, "");
    strcat (name, number);
    strcat (name, " e^{- #left(#frac{x - ");
    sprintf(number, "%.1f", (double)p1);
    strcat (name, number);
    strcat (name, "}{");
    sprintf(number, "%.1f", (double)p2);
    strcat (name, number);
    strcat (name, "} #right)^{2}}");

    gen6->Draw();

    leg_hist = new TLegend(0.6,0.45,0.89,0.60);
    leg_hist->AddEntry(gen6,"dp_{T}","l");
    leg_hist->AddEntry(fit, name,"l");
    leg_hist->Draw();

    c1->SaveAs("dpt_jet.ps");

    TH1F *gen5=(TH1F*) gDirectory->Get("pt_truth");
    TH1F *gen4=(TH1F*) gDirectory->Get("pt_reco");
    gen5->SetStats(0);
    gen5->SetTitle("p_{T} of jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(0.);
    Int_t xmax = gen5->GetXaxis()->FindBin(250.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.15);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" p_{T}^{jet} [GeV]");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.5,0.55,0.79,0.69);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");


    c0->SaveAs("pt_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    TH1F *gen5=(TH1F*) gDirectory->Get("pt_truth_1");
    TH1F *gen4=(TH1F*) gDirectory->Get("pt_reco_1");
    gen5->SetStats(0);
    gen5->SetTitle("p_{T} of leading jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(0.);
    Int_t xmax = gen5->GetXaxis()->FindBin(250.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.1);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" p_{T}^{j1} [GeV]");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.5,0.55,0.79,0.69);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");

    c0->SaveAs("leading_pt_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    TH1F *gen5=(TH1F*) gDirectory->Get("pt_truth_2");
    TH1F *gen4=(TH1F*) gDirectory->Get("pt_reco_2");
    gen5->SetStats(0);
    gen5->SetTitle("p_{T} of subleading jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(0.);
    Int_t xmax = gen5->GetXaxis()->FindBin(250.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.1);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" p_{T}^{j2} [GeV]");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.5,0.55,0.79,0.69);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");

    c0->SaveAs("subleading_pt_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    TH1F *gen5=(TH1F*) gDirectory->Get("eta_truth");
    TH1F *gen4=(TH1F*) gDirectory->Get("eta_reco");
    gen5->SetStats(0);
    gen5->SetTitle("#eta of jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(-3.);
    Int_t xmax = gen5->GetXaxis()->FindBin(3.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.1);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" #eta ");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.5,0.75,0.79,0.89);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");

    c0->SaveAs("eta_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    TH1F *gen5=(TH1F*) gDirectory->Get("eta_truth_1");
    TH1F *gen4=(TH1F*) gDirectory->Get("eta_reco_1");
    gen5->SetStats(0);
    gen5->SetTitle("#eta of leading jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(-3.);
    Int_t xmax = gen5->GetXaxis()->FindBin(3.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.1);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" #eta^{j1} ");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.5,0.75,0.79,0.89);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");

    c0->SaveAs("leading_eta_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    TH1F *gen5=(TH1F*) gDirectory->Get("eta_truth_2");
    TH1F *gen4=(TH1F*) gDirectory->Get("eta_reco_2");
    gen5->SetStats(0);
    gen5->SetTitle("#eta of subleading jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(-3.);
    Int_t xmax = gen5->GetXaxis()->FindBin(3.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.1);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" #eta^{j2} ");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.5,0.75,0.79,0.89);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");

    c0->SaveAs("subleading_eta_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    TH1F *gen5=(TH1F*) gDirectory->Get("ht_truth");
    TH1F *gen4=(TH1F*) gDirectory->Get("ht_reco");
    gen5->SetStats(0);
    gen5->SetTitle("HT (scalar sum of p_{T}) of jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(0.);
    Int_t xmax = gen5->GetXaxis()->FindBin(1000.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.15);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" HT [GeV] ");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.5,0.75,0.79,0.89);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");

    c0->SaveAs("ht_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    TH1F *gen5=(TH1F*) gDirectory->Get("n_truth");
    TH1F *gen4=(TH1F*) gDirectory->Get("n_reco");
    gen5->SetStats(0);
    gen5->SetTitle("Number of jets");
    gen5->SetLineColor(kRed);

    Int_t xmin = gen5->GetXaxis()->FindBin(0.);
    Int_t xmax = gen5->GetXaxis()->FindBin(1000.);

    gen5->GetXaxis()->SetRange(xmin, xmax);

    gen5->Sumw2();
    gen4->Sumw2();

    Double_t scale4 = 1/gen4->Integral();
    gen4->Scale(scale4);
    Double_t scale5 = 1/gen5->Integral();
    gen5->Scale(scale5);

    gen5->Sumw2();
    gen4->Sumw2();

    gen5->SetMaximum(0.4);
    gen5->SetMinimum(0.);

    gen5->GetXaxis()->SetTitle(" n^{jets} ");
    gen5->GetYaxis()->SetTitle(" Relative Occurence ");
    gen5->GetYaxis()->SetTitleOffset(1.3);

    TCanvas* c0 = new TCanvas("c0","",50,50,865,780);
    c0->cd();

    TPad *pad1 = new TPad("pad1","top pad",0,0.40,1,1);
    TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.40);

    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    gen5->DrawCopy();
    gen4->Draw("same");
    leg_hist = new TLegend(0.6,0.75,0.89,0.89);
    leg_hist->AddEntry(gen4,"Reconstructed","l");
    leg_hist->AddEntry(gen5,"Matchable Truth","l");
    leg_hist->Draw();

    pad2->cd();
    gen5->SetStats(0);
    gen5->SetTitle("");
    Int_t size_hist = gen5->GetSize() - 2;
    TH1F* div= new TH1F("div", "divded", size_hist, 0, 250);
    gen5->Sumw2();
    gen4->Sumw2();
    div->Divide(gen5, gen4);
    div->Sumw2();
    TH1F* newHist = new TH1F("newHist", "error", size_hist, 0, 250);
    for(int iBin = 1; iBin <= size_hist; ++iBin){

      float binUnc = div->GetBinError(iBin);

      newHist->SetBinContent(iBin, 1);
      newHist->SetBinError(iBin, binUnc);

      div->SetBinError(iBin, 0);

    }

    // this makes a bright yellow error band
    newHist->GetYaxis()->SetTitle("Truth/Reco");
    newHist->GetYaxis()->SetTitleSize(0.06);
    newHist->GetYaxis()->SetTitleOffset(0.5);
    newHist->GetXaxis()->SetTitle("");
    newHist->SetMaximum(2.0);
    newHist->SetMinimum(0.0);
    newHist->SetFillStyle(1001);
    newHist->SetLineColor(kYellow-7);
    newHist->SetFillColor(kYellow-7);
    newHist->SetLineWidth(0);
    newHist->SetMarkerStyle(0);
    newHist->SetStats(0);
    newHist->DrawCopy("E2");

    div->SetMarkerStyle(21);
    div->SetMarkerColor(1);
    div->DrawCopy("same ep");

    c0->SaveAs("njet_truth_reco.ps");
    delete c0;
    delete newHist;
    delete div;

    gROOT->ProcessLine(".q");

}
