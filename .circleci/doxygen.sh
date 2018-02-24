#!/bin/sh
mkdir -p ~/pages
cd doc
doxygen
cd ..
cp -a doc ~/pages/doxygen
