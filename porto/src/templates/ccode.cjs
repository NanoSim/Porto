@{
    var capi = require('soft.forge.capi');
    entity = soft.model.name.toLowerCase();
    ENTITY = soft.model.name.toUpperCase();
    
    constructorArgs = (function(){
	var args = [];	
	if (soft.model.dimensions != undefined) {   
	    soft.model.dimensions.forEach(function (d){		
		args.push("size_t " + d.name);		
	    });
	}
	return args;
    })();

    dimensions = (function(){
      var d = [];
      if (soft.model.dimensions != undefined) {
	soft.model.dimensions.forEach(function(dim){
	  d.push(dim.name);
	});
      }
      return d;
    })();
    attributes = (function(){
	var attr = [];
	soft.model.properties.forEach(function (entry){
	    var o = {};
	    o.name = entry.name;
	    o.type = capi.type_to_c(entry.type);
	    o.rank = (entry.dims != undefined ? entry.dims.length : 0);
	    o.desc = (entry.description != undefined ? "/* " + entry.description + " */" : "");
	    o.dims = entry.dims;
	    attr.push(o);
	});
	return attr;
    })();

    dataModelGetFunction = function(entry) {
	var getFunction = "softc_datamodel_get_";
	switch(entry.type) {
	case 'float':
	case 'double':
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += entry.type;
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'int32_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'int32';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'int8_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'int8';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'uint8_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'uint8';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'int16_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'int16';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'uint16_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'uint16';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'int64_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'int64';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'uint64_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'uint64';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'bool': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { getFunction += "array_"; }
	    getFunction += 'bool';
	    if (entry.rank > 1) {
		getFunction += "_" + entry.rank + "d";
	    }
	    break;

        case 'softc_string_s':
            if (entry.rank > 2) 
              throw("not implemented");
            else if (entry.rank == 1)
	      getFunction += 'string_list';
	    else
	      getFunction += 'string';
	    break;
        case 'softc_bytearray_s':
	    getFunction += 'blob';
	    break;	
	default:
	    throw ("Unimplemented type: " + entry.type);
	    getFunction += "undefined";
	};
	return getFunction;
    };
    
    dataModelAppendFunction = function(entry) {
	var appendFunction = "softc_datamodel_append_";
	switch (entry.type) {
	case 'float':
	case 'double':
	    if (entry.rank > 0) { appendFunction += "array_"; }
	    appendFunction += entry.type;
	    if (entry.rank > 1) {
		appendFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'int32_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { appendFunction += "array_"; }
	    appendFunction += 'int32';
	    if (entry.rank > 1) {
		appendFunction += "_" + entry.rank + "d";
	    }
	    break;
        case 'softc_string_s':
            if (entry.rank > 2) 
              throw("not implemented");
            else if (entry.rank == 1)
	      appendFunction += 'string_list';
	    else
	      appendFunction += 'string';
            break;
        case 'softc_bytearray_s':
	    appendFunction += 'blob';
	    break;	
	default:
	    appendFunction += "undefined";
	};
	return appendFunction;
    };
    
    setDataBlock = (function(){
	setList = [];
	attributes.forEach(function (entry) {
	    var str = dataModelAppendFunction(entry);
	    if (entry.rank == 0) {
	      str += " (data_model, \"" + entry.name + "\", self->props." + entry.name;
	    } else {    
	      str += " (data_model, \"" + entry.name + "\", (const " + entry.type + capi.dims_to_ptr(entry.rank) + ")self->props." + entry.name;
	    }	      
	    if (entry.rank > 0 && entry.type != "softc_string_s") {
          str += ", " + entry.dims.join(",");
	    }
	    str += ");";
	    setList.push(str);
	});
	return setList;
    })();

    /* List of dimensions for each property */
    propDimsList = (function(){
	var pdl = [];
	attributes.forEach(function (entry) {
	    var idx = 0;
	    if (entry.rank > 0) {
		entry.dims.forEach(function (dim){
		    pdl.push("size_t " + entry.name + "_" + idx.toString() + ";");
		    idx++;
		});
	    }
	});
	return pdl;
    })();
    
    getDataBlock = (function(){
	getList = [];
	attributes.forEach(function (entry) {
	    var str = dataModelGetFunction(entry);
	    str += " (data_model, \"" + entry.name + "\", &self->props." + entry.name;
	    if (entry.rank > 0) {
			var idx = 0;
			var dl = [];
			if (entry.type != "softc_string_s") {
				entry.dims.forEach(function(dim){
					dl.push("&" + entry.name + "_" + idx.toString());
					idx++;
				});
				str += ", " + dl.join(",");
			}
	    }
	    str += ");";
	    getList.push(str);
	});
	return getList;
    })();

    quote = function(list) {
      var ql = [];
      list.forEach(function(list){
	ql.push("\"" + list + "\"");
      });
      return ql;
    };
  
    paren = function(list) {
	var plist = [];
	list.forEach(function(entry){
	    plist.push("("+entry+")");
	});
	return plist;
    };

    attrInitList = (function(){
	var as = [];
	attributes.forEach(function(entry){
	    if (entry.rank == 0) {
		as.push("0");
	    } else {
	      as.push("(" + entry.type + capi.dims_to_ptr(entry.rank) + ")softc_allocatable_data(alloc->" + entry.name + ")");
	    }
	});
	return as;
    })();

    getDataFunctions = (function(){
	var as = [];
	attributes.forEach(function(entry){
            if (entry.rank > 0) {
		as.push(entry.type + capi.dims_to_ptr(entry.rank) + " " + entity + "_get_" + entry.name + " (" + entity + "_s *self)\n{");
		as.push("  return softc_allocatable_data(self->allocs->" + entry.name + ");");
		as.push("}");
	    }
	});
	as.push("\n");
	return as;
    })();
    
    zeroAttrInitList = (function(){
	var as = [];
	attributes.forEach(function(entry){
	    if (entry.rank == 0) {
		as.push("0");
	    } else {
		as.push("NULL");
	    }
	});
	return as;
    })();

    attrDeclList = (function(){
	var as = [];
	attributes.forEach(function(entry){
	    as.push(entry.type + " " + capi.dims_to_ptr(entry.rank) + entry.name + ";" + entry.desc);
	});
	return as;
    })();

    dimsList = (function(){
	var ds = [];
	if (soft.model.dimensions != undefined) {
	    soft.model.dimensions.forEach(function(entry){
		ds.push(entry.name);
	    });
	}
	return ds;	
    })();

    zeroDimsList = (function(){
	var ds = [];
	if (soft.model.dimensions != undefined) {
	    soft.model.dimensions.forEach(function(entry){
		ds.push("0");
	    });
	}
	return ds;
    })();
    
    dimsDeclList = (function(){
	var ds = [];
	if (soft.model.dimensions != undefined) {
	    soft.model.dimensions.forEach(function(entry){
		ds.push("size_t " + entry.name + ";");
	    });
	}
	return ds;
    })();

    freeProperties = (function(){
	var as = [];
	attributes.forEach(function(entry){
	    if (entry.rank > 0) {
		as.push("softc_allocatable_free(" + entry.type + ", self->allocs->" + entry.name+ ")");
	    }
	});
	return as;
    })();

    dimsSelfDeclList = (function(){
	var ds = [];
	if (soft.model.dimensions != undefined) {
	    soft.model.dimensions.forEach(function(entry){
		ds.push("size_t " + entry.name + " = self->dims." + entry.name + ";");
	    });
	}
	return ds;
    })();

    allocDeclList = (function(){
	var as = [];
	attributes.forEach(function(entry){
	 if (entry.rank > 0) {
	    as.push("softc_allocatable_s *" + entry.name + ";");
	    }
	});
	return as;
    })();


    initDataAllocatables = (function(){
    	var as = [];
      attributes.forEach(function(entry){
	if (entry.rank > 0) {
	  as.push("alloc->" + entry.name + "=" + "softc_allocatable_createv(" + entry.type + ", " + entry.rank + ", " + paren(entry.dims).join(",") + ");");
	}
      });
      
      return as;
    })();

  getDimensionsBody = (function(){
    var as  = [];

    if (soft.model.dimensions == undefined) {
      as.push("  *size = 0;");
      as.push("  return NULL;");      
    } else {
      var dimlength = soft.model.dimensions.length.toString();
      as.push(entity + "_s *self = (" + entity + "_s*) ptr;");
      as.push("const size_t ndims = " + dimlength + ";");
      as.push("size_t i;");
      as.push("char **dl = malloc(sizeof (*dl) * ndims);");
      soft.model.dimensions.forEach(function(dim, idx){
	as.push("dl["+idx+"] = malloc(sizeof(**dl) * " + (dim.name.length+1).toString() + ");");
	as.push("strcpy(dl["+idx+"], \"" + dim.name + "\");");
      });
      as.push("*size = " + dimlength.toString() + ";");
      as.push("return (const char**)dl;");
    }
    return as;
  })();

  getDimensionSizeBody = (function(){
    var txt = [];
    if (soft.model.dimensions != undefined) {   
      soft.model.dimensions.forEach(function (dim){		
	txt.push("if (strcmp(label, \"" + dim.name + "\") == 0) return self->dims."+dim.name+";");
      });
    }
    return txt;
  })();

  (function(){return soft.model.dimensions == undefined ? "0" : soft.model.dimensions.length.toString(); })();
    undefined;
}/* This file is autgenerated. Do not edit! */


#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <softc/softc.h>
#include <softc/softc-entity.h>
#include <softc/softc-datamodel.h>
#include <softc/softc-allocatable.h>    

#include "@{entity}.h"

static const char @{ENTITY}_META_TYPE[] = "@soft.model.name:@soft.model.version:@soft.model.namespace";
static const char @{ENTITY}_META_NAME[] = "@soft.model.name";
static const char @{ENTITY}_META_VERSION[] = "@soft.model.version";
static const char @{ENTITY}_META_NAMESPACE[] = "@soft.model.namespace";
static const char *static_dims[] = {@{quote(dimensions).join(',');}};

struct _@{entity}_allocatable_s
{
  @{allocDeclList.join("\n  ");}
};

struct _@{entity}_s {
  const struct softc_entity_vtable_ *vtable_;
  const char *id;
  @{entity}_dimensions_s dims;
  @{entity}_properties_s props;
  @{entity}_allocatable_s *allocs;
};

static void store (const softc_entity_t *ptr, softc_datamodel_t *data_model)
{
  const @{entity}_s *self = (const @{entity}_s*)ptr;
  @{dimsSelfDeclList.join('\n  ')}
  @{setDataBlock.join('\n  ')}
}

static void load (softc_entity_t *ptr, const softc_datamodel_t *data_model)
{
  @{entity}_s *self = (@{entity}_s*)ptr;
  @{propDimsList.join('\n  ')}
  @{getDataBlock.join('\n  ')}    
}

static const char ** get_dimensions(const softc_entity_t *ptr, size_t *size)
{
  @{getDimensionsBody.join('\n  ')}
}

static int get_dimension_size(const softc_entity_t *ptr, const char *label)
{
  @{entity}_s *self = (@{entity}_s*)ptr;
  @{getDimensionSizeBody.join('\n  ')}
  assert(false); /* Illegal label */   
  return 0;
}

static const char * get_meta_type()
{
  return @{ENTITY}_META_TYPE;
}

static const char * get_meta_name()
{
  return @{ENTITY}_META_NAME;
}

static const char * get_meta_namespace()
{
  return @{ENTITY}_META_NAMESPACE;
}

static const char * get_meta_version()
{
  return @{ENTITY}_META_VERSION;
}

static char* copy_string(const char *str)
{
  char *c = (char*)malloc(strlen(str)+1);
  strcpy(c, str);
  return c;
}

@{entity}_s *@{entity}_create0(const char *id)
{
  SOFTC_ENTITY_VTABLE(@{entity});
  @{entity}_s *self;
  self = malloc (sizeof *self);
  *self = (@{entity}_s) {SOFTC_ENTITY_VTABLE_NAME(@{entity}),
    (const char *)copy_string(id),
    (@{entity}_dimensions_s)
    {@{zeroDimsList.join(',')}},
    (@{entity}_properties_s)
    {@{zeroAttrInitList.join(',\n    ')}},
    (@{entity}_allocatable_s*){
    NULL
    }
  };
  return self;
}


@{entity}_s *@{entity}_create(@{constructorArgs.join(", ")})
{
  SOFTC_ENTITY_VTABLE(@{entity});
  @{entity}_s *self;
  self = malloc (sizeof *self);
   
  @{entity}_allocatable_s *alloc = malloc(sizeof *alloc);
  @{initDataAllocatables.join('\n  ');}
  
  *self = (@{entity}_s) {SOFTC_ENTITY_VTABLE_NAME(@{entity}),
    softc_uuidgen(),
    (@{entity}_dimensions_s)
    {@{dimsList.join(',')}},
    (@{entity}_properties_s)
    {@{attrInitList.join(',\n    ')}},
    (@{entity}_allocatable_s*){
      alloc
    }
  };
  return self;
}

void @{entity}_free(@{entity}_s *self)
{
  @{freeProperties.join(';\n  ')};
  free((char*)self->id);
  free (self);
}

@{entity}_properties_s *const @{entity}_props (@{entity}_s *self)
{
  return &self->props;
}

@{entity}_dimensions_s *const @{entity}_dims (@{entity}_s *self)
{
  return &self->dims;
}

@{entity}_allocatable_s *@{entity}_allocatables(@{entity}_s *self)
{
  return self->allocs;
}

@{getDataFunctions.join('\n');}
