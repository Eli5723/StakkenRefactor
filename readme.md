
![Logo](https://github.com/Eli5723/StakkenRefactor/blob/master/release/resources/textures/logo.png?raw=true)
# Stakken: A Cultris 2 Spritual Succesor

## What is it?
Stakken is a spiritual succesor to Cultris 2, a fast paced multiplayer tetris game that emphasises speed and technique over arbitrary garbage systems.

## Demo
https://youtu.be/b0AKdH6nMkU

## Build Instructions (Unix)
Run the following commands:

```
./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg install
mkdir build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build
```

## Build Instructions (Windows) (Currently Broken)
Run the following commands:

```
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install
mkdir build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build
```
