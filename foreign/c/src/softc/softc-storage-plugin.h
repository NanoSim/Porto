#ifndef SOFTC_STORAGE_PLUGIN_H
#define SOFTC_STORAGE_PLUGIN_H

#include "softc-macros.h"
#include "softc-datamodel.h"

SOFTC_BEGIN_DECLS

#define SOFTC_CAPABILITY_NONE   0
#define SOFTC_CAPABILITY_READ   0x01
#define SOFTC_CAPABILITY_WRITE  0x02

#define SOFTC_STATUS_OK       0
#define SOFTC_STATUS_FAILURE  1

int softc_plugin_identify (char* name, int maxlen);
int softc_plugin_capabilities ();
int softc_plugin_load (softc_datamodel_t* datamodel, const char* uri, const char* options);
int softc_plugin_save (const softc_datamodel_t* datamodel, const char* uri, const char* options);

SOFTC_END_DECLS

#endif 

