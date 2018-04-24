import ctypes

# load shared object of glibc
glibc = ctypes.CDLL('/lib/libc.so.6')

# load path into c-type char-array
path = ctypes.c_char_p(b'/bin/sh')

# execute 'system' from glibc
glibc.system(path)
