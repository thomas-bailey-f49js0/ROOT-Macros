{
   #include <iostream>
   #include <cmath>
   #include <iomanip>
   #include <cstdlib>
   #include <ctype.h>
   #include <cmath>
   #include <string>
   #include <fstream>
   #include <sstream>

   using namespace std;
   // List of available histograms
   //                   Index:   0    1    2     3       4       5         6        7        8        9
   vector<char *> histnames = {"A1","A2","A12","TOF","TOF:A1","TOF:A2","TOF:A12","A1:A2","A1:A12","TOF:Si"};
   // Hist to fit for every run
   int hist = 3;
   // Open file and if it doesn't open right then quit
   ifstream inFile("RunData.tsv");
   if (!inFile){
      cout<<"File can not be opened"<<endl;
      exit(1);
   }
   // Initialize all of the vectors and variable that will be needed
   // To be read in from file:
   vector<Int_t> RunNumbers, Masses;
   // To be determined from fit
   vector<Double_t> Means, Stdevs, Size;
   // Variables to store from file
   Int_t RunNumber, Cathode, Mass;
   // Individual values to get from fit
   float_t Mean, Stdev;
   // Used to check if a run should be skipped. In case there's no data and it segfaults
   Bool_t skip;
   // for iterating through each file
   char filename[100];
   // for making cuts on the correct histogram
   // char cutString[10];
   TString cutString;
   // sprintf(cutString,"%s>800",histnames[hist]);
   // Will contain a list of runs to skip. Will be dependent
   //    on which hist, e.g. some runs may be compactIC only
   vector<Int_t> SkippedRuns;
   // SkippedRuns.push_back();
   switch (hist){
      case 0:{
         SkippedRuns.insert(SkippedRuns.end(),{});
         cutString = "A1>800";
         break;}
      case 1:{
         SkippedRuns.insert(SkippedRuns.end(),{});
         cutString = "A2>800";
         break;}
      case 3:{
         SkippedRuns.insert(SkippedRuns.end(),{4148,4149,4150,4151,4152,4153,4154,4155,});
         cutString = "TOF>2100 && TOF<2300";
         break;}
      default:{
         cout<<"Invalid histogram: "<<histnames[hist]<<" is currently not supported" <<endl;
         exit(1);}
   }
   // Loops through each line of input file
   string line;
   getline(inFile, line);
   while (getline(inFile, line)){
      // Records values of line to relevant variables
      sscanf(line.c_str(), "%d %d %d", &RunNumber, &Cathode, &Mass);
      // Adds values to each appropriate vector
      RunNumbers.push_back(RunNumber);
      Masses.push_back(Mass);
      // cout<<"Run: "<<RunNumber<<" Mass: "<<Mass;
      // Assume we don't skip every file
      skip = kFALSE;
      // Loop through list of skipped runs
      for(const int& i : SkippedRuns){
         // Skip the run if its on the list
         if(i == RunNumber){
            cout<<"Skipping Run "<<RunNumber<<endl;
            skip = kTRUE;
            break;
         }
      }
      if (RunNumber < 3000){
         // cout<<"Transmission "<<RunNumber<<endl;
         skip = kTRUE;
      }
      // Two if statements:
      // 1) for skipping all bad runs and all at mass 236
      if (Mass != 236 && Mass !=0 && skip == kFALSE){
         cout<<"Analyzing Run: "<<RunNumber<<endl;
      // 2) for skipping all bad runs
      // if (Mass !=0 && skip == kFALSE){
         // Initialize root objects
         TFile *f;
         TTree *t;
         TF1 *fitFunc;
         // imbed the current run into the input file name
         sprintf(filename,"run%d.root",RunNumber);
         // open the file and tree
         f = new TFile(filename);
         t = (TTree*)f->Get("evtTree");
         // create the relevant histogram. It gets
         // stored as htemp. goff to avoid drawing
         t->Draw(histnames[hist],cutString,"goff");
         // cout<<"Fit for Run "<<RunNumber<<" Started"<<endl;
         // Fits to a gaus function. <Q>uiet mode prevents printing to terminal
         htemp->Fit("gaus","Q");
         // cout<<"Fit for Run "<<RunNumber<<" Succeeded!"<<endl;
         // cin.get();
         // Add the fit results to the vectors
         fitFunc = htemp->GetFunction("gaus");
         Means.push_back(fitFunc->GetParameter(1));
         Stdevs.push_back(fitFunc->GetParameter(2));
      } else {
         // If for any reason, we are skipping a run:
         // cout<<"Avoiding Run "<<RunNumber<<endl;
         Means.push_back(0);
         Stdevs.push_back(0);
      }
   }
   // Arrays to hold data for plots
   Double_t *xs = new Double_t[RunNumbers.size()];
   Double_t *ys = new Double_t[RunNumbers.size()];
   Double_t *ys2 = new Double_t[RunNumbers.size()];

   // Print results and fill arrays for plots
   cout<<"Results From Fitting "<<histnames[hist]<<": \n Run  Mean  StDev"<<endl;
   for (int i = 0; i<RunNumbers.size();i++){
      cout << RunNumbers[i]<< " "<<Means[i]<< " "<<Stdevs[i] << endl;
      xs[i] = RunNumbers[i];
      ys[i] = Means[i];
      ys2[i] = Stdevs[i];

   }
   // Setting up the canvas to draw the two graphs
   TCanvas *c1 = new TCanvas("c1","Cooler Plots",1600,700);
   // # of columns, # of rows
   c1->Divide(2,1);
   c1->cd(1);

   // Setting up the axes labels
   char graph1String[40];
   char graph2String[40];
   sprintf(graph1String,"%s Data;Run Number;%s Mean",histnames[hist],histnames[hist]);
   sprintf(graph2String,"%s Data;Run Number;%s StDev",histnames[hist],histnames[hist]);

   // Drawing the plots of mean and stdev vs run number
   TGraph *g1 = new TGraph(RunNumbers.size(),xs,ys);
   g1->SetTitle(graph1String);
   g1->Draw("A*");
   c1->cd(2);

   TGraph *g2 = new TGraph(RunNumbers.size(),xs,ys2);
   g2->SetTitle(graph2String);
   g2->Draw("A*");
}
