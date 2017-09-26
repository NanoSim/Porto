project (softpy C CXX)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(PY_SOURCES
  __init__.py
  errors.py
  utils.py
  entity.py
  collection.py
  storage.py
  metadata.py
  arithmetic_eval.py
  translators.py
)

set(SWIG_MODULE_softpy_EXTRA_DEPS
  numpy.i
  softpy.i
  softpy-typemaps.i
  softpy-bytearray.i
  softpy-string.i
  softpy-string-list.i
  softpy-datamodel.i
  softpy-entity.i
  softpy-collection.i
  softpy-python.i
)

set(CMAKE_SWIG_FLAGS "")

if(USE_PYTHON3)
  find_package(PythonInterp 3 REQUIRED)
else()
  find_package(PythonInterp 2 REQUIRED)
endif()
find_package(PythonLibs REQUIRED)
find_package(Numpy REQUIRED)

# Try to require correct version of swig depending on Python version
if(${PYTHON_VERSION_STRING} VERSION_LESS 3.0)
  find_package(SWIG REQUIRED 1.3.30)
elseif(${PYTHON_VERSION_STRING} VERSION_LESS 3.2)
  find_package(SWIG REQUIRED 1.3.37)
elseif(${PYTHON_VERSION_STRING} VERSION_LESS 3.3)
  find_package(SWIG REQUIRED 2.0.4)
elseif(${PYTHON_VERSION_STRING} VERSION_LESS 3.5)
  find_package(SWIG REQUIRED 3.0.0)
else()
  find_package(SWIG REQUIRED 3.0.8)
endif()

# Add macros for use with swig
include(${SWIG_USE_FILE})

include_directories(
  ${PYTHON_INCLUDE_DIRS}
  ${NUMPY_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${soft-kernel_SOURCE_DIR}
  ${softc_SOURCE_DIR}
  )

if(USE_PYTHON3)
  set_source_files_properties(softpy.i PROPERTIES SWIG_FLAGS "-py3;-Wall")
else()
  set_source_files_properties(softpy.i PROPERTIES SWIG_FLAGS "-Wall")
endif()

swig_add_module(softpy python softpy.i)
swig_link_libraries(softpy softc ${PYTHON_LIBRARIES})


# Copy the python sources to the binary dir such that we can run the tests
# without installation
foreach(src ${PY_SOURCES})
  configure_file(${src} ${src} COPYONLY)
endforeach()



# Install, Python module, follow the solution at
# http://developer.intra2net.com/git/?p=libftdi;a=blob;f=python/CMakeLists.txt;h=93410544fac2a45eec51cb8dd344708b3bbdfe67;hb=HEAD
if(UNIX)
  set(pyext_ext ".so")
else()
  set(pyext_ext ".pyd")
endif()

execute_process(COMMAND
  ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print(sysconfig.get_python_lib(plat_specific=True, prefix='${CMAKE_INSTALL_PREFIX}'))"
  OUTPUT_VARIABLE _ABS_PYTHON_MODULE_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

get_filename_component(_ABS_PYTHON_MODULE_PATH
  ${_ABS_PYTHON_MODULE_PATH} ABSOLUTE)
file(RELATIVE_PATH _REL_PYTHON_MODULE_PATH
  ${CMAKE_INSTALL_PREFIX} ${_ABS_PYTHON_MODULE_PATH})

set(PYTHON_MODULE_PATH
  ${_REL_PYTHON_MODULE_PATH}
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/_softpy${pyext_ext}
        ${CMAKE_CURRENT_BINARY_DIR}/softpy.py
  DESTINATION ${PYTHON_MODULE_PATH}/softpy
)
install(
  FILES ${PY_SOURCES}
  DESTINATION ${PYTHON_MODULE_PATH}/softpy
)
