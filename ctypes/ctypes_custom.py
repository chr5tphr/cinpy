import ctypes

# load shared object
somelib = ctypes.CDLL('./somelib.so')

# use our c-function 'print_version'
somelib.print_version()

#================================

# define c-array type of size 10
TenDoubles = ctypes.c_double * 10

# create c-type unsigned int of array length (here 10)
length = ctypes.c_uint(10)

# create new double c-array
arr_x = TenDoubles(*range(10))

# set the return type of our c-function (as default, int is assumed, so we have to change that)
csum = somelib.sum
csum.restype = ctypes.c_double

# use our c-function 'sum' on that array
ret = csum(length, arr_x)
print(ret)

#================================

# create another array
arr_y = TenDoubles(*range(0,20,2))

# and another one to store the output
arr_out = TenDoubles()

# now finally use our c-function 'add'
somelib.add(length, arr_x, arr_y, arr_out)
print(list(arr_out))
