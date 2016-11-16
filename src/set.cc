#include "libpy/set.h"
#include "libpy/utils.h"

const py::type::object<py::set::object>
py::set::type(reinterpret_cast<PyObject*>(&PySet_Type));

py::set::object::object() : py::object() {}

py::set::object::object(PyObject *pob) : py::object(pob) {
    set_check();
}

py::set::object::object(const py::object &pob) : py::object(pob) {
    set_check();
}

py::set::object::object(const py::set::object &cpfrom) : py::object(cpfrom.ob) {}

py::set::object::object(py::set::object &&mvfrom) noexcept : py::object(mvfrom.ob) {
    mvfrom.ob = nullptr;
}

void py::set::object::set_check() {
    if (ob && !PySet_Check(ob)) {
        ob = nullptr;
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_TypeError,
                            "cannot make py::set::object from non set");
        }
    }
}
