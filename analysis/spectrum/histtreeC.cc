#include <vector>
#include <iostream>
#include <fstream>

void histtreeC(const char* fname1,const char* fname2)
{
  //TCanvas *c1 = new TCanvas("c1","The Canvas",200,10,550,500);
  TCanvas *c1 = new TCanvas("c1","The Canvas",720,970);

  TPad *pad0 = new TPad("pad0","Pad 0",0,0.6666,0.5,1);
  pad0->Draw();
  pad0->SetFillColor(10);

  TPad *pad1 = new TPad("pad1","Pad 1",0,0.3333,1,0.6666);
  pad1->Draw();
  pad1->SetFillColor(10);

  TPad *pad2 = new TPad("pad2","Pad 2",0,0,1,0.3333);
  pad2->Draw();
  pad2->SetFillColor(10);

  TPad *pad3 = new TPad("pad3","Pad 3",0.5,0.6666,1,1);
  pad3->Draw();
  pad3->SetFillColor(10);
  

  double xx1,yy1,zz1,ww1,sum1;
  double xx2,yy2,zz2,ww2,sum2;

  TTree *t1 = new TTree("t1", "adc1");
  t1->Branch("xx1",        &xx1,        "xx1/D");
  t1->Branch("ww1",        &ww1,        "ww1/D");
  t1->Branch("sum1",       &sum1,       "sum1/D");

  TTree *t2 = new TTree("t2", "adc2");
  t2->Branch("xx2",        &xx2,        "xx2/D");
  t2->Branch("ww2",        &ww2,        "ww2/D");
  t2->Branch("sum2",       &sum2,       "sum2/D");


  TH2D *xw_1 = new TH2D("xw_1","ch0:ch3",500,0,8192,500,0,8192);
  TH1D *ch0_1 = new TH1D("ch0_1","ADC ch0 shaper",8192,0,8192);
  TH1D *ch3_1 = new TH1D("ch3_1","ADC ch3 shaper",8192,0,8192);
  TH1D *hsum_1 =  new TH1D("hsum_1","ADC ch0+ch3",4096,0,16384);
 
  TH2D *xw_2 = new TH2D("xw_2","ch0:ch3",500,0,8192,500,0,8192);
  TH1D *ch0_2 = new TH1D("ch0_2","ADC ch0 shaper",8192,0,8192);
  TH1D *ch3_2 = new TH1D("ch3_2","ADC ch3 shaper",8192,0,8192);
  TH1D *hsum_2 =  new TH1D("hsum_2","ADC ch0+ch3",4096,0,16384);

  ifstream data1(fname1, ios::in);
  if(!data1.good()){
    cerr << "Can not open " << fname1 << "!\n";
    return NULL;
  }

  ifstream data2(fname2, ios::in);
  if(!data2.good()){
    cerr << "Can not open " << fname2 << "!\n";
    return NULL;
  }

  
  char  cut[128],cut1[128],cut2[128];
  TCut ext,ext1,ext2;
  //sprintf( cut, "1==1 && 0==0 ");
  ext = cut;

  sprintf( cut1, "ww1< 1*xx1+600  && ww1 > 1*xx1-500 ");
  ext1 = cut1;
  sprintf( cut2, "ww2< 1*xx2+600  && ww2 > 1*xx2-500 ");
  ext2 = cut2;



  double x;  
  TF1 *f1 = new TF1("f1", "1*x+600",0,10000);
  TF1 *f2 = new TF1("f1", "1*x-1500",0,10000);

  while( data1 >> xx1 >> yy1 >>zz1 >>ww1 ){
    if (xx1>0 && ww1>0 ){ 
      sum1 = xx1 + ww1;  //Tekito!
      t1->Fill();
    }
  }
  t1->Print();
  int entrynumber1=t1->GetEntries();
  double N1 = double(entrynumber1);

  while( data2 >> xx2 >> yy2 >>zz2 >>ww2 ){
    if (xx2>0 && ww2>0 ){ 
      sum2 = xx2 + ww2;  //Tekito!
      t2->Fill();
    }
  }
  t2->Print();
  int entrynumber2=t2->GetEntries();
  double N2 = double(entrynumber2);

  pad0->cd();
  pad0->SetLeftMargin(0.2);
  pad0->SetRightMargin(0.1);

  t2->Project("xw_2","ww2:xx2");
  
  xw_2->GetXaxis()->SetTitle("ADC ch0");
  xw_2->GetYaxis()->SetTitle("ADC ch3"); 
  xw_2->GetXaxis()->CenterTitle(); 
  xw_2->GetYaxis()->CenterTitle(); 
  
  xw_2->SetMaximum(100);
  xw_2->Draw("contz");
  xw_2->Draw("colz");
  f1->Draw("same");
  f2->Draw("same");
  gStyle->SetPalette(1);

  
  pad1->cd();
  pad1->SetLeftMargin(0.1);
  pad1->SetRightMargin(0.1);
  ch0_1->SetLineColor(4);
  ch0_1->SetLineWidth(2);
  t1->Project("ch0_1","xx1", ext1);
  ch0_1->Scale(1./N1);

  ch0_2->SetLineColor(2);
  ch0_2->SetLineWidth(2);
  t2->Project("ch0_2","xx2", ext2);
  ch0_2->Scale(1./N2);

  ch0_1->GetXaxis()->SetTitle("ADC0");
  ch0_1->GetYaxis()->SetTitle("counts"); 
  ch0_1->GetXaxis()->CenterTitle(); 
  ch0_1->GetYaxis()->CenterTitle(); 

  ch0_1->GetXaxis()->SetRange(500,8092);
  ch0_2->GetXaxis()->SetRange(500,8092);
  //ch0->SetMaximum(4000);
  ch0_1->Draw();
  ch0_2->Draw("same");

  pad2->cd();
  pad2->SetLeftMargin(0.1);
  pad2->SetRightMargin(0.1);
  ch3_1->SetLineColor(4);
  ch3_1->SetLineWidth(2);

  t1->Project("ch3_1","ww1", ext1);
  ch3_1->Scale(1./N1);
 
  ch3_2->SetLineColor(2);
  ch3_2->SetLineWidth(2);
  t2->Project("ch3_2","ww2", ext2);
  ch3_2->Scale(1./N2);

  ch3_1->GetXaxis()->SetTitle("ADC3");
  ch3_1->GetYaxis()->SetTitle("counts"); 
  ch3_1->GetXaxis()->CenterTitle(); 
  ch3_1->GetYaxis()->CenterTitle(); 
  
  ch3_1->GetXaxis()->SetRange(500,8092);
  ch3_2->GetXaxis()->SetRange(500,8092);
  //ch3_1->SetMaximum(4000);
  ch3_1->Draw();
  ch3_2->Draw("same");


  pad3->cd();
  pad3->SetLeftMargin(0.1);
  pad3->SetRightMargin(0.1);
  hsum_1->SetLineColor(4);
  hsum_1->SetLineWidth(2);

  t1->Project("hsum_1","sum1", ext1);
  hsum_1->Scale(1./N1);  

  hsum_2->SetLineColor(2);
  hsum_2->SetLineWidth(2);

  t2->Project("hsum_2","sum2", ext2);
  hsum_2->Scale(1./N2);  
  
  
  hsum_1->GetXaxis()->SetTitle("ADC0+ADC3");
  hsum_1->GetYaxis()->SetTitle("counts"); 
  hsum_1->GetXaxis()->CenterTitle(); 
  hsum_1->GetYaxis()->CenterTitle(); 
  
  hsum_1->GetXaxis()->SetRange(100,4096);
  hsum_2->GetXaxis()->SetRange(100,4096);
  //hsum_1->SetMaximum(4000);
  hsum_1->Draw();
  hsum_2->Draw("same");

}

