#include <vector>
#include <iostream>
#include <fstream>

void fitPede(const char* fname)
{
  //TCanvas *c1 = new TCanvas("c1","The Canvas",200,10,550,500);
  TCanvas *c1 = new TCanvas("c1","The Canvas",720,970);

  TPad *pad0 = new TPad("pad0","Pad 0",0,0.6666,0.5,1);
  pad0->Draw();
  pad0->SetFillColor(10);

  TPad *pad1 = new TPad("pad1","Pad 1",0,0.3333,0.9,0.6666);
  pad1->Draw();
  pad1->SetFillColor(10);

  TPad *pad2 = new TPad("pad2","Pad 2",0,0,0.9,0.3333);
  pad2->Draw();
  pad2->SetFillColor(10);

  TPad *pad3 = new TPad("pad3","Pad 3",0.5,0.6666,1,1);
  pad3->Draw();
  pad3->SetFillColor(10);

  gStyle->SetOptStat(1111111);
  gStyle->SetOptFit(1111);

  double xx,yy,zz,ww,sum;

  TTree *t1 = new TTree("t1", "adc");
  t1->Branch("xx",        &xx,        "xx/D");
  t1->Branch("ww",        &ww,        "ww/D");
  t1->Branch("sum",       &sum,       "sum/D");

  TH2D *xw = new TH2D("xw","ch0:ch3",500,0,8192,500,0,8192);
  TH1D *ch0 = new TH1D("ch0","ADC ch0 shaper",8192,0,8192);
  TH1D *ch3 = new TH1D("ch3","ADC ch3 shaper",8192,0,8192);
  TH1D *hsum =  new TH1D("hsum","ADC ch0+ch3",16384,0,16384);

  ifstream data(fname, ios::in);
  if(!data.good()){
    cerr << "Can not open " << fname << "!\n";
    return NULL;
  }

  char  cut[128];
  TCut ext;
  //sprintf( cut, "1==1 && 0==0 ");
  sprintf( cut, "ww< 0.83*xx+600  && ww > 0.83*xx-500 ");
  ext = cut;

  double x;  
  TF1 *f1 = new TF1("f1", "0.83*x+600",0,10000);
  TF1 *f2 = new TF1("f1", "0.83*x-600",0,10000);

  while( data >> xx >> yy >>zz >>ww ){
    if (xx>0 && ww>0 ){ 
      sum = xx + ww;  //Tekito!
      t1->Fill();
    }
  }
  t1->Print();

  TF1 *g1 = new TF1("g1","gaus",90,101);
  g1->SetParameter(0,300);
  g1->SetParameter(1,100);
  g1->SetParameter(2,20);
  TF1 *g2 = new TF1("g2","gaus",90,101);
  g2->SetParameter(0,300);
  g2->SetParameter(1,100);
  g2->SetParameter(2,20);

  pad0->cd();
  pad0->SetLeftMargin(0.2);
  pad0->SetRightMargin(0.1);

  t1->Project("xw","ww:xx");
  
  xw->GetXaxis()->SetTitle("ADC ch0");
  xw->GetYaxis()->SetTitle("ADC ch3"); 
  xw->GetXaxis()->CenterTitle(); 
  xw->GetYaxis()->CenterTitle(); 
  
  xw->SetMaximum(100);
  xw->Draw("contz");
  xw->Draw("colz");
  f1->Draw("same");
  f2->Draw("same");
  gStyle->SetPalette(1);

  
  pad1->cd();
  pad1->SetLeftMargin(0.1);
  pad1->SetRightMargin(0.1);
  ch0->SetLineColor(4);
  ch0->SetLineWidth(2);

  t1->Project("ch0","xx", ext);
  
  ch0->GetXaxis()->SetTitle("ADC0");
  ch0->GetYaxis()->SetTitle("counts"); 
  ch0->GetXaxis()->CenterTitle(); 
  ch0->GetYaxis()->CenterTitle(); 
  
  ch0->SetMaximum(1500);
  ch0->Draw();
  ch0->Fit("g1","R");
  g1->SetLineColor(2);
  ch0->Draw("same");

  pad2->cd();
  pad2->SetLeftMargin(0.1);
  pad2->SetRightMargin(0.1);
  ch3->SetLineColor(4);
  ch3->SetLineWidth(2);

  t1->Project("ch3","ww", ext);
  
  ch3->GetXaxis()->SetTitle("ADC3");
  ch3->GetYaxis()->SetTitle("counts"); 
  ch3->GetXaxis()->CenterTitle(); 
  ch3->GetYaxis()->CenterTitle(); 
  
  ch3->SetMaximum(1500);
  ch3->Draw();
  ch3->Fit("g2","R");
  g2->SetLineColor(2);
  ch3->Draw("same");
  
  pad3->cd();
  pad3->SetLeftMargin(0.1);
  pad3->SetRightMargin(0.1);
  hsum->SetLineColor(4);
  hsum->SetLineWidth(2);

  t1->Project("hsum","sum", ext);
  
  hsum->GetXaxis()->SetTitle("ADC0+ADC3");
  hsum->GetYaxis()->SetTitle("counts"); 
  hsum->GetXaxis()->CenterTitle(); 
  hsum->GetYaxis()->CenterTitle(); 
  
  hsum->SetMaximum(500);
  hsum->Draw();

}

