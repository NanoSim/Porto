#ifndef TRIPLETSTORE_H_DEF
#define TRIPLETSTORE_H_DEF

#include "softns.h"
#include <string>
#include <list>

SOFT_BEGIN_NAMESPACE
class TripletStore
{
  class Private;
  Private *d;
public:
  TripletStore();
  virtual ~TripletStore();

  void clear();
  void addTriplet(std::string const &subj,
      std::string const &pred,
      std::string const &obj);
  // TODO: Refine what happens if predicate is not defined. Nothing returned? Hm.
  std::list<std::string> findTriplets(const std::string &subj,
              const std::string &pred = std::string()) const;

  int size() const;
  std::string toRdf() const;

  std::string toCSV() const;
  void fromCSV(std::string const &csv);

};

SOFT_END_NAMESPACE
#endif /* TRIPLETSTORE_H_DEF */
