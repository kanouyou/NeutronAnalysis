#include <iostream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include "DamageFactorBase.h"

using namespace std;

namespace Data {
  //const int n = 5;
  string path     = "/Users/YeYang/Documents/COMET/WorkStation/DetRadiation/analysiscode";
  string neutron  = path + "/dat/neutron.root";
  string electron = path + "/dat/electron.root";
  string pion     = path + "/dat/pion.root";
  string proton1  = path + "/dat/proton_huhtinen.root";
  string proton2  = path + "/dat/proton_summers.root";
}

DamageFactorBase::DamageFactorBase(const int pdg) {
  pdgcode = pdg;
  if (pdgcode==2112)
    OpenFile(Data::neutron);
  else if (pdgcode==11)
    OpenFile(Data::electron);
  else if (pdgcode==-211)
    OpenFile(Data::pion);
  else if (pdgcode==2212) {
    OpenFile(Data::proton1);
	OpenFile(Data::proton2);
  }
}

void DamageFactorBase::SetParticle(const int pdg) {
  pdgcode = pdg;
  if (pdgcode==2112)
    OpenFile(Data::neutron);
  else if (pdgcode==11)
    OpenFile(Data::electron);
  else if (pdgcode==-211)
    OpenFile(Data::pion);
  else if (pdgcode==2212) {
    OpenFile(Data::proton1);
	OpenFile(Data::proton2);
  }
}

void DamageFactorBase::OpenFile(string fileName) {
  TFile* file = new TFile(fileName.c_str());
  TTree* tree = (TTree*)file->Get("tree");

  double energy, dpa;
  tree->SetBranchAddress("energy", &energy);
  tree->SetBranchAddress("dpa", &dpa);

  TGraph* gr = new TGraph();
  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
	gr->SetPoint(i, energy, dpa);
  }
  
  spl.push_back(new TSpline3(fileName.c_str(), gr));
  
}

double DamageFactorBase::Interpolation(int pdg, double energy) {
  // upper energy and lower energy [MeV]
  // neutron:
  float rNeutron [2] = {1.025e-10, 8995.};
  // electron:
  float rElectron[2] = {0.3, 200.};
  // pion-:
  float rPion    [2] = {15., 9005.};
  // proton (1):
  float rProton1 [2] = {15., 9005.};
  // proton (2):
  float rProton2 [2] = {0.001, 15.};
  
  double factor;

  switch (pdg) {
    // neutron
    case 2112:
      if ( energy>=rNeutron[0] && energy<=rNeutron[1] )
	    factor = spl[0]->Eval(energy);
	  else {
        //cout << "Warning: Neutron energy is out of range! Neutron energy: " << energy
        //     << " Factor: 0.0 " << endl;
        return 0.;
	  }
	  break;
    // electron:
	case 11:
	  if ( energy>=rElectron[0] && energy<=rElectron[1] )
	    factor = spl[1]->Eval(energy);
	  else {
	    cout << "Warning: Electron energy is out of range! Electron energy: " << energy
             << "Factor: 0.0 " << endl;
	    return 0.;
	  }
	  break;
    // pion-:
	case -211:
	  if ( energy>=rPion[0] && energy<=rPion[1] )
	    factor = spl[2]->Eval(energy);
	  else {
	    //cout << "Warning: Pion energy is out of range! Poin energy: " << energy
        //     << " Factor: 0.0 " << endl;
	    return 0.;
	  }
	  break;
	// proton:
	case 2212:
	  if ( energy>=rProton1[0] && energy<=rProton1[1] )
	    factor = spl[3]->Eval(energy);
	  else if ( energy>=rProton2[0] && energy<rProton2[1] )
	    factor = spl[4]->Eval(energy);
	  else {
	    //cout << "Warning: Proton energy is out of range! Proton energy: " << energy
        //     << " Factor: 0.0 " << endl;
	    return 0.; 
	  }
	  break;
	// else
	default:
      //cout << "There is no data on this pdgcode: " << pdg << endl;
      return 0.;  break;
  }
  
  if (factor<0) {
    //cout << "error: factor is less than zero!" << endl;
    //cout << "  pdgcode: " << pdg << " , energy: " << energy << " , factor: " << spl[0]->Eval(energy) << endl;
    return 1.;
  }

  return factor;
}

