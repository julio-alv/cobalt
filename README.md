# Cobalt

A simple rendering engine

## Cloning the repo

This repo uses git submodules, use this command to properly download all the dependencies:

```sh
git clone --recurse-submodules <this repo>
```

## Requirements
- CMake 4.0.3
- Make
- Clang++ (or any other compiler that supports c++23

## Building the project

- create a build folder
- navitage to it
- run `cmake ..` to generate build files
- run `make` to build the project

## Running the project

After building the project, the executable `cobalt` will be available on the build folder.
