/*
 * Copyright (C) 2014-2015 Bing Sun <b.y.sun.cn@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public Licens
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */


#ifndef CACHE_CONN_H
#define CACHE_CONN_H

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/rcupdate.h>
#include <linux/completion.h>
#include <linux/list.h>
#include <linux/spinlock.h>

#include <linux/time.h>
#include <linux/kthread.h>

#include <linux/net.h>
#include <linux/socket.h>
#include <linux/tcp.h>

#include "../cache_def.h"
#include "../cache_dbg.h"


/* magic numbers used in meta data and network packets */
#define CACHE_MAGIC 0x835a

struct p_header80 {
	u16	  magic;
	u16	  command;
	u32	  length;	/* bytes of data after this header */
} __packed;

struct p_data {
	u64	    sector;    /* 64 bits sector number */
	u64	    block_id;
	u32	    seq_num;
	u32	    dp_flags;
} __packed;

struct p_block_ack {
	u64	    sector;
	u64	    block_id;
	u32	    blksize;
	u32	    seq_num;
} __packed;

struct p_block_wrote {
	u32	    seq_num;
	u32	    pad;
} __packed;

struct p_wrote_ack {
	u32	    seq_num;
	u32      pad;
} __packed;

enum cache_packet {
	P_DATA		      = 0x00,
	P_DATA_WRITTEN	      = 0x01, /* Used to delete data block written */
	P_DATA_ACK	      = 0x02, /* Response to P_DATA */ 
	P_WRITTEN_ACK	      = 0x03, /* Response to P_DATA_WRITTEN */ 
	
	/* special command ids for handshake */
	P_INITIAL_META	      = 0xfff1, /* First Packet on the MetaSock */
	P_INITIAL_DATA	      = 0xfff2, /* First Packet on the Socket */
};

struct packet_info {
	enum cache_packet cmd;
	int size;
	int vnr;
	void *data;
};

struct cache_work_queue {
	struct list_head q;
	spinlock_t q_lock;  /* to protect the list. */
	wait_queue_head_t q_wait;
};

struct cache_work {
	struct list_head list;
	int (*cb)(struct cache_work *);
};

struct cache_request{
	u32	    seq_num;
	struct list_head list;
	struct completion done;

	struct cache_connection *connection;
	struct cache_socket *cache_socket;
};

struct accept_wait_data {
	struct cache_connection *connection;
	struct socket *s_listen;
	struct completion door_bell;
	void (*original_sk_state_change)(struct sock *sk);

};

enum cache_thread_state {
	NONE,
	RUNNING,
	EXITING,
	RESTARTING
};

struct cache_socket{
	struct mutex mutex;
	struct socket    *socket;
	/* this way we get our
	 * send/receive buffers off the stack */
	void *sbuf;
	void *rbuf;
};

struct cache_thread{
	spinlock_t t_lock;
	struct task_struct *task;
	struct completion start;
	struct completion stop;
	enum cache_thread_state t_state;
	int (*function) (struct cache_thread *);
	struct cache_connection *connection;
	const char *name;	
};

struct cache_epoch {
	struct cache_connection *connection;
	struct list_head list;
	unsigned int barrier_nr;
	atomic_t epoch_size; /* increased on every request added. */
	atomic_t active;     /* increased on every req. added, and dec on every finished. */
	unsigned long flags;
};

/* The order of these constants is important.
 * The lower ones (<C_WF_REPORT_PARAMS) indicate
 * that there is no socket!
 * >=C_WF_REPORT_PARAMS ==> There is a socket
 */
enum cache_conn_state {
	C_STANDALONE,
	C_DISCONNECTING,  /* Temporal state on the way to StandAlone. */
	C_UNCONNECTED,    /* >= C_UNCONNECTED -> inc_net() succeeds */

	/* These temporal states are all used on the way
	 * from >= C_CONNECTED to Unconnected.
	 * The 'disconnect reason' states
	 * I do not allow to change between them. */
	C_TIMEOUT,
	C_BROKEN_PIPE,
	C_NETWORK_FAILURE,
	C_PROTOCOL_ERROR,
	C_TEAR_DOWN,

	C_WF_CONNECTION,
	C_WF_REPORT_PARAMS, /* we have a socket */
	C_CONNECTED,      /* we have introduced each other */
	C_STARTING_SYNC_S,  /* starting full sync by admin request. */
	C_STARTING_SYNC_T,  /* starting full sync by admin request. */
	C_WF_BITMAP_S,
	C_WF_BITMAP_T,
	C_WF_SYNC_UUID,

	/* All SyncStates are tested with this comparison
	 * xx >= C_SYNC_SOURCE && xx <= C_PAUSED_SYNC_T */
	C_SYNC_SOURCE,
	C_SYNC_TARGET,
	C_VERIFY_S,
	C_VERIFY_T,
	C_PAUSED_SYNC_S,
	C_PAUSED_SYNC_T,

	C_AHEAD,
	C_BEHIND,

	C_MASK = 31
};

struct cache_connection{
	struct dcache *dcache;
	struct list_head connections;
	enum cache_conn_state cstate;
	struct mutex cstate_mutex;	/* Protects graceful disconnects */
	struct kref kref;
	
	unsigned long flags;
	struct net_conf *net_conf;	/* content protected by rcu */
	wait_queue_head_t ping_wait;	/* Woken upon reception of a ping, and a state change */

	struct cache_socket data;		/* data/barrier/cstate/parameter packets */
	struct cache_socket meta;	/* ping/ack (metadata) packets */

	struct sockaddr_storage my_addr;
	int my_addr_len;
	struct sockaddr_storage peer_addr;
	int peer_addr_len;
	
	struct cache_thread receiver; /* used for receive data*/
	struct cache_thread worker;
	struct cache_thread asender; /* used for data ack and wrote index */

	atomic_t packet_seq;
	/* sender side */
	struct cache_work_queue sender_work;
	int ko_count;

	/* receiver side */
	struct cache_epoch *current_epoch;
	spinlock_t epoch_lock;
	unsigned int epochs;
	unsigned long last_received;

	struct list_head request_list;
	spinlock_t request_lock;
	atomic_t nr_cmnds;
};

struct data_cmd {
	int expect_payload;
	size_t pkt_size;
	int (*fn)(struct cache_connection *, struct packet_info *);
};

static inline enum cache_thread_state get_t_state(struct cache_thread *thi)
{
	/* THINK testing the t_state seems to be uncritical in all cases
	 * (but thread_{start,stop}), so we can read it *without* the lock.
	 *	--lge */

	smp_rmb();
	return thi->t_state;
}

static inline void cache_tcp_nodelay(struct socket *sock)
{
	int val = 1;
	(void) kernel_setsockopt(sock, SOL_TCP, TCP_NODELAY,
			(char*)&val, sizeof(val));
}

int cache_thread_start(struct cache_thread *thi);
void _cache_thread_stop(struct cache_thread *thi, int restart, int wait);
int cache_receiver(struct cache_thread *thi);
int cache_worker(struct cache_thread *thi);

static inline void cache_thread_stop(struct cache_thread *thi)
{
	_cache_thread_stop(thi, false, true);
}

static inline void cache_thread_stop_nowait(struct cache_thread *thi)
{
	_cache_thread_stop(thi, false, false);
}

static inline void cache_thread_restart_nowait(struct cache_thread *thi)
{
	_cache_thread_stop(thi, true, false);
}

unsigned int cache_header_size(struct cache_connection *conn);

int receive_first_packet(struct cache_connection *connection, struct socket *sock);
int send_first_packet(struct cache_connection *connection, struct cache_socket *sock,
			     enum cache_packet cmd);

void cache_socket_receive(struct cache_connection *connection);
int cache_msocket_receive(struct cache_connection *connection);

int cache_send_dblock(struct cache_connection *connection, struct page **pages, 
				int count, u32 size, sector_t sector, struct cache_request ** req);
int cache_send_wrote(struct cache_connection *connection, 
	pgoff_t *pages_index, int count, struct cache_request ** req);
int cache_send_data_ack(struct cache_connection *connection,  u32 seq_num, u64 sector);
int cache_send_wrote_ack(struct cache_connection *connection,  u32 seq_num);

struct cache_connection *cache_conn_init(struct dcache *dcache);
int cache_conn_exit(struct dcache *dcache);

struct cio *cio_alloc(int count);
void cio_put(struct cio *cio);
void cio_exit(void);
int cio_init(void);

void cache_request_enqueue(struct cache_request *req);
struct cache_request * cache_request_alloc(struct cache_connection *conn, u32 seq_num);
struct cache_request * get_ready_request(struct cache_connection *conn, u32 seq_num);
void cache_request_dequeue(struct cache_request *req);

#endif
