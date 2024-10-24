from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        'fourier_transform',
        ['fourier_transform.cpp'],
        include_dirs=[pybind11.get_include()],
        language='c++'
    ),
]

setup(
    name='fourier_transform',
    ext_modules=ext_modules,
)
