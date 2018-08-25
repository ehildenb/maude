TEST_EXTENSIONS = .maude
MAUDE_LOG_COMPILER = $(top_srcdir)/tests/maude-test-wrapper
AM_TESTS_ENVIRONMENT = \
  PATH="$(top_builddir)/src/Main:$$PATH" ; export PATH;       \
  MAUDE_LIB="$(top_srcdir)/src/Main"     ; export MAUDE_LIB;  \
  top_builddir="$(top_builddir)"         ; export top_builddir;
