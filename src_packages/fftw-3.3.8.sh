#!/bin/sh
# it should be noted that float and double version can not been compiled at one run
cd $(POND_ROOT)/src_packages
tar -xvf ./fftw-3.3.8.tar.gz
if [ -d fftw-build ]; then rm -r fftw-build; fi
mkdir fftw-build
cd fftw-build
../fftw-3.3.8/configure --prefix ${POND_ROOT}/packages/ \
		--enable-shared=yes \
		--enable-static=yes \
		--disable-fortran \
		--enable-mpi --enable-openmp \
		--enable-threads \

#		--enable-float \

make -j 4
make install

