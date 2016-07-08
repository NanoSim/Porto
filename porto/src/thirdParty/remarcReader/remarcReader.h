#ifndef REMARCREADER_H_DEF
#define REMARCREADER_H_DEF

namespace IO
{
  class RemarcReader
  {
    const std::string remarcFile;
  public:
    RemarcReader(std::string const &);
    ~RemarcReader();
    void read();
    void write();

    double T_min; // Lower temperature bound
    double T_max; // Upper temperature bound
    double dt;  // delta T
    double bh;  // estimated uncertainty in barrier height (electron volts)
    double ae;  // estimated uncertainty in adsorption energy (electron volts) 
    double gp;  // Gas pressure (atm)
    bool   frc; // Should frequencies be used to calculate rate constants?
    bool   fcs; // Should frequencies be used for clean surface?
    bool   qvp; // Quantum (true) or classical (false) vibrational partition
                // functions for rate constant calcuations?
    double prc; // Prefactor for rate constant calculations (s-1) if frequencies are not used
    double pdr; // Prefactor for desorption rate constant calculations (s-1) if frequencies are not used
      
  };  
}

#endif // REMARCREADER_H_DEF
