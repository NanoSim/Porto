#include <QtCore>
#include "remarcReader.h"

namespace IO
{

RemarcReader :: RemarcReader(std::string const &remarcFile)
  : remarcFile(remarcFile)
{}

RemarcReader :: ~RemarcReader()
{}

void RemarcReader :: write()
{
  QString filename = QString::fromStdString(remarcFile);
  QFileInfo info(QDir::current(), filename);
  QFile file(info.absoluteFilePath());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "Unable to read file " << info.absoluteFilePath() << ": " << file.errorString();
    return;
  }

  QTextStream stream(&file);
  stream << T_min << "\t#T_min (K)" << endl;
  stream << T_max << "\t#T_max (K)" << endl;
  stream << dt    << "\t#Delta_T (K)" << endl;
  stream << bh    << "\t#estimated uncertainty in barrier height (electron volts)" << endl;
  stream << ae    << "\t#estimated uncertainty in adsorption energy (electron volts)" << endl;
  stream << gp    << "\t#Gas pressure (atm)" << endl;
  stream << (frc ? 'Y' : 'N') << "\t#Should frequencies be used to calculate rate constants? (Y or N)" << endl;
  stream << (fcs ? 'Y' : 'N') << "\t#Should frequencies be used for clean surface? (Y or N)" << endl;
  stream << (qvp ? 'Q' : 'C') << "\t#Quantum (Q) or classical (C) vibrational partition functions for rate constant calcuations?" << endl;
  //stream.setRealNumberNotation(QTextStream::ScientificNotation);
  stream.setRealNumberNotation(QTextStream::SmartNotation);
  stream << prc << "\t#Prefactor for rate constant calculations (s-1) if frequencies are not used" << endl;
  stream << pdr << "\t#Prefactor for desorption rate constant calculations (s-1) if frequencies are not used" << endl;
}
 
void RemarcReader :: read()
{
  QString filename = QString::fromStdString(remarcFile);
  QFileInfo info(QDir::current(), filename);
  QFile file(info.absoluteFilePath());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Unable to read file " << info.absoluteFilePath() << ": " << file.errorString();
    return;
  }

  QString comment;
  QTextStream stream(&file);
  stream >> T_min; comment = stream.readLine();
  stream >> T_max; comment = stream.readLine();
  stream >> dt; comment = stream.readLine();
  stream >> bh; comment = stream.readLine();
  stream >> ae; comment = stream.readLine();
  stream >> gp; comment = stream.readLine();  // Gas pressure (atm)
  QChar pred;
  stream >> pred; comment = stream.readLine();  
  frc = (pred == 'Y');   // Should frequencies be used to calculate rate constants?
  stream >> pred; comment = stream.readLine();  
  fcs = (pred == 'Y');  // Should frequencies be used for clean surface?
  stream >> pred; comment = stream.readLine();
  qvp = (pred == 'Q'); //  Quantum (true) or classical (false) vibrational partition
  stream >> prc; comment = stream.readLine();  // Prefactor for rate constant calculations (s-1) if frequencies are not used
  stream >> pdr; comment = stream.readLine();  // Prefactor for desorption rate constant calculations (s-1) if frequencies are not use
    
}

}
