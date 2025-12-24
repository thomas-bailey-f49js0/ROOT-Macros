{
   char filename1[100];

   int runnum = 4104;
   int runend = 4107;
   sprintf(filename1,"run%d.root",runnum);
   TFile *f1 = new TFile(filename1);
   TTree *t1 = (TTree*)f1->Get("evtTree");
   TList *l1 = new TList;
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
   Double_t y235,y236,y238,y239,x,rx,ry;
   Double_t ToFShift = 2.0;
   Double_t nStDevs = 3;

   y239 = 2168.192 - ToFShift;
   y238 = 2182.163 - ToFShift;
   y235 = 2224.254 - ToFShift;
   y236 = 2210.194 - ToFShift;
   x = 1658.995;
   rx = 53.320 * (nStDevs + 4);
   ry = 1.558 * (nStDevs);

   TEllipse *ellipse235 = new TEllipse(x, y235, rx, ry);
   TEllipse *ellipse236 = new TEllipse(x, y236, rx, ry);
   TEllipse *ellipse238 = new TEllipse(x, y238, rx, ry);
   TEllipse *ellipse239 = new TEllipse(x, y239, rx, ry);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1193;
   color = new TColor(ci, 1, 1, 1, " ", 0);

   ellipse235->SetFillColor(ci);
   ellipse235->SetLineWidth(5);
   ellipse235->SetLineColor(2);
   ellipse236->SetFillColor(ci);
   ellipse236->SetLineWidth(5);
   ellipse236->SetLineColor(1);
   ellipse238->SetFillColor(ci);
   ellipse238->SetLineColor(3);
   ellipse238->SetLineWidth(5);
   ellipse239->SetFillColor(ci);
   ellipse239->SetLineColor(4);
   ellipse239->SetLineWidth(5);

   TH2F *h = new TH2F("h","",300,1200,2100,110,2135,2245);
   t1->Draw("TOF:A1>>h","A1>800 && A1<1950","colz");
   h->SetStats(0);
   h->GetYaxis()->SetTitle("Time of Flight (channels)");
   h->GetXaxis()->SetTitle("Energy on Anode 1 (channels)");
   h->GetXaxis()->CenterTitle(true);
   h->GetYaxis()->CenterTitle(true);
   ellipse235->Draw();
   ellipse236->Draw();
   ellipse238->Draw();
   ellipse239->Draw();

   TLegend *l = new TLegend(0.7,0.13,0.9,0.35);
   l->SetHeader("Mass Regions","C"); // option "C" allows to center the header
   l->AddEntry(ellipse235,"Mass 235","l");
   l->AddEntry(ellipse236,"Mass 236","l");
   l->AddEntry(ellipse238,"Mass 238","l");
   l->AddEntry(ellipse239,"Mass 239","l");
   l->SetTextSize(0.04f);
   l->Draw();
   c1->SaveAs("/Users/thomasbailey/Documents/AMS/Dissertation/Figures/Chapter5/ViennaBkgSubRegions.pdf");
}
