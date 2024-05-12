void calib(){
 gROOT->Reset();
 // double a=-21.0097;
 //double b=0.05913;
  double x,y;
  gStyle->SetOptLogy(1);
  TH1D *original= new TH1D("calib","calib",32768,-0.5,32767.5);
 
  ifstream data;
  data.open("20170301_1200V_241Am_001_eh_0.dat");
  TF1 * f = new TF1("f","[0]*exp(-(x-[1])/[2])+[3]*x+[4]");
  f->SetParameters(0,100);
  f->SetParameters(1,1200);
  f->SetParameters(2,14);
  f->SetParameters(3,0.025);
  f->SetParameters(4,-10);
  while(data>>x>>y) original->Fill(x,y);
  //  original->Fit("gaus","","",20,185);
  original->Fit(f,"","",1050,1450);
  original->Draw();
  /*
  TH1D *ene= new TH1D("","",32768,a+b*0.5,a+b*32768.5);
  for(int i=1;i<=32768;i++){
    ene->SetBinContent(i,original->GetBinContent(i));

  }
  ene->Draw();
  */
}
