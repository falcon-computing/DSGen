# DSGen
Design Space Generator used in Merlin DSE
## Prerequisites
Operating System
- CentOS 7.x

These system packages are required:
- CMake 3.10+
- boost 1.67.0
- ROSE compiler 0.11.4.14
- GCC 4.9.4
- RapidJSON 

# Building DSGen
    $ export MERLIN_PATH=/path/to/merlin-deps-binary-library-local/
    $ export llvm_path=$MERLIN_PATH/llvm
    $ export deps_path=$MERLIN_PATH/merlin-deps
    $ export rose_path=$MERLIN_PATH/merlin-deps/rose
    $ export rjson_path=$MERLIN_PATH/merlin-deps/rapidjson
    $ export gcc_path=$MERLIN_PATH/merlin-deps/gcc4.9.4
    $ export boost_path=$MERLIN_PATH/merlin-deps/boost_1_67

    $ export MERLIN_DEPS=$deps_path
    $ export LLVM_ROOT_DIR=$llvm_path
    $ export CLANG_ROOT_DIR=$clang_path
    $ export ROSE_ROOT_DIR=$rose_path
    $ export RAPIDJSON_ROOT_DIR=$rjson_path
    $ export GCC_ROOT_DIR=$gcc_path
    $ export BOOST_ROOT=$boost_path

    $export PATH=$llvm_path/bin/:$PATH
    
    $ cd build
    $ cmake3 -GNinja -DCMAKE_BUILD_TYPE=Release ..
    $ ninja

To build debug:

    $ cmake3 -GNinja -DCMAKE_BUILD_TYPE=Debug ..

To change the compiler:

    $ CC=`which gcc` CXX=`which g++` cmake3 -GNinja ..

To build after change in sources (`ninja clean` is never necessary):

    $ ninja

To clean everything except what's tracked by git:

    $ git clean -dfx

## Testing DSGen
To run unit tests:
  
    $ ctest

## Run DSGen
    $ export PATH=/path/to/merlin_compiler/bin:/path/to/dsgen/build/bin:$PATH
    $ ds_generator example.cpp
