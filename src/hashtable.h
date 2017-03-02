/*
 * hashtable.h
 *
 *  Created on: 2016. 11. 24.
 *      Author: root
 */

#ifndef SRC_HASHTABLE_H_
#define SRC_HASHTABLE_H_

#include <stdint.h>
#include <pthread.h>

#define MAX_NUM_BUCKETS 65536

struct _hash_bucket_t;

typedef struct _hash_iter_t
{
	int i;
	int elements;
	struct _hash_bucket_t *b;
} hash_iter_t;

#define INIT_HASH_ITER { .i=-1, .b=NULL, .elements=0, }

typedef struct _hash_bucket_t
{
	void *data;
	struct _hash_bucket_t *prev;
	struct _hash_bucket_t *next;
} hash_bucket_t;

typedef void  (*hash_update_t)(const void *);
typedef int   (*hash_compare_t)(const void*, const void*);
typedef int   (*hash_func_t)(const void *);
typedef void  (*hash_free_t)(void *);
typedef void* (*hash_cpdata_t)(const void *);
typedef void  (*hash_desc_t)(const void *);
typedef void  (*hash_dosomething)(void *, void *);

typedef struct _hash_op
{
	hash_compare_t hash_compare;
	hash_func_t hash_func;
	hash_free_t hash_free;
	hash_cpdata_t hash_cpdata;
	hash_desc_t hash_desc;
	hash_update_t hash_find_update;
	hash_update_t hash_inst_update;
	hash_dosomething hash_dosomething;
} hash_op;

typedef struct _hash_ctx_t
{
#define pCurBucket(ctx, i) (ctx->T[i])
#define pCurLock(ctx, i)   (&(ctx->lock[i]))

	int bucket_size;
	int replace;
	hash_bucket_t **T;
	pthread_mutex_t *lock;
	uint64_t num_elements;
	hash_op *op;
	int _thread_safe;
} hash_ctx_t;


hash_ctx_t *new_hash_thread_safe_ctx(hash_op *, int bucket_size, int replace);
hash_ctx_t *new_hash_single_thread_ctx(hash_op *, int bucket_size, int replace);
void free_hash_ctx(hash_ctx_t *ctx);
int insert_hash(hash_ctx_t *ctx, void *data);
int delete_hash(hash_ctx_t *ctx, void *data);
void *find_hash(hash_ctx_t *ctx, const void *data);
void debug_hash(hash_ctx_t *ctx);
void debug_hash2(hash_ctx_t *ctx);
void dosomething_hash(hash_ctx_t *ctx, void *data);
uint64_t num_elements_hash(hash_ctx_t *ctx);
int hash_foreach(hash_ctx_t *ctx, hash_iter_t *iter);
#endif /* SRC_HASHTABLE_H_ */
