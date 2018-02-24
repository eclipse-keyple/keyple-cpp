#!/bin/sh
mkdir -p ~/artifacts
cd doc
doxygen
cp -a . ~/artifacts/doxygen
