/*
 *
 * Code adapted from hw03 list.c
 *
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
 * Clear and deallocate all items in the linked list
 */
void list_clear(struct node *list) {
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp);
    }
}

/*
 * Print all items in the linked list
 */
void list_print(const struct node *list) {
    while (list != NULL) {
        printf("%d\n", list->num);
        list = list->next;
    }
}

/*
 * Delete a given integer from the linked list
 * Return 1 if the integer was successfully deleted
 * If not, return 0
 */
int list_delete(const int integer, struct node **head) {
	//if the first name is a match
	//adjust the head pointer and return
	struct node *temp = *head;
	if(temp->num == integer){
		*head=(*head)->next;
		return 1;
	}
	struct node *prev = *head;				//i-1
	struct node *current = (*head)->next;	//i
    while(current!=NULL){
    	//find the node we want to delete
    	//adjust the pointer so that the list skips the node
    	if(!(current->num - integer)){
    		prev->next = current->next;
    		return 1;
    	}
    	//advance pointers
    	prev = current;
    	current = current->next;
    }
    return 0;
}

/*
 * Append a given integer to the end of the linked list
 */
void list_append(const int integer, struct node **head) {
	//initialize the node to append
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    new_node->num = integer;
	new_node->next = NULL;
	
	//if the list is empty, replace its head with the new node
	if(*head==NULL){
		*head = new_node;
		return;
	}
	
    struct node *temp = *head;
    //advance the pointer till we reach the end
    while(temp->next!=NULL){
    	temp = temp->next;
    }        
    temp->next = new_node;
}

/*
 * Sort the linked list in ascending order
 */
void list_sort(struct node **head) {
    struct node *result = NULL;	//temporary list to accumulate nodes
    
    while(*head!=NULL){
    	struct node *current = (*head)->next;
		struct node *min = *head;
		
    	//find the smallest integer
		while(current!=NULL){
			if(min->num > current->num){
				min = current;
			}
			current = current->next;
		}		
		list_append(min->num,&result);	//add the chosen integer to the result 
		list_delete(min->num,head);		//delete the chosen integer from the head
    }        
    *head = result;	//update the original head pointer to the new one
}
