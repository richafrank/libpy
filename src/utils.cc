#include <Python.h>

#include "libpy/utils.h"

void pyutils::failed_null_check() {
    if (!PyErr_Occurred()) {
        PyErr_SetString(PyExc_AssertionError, "failed null check");
    }
}
