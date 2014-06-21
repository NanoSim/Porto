#include <QString>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QTextStream>
#include <QVector>

#include "hdf5reader.h"

namespace {
   static QString const PROCESSOR("processor");
   static QString const VECTORDATA("vectorData");
   static QString const SCALARDATA("scalarData");
   
   void reportError( QString const & message, int lineNumber)
   {
      qDebug() << "Error in line: " << lineNumber << " : " << message;
   }

   QVector<double> stringVectorToDoubleVector(QStringList const &stringVector, bool *ok)
   {
      QVector<double> retval;
      foreach(QString const &str, stringVector) {
	 double d = str.toDouble(ok);
	 if( !ok ) return QVector<double>();
	 
	 retval << d;
      }
      
      return retval;
   }

} // namespace

HDF5Reader :: HDF5Reader(QString const &fname)
  : filename(fname)
{}

HDF5Reader :: ~HDF5Reader()
{}


/*!  
  Returns a pair of strings containing the keyword and value of a \a
  buffer defined as <keyword>: <value>. 
 */
QPair<QString,QString> HDF5Reader :: keywordValuePair(QString const & buffer) const
{
  QRegExp re("(\\w+)\\:{1}\\s*(\\w+)");
  int pos = re.indexIn(buffer, 0);
  if( pos > -1)
    return qMakePair(re.cap(1).trimmed(), re.cap(2).trimmed());

  return QPair<QString,QString>();
}

QString HDF5Reader :: getValue(QString const & buffer, QString const &keyword) const
{
  if( !buffer.isEmpty() && buffer.at(0) == QChar('#') ) 
  {
    QStringList fields = buffer.mid(1).split("||");
    foreach(QString const &field, fields) 
    {
      auto kvPair = keywordValuePair(field.trimmed());
      if( kvPair.first == keyword ) 
      {
        return kvPair.second;	      
      }    
    }
    return QString();
  }
}

/*!
  Returns the value of the keyword 'processor' as integer if the input
  \a buffer is correctly formatted.
 */
int HDF5Reader :: getProcessor(QString const & buffer) const
{ 
  auto procNrStr = getValue(buffer, PROCESSOR);
  if( !procNrStr.isEmpty() ) {
    bool ok;
    int procNr = procNrStr.toInt(&ok, 10);
    if( ok ) return procNr;
  }
  
  return -1;
}

QString HDF5Reader :: getVectorDataName(QString const & buffer) const
{
  return getValue(buffer, VECTORDATA);
}

QString HDF5Reader :: getScalarDataName(QString const & buffer) const
{
  return getValue(buffer, SCALARDATA);
}

HDF5 HDF5Reader :: readHDF5() const
{
  QFile in(filename);
  if(!in.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream(stderr) << "Failed to open '" << filename << "': "<< in.errorString() << endl;
    return HDF5();
  }

  // Read header fields
  QString const procline = (!in.atEnd() ? in.readLine() : QString());
  QString const header   = (!in.atEnd() ? in.readLine() : QString());

  HDF5 HDF5;
  HDF5.processor = getProcessor(procline);
  HDF5.vectorDataName = getVectorDataName(header);
  HDF5.scalarDataName = getScalarDataName(header);

  int lineno = 0;
  while(!in.atEnd()) {
    QString const line = in.readLine();
    QStringList fields = line.split("||", QString::SkipEmptyParts);
    Q_ASSERT(fields.count() == 4);
    if(fields.count() != 4) {
      reportError("Error in file", lineno);
      break;
    }
    
    auto indexTime = fields[0].split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if(indexTime.count() != 2) {
      reportError("Error in file", lineno);
      break;
    }

    auto vectordata = fields[1].split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if( (vectordata.count() != 0) == (vectordata.count() != 3) ) {
      reportError("Error in vector data", lineno);
      break;
    }

    auto scalardata = fields[2].split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if( scalardata.count() != 0 ) {
      reportError("Error in scalar data", lineno);
      break;
    }

    auto positiondata = fields[3].split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if( (positiondata.count() != 3 ) == (positiondata.count() != 0) ) {
      reportError("Error in positional data", lineno);
      break;
    }

    bool ok;
    HDF5.index.append(indexTime.at(0).toInt(&ok, 10));
    if( !ok ) {
      reportError("Illegal index format", lineno);
    }

    HDF5.time.append(indexTime.at(1).toDouble(&ok));
    if( !ok) {
      reportError("Illegal time format", lineno);
    }

    HDF5.vectorData.append(stringVectorToDoubleVector(vectordata, &ok));
    if (!ok) {
      reportError("Illegal vector format", lineno);
    }

    if( !scalardata.empty() ) {
      HDF5.scalarData.append(scalardata.at(0).toDouble(&ok));
      if (!ok) {
	reportError("Illegal scalar format", lineno);
      }
    }

    HDF5.position.append(stringVectorToDoubleVector(positiondata, &ok));

    lineno++;
  }
  return HDF5;
}
