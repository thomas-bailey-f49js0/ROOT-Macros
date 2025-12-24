#include <TGClient.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TCutG.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>
#include <TGNumberEntry.h>
#include <TGSlider.h>
#include <TApplication.h>
#include <TGLayout.h>
#include <TGWindow.h>
#include <TString.h>
#include <TGLabel.h>
#include <TGListBox.h>
#include <TLatex.h>
#include <vector>
#include "TString.h"
// #include "cuts.h"



class MyMainFrame {
   //RQ_OBJECT("MyMainFrame")
private:


public:
  TGMainFrame         *fMain;
  TRootEmbeddedCanvas *fEcanvas;
  TGNumberEntry       *fNumber;
  TGNumberEntry       *fX;
  TGNumberEntry       *fY;
  TGNumberEntry       *fR1;
  TGNumberEntry       *fR2;
  TGNumberEntry       *fTheta;
  TGNumberEntry       *fSigma;
  // TGHSlider           *fSlider;
  TGCheckButton       *fCheckMulti;
  TGGroupFrame        *fGframe;
  TGLabel             *fLabel;
  TFile               *fFile;
  TGListBox           *fListBox;
  char                *hists[20];
  TList               *fSelected;
  TEllipse            *ellipse;
  TH1F                *fH;
  TTree               *t;
  // char                gCut[40];
  TCut                gCut;
  TCutG               eCut;
  TGHorizontalFrame   *cutButtons;
   MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
   virtual ~MyMainFrame();
   void DoOpen();
   void DoSetLabel();
   void DoOpenAll();
   void HandleButtons();
   void HandleCanvasClicked();
   void ellipseCut();
   void eDraw();
   void gaussFit();
   void openWithGaussCut();
   void addCutButton(int);
};
MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
  // Create a main frame
  fMain = new TGMainFrame(p,w,h);
  // Create canvas widget
  fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,1000,750);
  fEcanvas->GetCanvas()->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)"
                                    ,"MyMainFrame",this,"HandleCanvasClicked()");
  // Create a horizontal frame with many different objects
  // This is the main piece for the functionality of the GUI
  TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,400,400);
  // Within hframe there are many vertical frames, all
  // grouped so that they have similar buttons and other things
  // The first frame is openFrame, with a button for single histogram
  // drawing or all histogram drawing
  TGVerticalFrame *openFrame = new TGVerticalFrame(hframe,100,200);
  // button to open a single histogram
  TGTextButton *open = new TGTextButton(openFrame,"&Draw Single Hist");
  // This line connects the button with an action performed
  // by the user. In this case it's when the button is clicked.
  // At that point the DoOpen function is called
  open->Connect("Clicked()","MyMainFrame",this,"DoOpen()");
  // Each button can have a tooltip that appears if the cursor hovers
  open->SetToolTipText("Press to draw a single\n1D or 2D histogram");
  // This line adds the button the the vertical frame. four numbers
  // are the padding, left, right, top, bottom
  openFrame->AddFrame(open, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  // Another button. This time it opens all plots in a new canvas
  TGTextButton *openmulti = new TGTextButton(hframe,"&Draw All Hists");
  openmulti->Connect("Clicked()","MyMainFrame",this,"DoOpenAll()");
  openmulti->SetToolTipText("Press to draw all histograms");
  openFrame->AddFrame(openmulti, new TGLayoutHints(kLHintsCenterX,
                                           5,5,3,4));
  // Now that the vertical frame is finished, add it to the main
  // horizontal frame
  hframe->AddFrame(openFrame, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  // By using the group frame, We can have a nice box
  // and title around related items
  TGGroupFrame *histGroup = new TGGroupFrame(hframe, "Histogram Selection");
  // I do this so that the TList has global scope
  fSelected = new TList;
  // This is the widget to display all the possible leafs in the file
  fListBox = new TGListBox(histGroup, 15);
  // By having them in an array I can add them to something With
  // global scope. If there's a better way I'd love to know it
  // Index:                0    1    2    3     4       5       6        7        8        9       10
  char* histnames[20] = {"Si","A1","A2","A12","TOF","TOF:A1","TOF:A2","TOF:A12","A1:A2","A1:A12","TOF:Si"};
  // For loop populates the global array for hist names
  // as well as the ListBox widget
  for (int i = 0; i < 20; ++i) {
    hists[i] = histnames[i];
    fListBox->AddEntry(hists[i], i+1);
  }
  // Next we have the same process of adding to the main hframe
  fListBox->Resize(100,150);
  histGroup->AddFrame(fListBox, new TGLayoutHints(kLHintsTop | kLHintsLeft |
                                        kLHintsExpandX | kLHintsExpandY,
                                        0, 0, 0, 0));
  hframe->AddFrame(histGroup, new TGLayoutHints(kLHintsTop | kLHintsLeft |
                                        kLHintsExpandX | kLHintsExpandY,
                                        5, 5, 5, 5));

  // The next frame contains the run number and a checkbox for
  // selecting multiple hists to plot against each other
  TGVerticalFrame *vframe = new TGVerticalFrame(hframe,100,200);
  // This time we need to make the group frame inside of the
  // vframe since the "run number" text only applies to one part
  TGGroupFrame *numberGroup = new TGGroupFrame(vframe, "Run Number");
  // object to enter a run number. I set 3040 to the default
  // to make all the various testing easier.
  fNumber = new TGNumberEntry(numberGroup, 3685, 9,999, TGNumberFormat::kNESInteger,
                                                TGNumberFormat::kNEANonNegative,
                                                TGNumberFormat::kNELLimitMinMax,
                                                0, 99999);
  numberGroup->SetTitlePos(TGGroupFrame::kCenter);
  numberGroup->AddFrame(fNumber, new TGLayoutHints(kLHintsTop | kLHintsLeft |
                                        kLHintsExpandX | kLHintsExpandY,
                                        0, 0, 0, 0));
  vframe->AddFrame(numberGroup,new TGLayoutHints(kLHintsCenterX,
                                             5,5,3,4));

  // check button to select one or two hists to plot. if checked
  // then two items in the listbox can be selected. the first gets
  // plotted as y and the second as x. Future plans are to add the
  // 2D hists to the list box and use this button to be able to plot
  // various sums of anodes vs others.
  fCheckMulti = new TGCheckButton(vframe, "&Plot Log Scale", 10);
  fCheckMulti->Connect("Clicked()", "MyMainFrame", this, "HandleButtons()");
  vframe->AddFrame(fCheckMulti,new TGLayoutHints(kLHintsCenterX,
                                             5,5,3,4));
  hframe->AddFrame(vframe,new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));

  // The next frame has the buttons for cutting the hists. the plan
  // here is to have the cuts appear as buttons below the main hframe
  // so that they can be applied to other histograms as well.
  TGGroupFrame *cutGroup = new TGGroupFrame(hframe, "Cutting Buttons");
  TGVerticalFrame *cutFrame = new TGVerticalFrame(cutGroup,100,200);
  // The first button draws an ellipse on the canvas.
  TGTextButton *eDraw = new TGTextButton(cutFrame,"&Draw Ellipse");
  eDraw->Connect("Clicked()","MyMainFrame",this,"eDraw()");
  eDraw->SetToolTipText("Press to draw an ellipse for a 2D cut");
  cutFrame->AddFrame(eDraw, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  // The second button makes the elliptical cut
  TGTextButton *eCut = new TGTextButton(cutFrame,"&Cut Ellipse");
  eCut->Connect("Clicked()","MyMainFrame",this,"ellipseCut()");
  eCut->SetToolTipText("Press to cut the ellipse");
  cutFrame->AddFrame(eCut, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  // The third button makes a Gaussian cut
  TGTextButton *gCut = new TGTextButton(cutFrame,"&Fit Gaussian");
  gCut->Connect("Clicked()","MyMainFrame",this,"gaussFit()");
  gCut->SetToolTipText("Press to Fit a Gaussian");
  cutFrame->AddFrame(gCut, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  cutGroup->AddFrame(cutFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft |
                                        kLHintsExpandX | kLHintsExpandY,
                                        0, 0, 0, 0));
  hframe->AddFrame(cutGroup, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  // No longer aptly named, the ellipse group contains cutting
  // parameters. The parameters of an ellipse are displayed
  // but currently only the "theta" parameter is responsive.
  // the rest can only be changed by clicking and dragging the
  // ellipse in the canvas.
  TGGroupFrame *ellipseGroup = new TGGroupFrame(hframe, "Cutting Parameters");
  TGVerticalFrame *ellipseFrame = new TGVerticalFrame(ellipseGroup,150,400);
  ellipse = new TEllipse(1700.00,2178.167,205.45,7.056,0,360,0);
  // For these, I wanted to have a label associated with Each
  // parameter. So they all have to be in their own TGHorizontalFrame.
  // I don't care about the fLabel object though so I just reuse it.
  // X position of center
  TGHorizontalFrame *xFrame = new TGHorizontalFrame(ellipseFrame,150,20);
  fLabel = new TGLabel(xFrame, "X pos:");
  fX = new TGNumberEntry(xFrame, ellipse->GetX1(),8);
  xFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsCenterX,
                                            5,5,2,0));
  xFrame->AddFrame(fX, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  ellipseFrame->AddFrame(xFrame, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  // Y position of Center
  TGHorizontalFrame *yFrame = new TGHorizontalFrame(ellipseFrame,150,20);
  fLabel = new TGLabel(yFrame, "Y pos:");
  fY = new TGNumberEntry(yFrame, ellipse->GetY1(),8);
  yFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsCenterX,
                                            5,5,2,0));
  yFrame->AddFrame(fY, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  ellipseFrame->AddFrame(yFrame, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  // major axis when drawn (x axis)
  TGHorizontalFrame *r1Frame = new TGHorizontalFrame(ellipseFrame,150,20);
  fLabel = new TGLabel(r1Frame, "R1: ");
  fR1 = new TGNumberEntry(r1Frame, ellipse->GetR1(),8);
  r1Frame->AddFrame(fLabel, new TGLayoutHints(kLHintsCenterX,
                                            5,5,2,0));
  r1Frame->AddFrame(fR1, new TGLayoutHints(kLHintsCenterX,
                                            18,5,0,0));
  ellipseFrame->AddFrame(r1Frame, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  // minor axis when drawn (y axis)
  TGHorizontalFrame *r2Frame = new TGHorizontalFrame(ellipseFrame,150,20);
  fLabel = new TGLabel(r2Frame, "R2: ");
  fR2 = new TGNumberEntry(r2Frame, ellipse->GetR2(),8);
  r2Frame->AddFrame(fLabel, new TGLayoutHints(kLHintsCenterX,
                                            5,5,2,0));
  r2Frame->AddFrame(fR2, new TGLayoutHints(kLHintsCenterX,
                                            18,5,0,0));
  ellipseFrame->AddFrame(r2Frame, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  // Angle measured in degrees CCW from +x. I've found -35 to be good
  TGHorizontalFrame *thetaFrame = new TGHorizontalFrame(ellipseFrame,150,20);
  fLabel = new TGLabel(thetaFrame, "theta: ");
  fTheta = new TGNumberEntry(thetaFrame, ellipse->GetTheta(),8);
  // the "ValueChanged" part here updates the ellipse as you type
  // fTheta->GetNumberEntry()->Connect("ValueSet(=1)", "MyMainFrame", this, "eDraw()");
  fX->GetNumberEntry()->Connect("ReturnPressed()", "MyMainFrame", this,"eDraw()");
  fY->GetNumberEntry()->Connect("ReturnPressed()", "MyMainFrame", this,"eDraw()");
  fR1->GetNumberEntry()->Connect("ReturnPressed()", "MyMainFrame", this,"eDraw()");
  fR2->GetNumberEntry()->Connect("ReturnPressed()", "MyMainFrame", this,"eDraw()");
  fTheta->GetNumberEntry()->Connect("ReturnPressed()", "MyMainFrame", this,"eDraw()");

  thetaFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsCenterX,
                                            5,5,2,0));
  thetaFrame->AddFrame(fTheta, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  ellipseFrame->AddFrame(thetaFrame, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  // This refers to how many standard deviations to cut
  // with when doing a Gaussian cut, best to keep below 3
  TGHorizontalFrame *sigmaFrame = new TGHorizontalFrame(ellipseFrame,150,20);
  fLabel = new TGLabel(sigmaFrame, "sigma: ");
  fSigma = new TGNumberEntry(sigmaFrame, 2.0, 8);
  sigmaFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsCenterX,
                                            5,5,2,0));
  sigmaFrame->AddFrame(fSigma, new TGLayoutHints(kLHintsCenterX,
                                            2,5,0,0));
  ellipseFrame->AddFrame(sigmaFrame, new TGLayoutHints(kLHintsCenterX,
                                            5,5,0,0));
  ellipseGroup->AddFrame(ellipseFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft |
                                        kLHintsExpandX | kLHintsExpandY,
                                        0, 0, 0, 0));
  hframe->AddFrame(ellipseGroup, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  // Finally a button to exit ROOT. equivalent to
  // using .q in command line
  TGTextButton *exit = new TGTextButton(hframe,"&Exit",
                              "gApplication->Terminate(0)");
  exit->SetToolTipText("Press to exit ROOT");
  hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,
                                             2,2,2,2));
  cutButtons = new TGHorizontalFrame(fMain,20,400);
  TGTextButton *GaussCut = new TGTextButton(cutButtons,"Draw with Gauss Cut");
  GaussCut->Connect("Clicked()","MyMainFrame",this,"openWithGaussCut()");
  GaussCut->SetToolTipText("Press to draw all histograms");
  cutButtons->AddFrame(GaussCut, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  TGTextButton *ellipseCutDraw = new TGTextButton(cutButtons,"Draw with Ellipse Cut");
  ellipseCutDraw->Connect("Clicked()","MyMainFrame",this,"openWithCut(=2)");
  ellipseCutDraw->SetToolTipText("Press to draw all histograms");
  cutButtons->AddFrame(ellipseCutDraw, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
  fMain->AddFrame(cutButtons, new TGLayoutHints(kLHintsCenterX,
                                             2,2,2,2));
  fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX |
                   kLHintsExpandY, 1,10,10,10));
  // Name is purely a joke for Adam Clark who uses "cool plots"
  fMain->SetWindowName("Cooler Plots");
  // Leaving the generic comments here
  // Map all subwindows of main frame
  fMain->MapSubwindows();
  // Initialize the layout algorithm
  fMain->Resize(fMain->GetDefaultSize());
  // Map main frame
  fMain->MapWindow();
}
// Method for opening a single hist on the canvas on the GUI
void MyMainFrame::DoOpen() {
  // char arrays copied over from other scripts. Root doesn't
  // like strings, so keeping everything in this form works
  char filename1[100];
  char drawString[10];
  char hname[10];
  char p[] = "P";
  char a23[] = "3";
  // Gets the number in the box and tries to open that run
  sprintf(filename1,"run%ld.root",fNumber->GetNumberEntry()->GetIntNumber());
  TFile *f = new TFile(filename1);
  // initializes the tree. I think I need the scope global
  // to help with cutting. I couldn't draw histograms with
  // cuts for some reason and this worked for me.
  t = (TTree*)f->Get("evtTree");
  // clears out the fSelected list so that it can be used again
  fSelected->Clear();
  // Current setup: if 2 or more (error not caught btw) are selected,
  // Then it draws a 2D hist. If one, or if the button is unchecked,
  // Then a 1D hist is drawn
  if (fListBox->GetMultipleSelections()) {
    // populates fSelected the TGLBEntries
    fListBox->GetSelectedEntries(fSelected);
    // Grabs the first and second histogram names (Hence why only two
    // are actually used). These have an ID associated with them,
    // which is used in congruence with the hists array. I could only
    // figure out how to get the ID number, not the text associated
    // with it unfortunately :(
    TGLBEntry *ano1 = (TGLBEntry*)fSelected->At(0);
    TGLBEntry *ano2 = (TGLBEntry*)fSelected->At(1);
    sprintf(drawString, "%s:%s>>h%svs%s(1024,0,4095,1024,0,4095)",hists[ano1->EntryId()-1],hists[ano2->EntryId()-1],hists[ano1->EntryId()-1],hists[ano2->EntryId()-1]);
    t->Draw(drawString,"","colz");
    // Here I save the drawing to a 2D histogram. I could make it work
    // with the ellipse cuts, just not the gaussian ones.
    sprintf(hname, "h%svs%s",hists[ano1->EntryId()-1],hists[ano2->EntryId()-1]);
    fH = (TH1F*)gDirectory->Get(hname);
    // Makes the active canvas the one on the GUI
    TCanvas *fCanvas = fEcanvas->GetCanvas();
    fCanvas->cd();
    // Always have to update the canvas
    fCanvas->Update();
  } else {
    fListBox->GetSelectedEntries(fSelected);
    TGLBEntry *ano1 = (TGLBEntry*)fSelected->At(0);
    Int_t i = ano1->EntryId()-1;
    switch(i){
       case 5:
          sprintf(drawString,"%s>>h%s(300,1400,2000,100,2130,2230)",hists[i],hists[i]);
          t->Draw(drawString,"","colz");
          break;
       case 6:
       case 8:
       case 10:
          sprintf(drawString,"%s>>h%s(1024,0,4095,1024,0,4095)",hists[i],hists[i]);
          t->Draw(drawString,"","colz");
          break;
       case 7:
          sprintf(drawString,"%s>>h%s(1024,0,4095,1024,0,8191)",hists[i],hists[i]);
          t->Draw(drawString,"","colz");
          break;
       case 9:
          sprintf(drawString,"%s>>h%s(1024,0,8191,1024,0,4095)",hists[i],hists[i]);
          t->Draw(drawString,"","colz");
          break;
       case 3:
          sprintf(drawString,"%s>>h%s(512,0,8191)",hists[i],hists[i]);
          t->Draw(drawString);
          break;
       default:
          sprintf(drawString,"%s>>h%s(4096,0,4095)",hists[i],hists[i]);
          t->Draw(drawString,"A1>50");
          break;
       }



      // Here I save the drawing to a 2D histogram. I could make it work
      // with the ellipse cuts, just not the gaussian ones.
      // sprintf(hname, "h%s",hists[ano1->EntryId()-1]);

    fH = (TH1F*)gDirectory->Get(hname);
    cout<<"Established fH"<<endl;
    TCanvas *fCanvas = fEcanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
  }
}
// Opens all hists on a new canvas
void MyMainFrame::DoOpenAll(){
  TCanvas *c1 = new TCanvas("c1", "All Plots",1500, 1200);
  gStyle->SetTitleFontSize(.09);
  c1->Divide(3,5);
  c1->cd(1);
  char filename1[100];
  char drawString[40];
  sprintf(filename1,"run%ld.root",fNumber->GetNumberEntry()->GetIntNumber());
  TFile *f = new TFile(filename1);
  t = (TTree*)f->Get("evtTree");
  fSelected->Clear();
  for (int i = 0; i < 8; ++i) {
    if (i < 5) {
      sprintf(drawString,"%s>>h%s(1024,0,4096)",hists[i],hists[i]);
      c1->cd(i+1);
      t->Draw(drawString);
    }
    else {
       sprintf(drawString,"%s>>h%s(1024,0,4095,1024,0,4095)",hists[i],hists[i]);
       c1->cd(i+1);
       t->Draw(drawString,"","colz");
    }
  }
}
// Method to check if the check button is checked. Checkception!
void MyMainFrame::HandleButtons(){
   // Handle check button.
  Int_t id;
  TGButton *btn = (TGButton *) gTQSender;
  id = btn->WidgetId();
  gPad->SetLogy(0);
  if (id == 10){gPad->SetLogy(1);}


}
void MyMainFrame::HandleCanvasClicked(){
  // cout<<"Test"<<endl;
  fX->SetNumber(ellipse->GetX1());
  fY->SetNumber(ellipse->GetY1());
  fR1->SetNumber(ellipse->GetR1());
  fR2->SetNumber(ellipse->GetR2());

}
// Method for drawing the ellipse
void MyMainFrame::eDraw() {
    // ellipse is drawn based on numbers in the boxes on GUI
    ellipse->SetTheta(fTheta->GetNumberEntry()->GetNumber());
    ellipse->SetR1(fR1->GetNumberEntry()->GetNumber());
    ellipse->SetR2(fR2->GetNumberEntry()->GetNumber());
    ellipse->SetFillColorAlpha(0, 0.);
    ellipse->SetLineWidth(3);
    ellipse->Draw();
    gPad->Update();
}
// Method for making the elliptical cut. The general
// idea is to get the params of the ellipse and Then
// make a 100 sided polygon using TGCut that roughly
// follows the outline of the ellipse
void MyMainFrame::ellipseCut(){
  Double_t x1,y1,r1,r2,phi1,phi2,theta,pi;

  x1 = ellipse->GetX1();
  y1 = ellipse->GetY1();
  r1 = ellipse->GetR1();
  r2 = ellipse->GetR2();
  phi1 = ellipse->GetPhimin();
  phi2 = ellipse->GetPhimax();
  theta = ellipse->GetTheta();
  pi = 3.1416;
  Int_t n = 100;
  Double_t angle,dx,dy;
  Double_t dphi = (phi2-phi1)*pi/(180*n);
  Double_t ct   = TMath::Cos(pi*theta/180);
  Double_t st   = TMath::Sin(pi*theta/180);
  TString cutName = "ecut";
  TCutG *ecut = new TCutG(cutName,n);
  fListBox->GetSelectedEntries(fSelected);
  TGLBEntry *ano1 = (TGLBEntry*)fSelected->At(0);
  cout<<hists[ano1->EntryId()-1]<<endl;
  ecut->SetVarX("A1");
  ecut->SetVarY("TOF");
  for (Int_t i=0;i<=n;i++) {
    Int_t point = i;
    angle = phi1*pi/180 + Double_t(i)*dphi;
    dx    = r1*TMath::Cos(angle);
    dy    = r2*TMath::Sin(angle);
    ecut->SetPoint(point, x1 + dx*ct - dy*st, y1 + dx*st + dy*ct);
  }
  TEllipse *Run1dE12dE3OffEllipse = new TEllipse(1301.5,4046.18,384.751,153.506,0,360,40);
  x1 = Run1dE12dE3OffEllipse->GetX1();
  y1 = Run1dE12dE3OffEllipse->GetY1();
  r1 = Run1dE12dE3OffEllipse->GetR1();
  r2 = Run1dE12dE3OffEllipse->GetR2();
  phi1 = Run1dE12dE3OffEllipse->GetPhimin();
  phi2 = Run1dE12dE3OffEllipse->GetPhimax();
  theta = Run1dE12dE3OffEllipse->GetTheta();
  dphi = (phi2-phi1)*pi/(180*n);
  ct   = TMath::Cos(pi*theta/180);
  st   = TMath::Sin(pi*theta/180);
  cutName = "Run1Off2";
  TCutG *Run1Off2 = new TCutG(cutName,n);
  Run1Off2->SetVarX("A1");
  Run1Off2->SetVarY("TOF");

  for (Int_t i=0;i<=n;i++) {
    Int_t point = i;
    angle = phi1*pi/180 + Double_t(i)*dphi;
    dx    = r1*TMath::Cos(angle);
    dy    = r2*TMath::Sin(angle);
    Run1Off2->SetPoint(point, x1 + dx*ct - dy*st, y1 + dx*st + dy*ct);
  }
  TCanvas *fCanvas = fEcanvas->GetCanvas();
  fCanvas->cd();
  t->Draw("TOF:A2>>hTOF:A2(1024,0,4095,1024,0,4095)","ecut","colz");
  gPad->Update();
}
// {
//   fListBox->GetSelectedEntries(fSelected);
//   TGLBEntry *ano1 = (TGLBEntry*)fSelected->At(0);
//   char an1[3];
//   char an2[3];
//   sprintf(an1,"A6");
//   sprintf(an2,"A7");
//   ecut->SetVarX(an1);
//   ecut->SetVarY(an2);
//   cout<<"Test2"<<endl;
//   for (Int_t i=0;i<=n;i++) {
//     Int_t point = i;
//     angle = phi1*pi/180 + Double_t(i)*dphi;
//     dx    = r1*TMath::Cos(angle);
//     dy    = r2*TMath::Sin(angle);
//     ecut->SetPoint(point, x1 + dx*ct - dy*st, y1 + dx*st + dy*ct);
//   }
//   cout<<"Test3"<<endl;
//   TCanvas *fCanvas = fEcanvas->GetCanvas();
//   fCanvas->cd();
//   // fListBox->GetSelectedEntries(fSelected);
//   // sprintf(drawString, "%s>>h%s(1024,0,4095,1024,0,4095)",hists[ano1->EntryId()-1],hists[ano1->EntryId()-1]);
//   // t->Draw(drawString,ecut,"colz")
//   fH->Draw("colz [ecut]");
//   gPad->Update();
// }
// Function for making the gaussian cut.
void MyMainFrame::gaussFit(){
  TH1F *htemp, *hA1, *hA2, *hTOF;
  cout<<"Called gaussFit()"<<endl;
  char filename1[100];
  // //char anode[3];
  sprintf(filename1,"run%ld.root",fNumber->GetNumberEntry()->GetIntNumber());
  TFile *f = new TFile(filename1);
  t = (TTree*)f->Get("evtTree");
  //
  // gPad->Clear();
  char anode[10];
  char drawString[100];
  // // fH->SetBinError(0,0);
  // fH->Draw();
  cout<<"test1 gaussFit()"<<endl;
  fListBox->GetSelectedEntries(fSelected);
  TGLBEntry *ano1 = (TGLBEntry*)fSelected->At(0);
  Int_t i = ano1->EntryId()-1;
  switch(i){
     case 1:
         cout<<"Fit hA1"<<endl;
         hA1->Fit("gaus");
         break;
     case 2:
         hA2->Fit("gaus");
         break;
     case 4:
         hTOF->Fit("gaus");
         break;
      default:
         cout<<"Are you sure you wanted to fit that one?"<<endl;
         break;

 }
  // switch(i){
  //    case 5:
  //    case 6:
  //    case 8:
  //    case 10:
  //       sprintf(drawString,"%s>>fH(1024,0,4095,1024,0,4095)",hists[i],hists[i]);
  //       t->Draw(drawString,"","colz");
  //       break;
  //    case 7:
  //       sprintf(drawString,"%s>>fH(1024,0,4095,1024,0,8191)",hists[i],hists[i]);
  //       t->Draw(drawString,"","colz");
  //       break;
  //    case 9:
  //       sprintf(drawString,"%s>>fH(1024,0,8191,1024,0,4095)",hists[i],hists[i]);
  //       t->Draw(drawString,"","colz");
  //       break;
  //    case 3:
  //       sprintf(drawString,"%s>>fH(512,0,8191)",hists[i],hists[i]);
  //       t->Draw(drawString);
  //       break;
  //    default:
  //       sprintf(drawString,"%s>>fH(4096,0,4095)",hists[i],hists[i]);
  //       t->Draw(drawString,"TOF>10");
  //       break;
  //    }
  // htemp->Fit("gaus");
  // cout<<"Fit the function"<<endl;
  // TF1 *fit1 = fH->GetFunction("gaus");
  // Double_t mean = fit1->GetParameter(1);
  // Double_t sigma = fit1->GetParameter(2);
  // TCut cut1,cut2;
  // Double_t up = mean+fSigma->GetNumberEntry()->GetNumber()*sigma;
  // Double_t low = mean-fSigma->GetNumberEntry()->GetNumber()*sigma;
  // char lowCut[40];
  // char upCut[40];
  // fListBox->GetSelectedEntries(fSelected);
  // TGLBEntry *ano1 = (TGLBEntry*)fSelected->At(0);
  // // sprintf(lowCut, "%s>%f",hists[ano1->EntryId()-1],low);
  // // sprintf(upCut, "%s<%f",hists[ano1->EntryId()-1],up);
  // // // sprintf(gCut, "%s<%f&&%s>%f",hists[ano1->EntryId()-1],up,hists[ano1->EntryId()-1],low);
  // sprintf(drawString, "%s>>h%s(1024,0,4095)",hists[ano1->EntryId()-1],
  //                                            hists[ano1->EntryId()-1]);
  // // cut1 = (TCut)lowCut;
  // // cut2 = (TCut)upCut;
  // // gCut = cut1 && cut2;
  //
  // gPad->Clear();
  //
  // // t->Draw(drawString,gCut);
  // t->Draw(drawString);
  //
  // gPad->Update();

}
// Function to add buttons for cuts made.
void MyMainFrame::addCutButton(int type){
  cout <<"test"<<endl;
  // if type is 1, then it was a Gaussian Cut
  // if type is 2, then it was an ellipse cut
  if (type == 1){
    cout <<"test2"<<endl;
    TGTextButton *GaussCut = new TGTextButton(cutButtons,"Gauss Cut");
    GaussCut->Connect("Clicked()","MyMainFrame",this,"openWithGaussCut()");
    GaussCut->SetToolTipText("Press to draw all histograms");
    cutButtons->AddFrame(GaussCut, new TGLayoutHints(kLHintsCenterX,
                                              5,5,3,4));
  }
  fMain->MapSubwindows();
  // Initialize the layout algorithm
  fMain->Resize(fMain->GetDefaultSize());
  // Map main frame
  fMain->MapWindow();

}
void MyMainFrame::openWithGaussCut() {
  // cout <<"test3"<<endl;
  TList *l = gPad->GetListOfPrimitives();
  l->RemoveFirst();
  TH1F *h = (TH1F*)l->First()->DrawClone();
  // Double_t bins,up,low;
  // bins = h->GetNBinsX();
  // up = h->GetXaxis()->GetXmax();
  // low = h->GetXaxis()->GetXmin();
  // TH1F *h = (TH1F*)gPad->GetPrimitive("htemp");
  char drawString[10];
  h->GetTitle();
  sprintf(drawString,"%s>>hdummy(4096,0,4095)",h->GetTitle());
  cout <<drawString<<endl;
  // t->Draw(drawString),gCut,"colz");
  cout<<"Drew"<<endl;
  gPad->Update();
  cout<<"Drew2"<<endl;

}
MyMainFrame::~MyMainFrame() {
   // Clean up used widgets: frames, buttons, layout hints
  fMain->Cleanup();
  delete fMain;
}
void gui() {
  // Popup the GUI...
  new MyMainFrame(gClient->GetRoot(),200,200);

}
int main(){
  gui();
  return 0;
}
