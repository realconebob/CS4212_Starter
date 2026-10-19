# CS 4212: Graphics

This project uses CMake and vcpkg for managing C++ dependencies. It serves as a simple example to test your build setup before we get into more complicted code.

## Building

Just type `make` in the project root

```bash
make # builds with default config to buildVCPkg
make o3 # builds with optimizations enabled to build-O3
make all # builds every configuration of the project
make clean # removes current build folders
```

## Running Tests

To run a test, the executable for each must first exist. Follow the build instructions above and they will be outputted to `<path/to/this source>/build{VCPkg,-O2,-O3}/utests/`

Then, simply run the test: `./<path/to/this source>/build{VCPkg,-O2,-O3}/utests/utest_<DESIRED_TEST>`

For example, to run the `VecX` tests, run:

```bash
# If the project has yet to be built
make

cd buildVCPkg
./utests/utest_VecX
```

## Running Test Snippets

Test snippets are essentially small example programs. They'll end up in `./<path/to/this source>/build{VCPkg,-O2,-O3}/test_snippets`. These generally should output some file as a result

```bash
make o3
cd build-O3/test_snippets/
./{desired_test_snippet}
```
