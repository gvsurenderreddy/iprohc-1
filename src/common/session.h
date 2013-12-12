/*
 * This file is part of iprohc.
 *
 * iprohc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.
 *
 * iprohc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with iprohc.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file   session.h
 * @brief  The generic part of the session shared by server and client
 * @author Didier Barvaux <didier.barvaux@toulouse.viveris.com>
 */

#ifndef IPROHC_COMMON_SESSION__H
#define IPROHC_COMMON_SESSION__H

#include "rohc_tunnel.h"

#include <netinet/in.h>
#include <pthread.h>
#include <gnutls/gnutls.h>


/** The different session statuses */
typedef enum
{
	IPROHC_SESSION_PENDING_DELETE  = 0,  /**< The session can be deleted */
	IPROHC_SESSION_CONNECTING      = 1,  /**< The session is currently connecting */
	IPROHC_SESSION_CONNECTED       = 2,  /**< The session successfully connected */
} iprohc_session_status_t;


/** The generic part of the session shared by server and client */
struct iprohc_session
{
	int tcp_socket;                /**< The TCP socket for the control channel */
	gnutls_session_t tls_session;  /**< The TLS session for the control channel */
	struct in_addr local_address;  /**< The local address on the TUN interface */

	struct tunnel tunnel;          /**< The tunnel context */
	pthread_t thread_tunnel;       /**< The thread that handle the session */
	/**
	 * @brief The client lock
	 *
	 * Protect the client context against deletion by main thread when used by
	 * client thread
	 */
	pthread_mutex_t client_lock;

	/**
	 * @brief The status lock
	 *
	 * Protect the client status and keepalive timestamp against concurrent
	 * accesses by main thread and client thread.
	 */
	pthread_mutex_t status_lock;
	iprohc_session_status_t status;  /**< The session status */
	struct timeval last_keepalive;   /**< The time at which last keepalive was sent */
};

#endif
