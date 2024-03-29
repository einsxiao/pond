#/bin/bash
cd $POND_ROOT/src_packages
if ! [ -f git-2.9.5.tar.gz ];then
    #wget --no-check-certificate https://mirrors.edge.kernel.org/pub/software/scm/git/git-2.9.5.tar.gz
    wget --no-check-certificate http://ftp.ntu.edu.tw/software/scm/git/git-2.9.5.tar.gz
fi
if ! [ -d git-2.9.5 ]; then 
    tar -xvf git-2.9.5.tar.gz
fi
if [ -d git-2.9.5.build ];then
    rm -r git-2.9.5.build
fi
#mkdir git-2.9.5.build
cd git-2.9.5
../git-2.9.5/configure --prefix=$POND_ROOT/packages/git --with-openssl=$POND_ROOT/packages/openssl
#make -j 4
#make install
make prefix=$POND_ROOT/packages/git all
make prefix=$POND_ROOT/packages/git install
