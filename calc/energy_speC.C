

////// integrated f(v)/v my result
double g_m(double vmin,double v0,double ve, double vesc){
  vmin=vmin/v0;
  vesc=vesc/v0;
  ve=ve/v0;
  double gauss=3.1415926535897932**0.5;
  double result;
  if(vmin>=(ve+vesc)){
    result= 0.0;
  }else if(vmin<(vesc-ve)){
    result=(0.5*(-TMath::Erfc(vmin+ve)+TMath::Erfc(vmin-ve))-2.0*ve*exp(-(vesc**2.0))/gauss)/(ve*v0 *(TMath::Erf(vesc)-2/gauss*vesc*exp(-(vesc**2.0))));
  }else{
    result=(0.5*(-TMath::Erfc(vesc)+TMath::Erfc(vmin-ve))-(ve+vesc-vmin)*exp(-(vesc**2.0))/gauss)/(ve*v0 *(TMath::Erf(vesc)-2/gauss*vesc*exp(-(vesc**2.0))));
  }

  return result;
}

/////FORM FACTOR
double form_factor(double q,double A){
  double s= .9;
  double rn;
  double a=0.52;
  double R,temp;
  double c;
  c=1.23*pow(A,1./3.)-0.60;
  temp=c**2.0+7./3.*3.14*3.14*a*a-5.*s*s;
  if(temp>=0.0){
    R=temp**0.5;
  }else{
    R=0.0;
  }
  R=R/197.3;
  s=s/197.3;
  // return 3*(sin(q*R)-q*R*cos(q*R))/(q*R)**3.0*exp(-0.5*((q*s)**2.0));
  return 3*(sin(q*R)-q*R*cos(q*R))/(q*R)**3.0*exp(-0.5*((q*s)**2.0));
  
}




void energy_speC(){

  //TCanvas *c1 = new TCanvas("c1","The Canvas",200,10,550,500);
  
  gROOT->Reset();
  gStyle->SetOptLogy(1);
  double quench=0.2;
  //double quench=1.;
  double mass=0.06296; //ZnWO4 mass in kg
  //double mass=0.350; ///NaI mass in kg
  double pi=3.14159265358979;
  double v0 =220.0; //km/s
  double vesc =600.0;
  double ve =227.0;
  double ve_J=242.0; //June
  double ve_D=212.0; //December
  double AO =16.;
  double mn =0.932;
  double M_D =100.;
  double M_O =mn*AO;
  const double mu_O=(M_D*M_O)/(M_D+M_O);
  const double mun=(M_D*0.93957)/(M_D+0.93957);
  double r_O =4*M_D*M_O/(M_D+M_O)**2.0;
  double c =299792.458;
  double E_0 =0.5*M_D*(v0/c)**2.0*1e6.0;;
  double sigma_n=1.0e-5.; //pb
  double sigma=sigma_n*(AO*mu_O/mun)**2.0;
  double R_0O=503.138609093/M_D/M_O*sigma*0.3/0.4*v0/230.0;
  
  TH1D *Jun=new TH1D("Jun","Energy histgram",10000,0,200);
  TH1D *Dec=new TH1D("Dec","Energy histgram",10000,0,200);
  //TH1D *total=new TH1D("TOTAL","Energy histogram",1000,0,200);
  double Er;
  double vmin;
  double rate;
  double q,form;

  for(int i=1;i<=10000;i++){
    Er=Jun->GetXaxis()->GetBinCenter(i);
    q=(2*M_O*Er)**0.5;
    vmin=(Er/E_0/r_O)**0.5* v0;
    form =form_factor(q,AO);
    rate=R_0O/E_0/r_O*form*form*g_m(vmin/c,v0/c,ve_J/c,vesc/c)/2*v0/c*mass*16.0/150.0*pi**0.5;
    //cout<<vmin<<endl;
    Jun->Fill(Er*quench,rate);
    //total->Fill(Er*quench,rate);
  }
 
  for(int i=1;i<=10000;i++){
    Er=Dec->GetXaxis()->GetBinCenter(i);
    q=(2*M_O*Er)**0.5;
    form =form_factor(q,AO);
    vmin=(Er/E_0/r_O)**0.5* v0;
    rate=R_0O/E_0/r_O*form*form*g_m(vmin/c,v0/c,ve_D/c,vesc/c)/2*v0/c*mass*16.0/150.0*pi**0.5;
    Dec->Fill(Er*quench,rate);
    //total->Fill(Er*quench,rate);
  }

  //total->SetXTitle("Visible energy (keV)");
  //total->SetYTitle("Event rate (events/kg/day/keV)");
  Jun->SetXTitle("Visible energy[keV]");
  Jun->SetYTitle("Event rate[events/day/keV]");
  Jun->SetLineColor(kRed);
  Dec->SetLineColor(kBlue);
  //I_histo->SetLineColor(kGreen);
  //total->SetLineColor(kRed);
  // total->SetLineWidth(3);
  //Na_histo->Draw();
  //I_histo->Draw("same");
  //total->Draw();
  Jun->Draw();
  Dec->Draw("same");
}
