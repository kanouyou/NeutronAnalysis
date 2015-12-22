#include <iostream>
#include <string>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TVector3.h>
#include "IFluxAnalysis.h"

using namespace std;

namespace sim {
  const int n = 4;
  string file  [n] = {"FluxOfRECBE.root", "Flux1cmShield.root", "Flux2cmShield.root", "Flux3cmShield.root"};
  double shield[n] = {0., 1., 2., 3.};
  int    event [n] = {1000*1000, 1000*1000, 250*1000*5, 250*1000*5};
  int    color [n] = {kRed, kBlack, kBlue, kGreen+3};
}

/// calculate the surface area
double GetSurface(double l, double t, double w)
{ return 2*l*t + 2*l*w + 2*t*w; }

//
int main(int argc, char** argv) {
  
  int pdg = atoi(argv[1]);
  
  // ................................... //
  double I   = 2.5e+12;
  double area[6];
  double Length = 17.2;    // [cm]
  double Thick  = 1.1;
  area[0] = GetSurface(Length, Thick, 2*M_PI*79.6);
  area[1] = GetSurface(Length, Thick, 2*M_PI*(79.6-3.6));
  area[2] = GetSurface(Length, Thick, 2*M_PI*(79.6-3.6*2-1.1*2));
  area[3] = GetSurface(Length, Thick, 2*M_PI*(79.6-3.6*3-1.1*3));
  area[4] = GetSurface(Length, Thick, 2*M_PI*(79.6-3.6*3-1.1*4-5.2));
  area[5] = GetSurface(Length, Thick, 2*M_PI*(79.6-3.6*3-1.1*4-5.2-4.2));
  // .................................. //

  TApplication* app = new TApplication("app", &argc, argv);
  
  /*IFluxAnalysis* flux = new IFluxAnalysis(file);
  flux->SetIntensity(I);
  flux->SetEvent(evt);
  flux->SetArea(area[0]);
  flux->CheckParticle();
  flux->SetPosCondition(79.6, 79.6+1.1);
  flux->Plot();
  flux->Print();*/
  /// ---------------------------------------
  /// plot neutron flux
  /// ---------------------------------------
  IFluxAnalysis* flux[sim::n];
  TH1F* hist[sim::n];
  
  for (int i=0; i<sim::n; i++) {
    cout << "file: " << sim::file[i] << endl;
    flux[i] = new IFluxAnalysis(sim::file[i]);
    flux[i]->SetIntensity(I);
    flux[i]->SetEvent(sim::event[i]);
    flux[i]->SetArea(area[0]);
    flux[i]->SetPosCondition(79.6, 79.6+1.1);
    flux[i]->Print();
    hist[i] = flux[i]->GetFlux(pdg);
  }
//  flux[0]->CheckSource(pdg);

  TCanvas* c0 = new TCanvas("neutron", "neutron", 650, 500);
  c0->SetTicks(1,1);
  c0->SetLogx();
  c0->SetLogy();

  for (int i=0; i<sim::n; i++) {
    hist[i]->SetLineColor(sim::color[i]);
    hist[i]->SetLineWidth(2);
    if (i==0) hist[i]->Draw();
    else hist[i]->Draw("same");
  }
  
  c0->Print("neutron.eps");

  app->Run();
  return 0;
}
