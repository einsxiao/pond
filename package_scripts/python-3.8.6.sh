#/bin/bash
cd $POND_ROOT/src_packages
if ! [ -f Python-3.8.6.tgz ]; then
    wget --no-check-certificate https://www.python.org/ftp/python/3.8.6/Python-3.8.6.tgz
fi
if ! [ -d Python-3.8.6 ]; then
    tar -xvf Python-3.8.6.tgz
fi
if [ -d Python-3.8.6.build ];then
  rm -r Python-3.8.6.build
fi
mkdir Python-3.8.6.build
cd Python-3.8.6.build
../Python-3.8.6/configure --prefix=$POND_ROOT/packages/python3 --enable-checking=release --with-openssl=$POND_ROOT/packages/openssl
make -j 4
make install
