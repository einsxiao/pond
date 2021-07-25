#!/bin/sh

cd $POND_ROOT/src_packages
if ! [ -f openmpi-4.1.0.tar.bz2 ]; then
  wget --no-check-certificate http://download.open-mpi.org/release/open-mpi/v4.1/openmpi-4.1.0.tar.bz2
fi
if ! [ -d  openmpi-4.1.0 ]; then
    tar -xvf ./openmpi-4.1.0.tar.bz2
fi
if [ -d openmpi-build ]; then 
  rm -r openmpi-build;
fi
mkdir openmpi-build
cd openmpi-build
../openmpi-4.1.0/configure --prefix ${POND_ROOT}/packages/openmpi \
                           --enable-mpi-cxx \
                           --enable-cxx-exceptions \

make -j 4
make install
cd ..

