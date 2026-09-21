.PHONY: all

all:
	cmake --preset=default -DBUILD_DOCS=ON
	$(MAKE) -C buildVCPkg
