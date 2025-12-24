{
   // Runs from May 2022 experiment, were numebrs 10808 and 10810
TFile *f1 = new TFile("run3662.root");
TFile *f2 = new TFile("run3663.root");
// Runs from April 2025 Experiment
TFile *f3 = new TFile("run3721.root");
TFile *f4 = new TFile("run3722.root");

TTree *t1 = (TTree*)f1->Get("evtTree");
TTree *t2 = (TTree*)f2->Get("evtTree");
TTree *t3 = (TTree*)f3->Get("evtTree");
TTree *t4 = (TTree*)f4->Get("evtTree");

//
TH1F *h1 = new TH1F("h1","",60,2105,2165);
TH1F *h2 = new TH1F("h2","",60,2105,2165);
TH1F *h3 = new TH1F("h3","",50,2185,2235);
TH1F *h4 = new TH1F("h4","",50,2185,2235);
h1->SetStats(0);
h2->SetStats(0);
h3->SetStats(0);
h4->SetStats(0);


TCanvas *c1 = new TCanvas("c1");

t1->Draw("TOF>>h1","A1>800","");
t2->Draw("TOF>>h2","A1>800","SAME");
h1->Fit("gaus");
TF1 *fun1 = h1->GetFunction("gaus");
h1->GetXaxis()->SetTitle("ToF Channel");
h1->GetXaxis()->CenterTitle(true);
h2->Fit("gaus");
Double_t height1 = h1->GetFunction("gaus")->GetParameter(0);
Double_t height2 = h2->GetFunction("gaus")->GetParameter(0);
Double_t mean1 = h1->GetFunction("gaus")->GetParameter(1);
Double_t mean2 = h2->GetFunction("gaus")->GetParameter(1);
Double_t stdev1 = h1->GetFunction("gaus")->GetParameter(2);
Double_t stdev2 = h2->GetFunction("gaus")->GetParameter(2);
TLine *vl1 = new TLine(mean1, 0, mean1, height1);
TLine *vl2 = new TLine(mean2, 0, mean2, height2);
// TLine *vl1 = new TLine(h1->GetFunction("gaus")->GetParameter(1));
vl1->SetLineColor(kBlue);
vl1->SetLineWidth(3);
vl2->SetLineColor(kBlue);
vl2->SetLineWidth(3);
vl1->Draw("same");
vl2->Draw("same");
TPaveText *pt = new TPaveText(2145,16000,2163,18000);
pt->SetTextColor(kBlue);
pt->AddText("FWHM: 0.778 amu");
pt->SetFillColorAlpha(kWhite,0);
pt->Draw("same");

TPaveText *pt234 = new TPaveText(2106,4000,2116,6000);
pt234->SetTextColor(kBlack);
pt234->AddText("^{234}U");
pt234->SetFillColor(kWhite);
pt234->Draw("same");

TPaveText *pt235 = new TPaveText(2150,4000,2160,6000);
pt235->SetTextColor(kBlack);
pt235->AddText("^{235}U");
pt235->SetFillColor(kWhite);
pt235->Draw("same");

TBox *fwhm234 = new TBox(mean1-2.355/2*stdev1,0,mean1+2.355/2*stdev1,height1/2);
fwhm234->SetFillColorAlpha(kBlue,0);
fwhm234->SetLineColor(kBlue);
fwhm234->SetLineWidth(3);
fwhm234->Draw("l");


TBox *fwhm235 = new TBox(mean2-2.355/2*stdev2,0,mean2+2.355/2*stdev2,height2/2);
fwhm235->SetFillColorAlpha(kBlue,0);
fwhm235->SetLineColor(kBlue);
fwhm235->SetLineWidth(3);
fwhm235->Draw("l");
//

// c1->cd(2);
// t3->Draw("TOF>>h3","A1>800","");
// t4->Draw("TOF>>h4","A1>800","SAME");
// h3->Fit("gaus");
// TF1 *fun2 = h3->GetFunction("gaus");
// h3->GetXaxis()->SetTitle("ToF Channel");
// h3->GetYaxis()->SetTitle("ToF Channel");
// h4->Fit("gaus");

c1->SaveAs("/Users/thomasbailey/Documents/AMS/Dissertation/Figures/Chapter6/ResolutionCompOld.pdf");

}
