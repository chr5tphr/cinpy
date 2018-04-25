#!/usr/bin/env python3
from setuptools import setup, Extension

setup(
    ext_modules=[
        Extension(
            'minext',
            ['src/minext.c']
        ),
        #Extension(
        #    'gpuman',
        #    ['src/gpuman.c'],
        #    include_dirs=['/usr/local/cuda/include'],
        #    libraries=['nvidia-ml'],
        #    # WARNING driver branch can be other than 384!!
        #    library_dirs=['/usr/lib/nvidia-384']
        #)
    ]
)

