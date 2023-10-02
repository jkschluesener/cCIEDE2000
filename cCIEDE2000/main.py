"""
This module provides functions to compute the CIE deltaE (delta Empfindung)
between a gamut and a pixel in CIELAB colorspace.

Functions
---------
get_library()
    Get the c library
deltaE_matrix(gamut: ArrayLike, pixel: ArrayLike) -> ArrayLike
    Compute the deltaE between a gamut and a pixel
deltaE_min(gamut: ArrayLike, pixel: ArrayLike) -> int
    Find the index in gamut with the lowest deltaE.

Jan K. Schlüsener
https://github.com/jkschluesener/cCIEDE2000

MIT License
"""

import os
import site
import sysconfig
import ctypes
import numpy as np
from numpy.typing import ArrayLike

def get_library() -> ctypes.CDLL:
    """ Returns a ctypes.CDLL object representing the ciede2000 shared library.

    The function constructs the path to the cCIEDE2000 package's directory within site-packages. 
    Using the platform-specific shared library suffix, it loads the shared library using ctypes.CDLL
    and returns the resulting object.

    Returns
    -------
    ctypes.CDLL
        A ctypes.CDLL object representing the ciede2000 shared library.
    """

    site_packages_dir = site.getsitepackages()[0]
    package_dir = os.path.join(site_packages_dir, 'cCIEDE2000')

    library_suffix = sysconfig.get_config_var('EXT_SUFFIX')
    library_path = f'{package_dir}/ciede2000{library_suffix}'

    c_library = ctypes.CDLL(library_path)

    return c_library

def deltaE_matrix(gamut: ArrayLike, pixel: ArrayLike) -> ArrayLike:
    """Compute the deltaE between a gamut and a pixel

    Parameters
    ----------
    gamut : ArrayLike
        gamut to which deltaE will be computed in CIELAB colorspace.
    pixel : ArrayLike
        Pixel to which deltaE will be computed in CIELAB colorspace.

    Returns
    -------
    ArrayLike
        Array of deltaE values
    """    

    # ensure that `gamut` is a numpy array of type double
    gamut = gamut.astype(np.float64)

    # ensure that `pixel` is a numpy array of type double
    pixel = pixel.astype(np.float64)

    # create an empty numpy array of type double to store the output
    # the pointer will be passed to the c function, which will fill it
    output = np.zeros((gamut.shape[0], 1), dtype=np.float64)

    # get the c library
    c_library = get_library()

    # call the c function
    c_library.deltaE_matrix(
        gamut.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        ctypes.c_int(gamut.shape[0]),
        pixel.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        output.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    )

    return output

def deltaE_min(gamut: ArrayLike, pixel: ArrayLike) -> int:
    """Find the index in gamut with the lowest deltaE.

    Parameters
    ----------
    gamut : ArrayLike
        gamut to which deltaE will be computed in CIELAB colorspace.
    pixel : ArrayLike
        Pixel to which deltaE will be computed in CIELAB colorspace.

    Returns
    -------
    int
        Index of the row with the lowest deltaE
    """

    # ensure that `gamut` is a numpy array of type double
    gamut = gamut.astype(np.float64)

    # ensure that `pixel` is a numpy array of type double
    pixel = pixel.astype(np.float64)

    # get the library
    c_library = get_library()

    # call the c function
    lowest_row = c_library.deltaE_min(
        gamut.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        ctypes.c_int(gamut.shape[0]),
        pixel.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    )

    return lowest_row
