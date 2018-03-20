#include <nvml.h>
#include "Python.h"

static PyObject *NVMLError;

static PyObject * gpuman_nvidia_idle(PyObject *self, PyObject *args) {
  PyObject *retlist = NULL;

  // parse arguments
  if (!PyArg_ParseTuple(args,"")){
    return NULL;
  }

  retlist = PyList_New(0);
  if (!retlist){
      return NULL;
  }
  Py_INCREF(retlist);

  nvmlDevice_t handle;
  nvmlReturn_t retval;

  retval = nvmlInit();
  if (retval != NVML_SUCCESS) {
    PyErr_SetString(NVMLError, nvmlErrorString(retval));
    return NULL;
  }

  unsigned int numdev = 0;
  retval = nvmlDeviceGetCount(&numdev);
  if (retval != NVML_SUCCESS) {
    PyErr_SetString(NVMLError, nvmlErrorString(retval));
    // Py_DECREF(retlist);
    nvmlShutdown();
    return retlist;
  }

  unsigned int infoCount = 0;
  unsigned int i = 0;
  for (i=0;i<numdev;i++){
    nvmlDeviceGetHandleByIndex(i,&handle);
    retval = nvmlDeviceGetComputeRunningProcesses(handle,&infoCount,NULL);
    if (retval == NVML_SUCCESS) {
      // printf("%u\n",i);
      PyObject *pydev = PyLong_FromLong(i);
      if (!pydev) {
        nvmlShutdown();
        Py_DECREF(retlist);
        return PyErr_NoMemory();
      }
      Py_INCREF(pydev);
      PyList_Append(retlist,pydev);
    }
  }

  nvmlShutdown();

  return retlist;
}

static PyMethodDef methods[] = {
  {"nvidia_idle", gpuman_nvidia_idle, METH_VARARGS, "Get ids of idleing (unoccupied) nvidia gpu devices." },
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module_def = {
  PyModuleDef_HEAD_INIT,
  "gpuman",
  NULL,
  -1,
  methods
};

PyMODINIT_FUNC PyInit_gpuman() {
  Py_Initialize();
  PyObject *m = PyModule_Create(&module_def);
  if (m == NULL){
    return NULL;
  }
  NVMLError = PyErr_NewException("gpuman.NVMLError", NULL, NULL);
  Py_INCREF(NVMLError);
  PyModule_AddObject(m, "NVMLError", NVMLError);

  return m;
}
