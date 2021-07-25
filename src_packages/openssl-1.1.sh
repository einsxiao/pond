#/bin/bash
if ! [ -f openssl-1.1.1i.tar.gz ]; then
    wget --no-check-certificate https://www.openssl.org/source/openssl-1.1.1i.tar.gz
fi
if ! [ -d openssl-1.1.li ]; then
    tar -xvf openssl-1.1.1i.tar.gz
fi
if [ ! -d openssl-1.1.1i.build ];then
  mkdir openssl-1.1.1i.build
fi
cd openssl-1.1.1i.build
../openssl-1.1.1i/config --prefix=$POND_ROOT/packages/openssl --openssldir=$POND_ROOT/packages/openssl no-ssl2
make -j 4
make install
