#!/bin/bash

set -e

CFLAGS="-Wall -ansi -D_GNU_SOURCE"
OPTCFLAGS="${CFLAGS} -O2"
DBGCFLAGS="${CFLAGS} -ggdb3 -DDEBUG"

rm -f *.dbg

gcc ${DBGCFLAGS} -DORDERQUEUE_LONG  -DASCENDING -DDEBUG_INSERT catalog_random_ips.c orderqueue.c -o catalog_ips_ascending.dbg
gcc ${DBGCFLAGS} -DORDERQUEUE_LONG -DDESCENDING -DDEBUG_INSERT catalog_random_ips.c orderqueue.c -o catalog_ips_descending.dbg
