import os
import setuptools

current_script_dir = os.path.dirname(os.path.realpath(__file__))
package_dir = current_script_dir

c_module = setuptools.Extension(
    name="cCIEDE2000.ciede2000",
    sources=["cCIEDE2000/c_module/ciede2000.c"],
)

setuptools.setup(
    name="cCIEDE2000",
    version="1.0",
    packages=["cCIEDE2000"],
    ext_modules=[c_module],
)
