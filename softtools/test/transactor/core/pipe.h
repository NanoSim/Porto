#ifndef PIPE_H_DEF
#define PIPE_H_DEF

#include <string>
#include "entity.h"

class Pipe : public Entity
{
public:
  explicit Pipe(const char *ref);
  virtual ~Pipe();
  virtual void store (soft::QBson &bson) const;
  virtual void read (soft::QBson const &bson);

  double length() const;
  double diameter() const;
  double angle() const;
  std::string name() const;

  void setLength (const double &value);
  void setDiameter (const double &value);
  void setAngle (const double &angle);
  void setName (const std::string &name);
  
private: 
  double d_length;
  double d_diameter;
  double d_angle;
  std::string d_text;
};

#endif // PIPE_H_DEF
