#include <array>

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

TEST(Set, iteration) {
    auto expected = py::list::pack(0_p, 1_p, 2_p);
    py::list::object actual(0);
    auto ob = py::set::pack(0_p, 1_p, 2_p);
    std::size_t n = 0;

    ASSERT_EQ(ob.len(), 3);
    for (const auto &e : ob) {
        ASSERT_TRUE((std::is_same<decltype(e), const py::object&>::value)) <<
            "const iteration over ob does not yield correct type";
        actual.append(e);
        n++;
    }
    ASSERT_EQ(n, 3u) << "ran through too many iterations";
    EXPECT_EQ(PyList_Sort(actual), 0);
    EXPECT_EQ(expected, actual);
}

TEST(Set, uniqueness) {
    auto expected = py::set::pack(0_p, 1_p, 2_p);
    auto ob = py::set::pack(0_p, 1_p, 2_p, 0_p, 1_p);
    std::size_t n = 0;

    ASSERT_EQ(ob.len(), 3);
    ASSERT_EQ(ob, expected);
    for (const auto &e : ob) {
        ASSERT_TRUE((std::is_same<decltype(e), const py::object&>::value)) <<
            "const iteration over ob does not yield correct type";
        n++;
    }
    EXPECT_EQ(n, 3u) << "ran through too many iterations";
}
