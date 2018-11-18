#!/bin/bash
# run in mingw64 shell
_WIN64=1 CXXFLAGS="-fpermissive" LDFLAGS="-static -static-libstdc++" LIBS="-lws2_32" ./configure --without-cvc4 --without-yices2 --without-tecla
_WIN64=1 make
