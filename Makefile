.PHONY: all

all:
	cmake --preset=default
	$(MAKE) -C buildVCPkg
