#include <iostream>
#include <cmath>
#include <TCanvas.h>
#include <TLegend.h>
#include "IFluxAnalysis.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::map;

IFluxAnalysis::IFluxAnalysis(string file)
    : IAnalysisBase(file, "f"), fArea(1.), fFactor(new DamageFactorBase(11))
{
  double bin[] = {1.1e-9, 1.6e-6, 3.4e-6, 7.3e-6, 1.6e-5, 3.4e-5, 7.3e-5, 1.5e-4, 3.0e-4,
                  6.0e-4, 1.1e-3, 2.0e-3, 3.4e-3, 5.3e-3, 8.5e-3, 1.2e-2, 1.4e-2, 1.5e-2,
                  1.7e-2, 2.0e-2, 2.2e-2, 2.5e-2, 2.9e-2, 3.3e-2, 3.8e-2, 4.3e-2, 4.9e-2,
                  5.6e-2, 6.3e-2, 7.2e-2, 8.2e-2, 9.4e-2, 1.1e-1, 1.2e-1, 1.4e-1, 1.6e-1, 
                  1.8e-1, 2.1e-1, 2.3e-1, 2.7e-1, 3.0e-1, 3.5e-1, 3.9e-1, 4.5e-1, 5.1e-1, 
                  5.8e-1, 6.7e-1, 7.6e-1, 8.6e-1, 9.9e-1, 1.2e+0, 1.3e+0, 1.5e+0, 1.7e+0, 
                  1.9e+0, 2.2e+0, 2.5e+0, 2.8e+0, 3.2e+0, 3.6e+0, 4.2e+0, 4.7e+0, 5.4e+0, 
                  6.0e+0, 6.4e+0, 6.9e+0, 7.4e+0, 7.8e+0, 8.3e+0, 9.2e+0, 9.9e+0, 1.2e+1, 
                  1.3e+1, 1.5e+1, 1.7e+1, 1.9e+1, 2.2e+1, 2.4e+1, 2.5e+1, 2.8e+1, 3.2e+1, 
                  3.6e+1, 4.2e+1, 4.7e+1, 5.4e+1, 6.0e+1, 6.4e+1, 6.9e+1, 7.4e+1, 7.8e+1, 
                  8.3e+1, 9.2e+1, 9.9e+1, 1.2e+2, 1.3e+2, 1.5e+2, 1.7e+2, 1.9e+2, 2.2e+2, 
                  2.4e+2, 2.5e+2, 2.8e+2, 3.2e+2, 3.6e+2, 4.2e+2, 4.7e+2, 5.4e+2};
  
  fNbin = sizeof(bin) / sizeof(bin[0]);
  fBin = new double[fNbin];
  for (int i=0; i<fNbin; i++)
    fBin[i] = bin[i];
}

IFluxAnalysis::~IFluxAnalysis()
{
  delete fFactor;
  delete [] fBin;
}

void IFluxAnalysis::SetBin(double* bin)
{}

TH1F* IFluxAnalysis::GetFlux(int pdg)
{
  TH1F* hist = new TH1F(Form("flux_#%i",pdg), Form("flux_#%i",pdg), fNbin-1, fBin);
  
  if (buffPdgcode.size()==0) {
    for ( vector<int>::size_type i=0; i<fPdgCode.size(); i++ ) {
      if ( fPdgCode[i]==pdg )
        hist->Fill(fEnergy[i]);
    }
  }
  else {
    for ( vector<int>::size_type i=0; i<buffPdgcode.size(); i++ ) {
      if ( buffPdgcode[i]==pdg )
        hist->Fill(buffEnergy[i]);
    }
  }

  hist->Scale(1. * fIntensity / fEvent / fArea);
  
  return hist;
}

///
void IFluxAnalysis::SetPosCondition(TVector3& min, TVector3& max)
{
  for (vector<int>::size_type i=0; i<fPdgCode.size(); i++) {
    if ( fPos[i]->X()<max.X() && fPos[i]->X()>min.X() && fPos[i]->Y()<max.Y() &&
         fPos[i]->Y()>min.Y() && fPos[i]->Z()<max.Z() && fPos[i]->Z()>min.Z() ) {
      buffPdgcode.push_back(fPdgCode[i]);
      buffEnergy.push_back(fEnergy[i]);
    }
  }
}

///
void IFluxAnalysis::SetPosCondition(double rmin, double rmax)
{
  double r;
  for (vector<int>::size_type i=0; i<fPdgCode.size(); i++) {
    r = sqrt(pow(fPos[i]->Y(),2)+pow(fPos[i]->Z()+765,2));
    if ( r>rmin && r<rmax ) {
      buffPdgcode.push_back(fPdgCode[i]);
      buffEnergy.push_back(fEnergy[i]);
    }
  }
}

///
void IFluxAnalysis::Plot()
{
  AnalysisMap particle = GetParticle();
  
  // delete the information for neutrino
  AnalysisMap::iterator it = particle.find("nu_e_bar");
  particle.erase(it);
  it = particle.find("nu_mu");
  particle.erase(it);

  TCanvas* c0 = new TCanvas("flux", "flux", 650, 500);
  c0->SetTicks(1,1);
  c0->SetLogx();
  c0->SetLogy();
  
  const int n = particle.size();
  TH1F* hist[n];
  int cnt = 1;
  
  int color[6] = {kRed, kBlack, kAzure-3, kGreen+3, kOrange+7, kBlue};

  // plot the fist histogram ( note: make sure the first plot has enough event,
  // otherwise you have to scale the histogram )
  for (AnalysisMap::iterator i=particle.begin(); i!=particle.end(); ++i) {
    if (i->first=="neutron")
      hist[0] = GetFlux(i->second);
  }
  hist[0]->SetTitle("; Energy [MeV]; Flux [n/cm^{2}/sec]");
  hist[0]->Draw();
  
  // draw the other particles
  for (AnalysisMap::iterator i=particle.begin(); i!=particle.end(); ++i) {
    if (i->first!="neutron") {
      hist[cnt] = GetFlux(i->second);
      hist[cnt]->Draw("same");
      cnt++;
    }
  }

  for (int i=0; i<n; i++) {
    hist[i]->SetLineColor(color[i]);
	hist[i]->SetLineWidth(2);
  }

  cnt = 0;
  TLegend* lg = new TLegend(0.6, 0.6, 0.85, 0.85);
  lg->SetTextSize(0.045);
  lg->SetFillStyle(0);
  lg->SetBorderSize(0);
  for (AnalysisMap::iterator i=particle.begin(); i!=particle.end(); ++i) {
    lg->AddEntry(hist[cnt], i->first.c_str(), "l");
    cnt++;
  }
  lg->Draw();
}

///
double* IFluxAnalysis::GetEqNeutron()
{
  // 1st container of flux: 1 MeV equivalent neutron flux
  // 2nd container of flux: total neutron flux
  double* flux = new double[2];
  double eqNeu = 0.;
  int    neu   = 0;

  // to convert neutron flux to 1 MeV equivalent neutron flux
  if ( buffPdgcode.size()==0 ) {
    for (vector<int>::size_type i=0; i<fPdgCode.size(); i++) {
      eqNeu += fFactor->Interpolation(fPdgCode[i], fEnergy[i]);
      if (fPdgCode[i]==2112) neu++;
    }
  }
  else {
    for (vector<int>::size_type i=0; i<buffPdgcode.size(); i++) {
      eqNeu += fFactor->Interpolation(buffPdgcode[i], buffEnergy[i]);
      if (buffPdgcode[i]==2112) neu++;
    }
  }
  
  flux[0] = eqNeu * fIntensity / fEvent / fArea;
  flux[1] = (double)neu * fIntensity / fEvent / fArea;
  
  return flux;
}

///
void IFluxAnalysis::Print()
{
  cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;
  cout << "  Neutron Flux                                       " << endl;
  cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;

  cout << " Total neutron flux: " << GetEqNeutron()[1] << " [n/cm2/sec]" << endl;
  cout << " 1 MeV equivalent neutron flux: " << GetEqNeutron()[0] << " [n/cm2/sec]" << endl;
  
  cout << "\n";
}
