//
// Copyright (c) 2019-2023 Ruben Perez Hidalgo (rubenperez038 at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_MYSQL_HANDSHAKE_PARAMS_HPP
#define BOOST_MYSQL_HANDSHAKE_PARAMS_HPP

#include <boost/mysql/buffer_params.hpp>
#include <boost/mysql/collation.hpp>
#include <boost/mysql/string_view.hpp>

namespace boost {
namespace mysql {

/// Determines whether to use TLS for the connection or not; ignored if the underlying stream is not
/// SSL-capable.
enum class ssl_mode
{
    /// Never use TLS
    disable,

    /// Use TLS if the server supports it, fall back to non-encrypted connection if it does not.
    enable,

    /// Always use TLS; abort the connection if the server does not support it.
    require
};

/**
 * \brief Parameters defining how to perform the handshake with a MySQL server.
 */
class handshake_params
{
    string_view username_;
    string_view password_;
    string_view database_;
    collation connection_collation_;
    ssl_mode ssl_;
    buffer_params buffer_config_;

public:
    /**
     * \brief Initializing constructor.
     * \param username User name to authenticate as.
     * \param password Password for that username, possibly empty.
     * \param db Database name to use, or empty string for no database (this is the default).
     * \param connection_col The \ref collation to use for the connection.
     * Impacts how text queries and prepared statements are interpreted. Defaults to
     * `collation::utf8mb4_general_ci`.
     * \param mode The \ref ssl_mode to use with this connection; ignored if
     * the connection does not support SSL.
     * \param buffer_config Configuration about internal buffers.
     */
    handshake_params(
        string_view username,
        string_view password,
        string_view db = "",
        collation connection_col = collation::utf8mb4_general_ci,
        ssl_mode mode = ssl_mode::require,
        const buffer_params& buffer_config = {}
    )
        : username_(username),
          password_(password),
          database_(db),
          connection_collation_(connection_col),
          ssl_(mode),
          buffer_config_(buffer_config)
    {
    }

    /// Retrieves the username.
    string_view username() const noexcept { return username_; }

    /// Sets the username.
    void set_username(string_view value) noexcept { username_ = value; }

    /// Retrieves the password.
    string_view password() const noexcept { return password_; }

    /// Sets the password
    void set_password(string_view value) noexcept { password_ = value; }

    /// Retrieves the database.
    string_view database() const noexcept { return database_; }

    /// Sets the database
    void set_database(string_view value) noexcept { database_ = value; }

    /// Retrieves the connection collation.
    collation connection_collation() const noexcept { return connection_collation_; }

    /// Sets the connection collation
    void set_connection_collation(collation value) noexcept { connection_collation_ = value; }

    /// Retrieves SSL mode
    ssl_mode ssl() const noexcept { return ssl_; }

    /// Sets SSL mode
    void set_ssl(ssl_mode value) noexcept { ssl_ = value; }

    /// Retrieves the buffer configuration.
    buffer_params buffer_config() const noexcept { return buffer_config_; }

    /// Sets the buffer configuration.
    void set_buffer_config(const buffer_params& value) noexcept { buffer_config_ = value; }
};

}  // namespace mysql
}  // namespace boost

#endif
