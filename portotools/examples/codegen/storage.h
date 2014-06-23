#ifndef STORAGE_H_DEF
#define STORAGE_H_DEF

#include <memory>
#include <utility>
#include <map>
#include <string>

class StorageCreator;
class Storage
{
   Storage(){}
public:   
   Storage(Storage const &other);
   Storage(StorageCreator const &c);
   std::string driver() const;
   std::string option(std::string && k);
private:
   std::string const drv;
   std::map<std::string, std::string> opts;
};

class StorageCreator
{
   friend class Storage;
   std::map<std::string, std::string> opts;
   std::string driver;
public:
   StorageCreator(std::string const& drv);
   StorageCreator& addOption(std::string && k, std::string && v);
   static StorageCreator connect(std::string const &drv);
};

#endif // STORAGE_H_DEF
