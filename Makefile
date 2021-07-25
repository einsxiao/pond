
all: prepare makerelease

prepare:
	@echo 'prepare'
	make -C Template clean

makerelease:
	make -C src release
	make -C src debug

