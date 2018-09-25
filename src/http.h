/*****************************************************************************
#    uStreamer - Lightweight and fast MJPG-HTTP streamer.                    #
#                                                                            #
#    Copyright (C) 2018  Maxim Devaev <mdevaev@gmail.com>                    #
#                                                                            #
#    This program is free software: you can redistribute it and/or modify    #
#    it under the terms of the GNU General Public License as published by    #
#    the Free Software Foundation, either version 3 of the License, or       #
#    (at your option) any later version.                                     #
#                                                                            #
#    This program is distributed in the hope that it will be useful,         #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of          #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           #
#    GNU General Public License for more details.                            #
#                                                                            #
#    You should have received a copy of the GNU General Public License       #
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.  #
#                                                                            #
*****************************************************************************/


#include <stdbool.h>
#include <time.h>

#include <event2/event.h>
#include <event2/http.h>

#include "tools.h"
#include "stream.h"


struct stream_client_t {
	struct http_server_t	*server;
	struct evhttp_request	*request;
	bool					need_initial;

	struct stream_client_t	*prev;
	struct stream_client_t	*next;
};

struct exposed_t {
	struct picture_t	picture;
	unsigned			width;
	unsigned			height;
	bool				online;
};

struct http_server_runtime_t {
	struct event_base		*base;
	struct evhttp			*http;
	struct event			*refresh;
	struct stream_t			*stream;
	struct exposed_t		*exposed;
	struct timeval			refresh_interval;

	struct stream_client_t	*stream_clients;
};

struct http_server_t {
	char		*host;
	unsigned	port;
	unsigned	timeout;

	struct http_server_runtime_t *run;
};


struct http_server_t *http_server_init(struct stream_t *stream);
void http_server_destroy(struct http_server_t *server);

int http_server_listen(struct http_server_t *server);
void http_server_loop(struct http_server_t *server);
void http_server_loop_break(struct http_server_t *server);