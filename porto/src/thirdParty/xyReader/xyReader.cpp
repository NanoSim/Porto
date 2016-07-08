#include <QtCore>
#include "xyReader.h"

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

namespace IO {
  
XYReader :: XYReader(std::string const &xyfile)
  : xyfile(xyfile)
{
}

XYReader :: ~XYReader()
{}

void XYReader :: read()
{
  QString filename = QString::fromStdString(xyfile);
  QFileInfo info(QDir::current(), filename);
  QFile file(info.absoluteFilePath());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Unable to read file " << info.absoluteFilePath() << ": " << file.errorString();
    return;
  }

  NOT_IMPLEMENTED
}

void XYReader :: write()
{
  QString filename = QString::fromStdString(xyfile);
  QFileInfo info(QDir::current(), filename);
  QFile file(info.absoluteFilePath());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "Unable to read file " << info.absoluteFilePath() << ": " << file.errorString();
    return;
  }

  NOT_IMPLEMENTED
}

}
