#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
import ctypes
import cProfile

def pymandel(shape, maxiter):
    inds = np.add.outer(np.linspace(-1.j,1.j,shape[1]), np.linspace(-1.5,0.5,shape[0]))
    vals = np.zeros(shape,dtype=np.complex128)
    col = np.zeros(shape)
    active = np.ones(shape,dtype=bool)
    for i in range(maxiter):
        z = vals[active]**2 + inds[active]
        z[~np.isfinite(z)] = 3
        cond = np.abs(z) < 2
        active[active] *= cond
        vals[active] = z[cond]
        col[active] += active[active]

    return col

def cmandel(shape, maxiter):
    mbrot = ctypes.CDLL('./mandelbrot.so')
    height = ctypes.c_uint(shape[0])
    width = ctypes.c_uint(shape[1])
    maxiter = ctypes.c_uint(maxiter)
    Arr = ctypes.c_uint * (shape[0] * shape[1])
    out = Arr(0)
    mbrot.mandelbrot(height, width, maxiter, out)
    narr = np.array(out).reshape(shape)
    return narr

if __name__ == '__main__':
    print('Running profiler...')
    maxiter = 1000
    shape = (1024,1024)
    print('Mandelbrot with numpy:')
    cProfile.run('pyarr = pymandel(shape, maxiter)')
    print('\nMandelbrot with C:')
    cProfile.run('carr = cmandel(shape, maxiter)')

    plt.figure()
    plt.subplot(121)
    plt.title('Numpy')
    plt.imshow(pyarr)
    plt.subplot(122)
    plt.title('C')
    plt.imshow(carr)
    plt.show()


