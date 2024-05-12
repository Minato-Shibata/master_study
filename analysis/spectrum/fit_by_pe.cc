#include <vector>
#include <iostream>
#include <fstream>

void c2_fitCd_by_pe(/*const char* fname*/)
{
  double a, b;
  a = 105.3;
  b = 115.5;
  /*
  printf("Please enter the values of ch0's & ch3's pedestal\n");
  printf("ch0\n");
  scanf("%lf", &a);
  printf("ch3\n");
  scanf("%lf", &b);
  */
  
  //TCanvas *c1 = new TCanvas("c1","The Canvas",200,10,550,500);
  TCanvas *c1 = new TCanvas("c1","The Canvas",720,970);

  TPad *pad0 = new TPad("pad0","Pad 0",0,0.6666,0.5,1);
  pad0->Draw();
  pad0->SetFillColor(10);

  TPad *pad1 = new TPad("pad1","Pad 1",0,0.3333,1.0,0.6666);
  pad1->Draw();
  pad1->SetFillColor(10);

  TPad *pad2 = new TPad("pad2","Pad 2",0,0,1.0,0.3333);
  pad2->Draw();
  pad2->SetFillColor(10);

  TPad *pad3 = new TPad("pad3","Pad 3",0.5,0.6666,1,1);
  pad3->Draw();
  pad3->SetFillColor(10);
  

  double xx0,ww0,xx,yy,zz,ww,sum;

  TTree *t1 = new TTree("t1", "adc");
  t1->Branch("xx",        &xx,        "xx/D");
  t1->Branch("ww",        &ww,        "ww/D");
  t1->Branch("sum",       &sum,       "sum/D");

  TH2D *xw = new TH2D("xw","ch0:ch3",300,0,300,300,0,300);
  TH1D *ch0 = new TH1D("ch0","ADC ch0 shaper",8370,0,300);
  TH1D *ch3 = new TH1D("ch3","ADC ch3 shaper",7620,0,300);
  TH1D *hsum =  new TH1D("hsum","ADC ch0+ch3",8000,0,600);

  ifstream data("c2_Cd_20201112-0930.txt");
  /*
  ifstream data(fname, ios::in);
  if(!data.good()){
    cerr << "Can not open " << fname << "!\n";
    return NULL;
  }
  */
  
  char  cut[128];
  TCut ext;
  //sprintf( cut, "1==1 && 0==0 ");
  sprintf( cut, "ww< 1.6*xx+10  && ww > 1.2*xx-5");
  ext = cut;

  double x;  
  TF1 *f1 = new TF1("f1", "1.6*x+10",0,10000);
  TF1 *f2 = new TF1("f1", "1.2*x-5",0,10000);

  while( data >> xx0 >> yy >>zz >>ww0 ){
    if (xx0>0 && ww0>0 ){
      xx = (xx0 - a) / 27.9;
      ww = (ww0 - b) / 25.4;
      sum = xx + ww;
      t1->Fill();
    }
  }
  t1->Print();
 
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
  
  ch0->GetXaxis()->SetTitle("ADC0[p.e]");
  ch0->GetYaxis()->SetTitle("counts"); 
  ch0->GetXaxis()->CenterTitle(); 
  ch0->GetYaxis()->CenterTitle(); 
  
  ch0->SetMaximum(500);
  ch0->Draw();

  pad2->cd();
  pad2->SetLeftMargin(0.1);
  pad2->SetRightMargin(0.1);
  ch3->SetLineColor(4);
  ch3->SetLineWidth(2);

  t1->Project("ch3","ww", ext);

  ch3->GetXaxis()->SetTitle("ADC3[p.e.]");
  ch3->GetYaxis()->SetTitle("counts"); 
  ch3->GetXaxis()->CenterTitle(); 
  ch3->GetYaxis()->CenterTitle(); 
  
  ch3->SetMaximum(500);
  ch3->Draw();

  pad3->cd();
  pad3->SetLeftMargin(0.1);
  pad3->SetRightMargin(0.1);
  hsum->SetLineColor(4);
  hsum->SetLineWidth(2);

  t1->Project("hsum","sum", ext);

  TF1 *g1 = new TF1("g1","[0]+[1]*x+[2]*TMath::Gaus(x,[3],[4])+[5]*TMath::Gaus(x,[6],[7])",5,50);
  g1->SetParameter(0,0);
  g1->SetParameter(1,0);
  g1->SetParameter(2,145);
  g1->SetParameter(3,12);
  g1->SetParameter(4,5);
  g1->SetParameter(5,55);
  g1->SetParameter(6,40);
  g1->SetParameter(7,5);
  //g1->SetParLimits(6,1100,1300);

  TF1 *g2 = new TF1("g2","[0]+[1]*x+[2]*TMath::Gaus(x,[3],[4])",30,50);
  g2->SetParameter(0,0);
  g2->SetParameter(1,0);
  g2->SetParameter(2,20);
  g2->SetParameter(3,37);
  g2->SetParameter(4,7);

  hsum->GetXaxis()->SetTitle("ADC0+ADC3[p.e.]");
  hsum->GetYaxis()->SetTitle("counts"); 
  hsum->GetXaxis()->CenterTitle(); 
  hsum->GetYaxis()->CenterTitle(); 
  
  hsum->SetMaximum(200);
  hsum->Draw();

  g1->SetLineColor(2);
  hsum->Fit("g1","R");
  g2->SetLineColor(3);
  //hsum->Fit("g2","R+");
  hsum->Draw("same");  

}

