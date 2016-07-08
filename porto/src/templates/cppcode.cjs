@{
  var cppapi = require('soft.forge.cppapi');

  ENTITY = soft.model.name.toUpperCase();
  entity = soft.model.name.toLowerCase();
  Entity = entity.charAt(0).toUpperCase() + entity.slice(1);

  createArgs = (function(){
      var ret = [];
      if (soft.model.dimensions != undefined) {
	soft.model.dimensions.forEach(function (d){
	    ret.push("StdUInt " + d.name);
	  });
      }
      return ret;
    })();

  dims = (function(){
      var ret = [];
      if (soft.model.dimensions != undefined) {
	soft.model.dimensions.forEach(function (d){
	    ret.push(d.name);
	  });
      }
      return ret;
    })();

  quote = (function(xs) {
      var q = [];
      xs.forEach(function (x){
	  q.push("\"" + x + "\"");
	});
      return q;
    });
    
  attributes = (function(){
      var attr = [];
      soft.model.properties.forEach(function (entry){
	  var o = {};
	  o.name = entry.name;
	  o.type = cppapi.type_to_cpp(entry.type);
	  o.rank = (entry.dims != undefined ? entry.dims.length : 0);
	  o.desc = (entry.description != undefined ? "/* " + entry.description + " */" : "");
	  o.dims = entry.dims;
	  attr.push(o);
	});
      return attr;
    })();

  dataModelType = function(type, rank){
    var rank = typeof rank !== 'undefined' ?  rank : 0;
    switch (type) {
    case 'StdDouble':
      switch (rank) {
      case 0:
	return "Double";
      case 1:
	return "DoubleArray";
      case 2:
	return "DoubleArray2D";
      case 3:
	return "DoubleArray3D";
      case 4:
	return "DoubleArray4D";
      default:
	throw ("Undefined rank from Double type");
      };
    case 'StdFloat':
      return "Float";
    case 'StdInt':
      switch (rank) {
      case 0:
	return "Int32";
      case 1:
	return "Int32Array";
      case 2:
	return "Int32Array2D";
      case 3:
	return "Int32Array3D";
      case 4:
	return "Int32Array4D";
      default:
	throw("Unsupported rank from int-type");
      };
    case 'StdUInt':
      return "UInt32";
    case 'StdInt16':
      return "Int16";
    case 'StdUInt16':
      return "UInt16";
    case 'StdInt64':
      return "Int64";
    case 'StdUInt64':
      return "UInt64";
    case 'StdInt8':
      return "Int8";
    case 'StdUInt8':
      return "UInt8";
    case 'StdBool':
      return "Bool";
    case 'StdString':
      switch (rank) {
      case 0:
	return "String";
      case 1:
	return "StringArray";
      default:
	throw("Unsupported rank from stringtype");
      };
    case 'StdBlob':
        return "ByteArray";
    default:
      throw("Unimplemented" + entry.type);
      break;
    };
  };
      
  dataModelStoreFunction = function(entry){
    var appendFunction = "dataModel->append";
    appendFunction += dataModelType(entry.type, entry.rank);
    return appendFunction;
  };

  dataModelLoadFunction = function(entry){
    var getFunction = "dataModel->get";
    getFunction += dataModelType(entry.type, entry.rank);
    return getFunction;
  };
    
  appendDimsSection = (function(){
      var xs = [];
      if (typeof soft.model.dimensions !== 'undefined') {
	soft.model.dimensions.forEach(function (entry){
	    xs.push("dataModel->appendDimension(\"" + entry.name + "\", d->dims." + entry.name + ");");
	  });
      }
      return xs;
    })();

  getDimsSection = (function(){
      var xs = [];
      if (typeof soft.model.dimensions !== 'undefined') {
	soft.model.dimensions.forEach(function (entry){
	    xs.push("dataModel->getDimension(\"" + entry.name + "\", d->dims." + entry.name + ");");
	  });
      }
      return xs;
    })();
    
  saveSection = (function(){
      var getList = [];
      attributes.forEach( function (entry) {
	  var str = dataModelStoreFunction(entry);
	  str += " (\"" + entry.name + "\", " + entry.name + ");";
	  getList.push(str);
	});
      return getList;
    })();

  loadSection = (function(){
      var getList = [];
      attributes.forEach( function (entry) {
	  var str = dataModelLoadFunction(entry);
	  str += " (\"" + entry.name + "\", " + entry.name + ");";
	  getList.push(str);
	});
      return getList;
    })();

  undefined;
}
#include <stdexcept>
#include <idatamodel.h>
#include "@{entity}.hxx"

SOFT_BEGIN_NAMESPACE
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

struct @{Entity}Dims
{
  @{createArgs.length > 0 ? createArgs.join(";\n  ") : ""};
};

class @{Entity} :: Private
{
  friend class @{Entity};
  Private(@{createArgs.join(",")})
    : dims {@{dims.join(",")}}
  {} 
  @{createArgs.length > 0 ? "Private()\n  {}" : ""}
  @{Entity}Dims dims;
};

@{Entity} :: @{Entity} (@{createArgs.join(",")})
: IEntity()
, d(new @{Entity}::Private(@{dims.join(",")}))
{}

@{Entity} :: @{Entity}(std::string const &id)
: IEntity(id)
, d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(const IEntity *ptr)
: IEntity(ptr)
, d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(IDataModel const *dm)
: IEntity()
, d(new @{Entity}::Private())
{
  load(dm);
}

@{Entity} :: ~@{Entity} ()
{
  delete d;
}

IEntity* @{Entity} :: create (std::string const &uuid)
{
  NOT_IMPLEMENTED	
    return nullptr;	
}

void @{Entity} :: save (IDataModel *dataModel) const
{
  dataModel->setId(id());
  @{appendDimsSection.join("\n  ");}
  @{saveSection.join("\n  ");}
}

void @{Entity} :: load (IDataModel const *dataModel)
{
  setId(dataModel->id());
  @{getDimsSection.join("\n  ");}
  @{loadSection.join("\n  ");}
}

std::vector<std::string> @{Entity} :: dimensions() const
{
  return std::vector<std::string>({@{quote(dims).join(",")}});
}

int @{Entity} :: getDimensionSize(std::string const &) const
{
  NOT_IMPLEMENTED;
}

SOFT_END_NAMESPACE
