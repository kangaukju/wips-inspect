/*
 * llist.h
 *
 *  Created on: 2016. 12. 13.
 *      Author: root
 */

#ifndef SRC_LLIST_H_
#define SRC_LLIST_H_

#define LLIST_OBJ_FREE      1
#define LLIST_OBJ_NOT_FREE  0

typedef struct _llist
{
	struct _llist *next, *prev;
	void *obj;
} llist;


/**
 * @brief new_list
 * @param obj data
 * @return new list
 */
llist* new_list(void *obj);

/**
 * @brief add_list
 * @param head head of list
 * @param obj data
 * @return added new list
 */
llist* add_list(llist *head, void *obj);

/**
 * @brief pop_list
 * @param node for pop
 * @return head of list
 */
llist* pop_list(llist *node);

/**
 * @brief del_list
 * @param delete list
 * @param objfree free object?
 * @return head of list
 */
llist* del_list(llist *node, int objfree);

/**
 * @brief del_list_ex
 * @param delete list
 * @param free_func free function for obj
 * @return head of list
 */
llist* del_list_ex(llist *node, void (*free_func)(void *));

/**
 * @brief delall_list
 * @param head of list
 * @param objfree free object?
 */
void delall_list(llist *head, int objfree);

/**
 * @brief delall_list_ex
 * @param head of list
 * @param free_func free function for obj
 */
void delall_list_ex(llist *head, void (*free_func)(void *));

/**
 * @brief print_list
 * @param head of list
 * @param print function pointer
 */
void print_list(llist *head, void(*print)(void *));

/**
 * @brief head_list
 * @param any node
 * @return head of list
 */
llist* head_list(llist *node);

/**
 * @brief last_list
 * @param any node
 * @return last of list
 */
llist* last_list(llist *node);

#endif /* SRC_LLIST_H_ */
