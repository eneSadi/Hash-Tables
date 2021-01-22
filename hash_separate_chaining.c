#include<stdio.h>
#include<stdlib.h>

struct node{
	
	//char name[50];
	int key;
	struct node *next;
};

struct hash{
	
	struct node *head;
	int count;
};

void insert(struct hash hash_table[], int m, int key){
	
	int hash_index = key%m;
	
	if(hash_table[hash_index].head == NULL){
		
		struct node *new_node = (struct node*)malloc(sizeof(struct node));
		new_node->key = key;
		new_node->next = NULL;
		hash_table[hash_index].count +=1;
		hash_table[hash_index].head = new_node; 
	}
	else{
		struct node *new_node = (struct node*)malloc(sizeof(struct node));
		new_node->key = key;
		new_node->next = hash_table[hash_index].head;
		hash_table[hash_index].head = new_node;
		hash_table[hash_index].count +=1;
	}
	
}

void search(struct hash hash_table[], int m, int key){
	
	int hash_index = key%m;
	
	if(hash_table[hash_index].head == NULL){
		printf("Key %d is not in table!",key); 
	}
	else{
		struct node *tmp = hash_table[hash_index].head;
		
		while(tmp->key != key && tmp->next != NULL){
			tmp = tmp->next;
		}
		if(tmp->key == key){
			printf("Key %d is in %dth index!\n",key,hash_index);
		}
		else{
			printf("Key %d is not in table!\n",key);
		}
	}
}

void delet(struct hash hash_table[], int m, int key){
	
	int hash_index = key%m;
	
	if(hash_table[hash_index].head == NULL){
		printf("Key %d is not in table!",key); 
	}
	else if(hash_table[hash_index].head->key == key){
		hash_table[hash_index].head = hash_table[hash_index].head->next;
	}
	else{
		struct node *tmp = hash_table[hash_index].head;
		struct node *before = hash_table[hash_index].head;
		
		while(tmp->key != key && tmp->next != NULL){
			before = tmp;
			tmp = tmp->next;
		}
		if(tmp->key == key){
			before->next = tmp->next;
			free(tmp);
		}
		else{
			printf("Key %d is not in table!\n",key);
		}
	}
	
}

int main(){
	
	struct hash hash_table[7];
	int i, m = 7;
	
	for(i=0 ; i < m ; i++){
		hash_table[i].head = NULL;
		hash_table[i].count = 0;
	}
	
	int key = 10;
	insert(hash_table, m, key);
	insert(hash_table, m, 3);
	insert(hash_table, m, 17);
	insert(hash_table, m, 4);
	insert(hash_table, m, 2);
	
	for(i = 0 ; i < m; i++){
		if(hash_table[i].head){
		
			struct node *tmp = hash_table[i].head;
			printf("%dth index %d nodes: ", i,hash_table[i].count);
			while(tmp->next){
				printf("%d ->", tmp->key);
				tmp = tmp->next;
			} 
			printf(" %d\n",tmp->key);
		}
	}
	
	search(hash_table,m,10);
	
	delet(hash_table, m, 17);
	
	for(i = 0 ; i < m; i++){
		if(hash_table[i].head){
		
			struct node *tmp = hash_table[i].head;
			printf("%dth index %d nodes: ", i,hash_table[i].count);
			while(tmp->next){
				printf("%d ->", tmp->key);
				tmp = tmp->next;
			} 
			printf(" %d\n",tmp->key);
		}
	}
	
	search(hash_table,m,17);
	
	return 0;
}
