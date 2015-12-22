// to get and interpolate the damage factor of silicon
//

#ifndef IDamageFactor_HH
#define IDamageFactor_HH

#include <string>
#include <vector>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

class IDamageFactor {
  ///
  public:
    IDamageFactor();
    ~IDamageFactor();
  
    // set damage factor data's path
    void   SetPath(std::string);

    // load all files for damage factor
    void   Load();

    // interpolate energy on damage factor to get factor
    double Interpolation(int, double);

  protected:
    // open factor data's file
    void Open(int, std::string);
    
	// open proton data's file
    void Open(int, std::string, std::string);

  ///
  private:
    std::string fPath;
    gsl_interp_accel* fAcc;
    gsl_interp_accel* fAcc2;
    gsl_interp_accel* fAcc3;
    gsl_spline* fNeutron;
    gsl_spline* fElectron;
    gsl_spline* fPion;
};

#endif
