{
   //                   Index:   0    1    2     3       4       5         6        7        8        9
   vector<char *> histnames = {"A1","A2","A12","TOF","TOF:A1","TOF:A2","TOF:A12","A1:A2","A1:A12","TOF:Si"};
   // Hist to fit for every run
   int hist = 3;
   // Open file and if it doesn't open right then quit
   char cutString[10];
   sprintf(cutString,"%s>800",histnames[hist]);

   cout<<"Enter Run Number: "<<endl;
   Int_t RunNumber;
   cin>>RunNumber;
   TFile *f;
   TTree *t;
   char filename[100];
   char drawString[50];
   TF1 *fitFunc;
   // TH1F *htemp = new TH1F("htemp",histnames[hist],1024,0,4096);
   sprintf(filename,"run%d.root",RunNumber);
   sprintf(drawString,"%s>>htemp(4096,0,4096)",histnames[hist]);
   f = new TFile(filename);
   t = (TTree*)f->Get("evtTree");
   // t->Draw(drawString,"TOF>2649.9 && TOF<2661.36");
   t->Draw(drawString,"TOF>100");
   // cout<<"Fit for Run "<<RunNumber<<" Started"<<endl;
   // htemp->Fit("gaus","","",2660,2680);
   htemp->Fit("gaus");

   // cout<<"Fit for Run "<<RunNumber<<" Succeeded!"<<endl;
   // cin.get();
   // fitFunc = htemp->GetFunction("gaus");
}
