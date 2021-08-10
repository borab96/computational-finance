from setuptools import setup
from Cython.Build import cythonize

setup(
    name='recursive option price',
    ext_modules=cythonize("option_price.pyx"),
    zip_safe=False,
)
