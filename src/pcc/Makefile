#Created from stadard compile template of POND
#Current Module:Template
################################################################
DEPENDED_MODULES := 
LIBS := -ldl -fopenmp
SUBDIRS :=
#MPI_ENABLE := True
################################################################
################################################################
CPPCC := g++
################################################################
MAKE := make
################################################################
DFLAGS :=  -std=c++11 -g -D DEBUG -Wall -Wextra -Wno-unused-parameter -fopenmp
FLAGS := -std=c++11 -O2 -fopenmp
################################################################
INCLUDE :=  `pond-path pcc`
RPATH := 
EXCU := pcc

SRCS := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)

DOBJECTS := $(patsubst %.cpp, build/debug/%.tcpp.o, $(SRCS))

OBJECTS := $(patsubst %.cpp, build/release/%.tcpp.o, $(SRCS))

#################################################################
################################################################
in-all: debug

all: release debug

install: release

#debug: all-prepare dexe post-work

debug: all-prepare exe post-work

release: all-prepare exe post-work

ddebug: all-prepare dexe post-work

########################################################################################################
dexe: $(DOBJECTS)
	$(CPPCC) $(DFLAGS) -o $(EXCU) $(DOBJECTS) $(INCLUDE) $(LIBS) -lpd

build/debug/%.tcpp.o: %.cpp $(HEADERS)
	$(CPPCC) $(DFLAGS) -c -fPIC $< -o $@ -I. $(INCLUDE)

#########################################################################################################
exe: $(OBJECTS)
	$(CPPCC) $(FLAGS) -o $(EXCU) $(OBJECTS) $(INCLUDE)  $(LIBS)  -lpd

build/release/%.tcpp.o: %.cpp $(HEADERS)
	$(CPPCC) $(FLAGS) -c -fPIC $< -o $@ -I. $(INCLUDE)

#########################################################################################################
clean:
	@make -i cleandebug
	@make -i cleanrelease

cleandebug:
	@rm -rf build/debug
	@rm $(EXCU);

cleanrelease:
	rm -rf build/release;
	@rm $(EXCU);

#####################################################################################

all-prepare: 
	@echo ${POND_ROOT}
	$(shell if [ -f build ]; then rm build; mkdir build; elif ! [ -d build ]; then mkdir build; fi; )
	$(shell if [ -f build/debug ]; then rm build/debug; mkdir build/debug; elif ! [ -d build/debug ]; then mkdir build/debug; fi; )
	$(shell if [ -f build/release ]; then rm build/release; mkdir build/release; elif ! [ -d build/release ]; then mkdir build/release; fi; )

post-work:
	@rmcp $(EXCU) ../../bin/

upload:
	@echo "Upload to POND server"
	$(shell pond upload)
