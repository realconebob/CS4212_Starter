.PHONY: default o3 all t test ft fast-test c clean

BUILD_FOLDERS := buildVCPkg buildO3
MKFILE_PATH := ~/vcpkg/scripts/buildsystems/vcpkg.cmake

default:
	cmake --preset=default \
		-DBUILD_DOCS=ON \
		-DENABLE_CLANG_TIDY=ON \
		-DCMAKE_CXX_FLAGS="-fopenmp -fsanitize=thread -g"
	cmake --build buildVCPkg -j

o3:
	cmake -S . -B buildO3 -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS="-O3 -march=native -DNDEBUG" \
		-DENABLE_CLANG_TIDY=OFF \
		-DCMAKE_TOOLCHAIN_FILE=${MKFILE_PATH} \
		-DVCPKG_TARGET_TRIPLET=x64-linux
	cmake --build buildO3 -j

all:
	$(MAKE) default -j
	$(MAKE) o3 -j

t test:
	$(MAKE) default -j
	ctest --test-dir buildVCPkg --output-on-failure -j

ft fast-test:
	$(MAKE) o3 -j
	ctest --test-dir buildO3 --output-on-failure -j

c clean:
	rm -rvf ${BUILD_FOLDERS}
