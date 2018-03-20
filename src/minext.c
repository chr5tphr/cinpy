#include <stdlib.h>
#include <Python.h>

#define VERSION "0.01 alpha"

static PyObject * minext_version(PyObject *self, PyObject *args) {
  // parse arguments
  if (!PyArg_ParseTuple(args,"")){
    return NULL;
  }
  char *version = VERSION;
  PyObject *pystr = PyUnicode_FromString(version);

  return pystr;
}

static PyObject * minext_system(PyObject *self, PyObject *args) {
    const char *command;
    int retval;

    if (!PyArg_ParseTuple(args, "s", &command)){
        return NULL;
    }

    retval = system(command);
    return PyLong_FromLong(retval);
}

static PyMethodDef methods[] = {
  {"system", minext_system, METH_VARARGS, "Executes some command." },
  {"version", minext_version, METH_VARARGS, "Return some version Info." },
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module_def = {
  PyModuleDef_HEAD_INIT,
  "minext",
  NULL,
  -1,
  methods
};

PyMODINIT_FUNC PyInit_minext() {
  Py_Initialize();
  PyObject *m = PyModule_Create(&module_def);
  if (m == NULL){
    return NULL;
  }

  return m;
}
