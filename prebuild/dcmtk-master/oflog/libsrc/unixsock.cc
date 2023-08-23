// Module:  Log4CPLUS
// File:    socket-unix.cxx
// Created: 4/2003
// Author:  Tad E. Smith
//
//
// Copyright 2003-2010 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "dcmtk/oflog/config.h"
#if defined (DCMTK_LOG4CPLUS_USE_BSD_SOCKETS)

#include <cstring>
#include "dcmtk/ofstd/ofvector.h"
#include <algorithm>
#include <cerrno>
#include "dcmtk/oflog/internal/socket.h"
#include "dcmtk/oflog/helpers/loglog.h"
#include "dcmtk/oflog/thread/syncpub.h"
#include "dcmtk/oflog/spi/logevent.h"
#include "dcmtk/oflog/helpers/strhelp.h"

#ifdef DCMTK_LOG4CPLUS_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef DCMTK_LOG4CPLUS_HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if defined (DCMTK_LOG4CPLUS_HAVE_NETINET_IN_H)
#include <netinet/in.h>
#endif

#if defined (DCMTK_LOG4CPLUS_HAVE_NETINET_TCP_H)
#include <netinet/tcp.h>
#endif

#if defined (DCMTK_LOG4CPLUS_HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#endif
 
#if defined (DCMTK_LOG4CPLUS_HAVE_ERRNO_H)
#include <errno.h>
#endif

#ifdef DCMTK_LOG4CPLUS_HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef DCMTK_LOG4CPLUS_HAVE_UNISTD_H
#include <unistd.h>
#endif

// helper methods to fix old-style casts warnings
BEGIN_EXTERN_C
static unsigned short OFhtons(unsigned short us) { return htons(us); }
static in_addr_t OFinaddr_any() { return INADDR_ANY; }
END_EXTERN_C

namespace dcmtk {
namespace log4cplus { namespace helpers {


namespace
{


#if ! defined (DCMTK_LOG4CPLUS_SINGLE_THREADED)
// We need to use log4cplus::thread here to work around compilation
// problem on AIX.
static log4cplus::thread::Mutex ghbn_mutex;

#endif


static
int
get_host_by_name (char const * hostname, STD_NAMESPACE string * name,
    struct sockaddr_in * addr)
{
#if defined (DCMTK_LOG4CPLUS_HAVE_GETADDRINFO)
    struct addrinfo hints;
    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_CANONNAME;

    if (inet_addr (hostname) != OFstatic_cast(in_addr_t, -1))
        hints.ai_flags |= AI_NUMERICHOST;

    struct addrinfo * res = 0;
    int ret = getaddrinfo (hostname, 0, &hints, &res);
    if (ret != 0)
        return ret;

    struct addrinfo const & ai = *res;
    assert (ai.ai_family == AF_INET);
    
    if (name)
        *name = ai.ai_canonname;

    if (addr)
        memcpy (addr, ai.ai_addr, ai.ai_addrlen);

    freeaddrinfo (res);

#else
    #if ! defined (DCMTK_LOG4CPLUS_SINGLE_THREADED)
    // We need to use log4cplus::thread here to work around
    // compilation problem on AIX.
    log4cplus::thread::MutexGuard guard (ghbn_mutex);

    #endif

    struct ::hostent * hp = gethostbyname (hostname);
    if (! hp)
        return 1;
    assert (hp->h_addrtype == AF_INET);

    if (name)
        *name = hp->h_name;

    if (addr)
    {
        assert (hp->h_length <= sizeof (addr->sin_addr));
        memcpy (&addr->sin_addr, hp->h_addr_list[0], hp->h_length);
    }

#endif

    return 0;
}


} // namespace


/////////////////////////////////////////////////////////////////////////////
// Global Methods
/////////////////////////////////////////////////////////////////////////////

SOCKET_TYPE
openSocket(unsigned short port, SocketState& state)
{
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        return INVALID_SOCKET_VALUE;
    }

    struct sockaddr_in server = sockaddr_in ();
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = OFinaddr_any();
    server.sin_port = OFhtons(port);

    int optval = 1;
    socklen_t optlen = sizeof (optval);
    int ret = setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &optval, optlen );
    if (ret != 0)
    {
        helpers::getLogLog ().warn ("setsockopt() failed: "
            + helpers::convertIntegerToString (errno));
    }

    int retval = bind(sock, OFreinterpret_cast(struct sockaddr*, &server),
        sizeof(server));
    if (retval < 0)
        goto error;

    if (::listen(sock, 10))
        goto error;

    state = ok;
    return to_log4cplus_socket (sock);

error:
    close (sock);
    return INVALID_SOCKET_VALUE;
}


SOCKET_TYPE
connectSocket(const tstring& hostn, unsigned short port, bool udp, SocketState& state)
{
    struct sockaddr_in server;
    int sock;
    int retval;

    memset (&server, 0, sizeof (server));
    retval = get_host_by_name (DCMTK_LOG4CPLUS_TSTRING_TO_STRING(hostn).c_str(),
        0, &server);
    if (retval != 0)
        return INVALID_SOCKET_VALUE;

    server.sin_port = OFhtons(port);
    server.sin_family = AF_INET;

    sock = ::socket(AF_INET, (udp ? SOCK_DGRAM : SOCK_STREAM), 0);
    if(sock < 0) {
        return INVALID_SOCKET_VALUE;
    }

    socklen_t namelen = sizeof (server);
    while (
        (retval = ::connect(sock, OFreinterpret_cast(struct sockaddr*, &server),
            namelen))
        == -1
        && (errno == EINTR))
        ;
    if (retval == INVALID_OS_SOCKET_VALUE) 
    {
        ::close(sock);
        return INVALID_SOCKET_VALUE;
    }

    state = ok;
    return to_log4cplus_socket (sock);
}


namespace
{

//! Helper for accept_wrap().
template <typename T, typename U>
struct socklen_var
{
    typedef T type;
};


template <typename U>
struct socklen_var<void, U>
{
    typedef U type;
};


// Some systems like HP-UX have socklen_t but accept() does not use it
// as type of its 3rd parameter. This wrapper works around this
// incompatibility.
template <typename accept_sockaddr_ptr_type, typename accept_socklen_type>
static
SOCKET_TYPE
accept_wrap (
    int (* accept_func) (int, accept_sockaddr_ptr_type, accept_socklen_type *),
    SOCKET_TYPE sock, struct sockaddr * sa, socklen_t * len)
{
    typedef typename socklen_var<accept_socklen_type, socklen_t>::type
        socklen_var_type;
    socklen_var_type l = OFstatic_cast(socklen_var_type, *len);
    SOCKET_TYPE result
        = OFstatic_cast(SOCKET_TYPE,
            accept_func (to_os_socket(sock), sa,
                OFreinterpret_cast(accept_socklen_type *, &l)));
    *len = OFstatic_cast(socklen_t, l);
    return result;
}


} // namespace


SOCKET_TYPE
acceptSocket(SOCKET_TYPE sock, SocketState& state)
{
    struct sockaddr_in net_client;
    socklen_t len = sizeof(struct sockaddr);
    int clientSock;

    while(
        (clientSock = OFstatic_cast(int, accept_wrap (accept, sock,
            OFreinterpret_cast(struct sockaddr*, &net_client), &len)))
        == -1
        && (errno == EINTR))
        ;

    if(clientSock != INVALID_OS_SOCKET_VALUE) {
        state = ok;
    }

    return to_log4cplus_socket (clientSock);
}



int
closeSocket(SOCKET_TYPE sock)
{
    return ::close(to_os_socket (sock));
}



long
read(SOCKET_TYPE sock, SocketBuffer& buffer)
{
    long res, readbytes = 0;
 
    do
    { 
        res = ::read(to_os_socket (sock), buffer.getBuffer() + readbytes,
            buffer.getMaxSize() - readbytes);
        if( res <= 0 ) {
            return res;
        }
        readbytes += res;
    } while( readbytes < OFstatic_cast(long, buffer.getMaxSize()) );
 
    return readbytes;
}



long
write(SOCKET_TYPE sock, const SocketBuffer& buffer)
{
#if defined(MSG_NOSIGNAL)
    int flags = MSG_NOSIGNAL;
#else
    int flags = 0;
#endif
    return ::send( to_os_socket (sock), buffer.getBuffer(), buffer.getSize(),
        flags );
}


long
write(SOCKET_TYPE sock, const STD_NAMESPACE string & buffer)
{
#if defined(MSG_NOSIGNAL)
    int flags = MSG_NOSIGNAL;
#else
    int flags = 0;
#endif
    return ::send (to_os_socket (sock), buffer.c_str (), buffer.size (),
        flags);
}


tstring
getHostname (bool fqdn)
{
    char const * hostname = "unknown";
    int ret;
    OFVector<char> hn (1024, 0);

    while (true)
    {
        ret = ::gethostname (&hn[0], OFstatic_cast(int, hn.size ()) - 1);
        if (ret == 0)
        {
            hostname = &hn[0];
            break;
        }
#if defined (DCMTK_LOG4CPLUS_HAVE_ENAMETOOLONG)
        else if (ret != 0 && errno == ENAMETOOLONG)
            // Out buffer was too short. Retry with buffer twice the size.
            hn.resize (hn.size () * 2, 0);
#endif
        else
            break;
    }

    if (ret != 0 || (ret == 0 && ! fqdn))
        return DCMTK_LOG4CPLUS_STRING_TO_TSTRING (hostname);

    STD_NAMESPACE string full_hostname;
    ret = get_host_by_name (hostname, &full_hostname, 0);
    if (ret == 0)
        hostname = full_hostname.c_str ();

    return DCMTK_LOG4CPLUS_STRING_TO_TSTRING (hostname);
}


int
setTCPNoDelay (SOCKET_TYPE sock, bool val)
{
#if (defined (SOL_TCP) || defined (IPPROTO_TCP)) && defined (TCP_NODELAY)
#if defined (SOL_TCP)
    int level = SOL_TCP;

#elif defined (IPPROTO_TCP)
    int level = IPPROTO_TCP;

#endif

    int result;
    int enabled = OFstatic_cast(int, val);
    if ((result = setsockopt(to_os_socket(sock), level, TCP_NODELAY, &enabled,
                sizeof(enabled))) != 0)
        set_last_socket_error (errno);
    
    return result;

#else
    // No recognizable TCP_NODELAY option.
    return 0;

#endif
}


} } // namespace log4cplus
} // end namespace dcmtk

#endif // DCMTK_LOG4CPLUS_USE_BSD_SOCKETS
