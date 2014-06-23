#include "storage.h"

Storage :: Storage(StorageCreator const & c)
   : drv (c.driver)
   , opts (c.opts)
{}

Storage :: Storage(Storage const &other)
   : drv (other.drv)
   , opts (other.opts)
{}

std::string Storage :: driver() const
{
   return drv;
}

std::string Storage :: option(std::string && k)
{
   return opts[k];
}

StorageCreator :: StorageCreator(std::string const &drv)
   : driver (drv)
{
}

StorageCreator& StorageCreator :: addOption(std::string && k, std::string && v)
{
   opts[k] = v;
   return *this;
}

StorageCreator StorageCreator :: connect(std::string const &drv)
{
   StorageCreator creator(drv);
   return creator;
}

