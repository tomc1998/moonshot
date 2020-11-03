# Building

You need cmake 3.11 or higher!

```bash
mkdir build
cd build
cmake ..
make -j4
```

There should be a `game` executable in the build directory.

Builds currently won't work on windows, since we call `ln` as part of the build
