#ifndef DAMAGEFACTORBASE__HH
#define DAMAGEFACTORBASE__HH

#include <string>
#include <TSpline.h>

class DamageFactorBase {
  
  public:
    DamageFactorBase(const int pdg);
	DamageFactorBase() {}
	~DamageFactorBase() {}
	void   SetParticle  (const int pdg);
    void   OpenFile     (std::string fileName);
    double Interpolation(int pdg, double energy);

  private:
    int pdgcode;

  protected:
    std::vector<TSpline3*> spl;
};

#endif
