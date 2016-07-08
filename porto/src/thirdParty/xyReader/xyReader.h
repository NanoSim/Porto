#ifndef XYREADER_H_DEF
#define XYREADER_H_DEF

#include <list>
#include <vector>
#include <string>

namespace IO
{
  class XYReader
  {
    const std::string xyfile;
  public:
    XYReader(std::string const &);
    ~XYReader();
    void read();
    void write();

    std::list<std::string> labels;
    std::vector<std::vector<double>> data;
  };
}
#endif // XYREADER_H_DEF
