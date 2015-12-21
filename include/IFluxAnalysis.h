//

#ifndef IFluxAnalysis_HH
#define IFluxAnalysis_HH

#include <string>
#include <vector>
#include <map>
#include <TH1F.h>
#include <TVector3.h>
#include "DamageFactorBase.h"
#include "IAnalysisBase.h"

typedef std::map<std::string, int> AnalysisMap;

class IFluxAnalysis : public IAnalysisBase {
  //
  public:
    IFluxAnalysis(std::string);
    ~IFluxAnalysis();
    
    // set bin size
    void SetBin(double* bin);

    // set cross section [cm2]
    void SetArea(double A) { fArea = A; }
    
    // set the condition for particle position
    void SetPosCondition(TVector3& min, TVector3& max);
    void SetPosCondition(double rmin, double rmax);

    // get flux histogram of this particle
    TH1F* GetFlux(int);

	// plot the flux with each particle
    void  Plot();

    // print neutron fluence
    void  Print();

  protected:
    // convert to 1 MeV equivalent neutron
    double* GetEqNeutron();

  private:
    double* fBin;
    int     fNbin;
    double  fArea;
    DamageFactorBase*      fFactor;
    std::vector<TVector3*> buffPos;
    std::vector<double>    buffEnergy;
    std::vector<int>       buffPdgcode;

};

#endif
