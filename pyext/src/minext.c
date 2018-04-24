#include <stdlib.h>
#include <Python.h>

#define VERSION "0.01 alpha"

/* 
 * Give some version information in a Python-String 
 */
static PyObject * minext_version(PyObject *self, PyObject *args) {
    // we do not need to parse arguments, since we do not have any 
    // and declared this method with METH_NOARGS
    // args is therefore NULL in any case

    // our constant c-string containing the version info
    const char *version = VERSION;
    // before we can pass the string to python, we need to create a Python-String object
    PyObject *pystr = PyUnicode_FromString(version);

    return pystr;
}

/*
 * Execute some system command provided as a string
 */
static PyObject * minext_system(PyObject *self, PyObject *args) {
    // pointer to the first element of our command string
    const char *command;

    // the return value of the command
    int retval;

    // parse arguments
    if (!PyArg_ParseTuple(args, "s", &command)){
        return NULL;
    }

    // call systemfunction of libc with supplied command
    retval = system(command);

    // return return value as Python long
    return PyLong_FromLong(retval);
}

// array of method definitions for our module, terminated by empty definition struct instance
static PyMethodDef methods[] = {
    // { name of method, pointer to c impl, flag bits to indicate how call is constructed, docstring }
    {"system", minext_system, METH_VARARGS, "Executes some command." },
    {"version", minext_version, METH_NOARGS, "Return some version Info." },
    {NULL, NULL, 0, NULL}
};

// struct instance to define our module
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT, // always initialized to this value
    "minext", // name for the new module
    NULL, // docstring for the module, created with PtDoc_STRVAR()
    -1, // module state memory size, -1 to not support sub-interpreters (global state only)
    methods // array of functions for the new module
};

PyMODINIT_FUNC PyInit_minext() {
    // initialize the Python interpreter
    Py_Initialize();

    // create our new module
    PyObject *m = PyModule_Create(&module_def);
    if (m == NULL){
        // an error will be raised, already indicated by PyModule_Create
        return NULL;
    }

    // return our new module
    return m;
}
