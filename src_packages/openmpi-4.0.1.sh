#!/bin/sh
cd $(POND_ROOT)/src_packages

tar -xvf ./openmpi-4.0.1.tar.bz2
if [ -d openmpi-build ]; then rm -r openmpi-build; fi
mkdir openmpi-build
cd openmpi-build
../openmpi-4.0.1/configure --prefix ${POND_ROOT}/packages/ \
                           --enable-mpi-cxx \
                           --enable-cxx-exceptions \

#		--enable-float \

make -j 4
make install

