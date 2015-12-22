#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include "IAnalysisBase.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::map;

IAnalysisBase::IAnalysisBase()
    : fIntensity(2.5e+12), fEvent(1000*1000)
{
  //fPos    .clear();
  //fPdgCode.clear();
  //fEnergy .clear();
  fParticle = new TParticle();
}

IAnalysisBase::IAnalysisBase(string file, string type)
    : fFileName(file), fIntensity(2.5e+12), fEvent(1000*1000)
{
  Open(file, type);

  //fPos    .clear();
  //fPdgCode.clear();
  //fEnergy .clear();
  
  fParticle = new TParticle();
}

IAnalysisBase::~IAnalysisBase()
{}

void IAnalysisBase::Open(string file, string type)
{
  if ( type=="flux" || type=="f" ) {
    LoadFluxFile(file);
  }
  else if ( type=="deposit" || type=="d" ) {
    LoadDepositFile(file);
  }
}

void IAnalysisBase::LoadFluxFile(string xfile)
{
  TFile* file = new TFile(xfile.c_str());
  TTree* tree = (TTree*)file->Get("RooTracker");

  double pos[4];
  double p4 [4];
  int    pdgcode;
  
  // read data from root file
  tree->SetBranchAddress("EvtPDG", &pdgcode);
  tree->SetBranchAddress( "EvtV4", pos);
  tree->SetBranchAddress( "EvtP4", p4);

  // fill data into vector
  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    fPdgCode.push_back(pdgcode);
    fPos    .push_back(new TVector3(pos[0],pos[1],pos[2]));
    fEnergy .push_back(p4[3]);
  }
}

void IAnalysisBase::LoadDepositFile(string xfile) {
  TFile* file = new TFile(xfile.c_str());
  TTree* tree = (TTree*)file->Get("RooTracker");

  double prepos [4];
  double postpos[4];
  double edep;
  int    geo;
  int    pdg;
  
  // read data from root tree file
  tree->SetBranchAddress("EvtPDG", &pdg   );
  tree->SetBranchAddress("EvtGeo", &geo   );
  tree->SetBranchAddress("EvtDep", &edep  );
  tree->SetBranchAddress("EvtV4" , prepos );
  tree->SetBranchAddress("EvtV4C", postpos);
  
  // fill data into vector
  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    fPdgCode.push_back(pdg);
    fGeom.push_back(geo);
    fDeposit.push_back(edep);
    fPos.push_back(new TVector3(prepos[0] + (postpos[0]-prepos[0])/2,
                                prepos[1] + (postpos[1]-prepos[1])/2,
                                prepos[2] + (postpos[2]-prepos[2])/2));
  }
}

void IAnalysisBase::CheckParticle()
{
  vector<int> bufflist;
  int counter;
  
  for (vector<int>::size_type i=0; i<fPdgCode.size(); i++) {
	counter = 0;
	// check buff, if buffer size = 0, input the first data into it
	if (bufflist.size()==0)
	  bufflist.push_back(fPdgCode[i]);
    // if not, check the buffer list
    else {
      for (vector<int>::size_type j=0; j<bufflist.size(); j++) {
        if (fPdgCode[i]==bufflist[j]) 
          counter ++;
	  }
	  if (counter==0)
	    bufflist.push_back(fPdgCode[i]);
	}
  }

  cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;
  cout << "   Summary of particles in this file      " << endl; 
  cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;

  for (vector<int>::size_type i=0; i<bufflist.size(); i++) {
    fParticle->SetPdgCode(bufflist[i]);
    cout << " " << i+1 << ". PDG code: " << bufflist[i] << " , Particle: " << fParticle->GetName() << endl;
  }

  cout << "\n";
}

AnalysisMap IAnalysisBase::GetParticle()
{
  vector<int> bufflist;
  AnalysisMap amap;
  int counter;
  
  for (vector<int>::size_type i=0; i<fPdgCode.size(); i++) {
	counter = 0;
	// check buff, if buffer size = 0, input the first data into it
	if (bufflist.size()==0)
	  bufflist.push_back(fPdgCode[i]);
    // if not, check the buffer list
    else {
      for (vector<int>::size_type j=0; j<bufflist.size(); j++) {
        if (fPdgCode[i]==bufflist[j]) 
          counter ++;
	  }
	  if (counter==0)
	    bufflist.push_back(fPdgCode[i]);
	}
  }

  for (vector<int>::size_type i=0; i<bufflist.size(); i++) {
    fParticle->SetPdgCode(bufflist[i]);
    amap.insert( AnalysisMap::value_type( string(fParticle->GetName()), bufflist[i] ));
  }

  return amap;
}
