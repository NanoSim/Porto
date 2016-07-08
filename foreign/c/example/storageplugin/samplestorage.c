#include <stdio.h>
#include <softc/softc-storage-plugin.h>

#define MYAPI(t) extern t

MYAPI(int) softc_plugin_identify( char* name, int maxlen )
{
   return SOFTC_STATUS_FAILURE;
}

MYAPI(int) softc_plugin_capabilities()
{
   return SOFTC_CAPABILITY_NONE;
}

MYAPI(int) softc_plugin_load( softc_datamodel_t* datamodel, const char* uri, const char* options )
{
   return SOFTC_STATUS_FAILURE;
}

MYAPI(int) softc_plugin_save( const softc_datamodel_t* datamodel, const char* uri, const char* options )
{
   return SOFTC_STATUS_FAILURE;
}
