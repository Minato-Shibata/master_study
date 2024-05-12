//#include <fstream>
//#include "TROOT.h"
//#include "TH1D.h"

void hist()
{
  TCanvas *c1 = new TCanvas("c1","The Canvas",200,10,550,500);

  TH1D *h1 = new TH1D("h1","channel", 8196,0,8196);

  ifstream data("phadc.txt");
  double ch1,ch2;
  double w = 1.;
  while( data >> ch1 >> ch2 ){
    h1->Fill(ch1);
  }
  
  h1->GetXaxis()->SetTitle("PHADC Channel");
  h1->GetYaxis()->SetTitle("Counts/Ch");
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();
  //  h2->Draw("contz");
  //  h2->Draw("colz");
  // gStyle->SetPalette(1);
  h1->SetLineColor(4);
  h1->SetLineWidth(2);
  h1->Draw("PE");
  //gStyle->SetOptStat(10);
  gStyle->SetOptFit(0111);
  
  //TF1 *g1 = new TF1("g1","gaus",340,360);
  //g1->SetParameter(340,12);
  //TF1 *f1 = new TF1("f1","[0]*x+[1]",320,360);
  //f1->SetParameter(-1,10);
  //TF1 *f2 = new TF1("f2","g1+f1",320,360);
  //h1->Fit("g1","R");
  //h1->Draw("same");  
}
