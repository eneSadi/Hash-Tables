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
			hash_index = (key%m + i) % m;
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

int search(int hash_table[], int m, int key){
	
	int i = 0, hash_index = key%m;
	
	if(hash_table[hash_index] == -1){
		//printf("Key %d is not in table!\n",key);
		return 0;
	} 
	else{
		while(i<m && hash_table[hash_index] != -1 && hash_table[hash_index] != key){
			i++;
			hash_index = (key%m + i) % m;
		}
		if(i == m){
			//printf("Table is full and key is not in table!\n");
			return 0;
		}
		else if(hash_table[hash_index] == key){
			//printf("Key is in %dth index!\n",hash_index);
			return 1;
		}
		else{
			//printf("Key %d is not in table!\n");
			return 0;
		}
	}
}

int main(){
	
	int dizi1[10] = {1,2,3,4,5,6,7,8,9,10};
	int dizi2[10] = {8,9,10,11,12,13,14,15,16,17};
	
	
	
	int i, hash_table[15], m = 15;
	
	for(i = 0 ; i<m ; i++){
		hash_table[i] = -1;
	} 
	
	for(i = 0 ; i < 10 ; i++){
		insert(hash_table, m, dizi1[i]);
	}
	
	
	printf("\nHASH TABLE\n");
	for(i = 0 ; i<m ; i++){
		printf("%d : %d\n",i,hash_table[i]);
	}
	
	int k = 0,ctrl = 0, kesisim[10] = {0,0,0,0,0,0,0,0,0,0};
	for(i = 0 ; i < 10 ; i++){
		ctrl = search(hash_table, m, dizi2[i]);
		if(ctrl == 1){
			kesisim[k++] = dizi2[i];
		}
	}
	
	printf("\nKESISIM KUMESI\n");
	for(i=0 ; i<10 ; i++){
		if(kesisim[i] != 0){
			printf("%d ",kesisim[i]);
		}
	}
	
	return 0;
}
