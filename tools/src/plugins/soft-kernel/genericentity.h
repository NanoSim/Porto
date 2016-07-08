#ifndef GENERICENTITY_H_DEF
#define GENERICENTITY_H_DEF

#include <QScopedPointer>
#include <QVariant>
#include <Soft>
#include <string>
#include <vector>

SOFT_BEGIN_NAMESPACE

class GenericEntity : public IEntity
{
public:
  SOFT_ENTITY_METADATA("Generic", "SOFT5", "0.1-SNAPSHOT1")
  GenericEntity();  
  explicit GenericEntity(std::string const &id);
  explicit GenericEntity(const IEntity *other);
  virtual ~GenericEntity();
  void setSchema(std::string const &json);
  void save(IDataModel *) const override;
  void load(IDataModel const *) override;
  static IEntity* create (const std::string &uuid = std::string());
  
  std::vector<std::string> dimensions() const override;
  int getDimensionSize(std::string const &) const override;
  QVariant property(QString const&) const;
  void setProperty(QString const&, QVariant const&);
  void debug();
  
private:  
  struct Private;
  class QScopedPointer<Private> d;
};

SOFT_END_NAMESPACE

#endif // GENERICENTITY_H_DEF
