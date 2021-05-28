#include "TArrow.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TPad.h"
#include "TStyle.h"

#include "funUtil.h"

using namespace std;

const Double_t tinyOffset = 1.e-6;
const Int_t dirIdx = 1;

void plotZDC(Bool_t drawNeuRange = 0)
{
    Int_t    mFont = 42;
    gStyle->SetTextFont(mFont);
    gStyle->SetLegendFont(mFont);	
    gStyle->SetLabelFont(mFont,"xyz");
    gStyle->SetTitleFont(mFont,"xyz");

    gStyle->SetOptFit(0);

    TFile *fIn = TFile::Open("zdcEnergySpec.root");

    TH2D  *hZDCMinusvsZDCPlus = (TH2D *)fIn->Get("hZDCMinusvsZDCPlus");

    TH1D *hZDC[nDirs];
    hZDC[0] = (TH1D*)hZDCMinusvsZDCPlus->ProjectionX(Form("hZDC%s", mDir[0].Data()));
    hZDC[1] = (TH1D*)hZDCMinusvsZDCPlus->ProjectionY(Form("hZDC%s", mDir[1].Data()));
    hZDC[0]->GetXaxis()->SetTitleFont(mFont);
    hZDC[1]->GetXaxis()->SetTitleFont(mFont);

    TString triplegaus = "[0]*TMath::Gaus(x,[1],[2],1)"
        "+ [3]*TMath::Gaus(x,[4],[5],1)"
        "+ [6]*TMath::Gaus(x,[7],[5]*[8],1)";

    TF1 *multiGaus[nDirs];
    for(Int_t idir=0; idir<nDirs; idir++){
        multiGaus[idir] = new TF1(Form("multiGaus_%s", mDir[idir].Data()), triplegaus.Data(), 0, 5.e4);
        multiGaus[idir]->SetParNames("N1", "#mu1", "#sigma1", "N2", "#mu2", "#sigma2", "N3", "#mu3", "scale w.r.t. #sigma2");
        multiGaus[idir]->SetParLimits(8, 1, 2);
        setFun(multiGaus[idir], 1, 3);
    }

    Double_t pars[nDirs][9] = {
        { 2e7, 7.5e3, 1.6e3, 6e6, 15e3, 2.3e3, 1e6, 22.5e3, 1.4 },
        { 2e7, 12e3, 3.0e3, 6e6, 25e3, 4.2e3, 1e6, 35e3, 1.4 }
    };

    const Int_t nGaus = 3;
    TF1 *singleGaus[nDirs][nGaus];
    const Int_t mGausColor[3] = {kRed-4, kGreen+2, kViolet-3};
    for(Int_t idir=0; idir<nDirs; idir++){
        for (Int_t i = 0; i < nGaus; i++) {
            singleGaus[idir][i] = new TF1(Form("singleGaus_%s%d", mDir[idir].Data(), i), "[0]*TMath::Gaus(x,[1],[2],1)", 0, 5e4);
            setFun(singleGaus[idir][i], mGausColor[i], 2, 2);
        }
    }

    const Double_t arrY = 2e3;
    const Double_t arrSize = 7e-3;
    const Int_t    arrColor = kBlue - 4;
    const Int_t    arrWidth = 2;

    Double_t mTextSize = 0.07;

    TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 1200, 450);
    c1->Divide(2, 1);

    c1->cd(1);
    setPad(0.12, 0.08, 0.08, 0.13);
    //gPad->SetLogx(1);
    //gPad->SetLogy(1);
    gPad->SetLogz(1);
    hZDCMinusvsZDCPlus->Rebin2D(2, 2);
    hZDCMinusvsZDCPlus->GetXaxis()->SetTitleFont(mFont);
    hZDCMinusvsZDCPlus->GetYaxis()->SetTitleFont(mFont);
    hZDCMinusvsZDCPlus->GetXaxis()->SetTitle("ZDC_{Plus} (a.u.)");
    hZDCMinusvsZDCPlus->GetYaxis()->SetTitle("ZDC_{Minus} (a.u.)");
    hZDCMinusvsZDCPlus->GetXaxis()->SetLabelSize(0.055);
    hZDCMinusvsZDCPlus->GetXaxis()->SetTitleSize(0.065);
    hZDCMinusvsZDCPlus->GetXaxis()->SetTitleOffset(0.9);
    hZDCMinusvsZDCPlus->GetYaxis()->SetLabelSize(0.055);
    hZDCMinusvsZDCPlus->GetYaxis()->SetTitleSize(0.065);
    hZDCMinusvsZDCPlus->GetYaxis()->SetTitleOffset(0.9);
    hZDCMinusvsZDCPlus->GetXaxis()->SetRangeUser(0, 3e4);
    hZDCMinusvsZDCPlus->GetYaxis()->SetRangeUser(0, 3e4);
    hZDCMinusvsZDCPlus->Draw("col");
    drawLatex(0.60, 0.94, "PbPb 5.02 TeV", mFont, mTextSize, 1);
    drawLatex(0.20, 0.94, "CMS", 62, 0.085, 1);

    TGraphErrors *grZdcRatio[nDirs];
    TGraph       *grZdcPull[nDirs];
    TH1D         *hZDCPull[nDirs];
    TLegend *leg = new TLegend(0.64, 0.52, 0.94, 0.76);
    leg->SetFillColor(10);
    leg->SetFillStyle(0);
    leg->SetTextFont(mFont);
    leg->SetTextSize(0.07);

    c1->cd(2);
    setPad(0.12, 0.08, 0.08, 0.13);
    gPad->SetLogy(1);
    //hZDC[dirIdx]->RebinX(5);
    setHisto(hZDC[dirIdx], 20, 0.8, 1, 1, 1);
    if(dirIdx==0) hZDC[dirIdx]->GetXaxis()->SetRangeUser(0, 0.35e5);
    else          hZDC[dirIdx]->GetXaxis()->SetRangeUser(0, 0.5e5);
    hZDC[dirIdx]->GetXaxis()->SetLabelSize(0.055);
    hZDC[dirIdx]->GetXaxis()->SetTitleSize(0.065);
    hZDC[dirIdx]->GetXaxis()->SetTitleOffset(0.9);
    hZDC[dirIdx]->GetYaxis()->SetLabelSize(0.055);
    hZDC[dirIdx]->GetYaxis()->SetTitleSize(0.065);
    hZDC[dirIdx]->GetYaxis()->SetTitleOffset(1.0);
    hZDC[dirIdx]->GetYaxis()->SetRangeUser(9.999, 1e5);
    multiGaus[dirIdx]->SetParameters(pars[dirIdx]);
    multiGaus[dirIdx]->SetRange(mZdcFitLow[dirIdx], mZdcFitHi[dirIdx]);
    hZDC[dirIdx]->Fit(multiGaus[dirIdx], "R");
    hZDC[dirIdx]->Draw("p");
    if(drawNeuRange){
        for(Int_t ineu=0; ineu<2; ineu++){
            drawArrow(mNeuZDCLow[dirIdx][ineu], arrY, mNeuZDCHi[dirIdx][ineu], arrY, arrSize, "<|>", arrColor, arrWidth, 1);
            drawLine(mNeuZDCHi[dirIdx][ineu], 10, mNeuZDCHi[dirIdx][ineu], 1e4, arrColor, arrWidth, 2);
        }
        if(dirIdx==0){
            drawArrow(mNeuZDCLow[dirIdx][2], arrY, mNeuZDCLow[dirIdx][2]+6e3, arrY, arrSize, "|>", arrColor, arrWidth, 1);
        }
        else{
            drawArrow(mNeuZDCLow[dirIdx][2], arrY, mNeuZDCLow[dirIdx][2]+6e3*5/3.5, arrY, arrSize, "|>", arrColor, arrWidth, 1);
        }
    }
    hZDC[dirIdx]->Draw("psame");
    hZDC[dirIdx]->GetXaxis()->SetTitle(Form("ZDC_{%s} (a.u.)", mDir[dirIdx].Data()));
    if(dirIdx == 0){
        if(drawNeuRange){
            drawLatex(0.165, 0.64, "0n", mFont, mTextSize, arrColor);
            drawLatex(0.26 , 0.64, "1n", mFont, mTextSize, arrColor);
            drawLatex(0.40 , 0.64, "Xn", mFont, mTextSize, arrColor);
        }
    }
    else{
        if(drawNeuRange){
            drawLatex(0.16, 0.64, "0n", mFont, mTextSize, arrColor);
            drawLatex(0.28, 0.64, "1n", mFont, mTextSize, arrColor);
            drawLatex(0.42, 0.64, "Xn", mFont, mTextSize, arrColor);
        }
    }
    drawLatex(0.60, 0.94, "PbPb 5.02 TeV", mFont, mTextSize, 1);
    drawLatex(0.12, 0.94, "CMS", 62, 0.085, 1);
    hZDC[dirIdx]->GetYaxis()->SetTitle("Entries");
    //multiGaus[dirIdx]->DrawClone("same");
    for (Int_t i = 0; i < nGaus; i++) {
        if(i < nGaus-1) singleGaus[dirIdx][i]->SetParameters(multiGaus[dirIdx]->GetParameter(3*i), multiGaus[dirIdx]->GetParameter(3*i + 1), multiGaus[dirIdx]->GetParameter(3*i + 2));
        else            singleGaus[dirIdx][i]->SetParameters(multiGaus[dirIdx]->GetParameter(3*i), multiGaus[dirIdx]->GetParameter(3*i + 1), multiGaus[dirIdx]->GetParameter(3*i + 2)*multiGaus[dirIdx]->GetParameter(3*i - 1));

        singleGaus[dirIdx][i]->DrawClone("same");

        leg->AddEntry(singleGaus[dirIdx][i], Form("%dn", i+1), "l");
    }
    leg->Draw("same");

    Int_t binLow = hZDC[dirIdx]->GetXaxis()->FindBin(mZdcFitLow[dirIdx] + tinyOffset);
    Int_t binHi  = hZDC[dirIdx]->GetXaxis()->FindBin(mZdcFitHi[dirIdx] - tinyOffset);
    grZdcRatio[dirIdx] = new TGraphErrors(binHi - binLow + 1);
    grZdcPull[dirIdx]  = new TGraph(binHi - binLow + 1);
    hZDCPull[dirIdx]   = new TH1D(Form("hZDCPull%d",dirIdx),Form("; ZDC %s Pull; Entries", mDir[dirIdx].Data()), 50, -10, 10);
    for(Int_t ibin=binLow; ibin<=binHi; ibin++){
        Double_t x = hZDC[dirIdx]->GetBinCenter(ibin);
        Double_t y = hZDC[dirIdx]->GetBinContent(ibin);
        Double_t yErr = hZDC[dirIdx]->GetBinError(ibin);
        Double_t zdcFit = multiGaus[dirIdx]->Eval(x);

        grZdcRatio[dirIdx]->SetPoint(ibin-binLow, x, y/zdcFit);
        grZdcRatio[dirIdx]->SetPointError(ibin-binLow, 0, yErr/zdcFit);

        grZdcPull[dirIdx]->SetPoint(ibin-binLow, x, (y-zdcFit)/yErr);
        hZDCPull[dirIdx]->Fill((y-zdcFit)/yErr);
    }
    c1->SaveAs("ZDC_Energy.pdf");

    std::cout << "End of program !" << endl;
}
