.PHONY: default all o3 clean

BUILD_FOLDERS := buildVCPkg build-O2 build-O3
MKFILE_PATH := ~/vcpkg/scripts/buildsystems/vcpkg.cmake

default:
	cmake --preset=default \
		-DBUILD_DOCS=ON \
		-DENABLE_CLANG_TIDY=ON \
		-DCMAKE_CXX_FLAGS="-fopenmp -fsanitize=thread -g"
	cmake --build buildVCPkg -j

o3:
	cmake -S . -B build-O3 -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS="-O3 -march=native -DNDEBUG" \
		-DENABLE_CLANG_TIDY=OFF \
		-DCMAKE_TOOLCHAIN_FILE=${MKFILE_PATH} \
		-DVCPKG_TARGET_TRIPLET=x64-linux
	cmake --build build-O3 -j

all:
	$(MAKE) default -j
	$(MAKE) o3 -j

clean:
	rm -rvf ${BUILD_FOLDERS}
