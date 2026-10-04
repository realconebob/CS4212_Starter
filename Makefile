.PHONY: all o2 o3

all:
	cmake --preset=default -DBUILD_DOCS=ON
	cmake --build buildVCPkg -j

o2:
	cmake -S . -B build-O2 -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" \
		-DENABLE_CLANG_TIDY=OFF \
		-DCMAKE_TOOLCHAIN_FILE=/home/csugrads/walst110/vcpkg/scripts/buildsystems/vcpkg.cmake \
		-DVCPKG_TARGET_TRIPLET=x64-linux
	cmake --build build-O2 -j

o3:
	cmake -S . -B build-O3 -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -DNDEBUG" \
		-DENABLE_CLANG_TIDY=OFF \
		-DCMAKE_TOOLCHAIN_FILE=/home/csugrads/walst110/vcpkg/scripts/buildsystems/vcpkg.cmake \
		-DVCPKG_TARGET_TRIPLET=x64-linux
	cmake --build build-O3 -j