prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_LIBDIR@
includedir=${prefix}/include

Name: libcmime
Version: @CMIME_VERSION@
Description: Lightweight library for parsing and createion of mime messages
Libs: -L${libdir} -lcmime
Cflags: -I${includedir}
