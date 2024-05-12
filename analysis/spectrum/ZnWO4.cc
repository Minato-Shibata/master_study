#include <vector>
#include <iostream>
#include<fstream>

void ZnWO4(const char* fname)
{
  //TCanvas *c1 = new TCanvas("c1","",0,0,800,600);
  TCanvas *c1 = new TCanvas("c1","The Canvas",200,10,550,500);
  
  TH1D *h1 = new TH1D("h1","Energy vs Channel",1000,0,3000);
  
  ifstream data(fname, ios::in);
  if(!data.good()){
    cerr << "Can not open " << fname << "!\n";
    return NULL;
  }
  
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  c1->SetGridx();
  c1->SetGridy();

  double Ch,E,eCh,eE;  
  vector<double>Ch_vec,E_vec,eCh_vec,eE_vec;
  
  while(data >> Ch >> E >> eCh){
    Ch_vec.push_back(Ch);
     E_vec.push_back(E);
     eCh_vec.push_back(eCh);
     eE_vec.push_back(0);
  }
  //fin.close();
  
  h1->SetMinimum(0);
  h1->SetMaximum(1500);
  //h1->Draw();
  //TGraph *graph = new TGraph(Ch_vec.size(),&(Ch_vec.at(0)),&(E_vec.at(0)));
  TGraphErrors *graph = new TGraphErrors(Ch_vec.size(),&(Ch_vec.at(0)),&(E_vec.at(0)),&(eCh_vec.at(0)),&(eE_vec.at(0)));
  graph->SetMarkerStyle(8);
  h1->SetXTitle("PHADC");
  h1->SetYTitle("Energy");
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();
  h1->Draw();
  graph->Draw("PL");
  
  TF1* f1 = new TF1("f1","[0]*x+[1]",0,700);
  f1->SetParameters(0.0670723, -3.86917);
  graph->Fit(f1);
  
}
