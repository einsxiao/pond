#compile setting
PPCC = g++
DIRS = 
LIBS = -ldl
##################################################################################
DFLAGS :=  -std=c++11 -g3 -DDEBUG -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-sign-compare -Wno-unused-variable -Wno-comment
FLAGS := -std=c++11 -O2 

##################################################################################

INSTALLDIRS := $(DIRS:%=install-%)
CLEANDIRS := $(DIRS:%=clean-%)
HEADERS := $(wildcard *.h)

SRCS := $(wildcard *.cpp) 

DOBJECTS := $(patsubst %.cpp, build/debug/%.tcpp.o, $(SRCS))
DALLOBJECTS :=  $(DOBJECTS)

OBJECTS := $(patsubst %.cpp, build/release/%.tcpp.o, $(SRCS))
ALLOBJECTS := $(OBJECTS) 

INCLUDE = `pond-path gcc`
MAKE = make
OUT = libpd.so
DOUT = libpd-dev.so


debug: prepare-all debug-prepare DALL  post-work
debug-prepare:
	@echo "Debug Version"

release: prepare-all prepare ALL post-work
prepare:
	@echo "Release Version"

prepare-all:
	@rmcp *.h ../../include/
	@rmcp pond ../../include/
	$(shell if [ -f build ]; then rm build; mkdir build; elif ! [ -d build ]; then mkdir build; fi; )
	$(shell if [ -f build/debug ]; then rm build/debug; mkdir build/debug; elif ! [ -d build/debug ]; then mkdir build/debug; fi; )
	$(shell if [ -f build/release ]; then rm build/release; mkdir build/release; elif ! [ -d build/release ]; then mkdir build/release; fi; )

post-work:
	@rmcp *.so ../../lib/

########################################################################################################
DALL: debugsharelib 

debugsharelib: $(DALLOBJECTS)
	$(PPCC) $(DFLAGS) -shared -o $(OUT) $(DALLOBJECTS) $(INCLUDE)  $(LIBS)

build/debug/%.tcpp.o: %.cpp $(HEADERS)
	$(PPCC) $(DFLAGS) -c -fPIC $< -o $@ -I. $(INCLUDE)

#########################################################################################################
ALL: sharelib 

sharelib: $(ALLOBJECTS)
	$(PPCC) $(FLAGS) -shared -o $(OUT) $(ALLOBJECTS) $(INCLUDE)  $(LIBS)

build/release/%.tcpp.o: %.cpp $(HEADERS)
	$(PPCC) $(FLAGS) -c -fPIC $< -o $@ -I. $(INCLUDE)

#########################################################################################################

clean: 
	@rm *.so
	@rm -rf build
