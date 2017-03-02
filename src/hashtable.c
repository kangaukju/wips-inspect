/*
 * hashtable.cpp
 *
 *  Created on: 2016. 11. 24.
 *      Author: root
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashtable.h"

/**
 * @brief create hash table context
 * @param op: hash table operator structure
 *        bucket_size: hash table fixed bucket array size
 *        replace: when insert data, if find same data then replace data
 */
static hash_ctx_t *new_hash_ctx(
		hash_op *op,
		int bucket_size,
		int replace, int thread_safe)
{
	hash_ctx_t *ctx = NULL;
	int i = 0;

	if (op->hash_compare == NULL ||
			op->hash_func == NULL
	) {
		return NULL;
	}
	if (thread_safe) {
		if (op->hash_cpdata == NULL) {
			return NULL;
		}
	}

	if (bucket_size > MAX_NUM_BUCKETS) {
		printf("bucket size %d is too big than %d\n",
				bucket_size, MAX_NUM_BUCKETS);
		return NULL;
	}

	ctx = (hash_ctx_t*)malloc(sizeof(hash_ctx_t));
	ctx->T = (hash_bucket_t**)calloc(1,
			sizeof(hash_bucket_t*) * bucket_size);

	// mutex lock per bucket
	if (thread_safe) {
		ctx->lock = (pthread_mutex_t*)malloc(
			sizeof(pthread_mutex_t) * bucket_size);
		for (i = 0; i < bucket_size; i++) {
			if (pthread_mutex_init(&ctx->lock[i], NULL) != 0) {
				free_hash_ctx(ctx);
				return NULL;
			}
		}
	}
	ctx->_thread_safe = thread_safe;
	ctx->bucket_size = bucket_size;
	ctx->num_elements = 0;
	ctx->op = op;
	ctx->replace = replace;
	return ctx;
}

hash_ctx_t *new_hash_thread_safe_ctx(
		hash_op *op,
		int bucket_size,
		int replace)
{
	return new_hash_ctx(op, bucket_size, replace, 1);
}

hash_ctx_t *new_hash_single_thread_ctx(
		hash_op *op,
		int bucket_size,
		int replace)
{
	return new_hash_ctx(op, bucket_size, replace, 0);
}

int hash_foreach(hash_ctx_t *ctx, hash_iter_t *iter)
{
	assert(ctx->_thread_safe != 1);
	if (iter->b) {
		if (iter->b->next == NULL) {
			iter->b = NULL;
			goto search_head_bucket;
		}
		//printf("next bucket [%d]\n", iter->i);
		iter->b = iter->b->next;
		iter->elements++;
		return 0;
	}

search_head_bucket:
	// search head bucket
	while (iter->b == NULL) {
		iter->i++;
		//printf("search next head bucket [%d]\n", iter->i);
		if (iter->i >= ctx->bucket_size) {
			return -1;
		}
		iter->b = pCurBucket(ctx, iter->i);
	}
	// not found head bucket (It's rest all bucket no data)
	if (!iter->b) {
		//printf("empty bucket [%d]\n", iter->i);
		return -1;
	}

	iter->elements++;
	return 0;
}

/**
 * @brief clean up hash table context
 */
void free_hash_ctx(hash_ctx_t *ctx)
{
	hash_bucket_t *cur = NULL;
	hash_bucket_t *tmp = NULL;
	int i = 0;

	if (!ctx) return;

	for (i=0; i<ctx->bucket_size; i++) {
		if (ctx->_thread_safe) {
			pthread_mutex_lock(pCurLock(ctx, i));
		}
		{
			cur = pCurBucket(ctx, i);
			while (cur) {
				if (cur->data) {
					if (ctx->op->hash_free) {
						ctx->op->hash_free(cur->data);
					}
				}

				tmp = cur;
				cur = tmp->next;
				free(tmp);
				tmp = NULL;
			}
		}
		if (ctx->_thread_safe) {
			pthread_mutex_unlock(pCurLock(ctx, i));
			pthread_mutex_destroy(pCurLock(ctx, i));
		}
	}
	if (ctx->_thread_safe) {
		free(ctx->lock);
	}
	free(ctx->T);
	free(ctx);
	return;
}

/**
 * @brief insert data into hash table
 */
int insert_hash(hash_ctx_t *ctx, void *data)
{
	hash_bucket_t *cur = NULL;
	hash_bucket_t *newbee = NULL;
	int index = 0;
	int result = 0;

	index = ctx->op->hash_func(data) % ctx->bucket_size;

	if (ctx->_thread_safe) {
		pthread_mutex_lock(pCurLock(ctx, index));
	}
	{
		cur = pCurBucket(ctx, index);
		while (cur) {
			if (cur->data) {
				result = ctx->op->hash_compare(cur->data, data);
				if (result == 0) {
					if (ctx->replace == 0) {
						// insert update
						if (ctx->op->hash_inst_update) {
							ctx->op->hash_inst_update(cur->data);
						}
						if (ctx->_thread_safe) {
							pthread_mutex_unlock(pCurLock(ctx, index));
						}
						printf(__FILE__":insert_hash dup data [hash:%d]\n", index);
						return -1;
					}

					if (ctx->op->hash_free) {
						ctx->op->hash_free(cur->data);
					}

					cur->data = data;

					if (ctx->_thread_safe) {
						pthread_mutex_unlock(pCurLock(ctx, index));
					}
					return 0;
				}
			}
			cur = cur->next;
		}

		newbee = (hash_bucket_t*)malloc(sizeof(hash_bucket_t));
		memset(newbee, 0, sizeof(hash_bucket_t));
		newbee->data = data;

		newbee->next = pCurBucket(ctx, index);
		pCurBucket(ctx, index) = newbee;
		if (newbee->next)
			newbee->next->prev = newbee;

		ctx->num_elements++;
	}
	if (ctx->_thread_safe) {
		pthread_mutex_unlock(pCurLock(ctx, index));
	}

	return 0;
}

/**
 * @brief find data from hash table
 */
void *find_hash(hash_ctx_t *ctx, const void *data)
{
	hash_bucket_t *cur = NULL;
	int index = 0;
	int result = 0;
	void *newdata = NULL;

	if (!data) return NULL;

	index = ctx->op->hash_func(data) % ctx->bucket_size;
	if (ctx->_thread_safe) {
		pthread_mutex_lock(pCurLock(ctx, index));
	}
	{
		cur = pCurBucket(ctx, index);
		while (cur) {
			result = ctx->op->hash_compare(data, cur->data);
			if (result == 0) {
				// find update
				if (ctx->op->hash_find_update) {
					ctx->op->hash_find_update(cur->data);
				}
				// TODO: thread-safe
				if (ctx->_thread_safe) {
					newdata = ctx->op->hash_cpdata(cur->data);
					pthread_mutex_unlock(pCurLock(ctx, index));
					return newdata;
				}
				// TODO: thread-unsafe
				else {
					return cur->data;
				}
			}
			cur = cur->next;
		}
	}
	if (ctx->_thread_safe) {
		pthread_mutex_unlock(pCurLock(ctx, index));
	}
	return NULL;
}

/**
 * @brief delete data from hash table
 */
int delete_hash(hash_ctx_t *ctx, void *data)
{
	hash_bucket_t *cur = NULL;
	int index = 0;
	int result = 0;


	index = ctx->op->hash_func(data) % ctx->bucket_size;

	if (ctx->_thread_safe) {
		pthread_mutex_lock(pCurLock(ctx, index));
	}
	{
		cur = pCurBucket(ctx, index);
		if (!cur) {
			if (ctx->_thread_safe) {
				pthread_mutex_unlock(pCurLock(ctx, index));
			}
			return -1;
		}

		while (cur) {
			result = ctx->op->hash_compare(cur->data, data);
			if (result == 0) {
				if (ctx->op->hash_free) {
					ctx->op->hash_free(cur->data);
				}

				if (cur == pCurBucket(ctx, index)) {
					pCurBucket(ctx, index) = cur->next;
				}
				else {
					cur->prev->next = cur->next;
				}

				if (cur->next) {
					cur->next->prev = cur->prev;
				}
				free(cur);
				ctx->num_elements--;
				if (ctx->_thread_safe) {
					pthread_mutex_unlock(pCurLock(ctx, index));
				}
				return 0;
			}
			cur = cur->next;
		}
	}
	if (ctx->_thread_safe) {
		pthread_mutex_unlock(pCurLock(ctx, index));
	}
	return -1;
}

void dosomething_hash(hash_ctx_t *ctx, void *data)
{
	hash_bucket_t *cur = NULL;
	int i = 0, k = 0;;

	if (!ctx) return;
	if (!ctx->op->hash_dosomething) return;

	for (i = 0; i < ctx->bucket_size; i++) {
		if (ctx->_thread_safe) {
			pthread_mutex_lock(pCurLock(ctx, i));
		}
		{
			cur = pCurBucket(ctx, i);
			k = 0;
			while (cur) {
				ctx->op->hash_dosomething(cur->data, data);
				cur = cur->next;
			}
		}
		if (ctx->_thread_safe) {
			pthread_mutex_unlock(pCurLock(ctx, i));
		}
	}
	return;
}

/**
 * @brief for debug
 */
void debug_hash(hash_ctx_t *ctx)
{
	hash_bucket_t *cur = NULL;
	int i = 0, k = 0;;

	if (!ctx) return;
	if (!ctx->op->hash_desc) return;

	printf("total [%lu]\n", ctx->num_elements);
	for (i = 0; i < ctx->bucket_size; i++) {
		printf("[%d] ", i);
		if (ctx->_thread_safe) {
			pthread_mutex_lock(pCurLock(ctx, i));
		}
		{
			cur = pCurBucket(ctx, i);
			k = 0;
			while (cur) {
				printf("(%d) ", k++);
				ctx->op->hash_desc(cur->data);
				printf(", ");
				cur = cur->next;
			}
		}
		printf("\n");
		if (ctx->_thread_safe) {
			pthread_mutex_unlock(pCurLock(ctx, i));
		}
	}
	return;
}

void debug_hash2(hash_ctx_t *ctx)
{
	hash_bucket_t *cur = NULL;
	int i = 0, k = 0;;

	if (!ctx) return;
	if (!ctx->op->hash_desc) return;

	printf("total [%lu]\n", ctx->num_elements);
	for (i = 0; i < ctx->bucket_size; i++) {
		if (ctx->_thread_safe) {
			pthread_mutex_lock(pCurLock(ctx, i));
		}
		{
			cur = pCurBucket(ctx, i);
			k = 0;
			while (cur) {
				ctx->op->hash_desc(cur->data);
				printf("\n");
				cur = cur->next;
			}
		}
		if (ctx->_thread_safe) {
			pthread_mutex_unlock(pCurLock(ctx, i));
		}
	}
	return;
}

/**
 * @brief get number of hash tables data elements
 */
uint64_t num_elements_hash(hash_ctx_t *ctx)
{
	if (!ctx) {
		return 0;
	}
	return ctx->num_elements;
}


#ifdef HAVE_HASH_TEST

#include <assert.h>

struct D {
	int Di;
	char *Dp;
};

int test_hash_func(const void *d)
{
	struct D *da = (struct D*)d;
	return da->Di;
}

int test_compare(const void *a, const void *b)
{
	struct D *da = (struct D*)a;
	struct D *db = (struct D*)b;
#if 0
	if (da->Di - db->Di) return -1;
	if (da->Dp && !db->Dp) return -1;
	if (!da->Dp && db->Dp) return -1;
	if (da->Dp && db->Dp)
	{
		return memcmp(da->Dp, db->Dp, strlen(da->Dp));
	}
	return 0;
#else
	return (da->Di - db->Di);
#endif
}

void test_free(void *d)
{
	struct D *da = (struct D*)d;
	if (da) {
		//printf("free - %s", da->Dp);
		free(da->Dp);
		free(da);
	}
	return;
}

void *test_copy(const void *d)
{
	printf("call test_copy\n");
	struct D * newD = NULL;
	struct D *da = (struct D*)d;
	if (!d)
		return NULL;

	newD = (struct D*)malloc(sizeof(struct D));
	newD->Di = da->Di;
	newD->Dp = strdup(da->Dp);
	return newD;
}

void test_desc(const void *d)
{
	struct D *da = (struct D*)d;
	if (!d)
		printf("NIL");
	else
		printf("i=%d, p=%s", da->Di, da->Dp);
}


int main(int argc, char **argv)
{
	hash_op op = {
			.hash_func = test_hash_func,
			.hash_free = test_free,
			.hash_compare = test_compare,
			.hash_cpdata = test_copy,
			.hash_desc = test_desc,
	};

	hash_ctx_t *ctx = new_hash_ctx(&op, 32, 1);
	if (!ctx) exit(1);

	struct D *pd;

	// insert data
	for (int i=0; i<150; i++) {
		pd = (struct D *)malloc(sizeof(struct D));
		pd->Di = i;
		pd->Dp = (char*)malloc(10);
		snprintf(pd->Dp, 10, "%d", pd->Di);
		insert_hash(ctx, pd);
	}
	
	struct D d;

	d.Di = 123;
	assert(delete_hash(ctx, &d) == 1);

	d.Di = 123;
	pd = find_hash(ctx, &d);
	assert(pd == NULL);
	assert(ctx->num_elements == 149);
	
	d.Di = 44;
	pd = find_hash(ctx, &d);
	assert(pd != NULL);
	test_desc(pd);
	printf("\n");
	test_free(pd);
	

	debug_hash(ctx);
	free_hash_ctx(ctx);
}

#endif
