# SEQLIB

sequential structures library for c
- structures like vector, queue, list etc.
- generic algorithm for sequential structures, like sort and bsearch etc.
- using customized allocator for different structures

## Deps
- [cmth](https://github.com/9io9/cmth)
- [aLocas](https://github.com/9io9/aLocas)

## Getting Start

```bash
> mkdir -p build
> cd build
> cmake .. -Dcmth_DIR=absolute/path/of/cmth/library> -DaLocas_DIR=absolute/path/of/aLocas/library
> make
# wait for compilation
> cd ..
> cmake --install build --prefix path/of/install
```