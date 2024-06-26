
//////Velocity integral

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

//////Theoritical spectrum
double spe(double M_D,double Er){
  double mass=1.0; ///NaI mass in kg
  double pi=3.14159265358979;
  double v0 =220.0; //km/s
  double vesc =650.0; //////DM Escape velocity
  double ve =244.0; /////Earth velocity vs Galaxy
  double AI =127.; /////I mass num
  double ANa =23.; /////Na mass num
  double mn =0.932; //////nucleon mass
  double M_Na =mn*ANa; 
  const double mu_Na=(M_D*M_Na)/(M_D+M_Na);
  const double mun=(M_D*0.93957)/(M_D+0.93957);
  double M_I =mn*AI;
  const double mu_I=(M_D*M_I)/(M_D+M_I);
  double r_Na =4*M_D*M_Na/(M_D+M_Na)**2.0;
  double r_I =4*M_D*M_I/(M_D+M_I)**2.0;
  double c =299792.458; ///////light speed
  double E_0 =0.5*M_D*(v0/c)**2.0*1e6.0;;
  double sigma_n=1.0; //nucleon cross section
  double sigma=sigma_n*(ANa*mu_Na/mun)**2.0;
  double R_0Na=503.138609093/M_D/M_Na*sigma*0.3/0.4*v0/230.0;
  sigma=sigma_n*(AI*mu_I/mun)**2.0;
  double R_0I=503.138609093/M_D/M_I*sigma*0.3/0.4*v0/230.0;
  double Er;
  double vmin;
  double rate;
  double q,form;
  q=(2*M_Na*Er)**0.5;
  vmin=(Er/E_0/r_Na)**0.5* v0;
  form =form_factor(q,ANa);
  rate=R_0Na/E_0/r_Na*form*form*g_m(vmin/c,v0/c,ve/c,vesc/c)/2*v0/c*mass*23.0/150.0*pi**0.5;
  q=(2*M_I*Er)**0.5;
  form =form_factor(q,AI);
  vmin=(Er/E_0/r_I)**0.5* v0;
  rate+=R_0I/E_0/r_I*form*form*g_m(vmin/c,v0/c,ve/c,vesc/c)/2*v0/c*mass*127.0/150.0*pi**0.5;

  // if(vmin<1000)  cout<<vmin<<endl;
  // cout<<rate<<endl;
  return rate;
}

double tatami( double M_D, double Er, double sigma){
 
  double ene,rate;
  rate=0.0;
  for(int i=1;i<=40;i++){
    ene=(double)i*5.;
    double theta=sigma*sigma/ene;
    double k=ene/theta;
    rate+=5.*spe(M_D,ene)* ROOT::Math::gamma_pdf(Er,k,theta,0);

  }
  return rate;
}

void calc_limit(){
  gROOT->Reset();
  // gStyle->SetOptLogx();
  gStyle->SetOptLogy();

  const int point=30; ///////
  double q = .2; ////quenching factor
  double t=7200./86400.;   /////Exposure time //////CHANGE THIS
  double m = .360;  /////NaI mass 
  /////Visible energy = a+b*(bin)///
  double a=-9.44589; ///////CHANGE THIS
  double b=0.0567819;  ////////CHANGE THIS

  /////energy width of bin (keV)  
  double energy_width =10.0 ;

  /////////Threshold energy(Visible)
  double th_ene= 10.;
  
  TH1D *original= new TH1D("calib","calib",32768,-0.5,32767.5);
  
  double x,y;
  ifstream data;
  data.open("20170301_Shield_Bg_001_eh_0.dat"); 
  double er,rate;
  while(data>>x>>y) original->Fill(x,y);
  
  /////90%CL Histogram of recoil energy. Remove edge bin(=1,32768).
  TH1D *qene = new TH1D("qene","recoil energy; Recoil energy (keVnr); 90% CL reaction rate (dru)",32766, (a+b*(1.-.5))/q, (a+b*(32766.+.5))/q  );
  TH1D *riron = new TH1D("riron","recoil energy",32766, (a+b*(1.-.5))/q, (a+b*(32766.+.5))/q  );
  TH1D *riron0 = new TH1D("riron0","recoil energy",32766, (a+b*(1.-.5))/q, (a+b*(32766.+.5))/q  );

  for(int i=2;i<=32767;i++){
    
    y=original->GetBinContent(i);
    qene->SetBinContent(i-1,y+1.64*pow(y,.5));
    er=qene->GetXaxis()->GetBinCenter(i-1);
    rate=0.0;
  
    if(er>0 && er<1000){
      rate=tatami(100.,er,energy_width);
      riron->SetBinContent(i-1,rate);
      riron0->SetBinContent(i-1,spe(100.,er));
    }
    if(i%100==0)cout<<i<<","<<rate<<endl;
    
  }

  double bin_width=1.*b/q; /////Bin width;
  int comb =(int)(energy_width/q/bin_width);
  comb=129;
  qene->Rebin(comb);
  qene->Scale(1./m/t/bin_width/comb);
  cout<<"Rebin = "<<comb<<endl;
  FILE *fp;
  fp=fopen("sigma_shield.dat","w");
  int nbin =qene->GetXaxis()->GetNbins();
  double max,buf,buf2,buf3;
  double mass[point],sig[point],cross;
  cross=0.0;
  for(i=0;i<point;i++){
    mass[i]=pow(10.,1.47+((double)i+2)/point*1.53);
    cout<< "Analyzing WIMP mass ="<< mass[i]<<endl;
    max=1000000000000000;
    for(int j=1;j<nbin;j++){
      buf=qene->GetBinContent(j);
      er=qene->GetXaxis()->GetBinCenter(j);
      if(er>=50){
	buf2=tatami(mass[i],er,energy_width);
	
	if(buf2> 1e-20 && buf>0.){
	  buf3=buf/buf2;
	}else{
	  buf3=100000000000000000000000;
	}
	if(max>buf3){
	  max=buf3;
	  cross=er;
	}
      
      }
  
    }
      sig[i]=1.0*max;
      if(sig[i]>1e-70) {
	fprintf(fp,"%e %e\n",mass[i],sig[i]*1.0e-36);
	cout<<sig[i]<<","<<cross<<endl;
    }
  
  }
  
  fclose(fp);
  // qene->Draw();
   riron->SetLineColor(kRed);
  riron->Draw();
  riron0->SetLineColor(kBlue);
  riron0->Draw("same");
  
}
