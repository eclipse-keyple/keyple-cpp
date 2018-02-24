#!/bin/sh -x
mkdir -p ~/pages/doxygen
cd doc ; doxygen ; cd ..
cp -a doc/html/* ~/pages/doxygen
