{
   TFile *f1 = new TFile("run4082.root");
   TFile *f2 = new TFile("run4081.root");
   TTree *t1 = (TTree*)f1->Get("evtTree");
   TTree *t2 = (TTree*)f2->Get("evtTree");
   TH1F *h1 = new TH1F("h1","",50,2205,2255);
   TH1F *h2 = new TH1F("h2","",50,2205,2255);
   h1->SetStats(0);
   h2->SetStats(0);
   TCanvas *c1 = new TCanvas("c1");
   t2->Draw("TOF>>h1","A1>800","");
   t1->Draw("TOF>>h2","A1>800","SAME");
   h1->Fit("gaus");
   TF1 *fun1 = h1->GetFunction("gaus");
   h1->GetXaxis()->SetTitle("ToF Channel");
   h1->GetXaxis()->CenterTitle(true);
   // h2->GetYaxis()->SetTitle("ToF Channel");
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
   TPaveText *pt = new TPaveText(2206,3500,2223,4000);
   pt->SetTextColor(kBlue);
   pt->AddText("FWHM: 0.260 amu");
   // pt->AddText("#Delta t (ns): 1.758");
   pt->SetFillColorAlpha(kWhite,0);
   pt->Draw("same");

   TPaveText *pt234 = new TPaveText(2245,2000,2252,2500);
   pt234->SetTextColor(kBlack);
   pt234->AddText("^{234}U");
   pt234->SetFillColor(kWhite);
   pt234->Draw("same");

   TPaveText *pt235 = new TPaveText(2208,2000,2220,2500);
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

   c1->SaveAs("/Users/thomasbailey/Documents/AMS/Presentations/DefenseTalk/ResolutionCompNew.pdf");
}
