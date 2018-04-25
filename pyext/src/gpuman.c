#include <nvml.h>
#include "Python.h"

static PyObject *NVMLError;

/*
 * Return a list of ids of idleing NVidia GPU devices
 */
static PyObject * gpuman_nvidia_idle(PyObject *self, PyObject *args) {
    // pointer to hold the list of our available devices
    PyObject *retlist = NULL;

    // parse arguments
    if (!PyArg_ParseTuple(args,"")){
        // if PyArg_ParseTuple failed, error indicator will already be set
        return NULL;
    }

    // create a new Python list, we gain ownership of the instance
    retlist = PyList_New(0);

    // if creation failed
    if (!retlist){
        // error indicator is already set by failed PyList_New
        return NULL;
    }

    // some variables for use with nvml
    nvmlDevice_t handle;
    nvmlReturn_t retval;

    // initialize nvml
    retval = nvmlInit();

    // if unsuccessful
    if (retval != NVML_SUCCESS) {
        // raise exception, we have to set the error indicator ourselves
        PyErr_SetString(NVMLError, nvmlErrorString(retval));
        return NULL;
    }

    // get number of installed devices
    unsigned int numdev = 0;
    retval = nvmlDeviceGetCount(&numdev);

    // if there are no devices installed
    if (retval != NVML_SUCCESS) {
        // clean up and return our empty list
        // we could raise an error here, but our empty list is enough to
        // indicate that there are no devices available
        nvmlShutdown();
        return retlist;
    }

    unsigned int infoCount = 0;
    unsigned int i;

    // loop over all devices
    for (i=0;i<numdev;i++){
        // get nvml device-handle from device number
        nvmlDeviceGetHandleByIndex(i,&handle);

        // gets the number of running processes of the device
        // when infoCount=0 and infos is NULL, returns NVML_SUCCESS if no
        // processes are currently running
        retval = nvmlDeviceGetComputeRunningProcesses(handle,&infoCount,NULL);

        // if no processes are running on the device
        if (retval == NVML_SUCCESS) {
            // create python long
            PyObject *pydev = PyLong_FromLong(i);

            // check whether pydev was successfully created
            // and then append to retlist (and fail if unsuccessfull)
            // note that PyList_Append returns -1 on failure and 0 on success
            if (!pydev || PyList_Append(retlist,pydev)) {
                // clean up nvml
                nvmlShutdown();

                // we need to decrease the reference to retlist here
                // so it can be collected by Python's garbage collector
                Py_DECREF(retlist);

                // error indicator was already set by either PyLong_FromLong
                // or PyList_Append
                return NULL;
            }
        }
    }

    // clean up nvml
    nvmlShutdown();

    // return list of available devices
    return retlist;
}

// array of method definitions for our module, terminated by empty definition struct instance
static PyMethodDef methods[] = {
    {"nvidia_idle", gpuman_nvidia_idle, METH_VARARGS, "Get ids of idleing (unoccupied) nvidia gpu devices." },
    {NULL, NULL, 0, NULL}
};

// struct instance to define our module
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "gpuman",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_gpuman() {

    // initialize the Python interpreter
    Py_Initialize();

    // create our new module
    PyObject *m = PyModule_Create(&module_def);
    if (m == NULL){
        return NULL;
    }

    // create a new exception, we become the owner
    NVMLError = PyErr_NewException("gpuman.NVMLError", NULL, NULL);

    // increase reference, we retain our own copy because someone
    // could possibly remove the exception from the module and
    // NVMLError would become otherwise a dangling pointer
    Py_INCREF(NVMLError);
    PyModule_AddObject(m, "NVMLError", NVMLError);

    return m;
}
