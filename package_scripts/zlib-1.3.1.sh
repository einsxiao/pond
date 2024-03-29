#/bin/bash
cd $POND_ROOT/src_packages
if ! [ -f zlib-1.3.1.tar.gz ];then
    wget --no-check-certificate https://www.zlib.net/zlib-1.3.1.tar.gz
fi
if ! [ -d zlib-1.3.1 ]; then 
    tar -xvf zlib-1.3.1.tar.gz
fi
if [ -d zlib-1.3.1.build ];then
    rm -r zlib-1.3.1.build
fi
#mkdir zlib-1.3.1.build
cd zlib-1.3.1
../zlib-1.3.1/configure --prefix=$POND_ROOT/packages/zlib 
#make -j 4
#make install
make prefix=$POND_ROOT/packages/zlib all
make prefix=$POND_ROOT/packages/zlib install
