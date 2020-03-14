#!/bin/sh -x
mkdir -p ~/pages/doxygen
cd docs ; doxygen ; cd ..
cp -a docs/html/* ~/pages/doxygen
