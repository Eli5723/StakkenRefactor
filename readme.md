# Stakken: A Cultris 2 Spritual Succesor

## What is it?
Stakken is a spiritual succesor to Cultris 2, a fast paced multiplayer tetris game that emphasises speed and technique over arbitrary garbage systems.

## Build Instructions (Unix)
Run the following commands:

```
./vcpkg/vcpkg install
mkdir build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build
```

## Build Instructions (Windows)
Run the following commands:

```
./vcpkg/vcpkg install
mkdir build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build
```
