//Created by Thomas Bailey
//01.18.2021

//Simple macro for merging runs in root
{TTree *t2;
TTree *t;
TList *l = new TList;

int runnums[24];

int runnum = 4123;
int runend = 4130;
char filename1[100];

//change the array you're using to define runnums here

  // cout<<"hi! this is the program that will merge multiple runs into a single tree"<<endl;
  //
  // cout<<"what run do you want to start with?"<<endl;
  // cin>>runnum;
  // cout<<"what run do you want to end with?"<<endl;
  // cin>>runend;
  sprintf(filename1,"run%d.root",runnum);

TFile *f = new TFile(filename1);
TTree *t1 = (TTree*)f->Get("evtTree");
l->Add(t1);
  for(int i = runnum+1; i<= runend; i++){
    char filename2[100];
    sprintf(filename2,"run%d.root",i);
    TFile *f2 = new TFile(filename2);
    t2 = (TTree*)f2->Get("evtTree");
    l->Add(t2);
  }
t = TTree::MergeTrees(l);

// t->Draw("TOF:A1>>hToFvsA1(250,900,1400,150,2350,2500)","","colz");
Double_t x1,y1,r1,r2,phi1,phi2,theta,angle,dx,dy,dphi,ct,st,pi;
pi = 3.14159;;
Int_t n,nStDevs;
n= 100;
nStDevs = 3;
TString cutName,cutName2,cutString;
Double_t A,B,C;
A = -0.000430837;
B = 2.85526;
// Added 20 channels to accomodate slight skewness
C = -3564.52;
Int_t ci;      // for color index setting
TColor *color; // for color definition with alpha
ci = 1193;
color = new TColor(ci, 1, 1, 1, " ", 0);
x1 = A*runnum*runnum+B*runnum+C;
// 234: 2501.0444
// 235: 2488.0991
// 236: 2475.1537, modified by eye to 2473.153
// Moved this down 2 channels to fit region better. Larger tail
// on lower side combined with 235 contaminant on higher side
y1 = 2486.0991;
r1 = 44.11831882*nStDevs;
r2 = 2.278775991*nStDevs;
phi1 = 0;
phi2 = 360;
theta = 0;
dphi = (phi2-phi1)*pi/(180*n);
ct   = TMath::Cos(pi*theta/180);
st   = TMath::Sin(pi*theta/180);
cutName = "A1ToF235";
TCutG *A1ToF235 = new TCutG(cutName,n);
A1ToF235->SetVarX("A1");
A1ToF235->SetVarY("TOF");


for (Int_t i=0;i<=n;i++) {
 Int_t point = i;
 angle = phi1*pi/180 + Double_t(i)*dphi;
 dx    = r1*TMath::Cos(angle);
 dy    = r2*TMath::Sin(angle);
 A1ToF235->SetPoint(point, x1 + dx*ct - dy*st, y1 + dx*st + dy*ct);
}
cutName2 = "A1ToF236";
TCutG *A1ToF236 = new TCutG(cutName2,n);
A1ToF236->SetVarX("A1");
A1ToF236->SetVarY("TOF");
y1 = 2473.153;
for (Int_t i=0;i<=n;i++) {
 Int_t point = i;
 angle = phi1*pi/180 + Double_t(i)*dphi;
 dx    = r1*TMath::Cos(angle);
 dy    = r2*TMath::Sin(angle);
 A1ToF236->SetPoint(point, x1 + dx*ct - dy*st, y1 + dx*st + dy*ct);
}
// t->Draw("TOF:A1","A1ToF235 && A1ToF236","colz");
// TH1F *htemp = (TH1F*)gPad->GetPrimitive("htemp");
TEllipse *ellipse1 = new TEllipse(x1, y1, r1, r2);
TEllipse *ellipse2 = new TEllipse(x1, 2486.0091, r1, r2);
ellipse1->SetFillColor(ci);
ellipse1->SetLineWidth(5);
ellipse1->SetLineColor(2);
ellipse2->SetFillColor(ci);
ellipse2->SetLineWidth(5);
ellipse2->SetLineColor(1);

TCanvas *c1 = new TCanvas("c1","c1",800,1200);
TPad *p1 = new TPad("p1","p1",0.0,0.5,1,1);
TPad *p2 = new TPad("p2","p2",0.0,0.0,1.0,0.5);
p1->Draw();
p2->Draw();

p1->cd();
p1->SetLogy();

// t->Draw("TOF:A1>>hToFvsA1(250,1400,2000,150,2100,2250)","","colz");
t->Draw("TOF>>hToF(80,2155,2235)","","goff");
hToF->SetTitle("");
hToF->GetXaxis()->SetTitle("Channel");
hToF->GetXaxis()->CenterTitle();
hToF->SetStats(0);
hToF->GetYaxis()->SetTitle("Counts");
hToF->GetYaxis()->CenterTitle();
hToF->Draw();

p2->cd();
t->Draw("A1:TOF>>hA1vsToF(80,2155,2235,350,1350,2050)","","goff");
hA1vsToF->SetTitle("");
hA1vsToF->GetXaxis()->SetTitle("Channel");
hA1vsToF->GetXaxis()->CenterTitle();
hA1vsToF->SetStats(0);
hA1vsToF->GetYaxis()->SetTitle("Anode 1 (channels)");
hA1vsToF->GetYaxis()->CenterTitle();
hA1vsToF->Draw("colz");

c1->SaveAs("/Users/thomasbailey/Documents/AMS/Presentations/DefenseTalk/CompToAdamData.pdf");
// ellipse1->Draw();
// ellipse2->Draw();
}
