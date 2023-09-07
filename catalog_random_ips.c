/*
	A numerically ordered linked-list queue written in ANSI C
	Copyright (C) 2023 Brett Kuskie <fullaxx@gmail.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>

#include "orderqueue.h"

int g_shutdown = 0;

static void alarm_handler(int signum)
{
/*
	(void) alarm(1);
*/
}

static void sig_handler(int signum)
{
	switch(signum) {
		case SIGHUP:
		case SIGINT:
		case SIGTERM:
		case SIGQUIT:
			g_shutdown = 1;
			break;
	}
}

static void print_and_destroy(oqnode_t *p)
{
	uint32_t ip;
	char ip_str[16];

	if(!p) { return; }
	ip = (uint32_t)p->num;
	ip = htonl(ip);
	inet_ntop(AF_INET, &ip, ip_str, sizeof(ip_str));
	printf("%15s\n", ip_str);
	destroy_orphan(p);
}

static void add_random(long limit)
{
	oqtype_t ip;

	/* Generate a random number */
	ip = (rand() << 31) | rand();
	ip &= 0xFFFFFFFF;
	ip = ntohl(ip);

	/* Insert data package into the pipeline */
	(void)pipeline_insert(0, ip, 1);
	if(pipeline_count() > limit) { g_shutdown = 1; }
}

int main(int argc, char *argv[])
{
	oqnode_t *p;

	if(argc != 2) { fprintf(stderr, "%s: <limit>\n", argv[0]); return 1; }

	srand(time(NULL));

	signal(SIGINT,	sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGQUIT,	sig_handler);
	signal(SIGHUP,	sig_handler);
	signal(SIGALRM, alarm_handler);
	(void) alarm(1);

	long limit = atol(argv[1]);
	while(!g_shutdown) { add_random(limit); }

	printf("\n");

#ifdef ASCENDING
	while( (p = pipeline_pop(OQ_LOWEST)) ) {
		print_and_destroy(p);
	}
#endif

#ifdef DESCENDING
	while( (p = pipeline_pop(OQ_HIGHEST)) ) {
		print_and_destroy(p);
	}
#endif

	return 0;
}
