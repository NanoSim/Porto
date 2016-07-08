project (softpy C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)


find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${soft-kernel_SOURCE_DIR}
  ${softc_SOURCE_DIR}
  )

set(CMAKE_SWIG_FLAGS "")

set(SWIG_MODULE_softpy_EXTRA_DEPS
  numpy.i
  softpy-typemaps.i
  softpy-entity.i
  softpy-datamodel.i
  softpy-python.i
)

if(USE_PYTHON3)
  set_source_files_properties(softpy.i PROPERTIES SWIG_FLAGS "-py3;-Wall")
else()
  set_source_files_properties(softpy.i PROPERTIES SWIG_FLAGS "-Wall")
endif()

swig_add_module(softpy python softpy.i)
swig_link_libraries(softpy softc ${PYTHON_LIBRARIES})


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

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/_softpy${pyext_ext}
  DESTINATION ${PYTHON_MODULE_PATH})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/softpy.py
  DESTINATION ${PYTHON_MODULE_PATH})
