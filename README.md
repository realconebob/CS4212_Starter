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
cd buildO3/test_snippets/
./{desired_test_snippet}
```

## Running All Tests

If you want to run all tests at once, unit test and test snippet, there are 2 make commands for you:

```bash
make t  # tests without compiler optimizations enabled
make ft # tests with compiler optimizations enabled
```

Both of these options will build the necessary version and then run the test suite. You should see something like this:

```text
$ make ft
Test project /path/to/source/dir/buildO3
      Start  1: Relative Diff
 1/34 Test  #1: Relative Diff .............................................   Passed    0.01 sec
      Start  2: Max, Min, Clamp
 2/34 Test  #2: Max, Min, Clamp ...........................................   Passed    0.01 sec
      Start  3: Degrees to Radians
 3/34 Test  #3: Degrees to Radians ........................................   Passed    0.00 sec
```

From here, you can inspect the outputs of the test snippets by going to `{buildVCPkg,buildO3}/test_snippets/`
