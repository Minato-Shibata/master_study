#include <vector>
#include <iostream>
#include <fstream>

//Run example
//root [0] .x histtree2.cc("Cd20201007.txt")

void histtree4(const char* fname)
{
  //TCanvas *c1 = new TCanvas("c1","The Canvas",200,10,550,500);
  //TCanvas *c1 = new TCanvas("c1","The Canvas",720,970);
  TCanvas *c1 = new TCanvas("c1","The Canvas",970,720);

  TPad *pad0 = new TPad("pad0","Pad 0",0,0.74,0.25,1);
  pad0->Draw();
  pad0->SetFillColor(10);

  TPad *pad1 = new TPad("pad1","Pad 1",0.25,0.74,0.5,1);
  pad1->Draw();
  pad1->SetFillColor(10);

  TPad *pad2 = new TPad("pad2","Pad 2",0.5,0.74,0.75,1);
  pad2->Draw();
  pad2->SetFillColor(10);

  TPad *pad3 = new TPad("pad3","Pad 3",0.75,0.74,1,1);
  pad3->Draw();
  pad3->SetFillColor(10);

  TPad *pad4 = new TPad("pad4","Pad 4",0,0.37,0.5,0.74);
  pad4->Draw();
  pad4->SetFillColor(10);

  TPad *pad5 = new TPad("pad5","Pad 5",0.5,0.37,1,0.74);
  pad5->Draw();
  pad5->SetFillColor(10);

  TPad *pad6 = new TPad("pad6","Pad 6",0,0,0.5,0.37);
  pad6->Draw();
  pad6->SetFillColor(10);

  TPad *pad7 = new TPad("pad7","Pad 7",0.5,0,1,0.37);
  pad7->Draw();
  pad7->SetFillColor(10);
  
  double xx,yy,zz,ww,suml,sumh;

  TTree *t1 = new TTree("t1", "adc");
  t1->Branch("xx",        &xx,        "xx/D");
  t1->Branch("yy",        &yy,        "yy/D");
  t1->Branch("zz",        &zz,        "zz/D");
  t1->Branch("ww",        &ww,        "ww/D");

  t1->Branch("suml",       &suml,       "suml/D");
  t1->Branch("sumh",       &sumh,       "sumh/D");

  TH2D *xw = new TH2D("xw","ch0:ch3",500,0,8192,500,0,8192);
  TH2D *yz = new TH2D("yz","ch1:ch2",500,0,8192,500,0,8192);

  TH1D *ch0 = new TH1D("ch0","ADC ch0 shaper",8192,0,8192);
  TH1D *ch1 = new TH1D("ch1","ADC ch1 shaper",8192,0,8192);
  TH1D *ch2 = new TH1D("ch2","ADC ch2 shaper",8192,0,8192);
  TH1D *ch3 = new TH1D("ch3","ADC ch3 shaper",8192,0,8192);

  TH1D *hsuml =  new TH1D("hsuml","ADC ch0+ch3",8192,0,8192);
  TH1D *hsumh =  new TH1D("hsumh","ADC ch1+ch2",8192,-1000,7192);

  ifstream data(fname, ios::in);
  if(!data.good()){
    cerr << "Can not open " << fname << "!\n";
    return NULL;
  }

  char  cut[128],cut2[128];
  TCut ext,ext2;
  //sprintf( cut, "1==1 && 0==0 ");
  sprintf( cut, "ww< 1*xx+600  && ww > 1*xx-500");
  ext = cut;
  sprintf( cut2, "zz< 1*yy+1200  && zz > 1*yy+500");
  ext2 = cut2;
 

  double x;  
  TF1 *f1 = new TF1("f1", "1*x+600",0,10000);
  TF1 *f2 = new TF1("f2", "1*x-500",0,10000);
  TF1 *f3 = new TF1("f3", "1*x+100",0,10000);
  TF1 *f4 = new TF1("f4", "1*x-1200",0,10000);


  while( data >> xx >> yy >> zz >> ww ){
    if (xx>0 && yy>0 ){ 
      suml = xx - 100 + ww -100;   //subtract offsets
      sumh = yy - 1044 + zz -100;  //subtract offsets
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

  pad2->cd();
  pad2->SetLeftMargin(0.2);
  pad2->SetRightMargin(0.1);

  t1->Project("yz","zz:yy");
  
  yz->GetXaxis()->SetTitle("ADC ch1");
  yz->GetYaxis()->SetTitle("ADC ch2"); 
  yz->GetXaxis()->CenterTitle(); 
  yz->GetYaxis()->CenterTitle(); 
  
  yz->SetMaximum(100);
  yz->Draw("contz");
  yz->Draw("colz");
  f3->Draw("same");
  f4->Draw("same");
  gStyle->SetPalette(1);


  pad4->cd();
  pad4->SetLeftMargin(0.1);
  pad4->SetRightMargin(0.1);
  ch0->SetLineColor(4);
  ch0->SetLineWidth(2);

  t1->Project("ch0","xx", ext);
  
  ch0->GetXaxis()->SetTitle("ADC0");
  ch0->GetYaxis()->SetTitle("counts"); 
  ch0->GetXaxis()->CenterTitle(); 
  ch0->GetYaxis()->CenterTitle(); 
  
  //ch0->SetMaximum(200);
  ch0->Draw();

  pad5->cd();
  pad5->SetLeftMargin(0.1);
  pad5->SetRightMargin(0.1);
  ch1->SetLineColor(4);
  ch1->SetLineWidth(2);

  t1->Project("ch1","yy", ext);
  
  ch1->GetXaxis()->SetTitle("ADC1");
  ch1->GetYaxis()->SetTitle("counts"); 
  ch1->GetXaxis()->CenterTitle(); 
  ch1->GetYaxis()->CenterTitle(); 
  
  //ch1->SetMaximum(200);
  ch1->Draw();

  pad6->cd();
  pad6->SetLeftMargin(0.1);
  pad6->SetRightMargin(0.1);
  ch3->SetLineColor(4);
  ch3->SetLineWidth(2);

  t1->Project("ch3","ww", ext);
  
  ch3->GetXaxis()->SetTitle("ADC3");
  ch3->GetYaxis()->SetTitle("counts"); 
  ch3->GetXaxis()->CenterTitle(); 
  ch3->GetYaxis()->CenterTitle(); 
  
  //ch3->SetMaximum(200);
  ch3->Draw();

  pad7->cd();
  pad7->SetLeftMargin(0.1);
  pad7->SetRightMargin(0.1);
  ch2->SetLineColor(4);
  ch2->SetLineWidth(2);

  t1->Project("ch2","zz", ext);
  
  ch2->GetXaxis()->SetTitle("ADC2");
  ch2->GetYaxis()->SetTitle("counts"); 
  ch2->GetXaxis()->CenterTitle(); 
  ch2->GetYaxis()->CenterTitle(); 
  
  //ch2->SetMaximum(200);
  ch2->Draw();
 

  pad1->cd();
  pad1->SetLeftMargin(0.1);
  pad1->SetRightMargin(0.1);
  hsuml->SetLineColor(4);
  hsuml->SetLineWidth(2);

  t1->Project("hsuml","suml", ext);
  
  hsuml->GetXaxis()->SetTitle("ADC0+ADC3");
  hsuml->GetYaxis()->SetTitle("counts"); 
  hsuml->GetXaxis()->CenterTitle(); 
  hsuml->GetYaxis()->CenterTitle(); 
  
  //hsuml->SetMaximum(200);
  hsuml->Draw();

  pad3->cd();
  pad3->SetLeftMargin(0.1);
  pad3->SetRightMargin(0.1);
  hsumh->SetLineColor(4);
  hsumh->SetLineWidth(2);

  t1->Project("hsumh","sumh", ext);
  
  hsumh->GetXaxis()->SetTitle("ADC1+ADC2");
  hsumh->GetYaxis()->SetTitle("counts"); 
  hsumh->GetXaxis()->CenterTitle(); 
  hsumh->GetYaxis()->CenterTitle(); 
  
  //hsumh->SetMaximum(200);
  hsumh->Draw();

}

