#!/bin/bash
# it should be noted that float and double version can not been compiled at one run
cd $POND_ROOT/src_packages
if ! [ -f  fftw-3.3.9.tar.gz ]; then
    wget --no-check-certificate http://www.fftw.org/fftw-3.3.9.tar.gz
fi
if ! [ -d fftw-3.3.9 ]; then 
    tar -xvf ./fftw-3.3.9.tar.gz
fi

# double version
if [ -d fftw-build ]; then 
    rm -r fftw-build; 
fi
mkdir fftw-build
cd fftw-build
../fftw-3.3.9/configure --prefix ${POND_ROOT}/packages/fftw \
		--enable-shared=yes \
		--enable-static=yes \
		--disable-fortran \
		--enable-mpi --enable-openmp \
		--enable-threads

make -j 4
make install
cd ..

# float version
rm -r fftw-build
mkdir fftw-build
cd fftw-build
../fftw-3.3.9/configure --prefix ${POND_ROOT}/packages/fftw \
		--enable-shared=yes \
		--enable-static=yes \
		--disable-fortran \
		--enable-mpi --enable-openmp \
		--enable-threads \
		--enable-float

make -j 4
make install
