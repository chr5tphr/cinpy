# Using C in Python (A quick introduction)

## Why might we want to use C with Python?
- to implement Python wrappers for libraries which do not have any (yet)
- to get some more performance out of some algorithms/steps

## What are the options?
- **Python's ctypes library**
- **Extensions for CPython**
- Cython
- CFFI
- ...?

## Using ctypes
### with some library
Imagine we want to use some function of some library, e.g. *system* of *libc*.

See [ctypes/ctypes\_lib.py](ctypes/ctypes_lib.py) for the example.

### with our own library
This case does not differ all that much from the previous case, we just need to compile our own code to a shared object.
First, we write some code, like in [ctypes/src/somelib.c](ctypes/src/somelib.c).

This code is then compiled using, for example

```
gcc -fpic -shared src/somelib.c -o somelib.so
```

will generate the shared object *somelib.so*.

We can then call our library as shown in [ctypes/ctypes\_custom.py](ctypes/ctypes_custom.py).

More information on Python's ctypes library can be found [here](https://docs.python.org/3/library/ctypes.html).

## Using Extensions for CPython
Even though CPython is the most widely used Python interpreter, the C extension interface is not supported by any other interpreter.
Thus, writing extensions for CPython is less portable than using ctypes.
Extensions for CPython require somewhat more overhead.

A small example how one could write an extension is given in [pyext/src/minext.c](pyext/src/minext.c).
Just like when building any shared object, we can either just compile our extension as following:

```
gcc -fpic -shared src/minext.c -o minext.so
```

Alternatively, we can use [setuptools](https://setuptools.readthedocs.io/en/latest/) or similarily Python's *distutils* to define a [setup.py](pyext/setup.py) to compile extensions. For our example, we would call

```
python setup.py build\_ext --inplace
```

To show some real-world example, I have also supplied an extension to list idleing GPU devices using the [Nvidia Management Library](https://developer.nvidia.com/nvidia-management-library-nvml) in [pyext/src/gpuman.c](pyext/src/gpuman.c).

More information on CPython extensions can be found [here](https://docs.python.org/3/extending/extending.html).

## Measuring Performance
To give you some feeling of how much faster it would actually be to use Numpy or some C code, I have implemented an example to compute the mandelbrot set both in Python using Numpy as well as in C with an interface in python using ctypes.

Implementations can be found in [perform/mandelperf.py](perform/mandelperf.py) and [perform/src/mandelbrot.c](perform/src/mandelbrot.c) respectively.

The C-code can be compiled using

```
gcc -fpic -shared src/mandelbrot.c -o mandelbrot.so
```

The performance test using [cProfile](https://docs.python.org/3/library/profile.html) is then executed by simply calling

```
python mandelperf.py
```

## What about Cython?
What Cython actually does is to compile Python(-like) code to a fully-fledged CPython extension in C.
Thus, all benefits we gain from using CPython extensions, we also gain from using Cython (except writing beautiful C-code of course).

More information on Cython can be found [here](http://docs.cython.org/en/latest/src/tutorial/cython_tutorial.html).

