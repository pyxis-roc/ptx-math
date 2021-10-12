# SASS Math

SASSM provides bit-accurate implementations of the elementary function approximation instructions available in Nvidia's SASS ISA.
It is a fork of the [ptx-math](https://github.com/bcarlet/ptx-math/tree/8ebe53d86ca1634f1c843478c12a4f8116f5feb9) repository that decouples the semantics of those ptx instructions that map to multiple native device instructions.

The currently supported SASS functions are:

- `RRO.SINCOS`
- `MUFU.RRO`
- `MUFU.SIN`
- `MUFU.COS`
- `MUFU.LG2`
- `RRO.EX2`
- `MUFU.EX2`
- `MUFU.RSQ`
- `MUFU.RSQ64H`
- `MUFU.SQRT`
- `MUFU.RCP`
- `MUFU.RCP64H`



## Building

The minimum CMake version for building with Unix makefiles is 3.10. The Visual Studio generators require at least 3.11.

To build with a local installation:

    mkdir build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    cmake --build . --config Release --target install

Available options:

- `-DBUILD_SHARED_LIBS=ON` to build as a shared library
- `-DPTXM_USE_PIC=ON` to generate position-independent code (automatic for shared libraries)
- `-DPTXM_ENABLE_CUDA=OFF` to disable targets requiring CUDA (namely `ptxmvalidate`)

Building requires compilers for C11 and C++11. Components requiring CUDA are optional and can be disabled with the provided CMake option. The PTXM library requires processor support for the BMI2 x86 extension.

## Caveats
Note that compatibility varies with GPU architecture. Function aliases mapping to the correct implementations are defined for each supported architecture.
PTX assembly targets a virtual GPU architecture, and some PTX instructions may be compiled to multiple native device instructions. Consistency of PTX semantics is therefore dependent on consistent JIT compilation of PTX assembly to native (SASS) assembly by the GPU driver. Furthermore, SASS is architecture-dependent, and thus semantics are not necessarily preserved across architectures. The `ptxmvalidate` utility can be used to verify correctness of the PTXM implementations for a particular target device.


## License

Licensed under the MIT License.
