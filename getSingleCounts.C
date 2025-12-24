{
// #include "cuts.h"
// Reads in the Run Data into a DataFrame
// auto df = ROOT::RDF::MakeCsvDataFrame("Fits.csv");
// auto df1 = df.Filter("RunSet == 1");
// auto df2 = df.Filter("RunSet == 2");
// // Note: The CSV file can NOT have any empty spots, e.g. "5,,3"
// // These Data frames store info for a particular mass
// auto df2361 = df1.Filter("MassAnalyzed == 236 && RunNumber > 3000");
// auto df2351 = df1.Filter("MassAnalyzed == 235 && RunNumber > 3000");
// auto df2341 = df1.Filter("MassAnalyzed == 234 && RunNumber > 3000");
//
// auto df2362 = df2.Filter("MassAnalyzed == 236 && RunNumber > 3000");
// auto df2352 = df2.Filter("MassAnalyzed == 235 && RunNumber > 3000");
// auto df2342 = df2.Filter("MassAnalyzed == 234 && RunNumber > 3000");
// Here we are plotting the A1 235 data and using it to predict the
// appropriate 236 region after noticing a trend throughout the run
// auto h1 = df235.Histo1D({"Runs","Runs",1000,3000,4000},"RunNumber");
// auto graph = df235.Graph("RunNumber","A1Mean");
// graph->Fit("pol2");
// TF1 *f1 = graph->GetFunction("pol2");
// graph->DrawClone("APL");
Int_t RunNumber = 4137;
Int_t Mass = 236;

char filename1[100];
Double_t A1Mean,ToFMean,A1StDev,ToFStDev,phi1,phi2,theta,angle,dx,dy,dphi,ct,st,pi,nStDevs;
pi = 3.14159;;
Int_t n;
n= 100;
nStDevs = 3;
TFile *f;
TTree *t;
TString cutName,cutString;

Int_t ci;      // for color index setting
TColor *color; // for color definition with alpha
ci = 1193;
color = new TColor(ci, 1, 1, 1, " ", 0);
Double_t ToFShift = 2.0;
switch (Mass) {
    case 239:
        ToFMean = 2168.192 - ToFShift;
        ToFStDev = 1.558 * (nStDevs);
        A1Mean = 1616.199;
        A1StDev = 53.320 * (nStDevs + 4);
        break;
    case 238:
        ToFMean = 2182.163 - ToFShift;
        ToFStDev = 1.558 * (nStDevs);
        A1Mean = 1624.722;
        A1StDev = 53.320 * (nStDevs + 4);
        break;
    case 235:
        ToFMean = 2224.254 - ToFShift;
        ToFStDev = 1.548 * (nStDevs);
        A1Mean = 1650.399;
        A1StDev = 53.445 * (nStDevs + 4);
        break;
    case 234:
        ToFMean = 2238.344 - ToFShift;
        ToFStDev = 1.568 * (nStDevs);
        A1Mean = 1658.995;
        A1StDev = 53.194 * (nStDevs + 4);
        break;
    case 236:
        ToFMean = 2210.194 - ToFShift;
        ToFStDev = 1.558 * (nStDevs);
        A1Mean = 1641.822;
        A1StDev = 53.320 * (nStDevs + 4);
        break;
    case 231:
        ToFMean = 2280.795 - ToFShift;
        ToFStDev = 1.558 * nStDevs;
        A1Mean = 1684.892;
        A1StDev = 53.320 * (nStDevs + 4);
        break;
    default:
        cout<<"Unknown Mass"<<Mass<<endl;
        break;
   }

phi1 = 0;
phi2 = 360;
theta = 0;
dphi = (phi2-phi1)*pi/(180*n);
ct   = TMath::Cos(pi*theta/180);
st   = TMath::Sin(pi*theta/180);
cutName = "A1ToF";
TCutG *A1ToF = new TCutG(cutName,n);
A1ToF->SetVarX("A1");
A1ToF->SetVarY("TOF");
TEllipse *ellipse = new TEllipse(A1Mean,ToFMean,A1StDev,ToFStDev);
for (Int_t i=0;i<=n;i++) {
    Int_t point = i;
    angle = phi1*pi/180 + Double_t(i)*dphi;
    dx    = A1StDev*TMath::Cos(angle);
    dy    = ToFStDev*TMath::Sin(angle);
    A1ToF->SetPoint(point, A1Mean + dx*ct - dy*st, ToFMean + dx*st + dy*ct);
}

ellipse->SetFillColor(ci);
ellipse->SetLineWidth(3);
char PlotTitle[20];
sprintf(PlotTitle,"Run: %d",RunNumber);
sprintf(filename1,"run%d.root",RunNumber);
f = new TFile(filename1);
t = (TTree*)f->Get("evtTree");
t->Draw("TOF:A1>>hToFvsA1(200,1200,2000,150,2100,2250)","A1ToF","colz");
TH2F *h = (TH2F*)gPad->GetPrimitive("hToFvsA1");
h->SetTitle(PlotTitle);
h->GetYaxis()->SetTitle("ToF");
h->GetXaxis()->SetTitle("A1");
h->SetStats(0);
h->Draw("colz");
ellipse->Draw();
// cutBlank->Draw();
cout<<h->GetEntries()<<endl;
}
