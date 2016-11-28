#include "libpy/set.h"
#include "libpy/utils.h"

namespace s = py::set;
namespace f = py::frozenset;

const py::type::object<s::object>
s::type(reinterpret_cast<PyObject*>(&PySet_Type));

const py::type::object<f::object>
f::type(reinterpret_cast<PyObject*>(&PyFrozenSet_Type));

py::anyset::object::object() : py::object() {}

py::anyset::object::object(PyObject *pob) : py::object(pob) {
}

py::anyset::object::object(const py::object &pob) : py::object(pob) {
}

py::anyset::object::object(const py::anyset::object &cpfrom) : py::object(cpfrom.ob) {}

py::anyset::object::object(py::anyset::object &&mvfrom) noexcept : py::object(mvfrom.ob) {
    mvfrom.ob = nullptr;
}

py::ssize_t py::anyset::object::len() const {
    if (!is_nonnull()) {
        pyutils::failed_null_check();
        return -1;
    }
    return PySet_GET_SIZE(ob);
}

py::nonnull<py::anyset::object> py::anyset::object::as_nonnull() const {
    if (!is_nonnull()) {
        throw pyutils::bad_nonnull();
    }
    return nonnull<py::anyset::object>(ob);
}

py::tmpref<py::anyset::object> py::anyset::object::as_tmpref() && {
    tmpref<py::anyset::object> ret(ob);
    ob = nullptr;
    return ret;
}
