#compile setting
##################################################################################
#DIRS := system Algebraic System Arithmatics List Matrix OS Class MPI MatrixOperations Python String Guis Mathematica MatlabEngine
#DIRS := core pcc Algebraic System Arithmatics List Matrix OS Class MPI MatrixOperations Python String Guis Mathematica MatlabEngine
#DIRS := core pcc List System Arithmatics Matrix OS Class MPI MatrixOperations Python String
DIRS := core pcc List System Arithmatics Matrix OS Class MPI MatrixOperations String
##################################################################################
INCLUDE       = `pond-path nvcc` -D_MWAITXINTRIN_H_INCLUDED -D_FORCE_INLINES
LIB           = -lpd


##################################################################################

BUILDDIRS     = $(DIRS:%=build-%)
INSTALLDIRS   = $(DIRS:%=install-%)
CLEANDIRS     = $(DIRS:%=clean-%)

CUSRCS        = $(wildcard *.cu) 
OBJECTS       = $(patsubst %.cu, %.o, $(CUSRCS))
DOBJECTS      = $(patsubst %.cu, %.dev.o, $(CUSRCS))

NVCC         := nvcc
FLAGS        := -std=c++11 
MAKE          = make
SHELL        := /bin/bash

DNVFLAGS     := $(FLAGS) -G -g --compiler-options -g3  --compiler-options -Wall --compiler-options -Wextra -D DEBUG --compiler-options -Wno-unused-parameter --compiler-options -fopenmp  -Wno-deprecated-gpu-targets
NVFLAGS      := $(FLAGS) -O2 --compiler-options -fopenmp  -Wno-deprecated-gpu-targets

OUT           = pond
DOUT          = pond-dev

debug: all-prepare debug-prepare DALL
debug-prepare:
	@echo "Debug Version"
	$(eval MODE = debug )
release: all-prepare prepare ALL
prepare:
	@echo "Release Version"
	$(eval MODE = release )

all-prepare:


ALL: dir pond #pypond.so
	cp -f $(OUT) ../bin/

dir:$(BUILDDIRS) 
	@if [ $(POND_ENABLE_PYTHON) == "yes" ]; then cd Python; make $(MODE); cd ..; fi

pond: main.o
	$(NVCC) $(NVFLAGS) -o $(OUT) main.o $(INCLUDE) $(LIB)

pypond.so: pypond.o
	pcc $(NVFLAGS) -shared -o pypond.so pypond.o $(INCLUDE) $(LIB)

pypond.o: pypond.cu
	pcc $(NVFLAGS) -c -Xcompiler -fPIC $< -o $@ $(INCLUDE)

%.o: %.cu
	$(NVCC) $(NVFLAGS) -c -Xcompiler -fPIC $< -o $@ $(INCLUDE)

DALL: dir pond-dev 
	#cp -f $(OUT) ../bin/

pond-dev: main.dev.o
	$(NVCC) $(DNVFLAGS)  -o $(DOUT) main.dev.o $(INCLUDE) $(LIB)

pypond-dev.so: pypond-dev.o
	pcc $(DNVFLAGS) -shared -o pypond.so pypond-dev.o $(INCLUDE) $(LIB)

pypond-dev.o: pypond.cu
	pcc $(DNVFLAGS) -c -Xcompiler -fPIC $< -o $@ $(INCLUDE)

%.dev.o: %.cu
	$(NVCC) $(DNVFLAGS) -c -Xcompiler -fPIC $< -o $@ $(INCLUDE)

$(BUILDDIRS):
	@echo "Dealing dir $(@:build-%=%)"
#	@cp $(@:build-%=%)/*.h ../include/
	$(MAKE) -C $(@:build-%=%) $(MODE)
#	@cp $(@:build-%=%)/*.so ../lib/
install: 
	rmcp $(OUT) ../bin/
clean: $(CLEANDIRS)
	rm *.o; rm $(OUT)
#	rm ../lib/*
#	rm ../include/*
$(CLEANDIRS): 
	-$(MAKE) -C $(@:clean-%=%) clean
#install: $(INSTALLDIRS) release
#	rmcp $(OUT) ../bin
$(INSTALLDIRS):
	$(MAKE) -C $(@:install-%=%) install
