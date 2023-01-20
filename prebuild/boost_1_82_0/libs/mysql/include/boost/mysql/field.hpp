//
// Copyright (c) 2019-2023 Ruben Perez Hidalgo (rubenperez038 at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_MYSQL_FIELD_HPP
#define BOOST_MYSQL_FIELD_HPP

#include <boost/mysql/blob.hpp>
#include <boost/mysql/field_kind.hpp>
#include <boost/mysql/field_view.hpp>
#include <boost/mysql/string_view.hpp>

#include <boost/mysql/detail/auxiliar/field_impl.hpp>

#include <boost/variant2/variant.hpp>

#include <cstddef>
#include <ostream>
#include <string>
#ifdef __cpp_lib_string_view
#include <string_view>
#endif

namespace boost {
namespace mysql {

/**
 * \brief Variant-like class that can represent of any of the allowed database types.
 * \details
 * This is a regular variant-like class that can represent any of the types that MySQL allows. It
 * has value semantics (as opposed to \ref field_view). Instances of this class are not created
 * by the library. They should be created by the user, when the reference semantics of
 * \ref field_view are not appropriate.
 * \n
 * Like a variant, at any point, a `field` always contains a value of
 * certain type. You can query the type using \ref kind and the `is_xxx` functions
 * like \ref is_int64. Use `as_xxx` and `get_xxx` for checked and unchecked value
 * access, respectively. You can mutate a `field` by calling the `emplace_xxx` functions, by
 * assigning it a different value, or using the lvalue references returned by `as_xxx` and
 * `get_xxx`.
 */
class field
{
public:
    /// Constructs a `field` holding NULL (`this->kind() == field_kind::null`).
    field() = default;

    /// Copy constructor.
    field(const field&) = default;

    /**
     * \brief Move constructor.
     * \details All references into `other` are invalidated, including the ones obtained by calling
     * get_xxx, as_xxx and \ref field::operator field_view().
     */
    field(field&& other) = default;

    /**
     * \brief Copy assignment.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    field& operator=(const field&) = default;

    /**
     * \brief Move assignment.
     * \details Invalidates references to `*this` obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view(). All references into `other`
     * are invalidated, including the ones obtained by calling get_xxx, as_xxx and
     * \ref field::operator field_view().
     */
    field& operator=(field&& other) = default;

    /// Destructor.
    ~field() = default;

    /**
     * \copybrief field()
     * \details
     * Caution: `field(NULL)` will __NOT__ match this overload. It will try to construct
     * a `string_view` from a NULL C string, causing undefined behavior.
     */
    explicit field(std::nullptr_t) noexcept {}

    /// Constructs a `field` holding an `int64` (`this->kind() == field_kind::int64`).
    explicit field(signed char v) noexcept : repr_(std::int64_t(v)) {}

    /// \copydoc field(signed char)
    explicit field(short v) noexcept : repr_(std::int64_t(v)) {}

    /// \copydoc field(signed char)
    explicit field(int v) noexcept : repr_(std::int64_t(v)) {}

    /// \copydoc field(signed char)
    explicit field(long v) noexcept : repr_(std::int64_t(v)) {}

    /// \copydoc field(signed char)
    explicit field(long long v) noexcept : repr_(std::int64_t(v)) {}

    /// Constructs a `field` holding a `uint64` (`this->kind() == field_kind::uint64`).
    explicit field(unsigned char v) noexcept : repr_(std::uint64_t(v)) {}

    /// \copydoc field(unsigned char)
    explicit field(unsigned short v) noexcept : repr_(std::uint64_t(v)) {}

    /// \copydoc field(unsigned char)
    explicit field(unsigned int v) noexcept : repr_(std::uint64_t(v)) {}

    /// \copydoc field(unsigned char)
    explicit field(unsigned long v) noexcept : repr_(std::uint64_t(v)) {}

    /// \copydoc field(unsigned char)
    explicit field(unsigned long long v) noexcept : repr_(std::uint64_t(v)) {}

    /// \brief Constructs a `field` holding a string (`this->kind() == field_kind::string`).
    /// \details A `std::string` is constructed internally by copying `v`.
    explicit field(const std::string& v) : repr_(v) {}

    /// \copybrief field(const std::string&)
    /// \details A `std::string` is constructed internally by moving `v`.
    explicit field(std::string&& v) noexcept : repr_(std::move(v)) {}

    /// \copybrief field(const std::string&)
    /// \details A `std::string` is constructed internally from the NULL-terminated C string `v`.
    explicit field(const char* v) : repr_(boost::variant2::in_place_type_t<std::string>(), v) {}

    /// \copybrief field(const std::string&)
    /// \details A `std::string` is constructed internally from `v`.
    explicit field(string_view v) : repr_(boost::variant2::in_place_type_t<std::string>(), v) {}
#if defined(__cpp_lib_string_view) || defined(BOOST_MYSQL_DOXYGEN)

    /// \copydoc field(string_view)
    explicit field(std::string_view v) noexcept
        : repr_(boost::variant2::in_place_type_t<std::string>(), v)
    {
    }
#endif
    /// Constructs a `field` holding a `blob` (`this->kind() == field_kind::blob`).
    explicit field(blob v) noexcept : repr_(std::move(v)) {}

    /// Constructs a `field` holding a `float` (`this->kind() == field_kind::float`).
    explicit field(float v) noexcept : repr_(v) {}

    /// Constructs a `field` holding a `double` (`this->kind() == field_kind::double`).
    explicit field(double v) noexcept : repr_(v) {}

    /// Constructs a `field` holding a `date` (`this->kind() == field_kind::date`).
    explicit field(const date& v) noexcept : repr_(v) {}

    /// Constructs a `field` holding a `datetime` (`this->kind() == field_kind::datetime`).
    explicit field(const datetime& v) noexcept : repr_(v) {}

    /// Constructs a `field` holding a `time` (`this->kind() == field_kind::time`).
    explicit field(const time& v) noexcept : repr_(v) {}

    /**
     * \brief Constructs a `field` from a \ref field_view.
     * \details The resulting `field` has the same kind and value as the original `field_view`. The
     * resulting `field` is guaranteed to be valid even after `v` becomes invalid.
     */
    field(const field_view& v) { from_view(v); }

    /**
     * \copydoc emplace_null
     * \details Equivalent to \ref emplace_null.
     */
    field& operator=(std::nullptr_t) noexcept
    {
        emplace_null();
        return *this;
    }

    /**
     * \copydoc emplace_int64
     * \details Equivalent to \ref emplace_int64.
     */
    field& operator=(signed char v) noexcept
    {
        emplace_int64(v);
        return *this;
    }

    /// \copydoc operator=(signed char)
    field& operator=(short v) noexcept
    {
        emplace_int64(v);
        return *this;
    }

    /// \copydoc operator=(signed char)
    field& operator=(int v) noexcept
    {
        emplace_int64(v);
        return *this;
    }

    /// \copydoc operator=(signed char)
    field& operator=(long v) noexcept
    {
        emplace_int64(v);
        return *this;
    }

    /// \copydoc operator=(signed char)
    field& operator=(long long v) noexcept
    {
        emplace_int64(v);
        return *this;
    }

    /**
     * \copydoc emplace_uint64
     * \details Equivalent to \ref emplace_uint64.
     */
    field& operator=(unsigned char v) noexcept
    {
        emplace_uint64(v);
        return *this;
    }

    /// \copydoc operator=(unsigned char)
    field& operator=(unsigned short v) noexcept
    {
        emplace_uint64(v);
        return *this;
    }

    /// \copydoc operator=(unsigned char)
    field& operator=(unsigned int v) noexcept
    {
        emplace_uint64(v);
        return *this;
    }

    /// \copydoc operator=(unsigned char)
    field& operator=(unsigned long v) noexcept
    {
        emplace_uint64(v);
        return *this;
    }

    /// \copydoc operator=(unsigned char)
    field& operator=(unsigned long long v) noexcept
    {
        emplace_uint64(v);
        return *this;
    }

    /**
     * \copydoc emplace_string
     * \details Equivalent to \ref emplace_string.
     */
    field& operator=(const std::string& v)
    {
        emplace_string(v);
        return *this;
    }

    /// \copydoc operator=(const std::string&)
    field& operator=(std::string&& v)
    {
        emplace_string(std::move(v));
        return *this;
    }

    /// \copydoc operator=(const std::string&)
    field& operator=(const char* v)
    {
        emplace_string(v);
        return *this;
    }

    /// \copydoc operator=(const std::string&)
    field& operator=(string_view v)
    {
        emplace_string(v);
        return *this;
    }

#if defined(__cpp_lib_string_view) || defined(BOOST_MYSQL_DOXYGEN)
    /// \copydoc operator=(const std::string&)
    field& operator=(std::string_view v)
    {
        emplace_string(v);
        return *this;
    }
#endif

    /**
     * \copydoc emplace_blob
     * \details Equivalent to \ref emplace_blob.
     */
    field& operator=(blob v)
    {
        emplace_blob(std::move(v));
        return *this;
    }

    /**
     * \copydoc emplace_float
     * \details Equivalent to \ref emplace_float.
     */
    field& operator=(float v) noexcept
    {
        emplace_float(v);
        return *this;
    }

    /**
     * \copydoc emplace_double
     * \details Equivalent to \ref emplace_double.
     */
    field& operator=(double v) noexcept
    {
        emplace_double(v);
        return *this;
    }

    /**
     * \copydoc emplace_date
     * \details Equivalent to \ref emplace_date.
     */
    field& operator=(const date& v) noexcept
    {
        emplace_date(v);
        return *this;
    }

    /**
     * \copydoc emplace_datetime
     * \details Equivalent to \ref emplace_datetime.
     */
    field& operator=(const datetime& v) noexcept
    {
        emplace_datetime(v);
        return *this;
    }

    /**
     * \copydoc emplace_time
     * \details Equivalent to \ref emplace_time.
     */
    field& operator=(const time& v) noexcept
    {
        emplace_time(v);
        return *this;
    }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `v.kind()` and destroying any previous
     * contents.
     * \details Invalidates references to `*this` obtained by as_xxx and get_xxx functions, but not
     * the ones obtained by \ref field::operator field_view().
     *\n
     * `*this` is guaranteed to be valid even after `v` becomes invalid.
     */
    field& operator=(const field_view& v)
    {
        from_view(v);
        return *this;
    }

    /// Returns the type of the value this `field` is holding.
    field_kind kind() const noexcept { return repr_.kind(); }

    /// Returns whether this `field` is holding a `NULL` value.
    bool is_null() const noexcept { return kind() == field_kind::null; }

    /// Returns whether this `field` is holding an int64 value.
    bool is_int64() const noexcept { return kind() == field_kind::int64; }

    /// Returns whether this `field` is holding a uint64 value.
    bool is_uint64() const noexcept { return kind() == field_kind::uint64; }

    /// Returns whether this `field` is holding a string value.
    bool is_string() const noexcept { return kind() == field_kind::string; }

    /// Returns whether this `field` is holding a blob value.
    bool is_blob() const noexcept { return kind() == field_kind::blob; }

    /// Returns whether this `field` is holding a float value.
    bool is_float() const noexcept { return kind() == field_kind::float_; }

    /// Returns whether this `field` is holding a double value.
    bool is_double() const noexcept { return kind() == field_kind::double_; }

    /// Returns whether this `field` is holding a date value.
    bool is_date() const noexcept { return kind() == field_kind::date; }

    /// Returns whether this `field` is holding a datetime value.
    bool is_datetime() const noexcept { return kind() == field_kind::datetime; }

    /// Returns whether this `field` is holding a time value.
    bool is_time() const noexcept { return kind() == field_kind::time; }

    /**
     * \brief Retrieves a reference to the underlying `std::int64_t` value or throws an exception.
     * \details If `!this->is_int64()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const std::int64_t& as_int64() const { return repr_.as<std::int64_t>(); }

    /**
     * \brief Retrieves a reference to the underlying `std::uint64_t` value or throws an exception.
     * \details If `!this->is_uint64()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const std::uint64_t& as_uint64() const { return repr_.as<std::uint64_t>(); }

    /**
     * \brief Retrieves a reference to the underlying `std::string` value or throws an exception.
     * \details If `!this->is_string()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const std::string& as_string() const { return repr_.as<std::string>(); }

    /**
     * \brief Retrieves a reference to the underlying `blob` value or throws an exception.
     * \details If `!this->is_blob()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const blob& as_blob() const { return repr_.as<blob>(); }

    /**
     * \brief Retrieves a reference to the underlying `float` value or throws an exception.
     * \details If `!this->is_float()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const float& as_float() const { return repr_.as<float>(); }

    /**
     * \brief Retrieves a reference to the underlying `double` value or throws an exception.
     * \details If `!this->is_double()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const double& as_double() const { return repr_.as<double>(); }

    /**
     * \brief Retrieves a reference to the underlying `date` value or throws an exception.
     * \details If `!this->is_date()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const date& as_date() const { return repr_.as<date>(); }

    /**
     * \brief Retrieves a reference to the underlying `datetime` value or throws an exception.
     * \details If `!this->is_datetime()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const datetime& as_datetime() const { return repr_.as<datetime>(); }

    /**
     * \brief Retrieves a reference to the underlying `time` value or throws an exception.
     * \details If `!this->is_time()`, throws \ref bad_field_access.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const time& as_time() const { return repr_.as<time>(); }

    /// \copydoc as_int64
    std::int64_t& as_int64() { return repr_.as<std::int64_t>(); }

    /// \copydoc as_uint64
    std::uint64_t& as_uint64() { return repr_.as<std::uint64_t>(); }

    /// \copydoc as_string
    std::string& as_string() { return repr_.as<std::string>(); }

    /// \copydoc as_blob
    blob& as_blob() { return repr_.as<blob>(); }

    /// \copydoc as_float
    float& as_float() { return repr_.as<float>(); }

    /// \copydoc as_double
    double& as_double() { return repr_.as<double>(); }

    /// \copydoc as_date
    date& as_date() { return repr_.as<date>(); }

    /// \copydoc as_datetime
    datetime& as_datetime() { return repr_.as<datetime>(); }

    /// \copydoc as_time
    time& as_time() { return repr_.as<time>(); }

    /**
     * \brief Retrieves a reference to the underlying `std::int64_t` value (unchecked access).
     * \details If `!this->is_int64()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const std::int64_t& get_int64() const noexcept { return repr_.get<std::int64_t>(); }

    /**
     * \brief Retrieves a reference to the underlying `std::uint64_t` value (unchecked access).
     * \details If `!this->is_uint64()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const std::uint64_t& get_uint64() const noexcept { return repr_.get<std::uint64_t>(); }

    /**
     * \brief Retrieves a reference to the underlying `std::string` value (unchecked access).
     * \details If `!this->is_string()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const std::string& get_string() const noexcept { return repr_.get<std::string>(); }

    /**
     * \brief Retrieves a reference to the underlying `blob` value (unchecked access).
     * \details If `!this->is_blob()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const blob& get_blob() const noexcept { return repr_.get<blob>(); }

    /**
     * \brief Retrieves a reference to the underlying `float` value (unchecked access).
     * \details If `!this->is_float()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const float& get_float() const noexcept { return repr_.get<float>(); }

    /**
     * \brief Retrieves a reference to the underlying `double` value (unchecked access).
     * \details If `!this->is_double()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const double& get_double() const noexcept { return repr_.get<double>(); }

    /**
     * \brief Retrieves a reference to the underlying `date` value (unchecked access).
     * \details If `!this->is_date()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const date& get_date() const noexcept { return repr_.get<date>(); }

    /**
     * \brief Retrieves a reference to the underlying `datetime` value (unchecked access).
     * \details If `!this->is_datetime()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const datetime& get_datetime() const noexcept { return repr_.get<datetime>(); }

    /**
     * \brief Retrieves a reference to the underlying `time` value (unchecked access).
     * \details If `!this->is_time()`, results in undefined behavior.
     *
     * The returned reference is valid as long as `*this` is alive and no function that invalidates
     * references is called on `*this`.
     */
    const time& get_time() const noexcept { return repr_.get<time>(); }

    /// \copydoc get_int64
    std::int64_t& get_int64() noexcept { return repr_.get<std::int64_t>(); }

    /// \copydoc get_uint64
    std::uint64_t& get_uint64() noexcept { return repr_.get<std::uint64_t>(); }

    /// \copydoc get_string
    std::string& get_string() noexcept { return repr_.get<std::string>(); }

    /// \copydoc get_blob
    blob& get_blob() noexcept { return repr_.get<blob>(); }

    /// \copydoc get_float
    float& get_float() noexcept { return repr_.get<float>(); }

    /// \copydoc get_double
    double& get_double() noexcept { return repr_.get<double>(); }

    /// \copydoc get_date
    date& get_date() noexcept { return repr_.get<date>(); }

    /// \copydoc get_datetime
    datetime& get_datetime() noexcept { return repr_.get<datetime>(); }

    /// \copydoc get_time
    time& get_time() noexcept { return repr_.get<time>(); }

    /**
     * \brief Replaces `*this` with a `NULL`, changing the kind to `null` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_null() noexcept { repr_.data.emplace<detail::field_impl::null_t>(); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `int64` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_int64(std::int64_t v) noexcept { repr_.data.emplace<std::int64_t>(v); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `uint64` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_uint64(std::uint64_t v) noexcept { repr_.data.emplace<std::uint64_t>(v); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `string` and destroying any previous
     * contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_string(const std::string& v) { repr_.data.emplace<std::string>(v); }

    /// \copydoc emplace_string(const std::string&)
    void emplace_string(std::string&& v) { repr_.data.emplace<std::string>(std::move(v)); }

    /// \copydoc emplace_string(const std::string&)
    void emplace_string(const char* v) { repr_.data.emplace<std::string>(v); }

    /// \copydoc emplace_string(const std::string&)
    void emplace_string(string_view v) { repr_.data.emplace<std::string>(v); }

#if defined(__cpp_lib_string_view) || defined(BOOST_MYSQL_DOXYGEN)
    /// \copydoc emplace_string(const std::string&)
    void emplace_string(std::string_view v) { repr_.data.emplace<std::string>(v); }
#endif

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `blob` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_blob(blob v) { repr_.data.emplace<blob>(std::move(v)); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `float_` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_float(float v) noexcept { repr_.data.emplace<float>(v); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `double` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_double(double v) noexcept { repr_.data.emplace<double>(v); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `date` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_date(const date& v) noexcept { repr_.data.emplace<date>(v); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `datetime` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions,
     * but not the ones obtained by \ref field::operator field_view().
     */
    void emplace_datetime(const datetime& v) noexcept { repr_.data.emplace<datetime>(v); }

    /**
     * \brief Replaces `*this` with `v`, changing the kind to `time` and destroying any
     * previous contents.
     * \details Invalidates references obtained by as_xxx and get_xxx functions, but not
     */
    void emplace_time(const time& v) noexcept { repr_.data.emplace<time>(v); }

    /**
     * \brief Constructs a \ref field_view pointing to `*this`.
     * \details The resulting `field_view` has the same kind and value as `*this`. It acts as a
     * reference to `*this`, and will be valid as long as `*this` is alive.
     */
    inline operator field_view() const noexcept { return field_view(&repr_); }

private:
    detail::field_impl repr_;

    inline void from_view(const field_view& v);
};

/**
 * \relates field
 * \brief Tests for equality.
 * \details The same considerations as \ref field_view::operator== apply.
 */
inline bool operator==(const field& lhs, const field& rhs) noexcept
{
    return field_view(lhs) == field_view(rhs);
}

/**
 * \relates field
 * \brief Tests for inequality.
 */
inline bool operator!=(const field& lhs, const field& rhs) noexcept { return !(lhs == rhs); }

/**
 * \relates field
 * \brief Tests for equality.
 * \details The same considerations as \ref field_view::operator== apply.
 */
inline bool operator==(const field_view& lhs, const field& rhs) noexcept
{
    return lhs == field_view(rhs);
}

/**
 * \relates field
 * \brief Tests for inequality.
 */
inline bool operator!=(const field_view& lhs, const field& rhs) noexcept { return !(lhs == rhs); }

/**
 * \relates field
 * \brief Tests for equality.
 * \details The same considerations as \ref field_view::operator== apply.
 */
inline bool operator==(const field& lhs, const field_view& rhs) noexcept
{
    return field_view(lhs) == rhs;
}

/**
 * \relates field
 * \brief Tests for inequality.
 */
inline bool operator!=(const field& lhs, const field_view& rhs) noexcept { return !(lhs == rhs); }

/**
 * \relates field
 * \brief Streams a `field`.
 */
inline std::ostream& operator<<(std::ostream& os, const field& v);

}  // namespace mysql
}  // namespace boost

#include <boost/mysql/impl/field.hpp>

#endif
