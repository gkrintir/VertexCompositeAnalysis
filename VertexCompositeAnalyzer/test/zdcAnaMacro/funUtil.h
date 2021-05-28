const Int_t    nDirs = 2; // 0 - Plus; 1 - Minus;
const TString  mDir[nDirs] = {"Plus", "Minus"};
const Double_t mZdcFitLow[nDirs]  = {4.2e3, 6.e3};
const Double_t mZdcFitHi[nDirs]   = {25.e3, 37.5e3};

const Int_t nNeus = 3;

const Double_t mNeuZDCLow[nDirs][nNeus] = {
    {0, 4.2e3, 10.e3},
    {0, 6.0e3, 16.e3}
};
const Double_t mNeuZDCHi[nDirs][nNeus] = {
    {4.2e3, 10.e3, 5.e5},
    {6.0e3, 16.e3, 6.e5}
};

//__________________________________________________
void setPad(Double_t left, Double_t right, Double_t top, Double_t bottom)
{
    gPad->SetFillColor(10);
    gPad->SetBorderMode(0);
    gPad->SetBorderSize(0);
    gPad->SetFrameFillColor(10);
    gPad->SetFrameBorderMode(0);
    gPad->SetFrameBorderSize(0);
    gPad->SetLeftMargin(left);
    gPad->SetRightMargin(right);
    gPad->SetTopMargin(top);
    gPad->SetBottomMargin(bottom);
}

//__________________________________________________
void setFun(TF1 *f, Int_t lineColor, Int_t lineWidth=2, Int_t lineStyle=1)
{
    f->SetLineColor(lineColor);
    f->SetLineWidth(lineWidth);
    f->SetLineStyle(lineStyle);
}

//__________________________________________________
void setHisto(TH1D *h,Int_t markerStyle, Double_t markerSize, Int_t markerColor,Int_t lineColor,Int_t lineWidth=1)
{
    h->SetMarkerStyle(markerStyle);
    h->SetMarkerSize(markerSize);
    h->SetMarkerColor(markerColor);
    h->SetLineColor(lineColor);
    h->SetLineWidth(lineWidth);
};

//__________________________________________________
TArrow*  drawArrow(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t arrowSize=0.05, TString arrowOption="|>", Int_t lineColor=1, Int_t lineWidth=1, Int_t lineStyle=1)
{
    TArrow *arrow = new TArrow(x1, y1, x2, y2, arrowSize, arrowOption.Data());
    arrow->SetFillColor(lineColor);
    arrow->SetLineColor(lineColor);
    arrow->SetLineWidth(lineWidth);
    arrow->SetLineStyle(lineStyle);
    arrow->Draw();
    return arrow;
}

//__________________________________________________
TLatex* drawLatex(Double_t x, Double_t y, TString text, Int_t textFont, Double_t textSize, Int_t colorIndex, Double_t textAngle=0)
{
    TLatex *latex = new TLatex(x,y,text.Data());
    latex->SetNDC();
    latex->SetTextFont(textFont);
    latex->SetTextSize(textSize);
    latex->SetTextColor(colorIndex);
    latex->SetTextAngle(textAngle);
    latex->Draw("same");
    return latex;
}

//__________________________________________________
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Int_t lineColor, Int_t lineWidth, Int_t lineStyle)
{
    TLine *l1 = new TLine(xlow,ylow,xup,yup);
    l1->SetLineColor(lineColor);
    l1->SetLineWidth(lineWidth);
    l1->SetLineStyle(lineStyle);
    l1->Draw("same");
    return l1;
}
