void graph(){
  gROOT->Reset();
  gStyle->SetOptLogy();
  gStyle->SetOptLogx();
  gStyle->SetPadGridY(1);
  gStyle->SetPadGridX(1);
  TGraph *g =new TGraph("sigma_bare.dat","%lf %lf");
  TGraph *g1 =new TGraph("sigma_shield.dat","%lf %lf");
  g->Draw("P");

  
  TH1* frame=gPad->DrawFrame(30,1e-39,1000,1e-34,"WIMP-nucleon sigma Upper limit (90%CL); WIMP mass (GeV); cross section Upper limit (cm^2)");
  frame->Draw();
  g->SetMarkerStyle(2);
  g->SetMarkerSize(0.5);
  
  g1->SetMarkerStyle(3);
  g1->SetMarkerSize(0.5);
  g->Draw("P");
  g1->Draw("P");
  

}
