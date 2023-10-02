# cCIEDE2000

Python package for computing the CIE Delta Empfindung with a C submodule.

## What this Code does

This Code is a Python wrapper around a C implementation of the CIEDE2000 algorithm.

## Documentation

### Installation

This project is not on PyPI.  
Please drop me a line / open an issue if you would consider it useful to be included there.

```bash
pip install git+https://github.com/jkschluesener/cCIEDE2000.git@main``
```

### Import

```python
from cCIEDE2000 import deltaE_min, deltaE_matrix
```

### deltaE_min

```python
# both gamut and pixel should be in CIELAB colorspace.
gamut = np.random.rand(2**24, 3) # pseudo screen gamut
pixel = np.random.rand(3) # pseudo pixel

lowest_deltaE: int = deltaE_min(gamut, pixel)
```

### deltaE_matrix

```python
from numpy.typing import ArrayLike # only for typing

# both gamut and pixel should be in CIELAB colorspace.
gamut = np.random.rand(2**24, 3) # pseudo screen gamut
pixel = np.random.rand(3) # pseudo pixel

deltaE_px_gamut: ArrayLike = deltaE_min(gamut, pixel)
```

## Motivation

Motivation for this small project:

- Exploring the possibility of a faster implementation of the CIEDE2000 algorithm for another project
- Comparing the execution speed and memory consumption of a Python wrapper around C, compared to a vectorized Numpy function
- Create a blueprint for how to wrap a C function in Python, handing over pointers to arrays and compiling C through python on `pip install`

:thinking: Check `benchmarking.ipynb` for benchmarking results.  
Short Summary: Non-exhaustive evaluation showed that the C implementation is faster and more memory efficient than the vectorized numpy function `deltaE_ciede2000` from scikit-image, at least for very large color gamuts.

## Attribution

The C functions herein were taken from:
<https://github.com/gfiumara/CIEDE2000/>

MIT License

Commit Hash `af1de42515f3916c16e75980a4635962489af56a` from `2017-Oct-26`, accessed `2023-Sep-26`.

I did some minor modifications of the code and included some C wrapper functions.
