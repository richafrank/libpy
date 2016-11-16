
#include "gtest/gtest.h"
#include <Python.h>

#include "libpy/libpy.h"
#include "utils.h"

using py::operator""_p;

TEST(Set, type) {
    ASSERT_EQ(static_cast<PyObject*>(py::set::type),
              reinterpret_cast<PyObject*>(&PySet_Type));
    auto t = py::set::type();

    EXPECT_EQ(static_cast<PyObject*>(t.type()),
              reinterpret_cast<PyObject*>(&PySet_Type));
}