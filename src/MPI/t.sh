mpic++ -std=c++11 -g -G -D DEBUG -lmpi --compiler-options -g3 --compiler-options -Wall --compiler-options -Wextra --compiler-options -Wno-unused --compiler-options -Wno-unused-parameter --compiler-options -fno-inline-functions -o exe build/debug/MPIModule.tcpp.o -L./ -Xlinker -rpath=$ORIGIN/./ -I./ -I/opt/pond/python3/include -I/opt/pond/python/include/python2.7 -I/opt/pond/python/include -I/opt/pond/pond/include -I/opt/pond/gdb/include -I/opt/pond/openmpi/include -I/opt/pond/cuda-7.0/include -I/opt/pond/gcc/include -L/opt/pond/python3/lib -Xlinker -rpath=/opt/pond/python3/lib -L/opt/pond/python/lib -Xlinker -rpath=/opt/pond/python/lib -L/opt/pond/pond/lib -Xlinker -rpath=/opt/pond/pond/lib -L/opt/pond/gdb/lib64 -Xlinker -rpath=/opt/pond/gdb/lib64 -L/opt/pond/gdb/lib -Xlinker -rpath=/opt/pond/gdb/lib -L/opt/pond/openmpi/lib -Xlinker -rpath=/opt/pond/openmpi/lib -L/opt/pond/cuda-7.0/lib64 -Xlinker -rpath=/opt/pond/cuda-7.0/lib64 -L/opt/pond/cuda-7.0/lib -Xlinker -rpath=/opt/pond/cuda-7.0/lib -L/opt/pond/gcc/lib64 -Xlinker -rpath=/opt/pond/gcc/lib64 -L/opt/pond/gcc/lib -Xlinker -rpath=/opt/pond/gcc/lib -lmpi -leva -Wno-deprecated-gpu-targets --compiler-options -fopenmp -DEVA_OS_LINUX -lmpi