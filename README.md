## Requirements

* C++17 compliant compiler
* CMake 3.15 or newer
* Qt 5.15

## Building from source

```shell
git clone https://github.com/ovfireandvoid/word_counter.git
cd word_counter
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Testing

```shell
git clone https://github.com/ovfireandvoid/word_counter.git
cd word_counter
mkdir build
cd build
cmake .. -DBUILD_TESTS=ON
ctest .
```
