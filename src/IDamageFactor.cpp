#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include "IDamageFactor.h"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

enum PID {
  kProton   = 2212,
  kElectron = 11,
  kPion     = -211,
  kNeutron  = 2112,
};

IDamageFactor::IDamageFactor()
    : fPath("/Users/YeYang/Documents/COMET/WorkStation/DetRadiation/analysiscode/dat")
{
  fAcc = gsl_interp_accel_alloc();
}

///
IDamageFactor::~IDamageFactor()
{
  gsl_spline_free(fNeutron);
  gsl_spline_free(fElectron);
  gsl_spline_free(fPion);
  gsl_interp_accel_free(fAcc);
  gsl_interp_accel_free(fAcc2);
  gsl_interp_accel_free(fAcc3);
}

///
void IDamageFactor::SetPath(string path)
{ fPath = path; }

///
void IDamageFactor::Open(int pdg, string name)
{
  string data = fPath + name;
  TFile* file = new TFile(data.c_str());

  if (!file) {
    cerr << " Error: the file dose not exist! " << endl;
    return ;
  }

  TTree* tree = (TTree*)file->Get("tree");
  
  double dpa[tree->GetEntries()], energy[tree->GetEntries()]; 
  double factor, E;
  tree->SetBranchAddress("energy", &E     );
  tree->SetBranchAddress("dpa"   , &factor);
    
  for (int i=0; i<tree->GetEntries(); i++) {  
    tree->GetEntry(i);
    energy[i] = E;
    dpa   [i] = factor;
  }
  
  int npt = sizeof(dpa)/sizeof(dpa[0]);

  switch (pdg) {
    // electron
	case kElectron:
      fElectron = gsl_spline_alloc(gsl_interp_linear, npt);
      gsl_spline_init(fElectron, energy, dpa, npt);
      break;
    // neutron
    case kNeutron:
      fNeutron = gsl_spline_alloc(gsl_interp_linear, npt);
      gsl_spline_init(fNeutron, energy, dpa, npt);
      break;
    // pion
    case 211:
    case kPion:
      fPion = gsl_spline_alloc(gsl_interp_linear, npt);
      gsl_spline_init(fPion, energy, dpa, npt);
      break;
    // others
    default:
      cerr << " Error: there is no file for this particle!" << endl;
      return ;
  }
}

///
void IDamageFactor::Open(int pdg, string name1, string name2)
{}

///
void IDamageFactor::Load()
{
  // load neutron file
  Open(kNeutron, "/neutron.root");
  // load electron file
  Open(kElectron, "/electron.root");
  // load pion file
  Open(kPion, "/pion.root");
}

///
double IDamageFactor::Interpolation(int pdg, double energy) 
{
  double factor = 1.;

  switch (pdg) {
    // electron
    case -11:
    case kElectron:
      if ( energy>=0.3 && energy<=200. )
        factor = gsl_spline_eval(fElectron, energy, fAcc);
      else
        return 0.;
      break;
    // neutron
    case kNeutron:
      if ( energy>=1.000e-10 && energy<=8990. )
        factor = gsl_spline_eval(fNeutron, energy, fAcc2);
      else
        return 0.;
      break;
    // pion
    case 211:
    case kPion:
      if ( energy>=15. && energy<=9005. )
        factor = gsl_spline_eval(fPion, energy, fAcc3);
      else
        return 0.;
      break;
    // others
    default:
      return 0.;
      break;
  }
  
  if (factor<0) return 1.;

  return factor;
}
