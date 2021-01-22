#include<stdio.h>

void insert(int hash_table[], int m, int key){
	
	int i = 0, hash_index = key%m;
	
	if(hash_table[hash_index] == -1){
		hash_table[hash_index] = key;
		printf("Insertion occured : %d. index %d\n", hash_index, hash_table[hash_index]);
	} 
	else{
		while(i<m && hash_table[hash_index] != -1 && hash_table[hash_index] != key){
			i++;
			hash_index = (key%m + i*i) % m;
		}
		if(i == m){
			printf("Table is full!\n");
		}
		else if(hash_table[hash_index] == key){
			printf("Key already exists!\n");
		}
		else{
			hash_table[hash_index] = key;
			printf("Insertion occured : %d. index %d\n", hash_index, hash_table[hash_index]);
		}
	}
}

void search(int hash_table[], int m, int key){
	
	int i = 0, hash_index = key%m;
	
	if(hash_table[hash_index] == -1){
		printf("Key %d is not in table!\n",key);
	} 
	else{
		while(i<m && hash_table[hash_index] != -1 && hash_table[hash_index] != key){
			i++;
			hash_index = (key%m + i*i) % m;
		}
		if(i == m){
			printf("Table is full and key is not in table!\n");
		}
		else if(hash_table[hash_index] == key){
			printf("Key is in %dth index!\n",hash_index);
		}
		else{
			printf("Key %d is not in table!\n");
		}
	}
}

int main(){
	
	int i, hash_table[7], m = 7;
	
	for(i = 0 ; i<m ; i++){
		hash_table[i] = -1;
	} 
	
	int key = 10;
	insert(hash_table, m, key);
	insert(hash_table, m, 3);
	insert(hash_table, m, 17);
	
	printf("\n\n");
	for(i = 0 ; i<m ; i++){
		printf("%d : %d\n",i,hash_table[i]);
	}
	
	key = 17;
	search(hash_table, m, key);
	
	return 0;
}
