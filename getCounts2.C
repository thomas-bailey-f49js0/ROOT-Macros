{
   // #include "cuts.h"
// Output file for fit data

// Reads in the Run Data into a DataFrame
auto df = ROOT::RDF::MakeCsvDataFrame("Fits.csv");
// Note: The CSV file can NOT have any empty spots, e.g. "5,,3"
// These Data frames store info for a particular mass
auto df236 = df.Filter("MassAnalyzed == 236 && RunNumber > 3387");
auto df235 = df.Filter("MassAnalyzed == 235 && RunNumber > 3387");
auto df234 = df.Filter("MassAnalyzed == 234 && RunNumber > 3387");

// For some reason, Not having this bit
// here causes the whole code to fail.
// It will take issue with RunNumber and Mass
// Getting filled as Long64_t and not int
// like they should be.
auto graph = df235.Graph("RunNumber","A1Mean");
graph->Fit("pol2");
ofstream outFile("counts.txt",std::ios_base::app);
if (!outFile){
   cout<<"File can not be opened"<<endl;
   exit(1);
} else {
   remove("counts.txt");
}
// TH2F* h1;
// TF1 *f1 = graph->GetFunction("pol2");
cout<<"Start For Loop"<<endl;
df.Foreach([](int RunNumber,int Mass){
   ofstream outFile("counts.txt",std::ios_base::app);
   if (!outFile){
      cout<<"File can not be opened"<<endl;
      exit(1);
   }
   // Can use this to force using the same cut region on each run
   int DesiredMass = 236;
   // Use the cut regions established using only a subset of the data
   // outFile<<""<<RunNumber<<", ";
   if (Mass == 0){
      outFile<< "0"<<endl;
   } else {
      char filename1[100];
      Double_t A1Mean,ToFMean,A1StDev,ToFStDev,phi1,phi2,theta,angle,dx,dy,dphi,ct,st,pi;
      pi = 3.14159;;
      Double_t n,nStDevs;
      n= 100;
      nStDevs = 6.0;
      TFile *f;
      // TH2F *htemp;
      TTree *t;
      TString cutName,cutString;
      Int_t ci;      // for color index setting
      TColor *color; // for color definition with alpha
      ci = 1193;
      color = new TColor(ci, 1, 1, 1, " ", 0);
      Double_t ToFShift = 1.0;
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
      TEllipse *ellipse = new TEllipse(A1Mean, ToFMean, A1StDev, ToFStDev);

      ellipse->SetFillColor(ci);
      ellipse->SetLineWidth(3);

      for (Int_t i=0;i<=n;i++) {
        Int_t point = i;
        angle = phi1*pi/180 + Double_t(i)*dphi;
        dx    = A1StDev*TMath::Cos(angle);
        dy    = ToFStDev*TMath::Sin(angle);
        A1ToF->SetPoint(point, A1Mean + dx*ct - dy*st, ToFMean + dx*st + dy*ct);
      }

      TCutG *A1A2Cut = new TCutG("A1A2Cut",7);
      A1A2Cut->SetVarX("A2");
      A1A2Cut->SetVarY("A1");
      A1A2Cut->SetTitle("Graph");
      A1A2Cut->SetFillStyle(1000);
      A1A2Cut->SetPoint(0,1076.79,1526.32);
      A1A2Cut->SetPoint(1,1065.33,1817.5);
      A1A2Cut->SetPoint(2,1145.56,1891.32);
      A1A2Cut->SetPoint(3,1483.67,1624.74);
      A1A2Cut->SetPoint(4,1437.82,1514.01);
      A1A2Cut->SetPoint(5,1179.94,1464.8);
      A1A2Cut->SetPoint(6,1076.79,1526.32);

      sprintf(filename1,"run%d.root",RunNumber);
      f = new TFile(filename1);
      t = (TTree*)f->Get("evtTree");
      t->Draw("A1","A1ToF","");
      TH1F *htemp = (TH1F*)gPad->GetPrimitive("htemp");

      // cin.get();
      try{
         outFile<<htemp->GetEntries()<<endl;

      }
      catch(...){
         outFile<<"0"<<endl;
      }

      // outFile<<htemp->GetEntries()<<endl;
      // ellipse->Draw();
   }
}, {"RunNumber","MassAnalyzed"});
}
