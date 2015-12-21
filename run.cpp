#include <iostream>
#include <string>
#include <TApplication.h>
#include <TH1F.h>
#include <TVector3.h>
#include "IFluxAnalysis.h"

using namespace std;

/// calculate the surface area
double GetSurface(double l, double t, double w)
{ return 2*l*t + 2*l*w + 2*t*w; }

//
int main(int argc, char** argv) {
  
  string file = string(argv[1]);
  
  TApplication* app = new TApplication("app", &argc, argv);
  
  IFluxAnalysis* flux = new IFluxAnalysis(file);
  flux->CheckParticle();
  flux->SetPosCondition(79.6, 79.6+1.1);
  flux->Plot();

  app->Run();
  return 0;
}
