#Created from stadard compile template of Pond
#Current Module: Template
################################################################
DEPENDED_MODULES  := 
INCLUDE           :=
LIBS              := -lMatrixModule 
SUBDIRS           :=
#MPI_ENABLE        := True
################################################################
################################################################
CC                := pcc
FORCC             := gfortran
MPIFORCC          := mpifort
################################################################
MAKE              := make
SHELL             := /bin/bash
################################################################
FLAGS     := -std=c++11 -O2
DFLAGS    := -std=c++11 -g -G -D DEBUG --compiler-options -g --compiler-options -Wall --compiler-options -Wextra --compiler-options -Wno-unused --compiler-options -Wno-unused-parameter  --compiler-options -fno-inline-functions
DFLAGS    := -std=c++11 -g -DDEBUG
FORFLAGS  := -O2
DFORFLAGS := -g -D DEBUG -Wall -Wextra  -Wno-unused -Wno-unused-parameter 
################################################################
MODULE_NAME := $(notdir $(CURDIR))Module
CUDAVERSION := $(wildcard *.cu)
ifdef CUDAVERSION
OBJFIX  :=tcu
else
OBJFIX  :=tcpp
endif
EXCU    := exe

ifeq ($(MPI_ENABLE),True)
LIBS   := $(LIBS) -lmpi
FORCC  := $(MPIFORCC)
endif

INCLUDE      := $(INCLUDE) `pond-path pcc $(DEPENDED_MODULES)` 

SRCS         := $(wildcard *.cpp)
CSRCS        := $(wildcard *.c)
CUSRCS       := $(wildcard *.cu)
CCSRCS       := $(wildcard *.cc)
FOR90SRCS    := $(wildcard *.f90)
FOR95SRCS    := $(wildcard *.f95)
FOR03SRCS    := $(wildcard *.f03)
FOR08SRCS    := $(wildcard *.f08)
FORSRCS      := $(wildcard *.f90 *.f95 *.f03 *.f08)
HEADERS      := $(wildcard *.h) $(POND_ROOT)/include/pond.h

DOBJECTS     := $(patsubst %.cpp, build/debug/%.tcpp.o, $(SRCS))
DCUOBJECTS   := $(patsubst %.cu, build/debug/%.tcu.o, $(CUSRCS))
DCOBJECTS    := $(patsubst %.c, build/debug/%.tc.o, $(CSRCS))
DFOROBJECTS  := $(patsubst %.f90, build/debug/%.tf90.o, $(FOR90SRCS)) $(patsubst %.f95, build/debug/%.tf95.o, $(FOR95SRCS)) $(patsubst %.f03, build/debug/%.tf03.o, $(FOR03SRCS)) $(patsubst %.f08, build/debug/%.tf08.o, $(FOR08SRCS))
DALLOBJECTS  := $(DOBJECTS) $(DCOBJECTS) $(DCUOBJECTS) $(DFOROBJECTS)

OBJECTS      := $(patsubst %.cpp, build/release/%.tcpp.o, $(SRCS))
CUOBJECTS    := $(patsubst %.cu, build/release/%.tcu.o, $(CUSRCS))
COBJECTS     := $(patsubst %.c, build/release/%.tc.o, $(CSRCS))
FOROBJECTS   := $(patsubst %.f90, build/release/%.tf90.o, $(FOR90SRCS)) $(patsubst %.f95, build/release/%.tf95.o, $(FOR95SRCS)) $(patsubst %.f03, build/release/%.tf03.o, $(FOR03SRCS)) $(patsubst %.f08, build/release/%.tf08.o, $(FOR08SRCS))
ALLOBJECTS   := $(OBJECTS) $(COBJECTS) $(CUOBJECTS) $(FOROBJECTS)

ifdef FORSRCS
LIBS   := $(LIBS) -gfortran
endif

################################################################
in-all: debug

all: release debug

install: release upload

debug: all-prepare DALL post-work

release: all-prepare ALL post-work


##############################################################################
DALL: dexe debugsharelib

debugsharelib: $(DALLOBJECTS)
	$(CC) $(DFLAGS) -shared -o lib$(MODULE_NAME)-dev.so $(DALLOBJECTS) $(INCLUDE)  $(LIBS)
build/debug/%.tcu.o: %.cu $(HEADERS) $(CCSRCS)
	$(CC) $(DFLAGS) -Xcompiler -fPIC -c $< -o $@ -I. $(INCLUDE)
build/debug/%.tcpp.o: %.cpp $(HEADERS) $(CCSRCS)
	$(CC) $(DFLAGS) -Xcompiler -fPIC -c $< -o $@ -I. $(INCLUDE)
build/debug/%.tc.o: %.c $(HEADERS)
	$(CC) $(DFLAGS) -Xcompiler -fPIC -c $< -o $@ -I. $(INCLUDE)
build/debug/%.tf90.o: %.f90 $(HEADERS)
	$(FORCC) $(DFORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)
build/debug/%.tf95.o: %.f95 $(HEADERS)
	$(FORCC) $(DFORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)
build/debug/%.tf03.o: %.f03 $(HEADERS)
	$(FORCC) $(DFORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)
build/debug/%.tf08.o: %.f08 $(HEADERS)
	$(FORCC) $(DFORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)

#############################################################################
ALL: sharelib

sharelib: $(ALLOBJECTS)
	$(CC) $(FLAGS) -shared -o lib$(MODULE_NAME).so $(ALLOBJECTS) $(INCLUDE)  $(LIBS) 
build/release/%.tcu.o: %.cu $(HEADERS) $(CCSRCS)
	$(CC) $(FLAGS) -Xcompiler -fPIC -c $< -o $@ -I. $(INCLUDE)
build/release/%.tcpp.o: %.cpp $(HEADERS) $(CCSRCS)
	$(CC) $(FLAGS) -Xcompiler -fPIC -c $< -o $@ -I. $(INCLUDE)
build/release/%.tc.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -Xcompiler -fPIC -c $< -o $@ -I. $(INCLUDE)
build/release/%.tf90.o: %.f90 $(HEADERS)
	$(FORCC) $(FORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)
build/release/%.tf95.o: %.f95 $(HEADERS)
	$(FORCC) $(FORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)
build/release/%.tf03.o: %.f03 $(HEADERS)
	$(FORCC) $(FORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)
build/release/%.tf08.o: %.f08 $(HEADERS)
	$(FORCC) $(FORFLAGS) -fPIC -c $< -o $@ -I. $(INCLUDE)

#############################################################################
clean: cleandebug cleanrelease

cleandebug:
	@-rm -rf build/debug
	$(shell if [ -f core ]; then rm core; fi; )
	$(shell if [ -f exe ]; then rm exe; fi; )
	$(shell if [ -f lib$(MODULE_NAME)-dev.so ]; then rm lib$(MODULE_NAME)-dev.so; fi; )
	$(shell if [ -f lib$(MODULE_NAME).so ]; then rm lib$(MODULE_NAME).so; fi; )

cleanrelease:
	@-rm -rf build/release;
	$(shell if [ -f lib$(MODULE_NAME).so ]; then rm lib$(MODULE_NAME).so; fi; )

#############################################################################
dexe: $(DALLOBJECTS)
	$(CC) $(DFLAGS) -o $(EXCU) $(DALLOBJECTS) $(INCLUDE) $(DLIBS) $(LIBS) -lpd-dev

all-prepare: 
	@echo "dealing with " $(MODULE_NAME)
	@if [ "x`pond-path pcc $(DEPENDED_MODULES)`" == "x" ]; \
		then echo "Error: Dependency failed.";\
		exit 2;\
		else echo "$(DEPENDED_MODULES)">.requirements;\
		fi 
	$(shell if [ -f build ]; then rm build; mkdir build; elif ! [ -d build ]; then mkdir build; fi; )
	$(shell if [ -f build/debug ]; then rm build/debug; mkdir build/debug; elif ! [ -d build/debug ]; then mkdir build/debug; fi; )
	$(shell if [ -f build/release ]; then rm build/release; mkdir build/release; elif ! [ -d build/release ]; then mkdir build/release; fi; )
	$(shell if [ -w $(POND_ROOT) ]; then cp $(MODULE_NAME).h $(POND_ROOT)/include/; fi )
post-work:
	$(shell if [ -w $(POND_ROOT) ]; then cp *.so $(POND_ROOT)/lib/; fi)

