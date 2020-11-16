#compile setting
NVCC := nvcc
FLAGS := -std=c++11 

DNVFLAGS := $(FLAGS) -G -g --compiler-options -g3  --compiler-options -Wall --compiler-options -Wextra -D DEBUG --compiler-options -Wno-unused-parameter --compiler-options -fopenmp  -Wno-deprecated-gpu-targets
NVFLAGS := $(FLAGS) -O2 --compiler-options -fopenmp  -Wno-deprecated-gpu-targets

##################################################################################
#DIRS := system Algebraic System Arithmatics List Matrix OS Class MPI MatrixOperations Python String Guis Mathematica MatlabEngine
#DIRS := core pcc Algebraic System Arithmatics List Matrix OS Class MPI MatrixOperations Python String Guis Mathematica MatlabEngine
DIRS := core pcc List System Arithmatics Matrix OS Class MPI MatrixOperations Python String
##################################################################################
BUILDDIRS = $(DIRS:%=build-%)
INSTALLDIRS = $(DIRS:%=install-%)
CLEANDIRS = $(DIRS:%=clean-%)

CUSRCS = $(wildcard *.cu) 
OBJECTS = $(patsubst %.cu, %.o, $(CUSRCS))
DOBJECTS = $(patsubst %.cu, %.dev.o, $(CUSRCS))

INCLUDE = `pond-path pcc` 
#RPATH = $(shell evarpath tcu MAIN_EVA)
MAKE = make
OUT = pond
DOUT = pond

debug: debug-prepare DALL
debug-prepare:
	@echo "Debug Version"
	$(eval MODE = debug )
release: prepare ALL
prepare:
	@echo "Release Version"
	$(eval MODE = release )

ALL: $(BUILDDIRS) pond
	cp -f $(OUT) ../bin/
	rm  ../lib/*-dev.so

pond: $(OBJECTS)
	$(NVCC) $(NVFLAGS) -o $(OUT) $(OBJECTS) $(INCLUDE) -lpd -lmpi

%.o: %.cu
	$(NVCC) $(NVFLAGS) -c -Xcompiler -fPIC $< -o $@ $(INCLUDE)

DALL: $(BUILDDIRS) pond-dev
	cp -f $(OUT) ../bin/

pond-dev: $(DOBJECTS)
	$(NVCC) $(DNVFLAGS)  -o $(DOUT) $(DOBJECTS) $(INCLUDE) -lpd -lmpi

%.dev.o: %.cu
	$(NVCC) $(DNVFLAGS) -c -Xcompiler -fPIC $< -o $@ $(INCLUDE)

$(BUILDDIRS):
	@echo "Dealing dir $(@:build-%=%)"
#	@cp $(@:build-%=%)/*.h ../include/
	$(MAKE) -C $(@:build-%=%) $(MODE)
#	@cp $(@:build-%=%)/*.so ../lib/
install: 
	rmcp -f $(OUT) ../bin/
clean: $(CLEANDIRS)
	rm *.o;rm $(OUT)
$(CLEANDIRS): 
	-$(MAKE) -C $(@:clean-%=%) clean
#install: $(INSTALLDIRS) release
#	rmcp -f $(OUT) ../bin
$(INSTALLDIRS):
	$(MAKE) -C $(@:install-%=%) install
