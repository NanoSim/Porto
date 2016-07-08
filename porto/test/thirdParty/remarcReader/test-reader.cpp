#include <gtest/gtest.h>
#include <remarcReader.h>

TEST(RemarcReaderTest, Write)
{
  IO::RemarcReader config("remarc.out");
  config.T_min = 20.0;
  config.T_max = 200.0;
  config.dt = 10.0;  // delta T
  config.bh = 32.0;  // estimated uncertainty in barrier height (electron volts)
  config.ae = 12.2;  // estimated uncertainty in adsorption energy (electron volts) 
  config.gp = 1.0;  // Gas pressure (atm)
  config.frc = true; // Should frequencies be used to calculate rate constants?
  config.fcs = false; // Should frequencies be used for clean surface?
  config.qvp = true; // Quantum (true) or classical (false) vibrational partition
                // functions for rate constant calcuations?
  config.prc = 1.0E20; // Prefactor for rate constant calculations (s-1) if frequencies are not used
  config.pdr = 1.0E12; // Prefactor for desorption rate constant calculations (s-1) if frequencies are not use
  config.write();
}

TEST(RemarcReaderTest, Read)
{
  IO::RemarcReader reader("remarc.out");
  reader.read();
  ASSERT_DOUBLE_EQ(reader.T_min, 20.0);
  ASSERT_DOUBLE_EQ(reader.T_max, 200.0);
  ASSERT_DOUBLE_EQ(reader.dt, 10.0);
  ASSERT_DOUBLE_EQ(reader.bh, 32.0);
  ASSERT_DOUBLE_EQ(reader.ae, 12.2);
  ASSERT_DOUBLE_EQ(reader.gp, 1.0);
  ASSERT_TRUE(reader.frc);
  ASSERT_FALSE(reader.fcs);
  ASSERT_TRUE(reader.qvp);
  ASSERT_DOUBLE_EQ(reader.prc,1.0E20);
  ASSERT_DOUBLE_EQ(reader.pdr, 1.0E12);
}

