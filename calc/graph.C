void graph(){
  gROOT->Reset();
  gStyle->SetOptLogy();
  gStyle->SetOptLogx();
  gStyle->SetPadGridY(1);
  gStyle->SetPadGridX(1);
  TGraph *g =new TGraph("sigma.dat","%lf %lf");
  g->Draw("P");

  g->SetMarkerStyle(21);
  g->SetMarkerSize(0.3);
  
  TH1* frame=gPad->DrawFrame(30,1e-39,1000,1e-34,"WIMP-nucleon sigma Upper limit (90%CL); WIMP mass (GeV); cross section Upper limit (cm^2)");
  frame->Draw();
  g->SetMarkerStyle(2);
  g->SetMarkerSize(0.5);
  g->Draw("P");

}
