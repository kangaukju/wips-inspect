/*
 * list.c
 *
 *  Created on: 2016. 12. 13.
 *      Author: root
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "llist.h"


llist* new_list(void *obj)
{
	llist *list = (llist*)malloc(sizeof(llist));
	list->next = NULL;
	list->prev = NULL;
	list->obj = obj;
	return list;
}

llist *add_list(llist *head, void *obj)
{
	llist *list = NULL;

	list = new_list(obj);

	if (head) {
		list->next = head;
		head->prev = list;
	}

	return list;
}

llist* head_list(llist *node)
{
	llist *cur = NULL;
	llist *head = NULL;

	cur = node;
	while (cur) {
		head = cur;
		cur = cur->prev;
	}
	return head;
}

llist* last_list(llist *node)
{
	llist *cur = NULL;
	llist *last = NULL;

	cur = node;
	while (cur) {
		last = cur;
		cur = cur->next;
	}
	return last;
}

llist *pop_list(llist *node)
{
	llist *head = NULL;

	if (!node) {
		return NULL;
	}
	if (node->prev) {
		head = node->prev;
		node->prev->next = node->next;
	}
	else {
		head = node->next;
	}
	if (node->next) {
		node->next->prev = node->prev;
	}

	if (!head) {
		return NULL;
	}
	return head_list(head);
}

llist* del_list(llist *node, int objfree)
{
	llist *head = NULL;

	if (!node){
		return NULL;
	}

	head = pop_list(node);

	if (objfree && node->obj) {
		free(node->obj);
		node->obj = NULL;
	}
	free(node);

	return head;
}

llist* del_list_ex(llist *node, void (*free_func)(void *))
{
	llist *head;

	if (!node) {
		return NULL;
	}

	head = pop_list(node);

	if (free_func) {
		free_func(node->obj);
		node->obj = NULL;
	}
	free(node);

	return head;
}

void delall_list(llist *head, int objfree)
{
	llist *cur = head;
	llist *next = NULL;

	if (!head) {
		return;
	}

	head = head_list(head);

	while (cur) {
		if (objfree && cur->obj) {
			free(cur->obj);
			cur->obj = NULL;
		}
		next = cur->next;
		free(cur);
		cur = next;
	}
}

void delall_list_ex(llist *head, void (*free_func)(void *))
{
	llist *cur = head;
	llist *next = NULL;

	if (!head) {
		return;
	}

	head = head_list(head);

	while (cur) {
		if (free_func) {
			free_func(cur->obj);
			cur->obj = NULL;
		}
		next = cur->next;
		free(cur);
		cur = next;
	}
}

void print_list(llist *head, void(*print)(void *))
{
	llist *cur = head;

	if (!print) {
		return;
	}

	while (cur) {
		print(cur->obj);
		cur = cur->next;
	}
}


#if 0

void lprint(void *o)
{
	int *p = (int *)o;
	printf("%d -> ", *p);
}

int main()
{
	int *i1, *i2, *i3, *i4;
	i1 = malloc(sizeof(int));
	i2 = malloc(sizeof(int));
	i3 = malloc(sizeof(int));
	i4 = malloc(sizeof(int));

	*i1 = 20;
	*i2 = 31;
	*i3 = 84;
	*i4 = 9;

	llist *head = NULL;
	llist *cur;

	head = add_list(head, i1);
	head = add_list(head, i2);
	head = add_list(head, i3);
	head = add_list(head, i4);

	printf("[init]\n");
	print_list(head, lprint);
	printf("\n");

	head = del_list(head, 1);

	printf("[del head]\n");
	print_list(head, lprint);
	printf("\n");

	cur = head;
	while (cur)
	{
		if (*((int*)cur->obj) == 84) {
			head = del_list(cur, 1);
		}
		cur = cur->next;
	}

	printf("[del 84]\n");
	print_list(head, lprint);
	printf("\n");

	delall_list(head, 1);

}
#endif
