//
// Copyright (c) 2019-2023 Ruben Perez Hidalgo (rubenperez038 at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_MYSQL_DATETIME_HPP
#define BOOST_MYSQL_DATETIME_HPP

#include <boost/mysql/days.hpp>

#include <boost/config.hpp>

#include <chrono>
#include <cstdint>
#include <iosfwd>
#include <ratio>

namespace boost {
namespace mysql {

/**
 * \brief Type representing MySQL `__DATETIME__` and `__TIMESTAMP__` data types.
 * \details Represents a broken datetime by its year, month, day, hour, minute, second and
 * microsecond components. This type is close to the protocol and should not be used as a vocabulary
 * type. Instead, cast it to a `std::chrono::time_point` by calling \ref as_time_point or \ref
 * get_time_point.
 *\n
 * As opposed to `time_point`, this type allows representing invalid and zero datetimes.
 */
class datetime
{
public:
    /**
     * \brief A `std::chrono::time_point` that can represent any valid datetime.
     * \details Represents microseconds, with the same precision for all architectures.
     */
    using time_point_type = std::chrono::
        time_point<std::chrono::system_clock, std::chrono::duration<std::int64_t, std::micro>>;

    /**
     * \brief Constructs a zero datetime.
     * \details Results in a datetime with all of its components set to zero.
     */
    constexpr datetime() noexcept = default;

    /**
     * \brief Constructs a datetime from its individual components.
     */
    constexpr datetime(
        std::uint16_t year,
        std::uint8_t month,
        std::uint8_t day,
        std::uint8_t hour = 0,
        std::uint8_t minute = 0,
        std::uint8_t second = 0,
        std::uint32_t microsecond = 0
    ) noexcept
        : year_(year),
          month_(month),
          day_(day),
          hour_(hour),
          minute_(minute),
          second_(second),
          microsecond_(microsecond)
    {
    }

    /**
     * \brief Constructs a datetime from a `time_point`.
     * \details If the supplied `time_point` would result in a `datetime` object
     * out of the [\ref min_datetime, \ref max_datetime] range, throws `std::out_of_range`.
     */
    BOOST_CXX14_CONSTEXPR inline explicit datetime(time_point_type tp);

    /**
     * \brief Retrieves the year component.
     */
    constexpr std::uint16_t year() const noexcept { return year_; }

    /**
     * \brief Retrieves the month component.
     */
    constexpr std::uint8_t month() const noexcept { return month_; }

    /**
     * \brief Retrieves the day component.
     */
    constexpr std::uint8_t day() const noexcept { return day_; }

    /**
     * \brief Retrieves the hour component.
     */
    constexpr std::uint8_t hour() const noexcept { return hour_; }

    /**
     * \brief Retrieves the minute component.
     */
    constexpr std::uint8_t minute() const noexcept { return minute_; }

    /**
     * \brief Retrieves the second component.
     */
    constexpr std::uint8_t second() const noexcept { return second_; }

    /**
     * \brief Retrieves the microsecond component.
     */
    constexpr std::uint32_t microsecond() const noexcept { return microsecond_; }

    /**
     * \brief Returns `true` if `*this` represents a valid `time_point`.
     * \details If any of the individual components is out of range, the datetime
     * doesn't represent an actual `time_point` (e.g. `datetime(2020, 2, 30)`) or
     * the datetime is not in the [\ref min_date, \ref max_date] validity range,
     * returns `false`. Otherwise, returns `true`.
     */
    constexpr bool valid() const noexcept;

    /**
     * \brief Converts `*this` into a `time_point` (unchecked access).
     * \details If `!this->valid()`, results in undefined behavior.
     */
    BOOST_CXX14_CONSTEXPR inline time_point_type get_time_point() const noexcept;

    /**
     * \brief Converts `*this` into a `time_point` (checked access).
     * \details If `!this->valid()`, throws `std::invalid_argument`.
     */
    BOOST_CXX14_CONSTEXPR inline time_point_type as_time_point() const;

    /**
     * \brief Tests for equality.
     * \details Two datetimes are considered equal if all of its individual components
     * are equal. This function works for invalid datetimes, too.
     */
    constexpr bool operator==(const datetime& rhs) const noexcept;

    /**
     * \brief Tests for inequality.
     */
    constexpr bool operator!=(const datetime& rhs) const noexcept { return !(*this == rhs); }

private:
    std::uint16_t year_{};
    std::uint8_t month_{};
    std::uint8_t day_{};
    std::uint8_t hour_{};
    std::uint8_t minute_{};
    std::uint8_t second_{};
    std::uint32_t microsecond_{};

    BOOST_CXX14_CONSTEXPR inline time_point_type unch_get_time_point() const noexcept;
};

/**
 * \relates datetime
 * \brief Streams a datetime.
 * \details This function works for invalid datetimes, too.
 */
inline std::ostream& operator<<(std::ostream& os, const datetime& v);

/// The minimum allowed value for \ref datetime.
constexpr datetime min_datetime(0u, 1u, 1u);

/// The maximum allowed value for \ref datetime.
constexpr datetime max_datetime(9999u, 12u, 31u, 23u, 59u, 59u, 999999u);

}  // namespace mysql
}  // namespace boost

#include <boost/mysql/impl/datetime.hpp>

#endif
