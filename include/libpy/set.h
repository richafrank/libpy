#pragma once

#include "libpy/object.h"
#include "libpy/type.h"

namespace py {
namespace set {
/**
   A subclass of `py::object` for optional sets.
*/
class object : public py::object {
private:
    /**
       Function called to verify that `ob` is a set and
       correctly raise a python exception otherwies.
    */
    void set_check();
public:
    friend class py::tmpref<object>;

    /**
       Default constructor. This will set `ob` to nullptr.
    */
    object();

    /**
       Constructor from `PyObject*`. If `pob` is not a `set` then
       `ob` will be set to `nullptr`.
    */
    object(PyObject *pob);

    /**
       Constructor from `py::object`. If `pob` is not a `set` then
       `ob` will be set to `nullptr`.
    */
    object(const py::object &pob);

    object(const object &cpfrom);
    object(object &&mvfrom) noexcept;

    using py::object::operator=;

    /**
       Get the length of the object.

       This is equivalent to `len(this)`.

       @return The length of the object or -1 if an exception occured.
    */
    py::ssize_t len() const;

    /**
       Coerce to a `nonnull` object.

       @see nonnull
       @throws pyutil::bad_nonnull Thrown when `ob == nullptr`.
       @return this converted to a `nonnull` object.
    */
    nonnull<object> as_nonnull() const;

    /**
       Create a temporary reference. This is a reference that will
       decref the object when it is destroyed.

       @return this converted into a tmpref.
    */
    tmpref<object> as_tmpref() &&;
};

/**
   The type of Python `set` objects.

   This is equivalent to: `set`.
*/
extern const type::object<set::object> type;

/**
   Check if an object is an instance of `set`.

   @param t The object to check
   @return  1 if `ob` is an instance of `set` or `frozenset`, 0 if `ob` is not an
            instance of `set` or `frozenset`, -1 if an exception occured.
*/
template<typename T>
inline int check(const T &t) {
    if (!t.is_nonnull()) {
        pyutils::failed_null_check();
        return -1;
    }
    return PySet_Check(t);
}

inline int check(const nonnull<object>&) {
    return 1;
}

/**
   Check if an object is an instance of `set` or `frozenset` but not a subclass.

   @param t The object to check
   @return  1 if `ob` is an instance of `set` or `frozenset`, 0 if `ob` is not an
            instance of `set` or `frozenset`, -1 if an exception occured.
*/
template<typename T>
inline int checkexact(const T &t) {
    if (!t.is_nonnull()) {
        pyutils::failed_null_check();
        return -1;
    }
    return PyAnySet_CheckExact(t);
}

inline int checkexact(const nonnull<object>&) {
    return 1;
}
}
}