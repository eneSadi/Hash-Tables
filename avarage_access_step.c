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

int search(int hash_table[], int m, int key, int *adim_toplami){
	
	int i = 0, hash_index = key%m;
	
	if(hash_table[hash_index] == -1){
		//printf("Key %d is not in table!\n",key);
		return 0;
	}
	else if(hash_table[hash_index] == key){
		(*adim_toplami) += 1;
		return 1;
	}
	else{
		while(i<m && hash_table[hash_index] != -1 && hash_table[hash_index] != key){
			i++;
			hash_index = (key%m + i) % m;
			(*adim_toplami) += 1;
			//printf("--%d--\n",*adim_toplami);
		}
		if(i == m){
			//printf("Table is full and key is not in table!\n");
			return 0;
		}
		else if(hash_table[hash_index] == key){
			//(*adim_toplami) += 1;
			//printf("--%d--\n", *adim_toplami);
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
	
	int dizi1[7] = {1,2,3, 8, 9, 10,11};
	
	int i, hash_table[7], m = 7;
	
	for(i = 0 ; i<m ; i++){
		hash_table[i] = -1;
	} 
	
	for(i = 0 ; i < 7 ; i++){
		insert(hash_table, m, dizi1[i]);
	}
	
	
	printf("\nHASH TABLE\n");
	for(i = 0 ; i<m ; i++){
		printf("%d : %d\n",i,hash_table[i]);
	}
	
	
	int aranacaklar[6] = {1,2,3,8,9,10};
	
	int adim_toplami = 0;
	int arama_toplami = 0;
	for(i = 0 ; i < 6 ; i++){
		if(search(hash_table, m, aranacaklar[i], &adim_toplami) != 0){
			arama_toplami++;
		}
	}
	
	float ort = (float)adim_toplami/(float)arama_toplami;
	printf("\n\nAvarage Access Step Count (%d/%d): %f\n\n", adim_toplami,arama_toplami, ort);
	
	return 0;
}
