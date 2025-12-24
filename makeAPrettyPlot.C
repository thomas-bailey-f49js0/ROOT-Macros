{
   #include "cuts.h"

   TList *l1 = new TList;
   TList *l2 = new TList;
   TList *l3 = new TList;
   TList *l4 = new TList;


   char filename1[100];

   int runnum = 3651;
   int runend = 3655;
   sprintf(filename1,"run%d.root",runnum);
   TFile *f1 = new TFile(filename1);
   TTree *t1 = (TTree*)f1->Get("evtTree");
   TTree *t;
   l1->Add(t1);
     for(int i = runnum+1; i<= runend; i++){
       char filename2[100];
       sprintf(filename2,"run%d.root",i);
       TFile *f = new TFile(filename2);
       t = (TTree*)f->Get("evtTree");
       l1->Add(t);
     }
   t1 = TTree::MergeTrees(l1);

   runnum = 3603;
   runend = 3610;
   sprintf(filename1,"run%d.root",runnum);
   f1 = new TFile(filename1);
   TTree *t2 = (TTree*)f1->Get("evtTree");
   l2->Add(t2);
     for(int i = runnum+1; i<= runend; i++){
       char filename2[100];
       sprintf(filename2,"run%d.root",i);
       TFile *f = new TFile(filename2);
       t = (TTree*)f->Get("evtTree");
       l2->Add(t);
     }
   t2 = TTree::MergeTrees(l2);

   runnum = 3616;
   runend = 3623;
   sprintf(filename1,"run%d.root",runnum);
   f1 = new TFile(filename1);
   TTree *t3 = (TTree*)f1->Get("evtTree");
   l3->Add(t3);
     for(int i = runnum+1; i<= runend; i++){
       char filename2[100];
       sprintf(filename2,"run%d.root",i);
       TFile *f = new TFile(filename2);
       t = (TTree*)f->Get("evtTree");
       l3->Add(t);
     }
   t3 = TTree::MergeTrees(l3);

   runnum = 3628;
   runend = 3635;
   sprintf(filename1,"run%d.root",runnum);
   f1 = new TFile(filename1);
   TTree *t4 = (TTree*)f1->Get("evtTree");
   l4->Add(t4);
     for(int i = runnum+1; i<= runend; i++){
       char filename2[100];
       sprintf(filename2,"run%d.root",i);
       TFile *f = new TFile(filename2);
       t = (TTree*)f->Get("evtTree");
       l4->Add(t);
     }
   t4 = TTree::MergeTrees(l4);

   TH2F *h1 = new TH2F("h1","",250,880,1380,150,2405,2555);
   TH2F *h2 = new TH2F("h2","",250,880,1380,150,2405,2555);
   TH2F *h3 = new TH2F("h3","",250,880,1380,150,2405,2555);
   TH2F *h4 = new TH2F("h4","",250,880,1380,150,2405,2555);
   TH2F *h5 = new TH2F("h5","",250,880,1380,150,2405,2555);

   h1->SetStats(0);
   h2->SetStats(0);
   h3->SetStats(0);
   h4->SetStats(0);
   h5->SetStats(0);

   Double_t x1,y1,r1,r2,phi1,phi2,theta,angle,dx,dy,dphi,ct,st,pi;
   pi = 3.14159;;
   Int_t n,nStDevs;
   n= 100;
   nStDevs = 3;

   TString cutName,cutString;
   Double_t A,B,C;
   A = -0.000430837;
   B = 2.85526;
   // Added 20 channels to accomodate slight skewness
   C = -3564.52;
   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1193;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   x1 = 1153.200;
   // 234: 2501.0444
   // 235: 2488.0991
   // 236: 2475.1537
   // Moved this down 2 channels to fit region better. Larger tail
   // on lower side combined with 235 contaminant on higher side
   // y1 = 2473.153;
   // r1 = 44.11831882*nStDevs;
   // r2 = 2.278775991*nStDevs;
   // phi1 = 0;
   // phi2 = 360;
   // theta = 0;
   // dphi = (phi2-phi1)*pi/(180*n);
   // ct   = TMath::Cos(pi*theta/180);
   // st   = TMath::Sin(pi*theta/180);
   // cutName = "A1ToF";
   // TCutG *A1ToF = new TCutG(cutName,n);
   // A1ToF->SetVarX("A1");
   // A1ToF->SetVarY("TOF");
   // TEllipse *ellipse1 = new TEllipse(x1, y1, r1, r2);
   // TEllipse *ellipse2 = new TEllipse(x1, 2488.099, r1, r2);
   // TEllipse *ellipse3 = new TEllipse(x1, 2444, r1, r2);
   //
   // ellipse1->SetFillColor(ci);
   // ellipse1->SetLineWidth(5);
   // ellipse1->SetLineColor(2);
   // ellipse2->SetFillColor(ci);
   // ellipse2->SetLineWidth(5);
   // ellipse2->SetLineColor(1);
   // ellipse3->SetFillColor(ci);
   // ellipse3->SetLineColor(3);
   // ellipse3->SetLineWidth(5);
   //
   // for (Int_t i=0;i<=n;i++) {
   //  Int_t point = i;
   //  angle = phi1*pi/180 + Double_t(i)*dphi;
   //  dx    = r1*TMath::Cos(angle);
   //  dy    = r2*TMath::Sin(angle);
   //  A1ToF->SetPoint(point, x1 + dx*ct - dy*st, y1 + dx*st + dy*ct);
   // }

   t1->Draw("TOF:A1>>h1","","goff");
   t2->Draw("TOF:A1>>h2","","goff");
   t3->Draw("TOF:A1>>h3","","goff");
   t4->Draw("TOF:A1>>h4","","goff");
   t4->Draw("TOF:A1>>h5","","goff");

   // h1->GetXaxis()->SetTitle("Energy in A1 (channels)");
   // h1->GetYaxis()->SetTitle("Time of Flight (channels)");
   // h1->SetTitle("^{236}U Spectra: Control-1");
   // h1->SetStats(false);
   // h2->GetXaxis()->SetTitle("Energy in A1 (channels)");
   // h2->GetYaxis()->SetTitle("Time of Flight (channels)");
   // h2->SetTitle("");
   // h2->SetStats(false);
   TCanvas *c1 = new TCanvas("c1","Data Plots",1200,800);
   c1->Draw();
   TPad *p1 = new TPad("p1","p1",0.05,0.54,0.485,0.99);
   TPad *p2 = new TPad("p2","p2",0.48,0.54,0.92,0.99);
   TPad *p3 = new TPad("p3","p3",0.05,0.09,0.485,0.545);
   TPad *p4 = new TPad("p4","p4",0.48,0.09,0.92,0.545);
   TPad *p5 = new TPad("p5","p5",0.0,0.0,1.0,1.0);
   p5->Draw();
   p1->Draw();
   p2->Draw();
   p3->Draw();
   p4->Draw();


   p1->cd();

   h1->GetZaxis()->SetRangeUser(0,90);
   gPad->SetBottomMargin(0);
   gPad->SetRightMargin(0);
   h1->GetXaxis()->SetLabelSize(0);
   h1->Draw("col");
   TPaveText *text1 = new TPaveText(.2,.8,.6,.9,"NDCNB");
   text1->AddText("Control-1");
   text1->Draw();
   text1->SetFillStyle(0);
   text1->Draw();

   p2->cd();
   h2->GetZaxis()->SetRangeUser(0,90);
   gPad->SetBottomMargin(0);
   gPad->SetLeftMargin(0);
   h2->GetXaxis()->SetLabelSize(0);
   h2->GetYaxis()->SetLabelSize(0);
   h2->Draw("col");
   TPaveText *text2 = new TPaveText(.2,.8,.6,.9,"NDCNB");
   text2->AddText("USRM-1");
   text2->Draw();
   text2->SetFillStyle(0);
   text2->Draw();

   p3->cd();
   h3->GetZaxis()->SetRangeUser(0,90);
   gPad->SetTopMargin(0);
   gPad->SetRightMargin(0);
   h3->Draw("col");
   TPaveText *text3 = new TPaveText(.2,.9,.6,1.0,"NDCNB");
   text3->AddText("USRM-2");
   text3->Draw();
   text3->SetFillStyle(0);
   text3->Draw();

   p4->cd();
   h4->GetZaxis()->SetRangeUser(0,90);
   gPad->SetTopMargin(0);
   gPad->SetLeftMargin(0);
   h4->GetYaxis()->SetLabelSize(0);
   h4->Draw("col");
   TPaveText *text4 = new TPaveText(.2,.9,.6,1.0,"NDCNB");
   text4->AddText("USRM-3");
   text4->Draw();
   text4->SetFillStyle(0);
   text4->Draw();

   p5->cd();
   h5->GetZaxis()->SetRangeUser(0,90);
   h5->GetXaxis()->SetTitle("Energy on Anode 1");
   h5->GetXaxis()->CenterTitle(true);
   h5->SetLabelSize(0.000005,"XY");
   h5->GetYaxis()->SetTitle("ToF Channel");
   h5->GetYaxis()->CenterTitle(true);
   h5->Draw("colz");

   c1->SaveAs("../../../Papers/236U/StandardsPaper/Spectra.pdf");
}
