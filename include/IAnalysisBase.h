// Base class for analysis of output from SimPHITS
// Author: Y.Yang
// 

#ifndef IAnalysisBase_HH
#define IAnalysisBase_HH

#include <vector>
#include <string>
#include <map>
#include <TVector3.h>
#include <TParticle.h>

typedef std::map<std::string, int> AnalysisMap; 

class IAnalysisBase {
  // 
  public:
    IAnalysisBase();
    IAnalysisBase(std::string, std::string);
    ~IAnalysisBase();
    
    // open a flux or deposit file,
    // and fill the data into vector
    void Open(std::string, std::string);
    
    // check what kinds of particles in this file
    virtual void CheckParticle();

    // get the pdgcode of particles
    virtual AnalysisMap GetParticle();

    // set beam intensity
    void SetIntensity(double I) { fIntensity = I; }

    // set event number
    void SetEvent(const int evt) { fEvent = evt; }

    // return the position of current particle
    std::vector<TVector3*> GetPosition() { return fPos; }

    // return the pdgcode
    std::vector<int>       GetPdgCode()  { return fPdgCode; }

    // return the energy 
    std::vector<double>    GetEnergy()   { return fEnergy; }

    // return energy deposit
    std::vector<double>    GetEnergyDeposit() { return fDeposit; }

  protected:
    // load flux file's data
    void LoadFluxFile(std::string);

    // load energy deposit file's data
    void LoadDepositFile(std::string);

    double fIntensity;
    int    fEvent;
    std::vector<TVector3*> fPos;
    std::vector<int>       fPdgCode;
    std::vector<double>    fEnergy;
    std::vector<double>    fDeposit;
    std::vector<int>       fGeom;

  private:
    std::string fFileName;
    TParticle* fParticle;
};

#endif
