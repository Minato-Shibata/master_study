

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




void energy_spe(){
  gROOT->Reset();
  gStyle->SetOptLogy(1);
  double quench=1.;
  double mass=0.350; ///NaI mass in kg
  double pi=3.14159265358979;
  double v0 =220.0; //km/s
  double vesc =650.0;
  double ve =244.0;
  double AI =127.;
  double ANa =23.;
  double mn =0.932;
  double M_D =100.;
  double M_Na =mn*ANa;
  const double mu_Na=(M_D*M_Na)/(M_D+M_Na);
  const double mun=(M_D*0.93957)/(M_D+0.93957);
  double M_I =mn*AI;
  const double mu_I=(M_D*M_I)/(M_D+M_I);
  double r_Na =4*M_D*M_Na/(M_D+M_Na)**2.0;
  double r_I =4*M_D*M_I/(M_D+M_I)**2.0;
  double c =299792.458;
  double E_0 =0.5*M_D*(v0/c)**2.0*1e6.0;;
  double sigma_n=1.0e-5.; //pb
  double sigma=sigma_n*(ANa*mu_Na/mun)**2.0;
  double R_0Na=503.138609093/M_D/M_Na*sigma*0.3/0.4*v0/230.0;
  sigma=sigma_n*(AI*mu_I/mun)**2.0;
  double R_0I=503.138609093/M_D/M_I*sigma*0.3/0.4*v0/230.0;

  TH1D *Na_histo=new TH1D("Na_histo","Na Energy",1000,0,200);
  TH1D *I_histo=new TH1D("I_histo","I Energy",1000,0,200);
  TH1D *total=new TH1D("TOTAL","Energy histogram",1000,0,200);
  double Er;
  double vmin;
  double rate;
  double q,form;

  for(int i=1;i<=1000;i++){
    Er=Na_histo->GetXaxis()->GetBinCenter(i);
    q=(2*M_Na*Er)**0.5;
    vmin=(Er/E_0/r_Na)**0.5* v0;
    form =form_factor(q,ANa);
    rate=R_0Na/E_0/r_Na*form*form*g_m(vmin/c,v0/c,ve/c,vesc/c)/2*v0/c*mass*23.0/150.0*pi**0.5;
    //cout<<vmin<<endl;
    Na_histo->Fill(Er*quench,rate);
    total->Fill(Er*quench,rate);
  }
 
  for(int i=1;i<=1000;i++){
    Er=I_histo->GetXaxis()->GetBinCenter(i);
    q=(2*M_I*Er)**0.5;
    form =form_factor(q,AI);
    vmin=(Er/E_0/r_I)**0.5* v0;
    rate=R_0I/E_0/r_I*form*form*g_m(vmin/c,v0/c,ve/c,vesc/c)/2*v0/c*mass*127.0/150.0*pi**0.5;
    I_histo->Fill(Er*quench,rate);
    total->Fill(Er*quench,rate);
  }
  total->SetXTitle("Visible energy (keV)");
  total->SetYTitle("Event rate (events/kg/day/keV)");
  I_histo->SetLineColor(kGreen);
  total->SetLineColor(kRed);
  // total->SetLineWidth(3);
  //Na_histo->Draw();
  //I_histo->Draw("same");
  total->Draw();

}
