# CS 4212: Graphics

This project uses CMake and vcpkg for managing C++ dependencies. It serves as a simple example to test your build setup before we get into more complicted code.

## Building Using CMake Presets

We have several CMake Build Presets that are outlined in the CMakePresets.json. Some are for building for Release or Debug mode. Running the default setup is just fine too.

```sh
cd <path/to/this source>
cmake --preset=default
```

Each preset defines its own build directory and various build variables that are important on that system.

Then, to build this source, you would

```sh
cd buildVCPkg
cmake --build .
```

Your executables will then be in the build folder. They may be in sub-folders depending on the environment.

## Running Tests

To run a test, the executable for each must first exist. Follow the build instructions above and they will be outputted to `<path/to/this source>/buildVCPkg/utests/`

Then, simply run the test: `./<path/to/this source>/buildVCPkg/utests/utest_<DESIRED_TEST>`

For example, to run the `VecX` tests, run:

```bash
# If the project has yet to be built
cd <path/to/this source>
cmake --preset=default

cd buildVCPkg
cmake --build .

./utests/utest_VecX


# If you only need to rebuild the tests
cd <path/to/this source>/buildVCPkg
cmake --build .
./utests/utest_VecX
```
