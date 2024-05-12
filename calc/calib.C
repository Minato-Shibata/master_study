void calib(){
 gROOT->Reset();
 double a=-21.0097;
 double b=0.05913;
  double x,y;
  gStyle->SetOptLogy(1);
  TH1D *original= new TH1D("calib","calib",32768,-0.5,32767.5);
 
  ifstream data;
  data.open("data/20170210DM_010_eh_0.dat"); 
  while(data>>x>>y) original->Fill(x,y);
  original->Fit("gaus","","",20,185);
  original->Draw();
  TH1D *ene= new TH1D("","",32768,a+b*0.5,a+b*32768.5);
  for(int i=1;i<=32768;i++){
    ene->SetBinContent(i,original->GetBinContent(i));

  }
  ene->Draw();

}
