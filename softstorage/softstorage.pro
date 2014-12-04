TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = src
contains(CONFIG, examples) {
  SUBDIRS += src
}

contains(CONFIG, tests) {
  SUBDIRS += tests
}