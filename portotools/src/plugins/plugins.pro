TEMPLATE   = subdirs
CONFIG    += ordered
SUBDIRS    = mongo \
             hdf5

contains (PLUGINS, gsl) {
  SUBDIRS += gsl
}

