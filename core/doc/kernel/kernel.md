\module Core
\brief Core functionality

\defgroup soft-kernel SOFT Kernel 
\brief The SOFT Kernel modules provies a set of interfaces and basic classes for developing SOFT5-enabled applications


\class Collection
\ingroup soft-kernel 
\brief The Collection class defines relationship between entities

The Collection class is a specific type of an IEntity that defines a
set of operations to define relationships between entities. The
Collection is an essential tool for building domain specific
information about a system. While the entity as a concept can cross
between different domain, the collection is specific to given
domain. This we sepearate relational information from data
descriptions. This also implies that Entites themself does not contain
other entities as properties, as this is domain-specific
knowledge. This should rather be resolved by using a Collection. Note
that since a Collection is an entity, it can also define relations on
collections.
  
\sa IEntity


\class IEntity 
\ingroup soft-kernel 
\brief The IEntity class defines the behaviour of all entities

The entity and the IEntity interface is the fundament of the
data-centric idea behind SOFT. The entity has its origin from early
entity entity-relationship modelling concepts, and defines a
self-contained type for a specific domain. The entity is defined by
its name, namespace (domain name) and version number. An instance of
an entity will also have its unique identifier.

\class PluginLoader
\ingroup soft-kernel
\brief the PluginLoader class loads a SOFT5 plugin at runtime
	
\file softns.h
\ingroup soft-kernel
\brief The softns.h defines the SOFT_NAMESPACE macro

\class Storage
\ingroup soft-kernel 
\brief The Storage class is a context class for a StorageStrategy

The Storage class is a context class for a StorageStrategy (i.e. the
behaviour of the Storage class will depend on the selected strategy,
which in turn is a plug-in. The format of which Storage will choose to
read/write will depend on the selected strategy at runtine.

\sa IStorageStrategy

\file variant.h
\ingroup soft-kernel
\brief Template magic to define custom variants

Based on "Implementing a variant type from scratch in C++" Posted
by Tibor Djurica Potpara on October 10, 2013 in "C++, Programming"

\class IStorageStrategy 
\ingroup soft-kernel 
\brief The IStorageStrategy class provides the API for storing/retrieving data

The storage strategy defines the behaviour when storing and
retriving information to/from a \a IDataModel. Note that storing a data
model as a single operation, while retrieving information might
involve a cleanup step. 

\sa IDataModel

\class ScriptEngine
\ingroup soft-kernel
\brief The ScriptEngine class provides a simple interface to evaluate JavaScript expressions

\file softtypes.h       
\ingroup soft-kernel
\brief Defines types that should be preferred when using SOFT

\class Transactor
\ingroup soft-kernel
\brief The Transactor class provides immutable storage capabilities

\class IDataModel
\inmodule soft-kernel
\brief The IDataModel class defines the interface for derived data models
  
A data model is defines a group of information that relates to an
  entity, i.e. an entity will use a data model to store and retrieve
  its own state. The IDataModel defines the interface for a data
  model, but it is up to each Strategy \a (IStorageStrategy) to choose
  an appropriate data model.

\sa IStorageStrategy 

\class IStrategyPlugin
\ingroup soft-kernel 
\brief The IStrategyPlugin class provides a portable storage plugin mechanism

The StrategyPlugin provides interfaces for a portable storage plugin
mechanism build on the QtPlugin module. This will typically be
'internal' back-ends that are able to handle I/O of any generic type.

\file soft.h          
\ingroup soft-kernel
\brief Provides API for initializing SOFT and the handling of plugins

\class StorageFactory
\ingroup soft-kernel
\brief The StorageFactory class instanciates a IStorageStrategy 

\todo {Add more text}

\class TripletStore
\ingroup soft-kernel
\brief The TripletStore provides a minimalistic class for operating on triplet based semantic triplet










