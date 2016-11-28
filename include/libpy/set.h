#pragma once
#include <tuple>

#include "libpy/object.h"
#include "libpy/type.h"
#include "libpy/list.h"

namespace py {
namespace anyset {
/**
   A subclass of `py::object` for optional sets and frozensets.
*/
class object : public py::object {
protected:
    friend class py::tmpref<object>;

    /**
       Default constructor. This will set `ob` to nullptr.
    */
    object();

    /**
       Constructor from `PyObject*`. If `pob` is not a `set` or `frozenset` then
       `ob` will be set to `nullptr`.
    */
    object(PyObject *pob);

    /**
       Constructor from `py::object`. If `pob` is not a `set` or `frozenset` then
       `ob` will be set to `nullptr`.
    */
    object(const py::object &pob);

    object(const object &cpfrom);
    object(object &&mvfrom) noexcept;
public:
    using py::object::operator=;

    /**
       Get the length of the object.

       This is equivalent to `len(this)`.

       @return The length of the object or -1 if an exception occurred.
    */
    py::ssize_t len() const;


    /**
       Add an element to a _set.

       @param elem The element to add.
       @return -1 on failure, otherwise zero.
    */
    template<typename T>
    int add(const T &elem) {
        if (!pyutils::all_nonnull(*this, elem)) {
            pyutils::failed_null_check();
            return -1;
        }
        return PySet_Add(*this, elem);
    }

    template<typename T>
    int remove(const T &elem) {
        if (!pyutils::all_nonnull(*this, elem)) {
            pyutils::failed_null_check();
            return -1;
        }
    	return PySet_Discard(*this, elem);
    }

    inline int clear() {
    	return PySet_Clear(*this);
    }

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

template<int *check_func(PyObject*), typename T>
inline int check_py(const T &t) {
    if (!t.is_nonnull()) {
        pyutils::failed_null_check();
        return -1;
    }
    return check_func(t);
}

int pyanyset_check(PyObject* ob) {
	return PyAnySet_Check(ob);
}

/**
   Check if an object is an instance of `set` or `frozenset`.

   @param t The object to check
   @return  1 if `ob` is an instance of `set` or `frozenset`, 0 if `ob` is not an
            instance of `set` or `frozenset`, -1 if an exception occurred.
*/
template<typename T>
inline int check(const T &t) {
	return check_py<pyanyset_check>(t);
}

/**
   Check if an object is an instance of `set` or `frozenset` but not a subclass.

   @param t The object to check
   @return  1 if `ob` is an instance of `set` or `frozenset`, 0 if `ob` is not an
            instance of `set` or `frozenset`, -1 if an exception occurred.
*/
template<typename T>
inline int checkexact(const T &t) {
    if (!t.is_nonnull()) {
        pyutils::failed_null_check();
        return -1;
    }
    return PyAnySet_CheckExact(t);
}


template<PyObject* new_func(PyObject*), typename T>
inline PyObject* new_set(const T &iterable) {
	if (!iterable.is_nonnull()) {
        pyutils::failed_null_check();
        return nullptr;
    }
    return new_func(iterable);
}
}

/**
   A `py::list::object` where `ob` is known to be nonnull.
   This is used to skip null checks for performance.

   This class should be used where users want to trade the ability to
   write a nested expression for performance.
*/
template<>
class nonnull<anyset::object> : public anyset::object {
protected:
    nonnull() = delete;
    explicit nonnull(PyObject *ob) : anyset::object(ob) {}
public:
    friend class object;

    nonnull(const nonnull &cpfrom) : anyset::object(cpfrom) {}
    nonnull(nonnull &&mvfrom) noexcept : anyset::object(mvfrom.ob) {
        mvfrom.ob = nullptr;
    }

    nonnull &operator=(const nonnull &cpfrom) {
        nonnull<anyset::object> tmp(cpfrom);
        return (*this = std::move(tmp));
    }

    nonnull &operator=(nonnull &&mvfrom) noexcept {
        ob = mvfrom.ob;
        mvfrom.ob = nullptr;
        return *this;
    }

    /**
       Get the length of the object.

       This is equivalent to `len(this)`.

       @return The length of the object or -1 if an exception occurred.
    */
    py::ssize_t len() const {
        return PySet_GET_SIZE(ob);
    }
};

namespace set {
/**
   A subclass of `py::object` for optional sets.
*/
class object : public anyset::object {
public:
	object(PyObject* iterable) : anyset::object(anyset::new_set<PySet_New>(iterable)) { }

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

inline int pyset_check(PyObject* ob) {
	return PySet_Check(ob);
}

/**
   Check if an object is an instance of `set`.

   @param t The object to check
   @return  1 if `ob` is an instance of `set`, 0 if `ob` is not an
            instance of `set`, -1 if an exception occurred.
*/
template<typename T>
inline int check(const T &t) {
	return anyset::check_py<pyset_check>(t);
}

/**
   Pack variadic arguments into a Python `list` object.

   @param elems The elements to pack.
   @return      The elements packed as a Python `list`.
*/
template<typename... Ts>
tmpref<object> pack(const Ts&... elems) {
	py::list::object l = py::list::pack(elems...);

    if (!l.is_nonnull()) {
        return nullptr;
    }
    object s(l);

    return s.as_nonnull();
}
}

namespace frozenset {
/**
   A subclass of `py::object` for optional frozensets.
*/
class object : public anyset::object {
public:
	object(PyObject* iterable) : anyset::object(anyset::new_set<PyFrozenSet_New>(iterable)) { }

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
   The type of Python `frozenset` objects.

   This is equivalent to: `frozenset`.
*/
extern const type::object<frozenset::object> type;

inline int pyfrozenset_check(PyObject* ob) {
	return PyFrozenSet_Check(ob);
}

/**
   Check if an object is an instance of `frozenset`.

   @param t The object to check
   @return  1 if `ob` is an instance of `frozenset`, 0 if `ob` is not an
            instance of `frozenset`, -1 if an exception occurred.
*/
template<typename T>
inline int check(const T &t) {
	return anyset::check_py<pyfrozenset_check>(t);
}
}
}
