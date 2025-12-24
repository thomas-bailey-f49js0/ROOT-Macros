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
   h2->GetYaxis()->SetTitle("ToF Channel");
   h2->Fit("gaus");
   Double_t mean1 = h1->GetFunction("gaus")->GetParameter(1);
   Double_t mean2 = h2->GetFunction("gaus")->GetParameter(1);
   TLine *vl1 = new TLine(mean1, 0, mean1, 1810);
   TLine *vl2 = new TLine(mean2, 0, mean2, 1810);
   // TLine *vl1 = new TLine(h1->GetFunction("gaus")->GetParameter(1));
   vl1->SetLineColor(kBlue);
   vl1->SetLineWidth(3);
   vl2->SetLineColor(kBlue);
   vl2->SetLineWidth(3);
   vl1->Draw("same");
   vl2->Draw("same");
   TPaveText *pt = new TPaveText(2215,1400,2233,1800);
   pt->SetTextColor(kBlue);
   pt->AddText("#Delta t (channels): 13.95");

   pt->AddText("#Delta t (ns): 1.758");
   pt->SetFillColor(kWhite);
   pt->Draw("same");

   TPaveText *pt234 = new TPaveText(2215,600,2220,800);
   pt234->SetTextColor(kBlack);
   pt234->AddText("^{234}U");
   pt234->SetFillColor(kWhite);
   pt234->Draw("same");

   TPaveText *pt235 = new TPaveText(2210,600,2220,800);
   pt235->SetTextColor(kBlack);
   pt235->AddText("^{235}U");
   pt235->SetFillColor(kWhite);
   pt235->Draw("same");

   c1->SaveAs("/Users/thomasbailey/Documents/AMS/Presentations/DefenseTalk/ResolutionPlot.pdf");
}
