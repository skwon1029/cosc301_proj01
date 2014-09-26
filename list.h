#ifndef __LIST_H__
#define __LIST_H__

struct node{
	int num;
	struct node *next;
};

void list_clear(struct node *head);
void list_print(const struct node *head);
int list_delete(const int integer, struct node **head);
void list_append(const int integer, struct node **head);
void list_sort(struct node **head);

#endif // __LIST_H__
