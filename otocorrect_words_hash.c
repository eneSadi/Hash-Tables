#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAX_WORD 50 //Olabilecek en uzun kelime
#define MAX_DICT 997 //Hash tablosunun boyutu
#define MAX_SENTENCE 500 // Kullanicidan alinabilecek en uzun cumle

struct misspell_node{ //Hatali kelimelerin kullanici seciminden sonra depolanacagi hash tablosu
	unsigned long long int key; //Horner'den donen key degeri
	char word[MAX_WORD]; //Hatali kelimeye onerilen kelime
};

//Onceki odevden aldim
unsigned long long int horner(char word[MAX_WORD]){ 
	//Horner's Method kullanarak string tipindeki key'leri sayiya cevirdigim fonksiyon.
	unsigned long long int key = 0;
	int R = 31;
	int i,len = strlen(word);
	
	for(i = 0; i < len ; i++){
		key += (pow(R,(len-i-1))*word[i]);
	}
	return key;
}

//Sozluk kelimelerimizin tutuldugu hash'e kelime insert etmemize yarayan fonksiyon
void insert(char hash_table[MAX_DICT][MAX_WORD], unsigned long long int key, char word[MAX_WORD]){
	//Double hashing yontemi ile tutulur
	int i = 0, hash_index = key%MAX_DICT; 
	int hash2 = key%(MAX_DICT - 1) + 1;
	
	if(strcmp(hash_table[hash_index],"NULL") == 0){
		strcpy(hash_table[hash_index],word);
		printf("Insertion occured : %d. index %s\n", hash_index, hash_table[hash_index]);
	} 
	else{
		while(i<MAX_DICT && strcmp(hash_table[hash_index],"NULL") != 0 && strcmp(hash_table[hash_index],word) != 0){
			i++;
			hash_index = (key%MAX_DICT + i*hash2) % MAX_DICT;
		}
		if(i == MAX_DICT){
			printf("Table is full!\n");
		}
		else if(strcmp(hash_table[hash_index],word) == 0){
			printf("Key already exists!\n");
		}
		else{
			strcpy(hash_table[hash_index],word);
			printf("Insertion occured : %d. index %s\n", hash_index, hash_table[hash_index]);
		}
	}
}

//Sozluk kelimelerimizin tutuldugu hash'de eleman ararken kullandigimiz fonksiyon
int search(char hash_table[MAX_DICT][MAX_WORD], unsigned long long int key, char word[MAX_WORD]){
	
	int i = 0, hash_index = key%MAX_DICT;
	int hash2 = key%(MAX_DICT - 1) + 1;
	
	if(strcmp(hash_table[hash_index],"NULL") == 0){
		return 0;
	} 
	else{
		while(i<MAX_DICT && strcmp(hash_table[hash_index],"NULL") != 0 && strcmp(hash_table[hash_index],word) != 0){
			i++;
			hash_index = (key%MAX_DICT + i*hash2) % MAX_DICT;
		}
		if(i == MAX_DICT){
			return 0;
		}
		else if(strcmp(hash_table[hash_index],word) == 0){
			return 1;
		}
		else{
			return 0;
		}
	}
}

//Sozlukteki kelimeleri tutan hash tablosunun dosyaya yazma isleminin yapildigi fonksiyon
void writeFile(char hash_table[MAX_DICT][MAX_WORD]){

	FILE *fp = fopen("save.txt","w");
	if(fp == NULL){
		printf("Writing operation is not done!\n");
		return;
	}
	int i;
	
	for(i = 0 ; i<MAX_DICT ; i++){
		if(hash_table[i] != 0){
			//index ve kelime bilgisi satir satir dosyaya yazilir.
			fprintf(fp,"%d %s\n",i,hash_table[i]);
		}
	}
	fclose(fp);
	printf("Writing operation is done correctly!\n");
}

//Program calismaya baslar baslamaz eger varsa dosyadan Sozluk olarak kullanilan hash tablasu doldurur
void readFile(char hash_table[MAX_DICT][MAX_WORD]){
	
	FILE *fp = fopen("save.txt","r");
	
	int index;
	char word[MAX_WORD];
	
	//Eger icine daha onceden kaydedilmis bir dosya yoksa okumadan bos tablo dondurur.
	if(fp == NULL){
		fclose(fp);
		return;
	}
	else{
		//eger var ise hash table'i dosyaya gore doldurur
		while(!feof(fp)){
			//index ve kelime bilgisi satirin ilk iki elemani olarak okunur.
			fscanf(fp,"%d %s\n",&index, word);
			strcpy(hash_table[index], word);
		}
		fclose(fp);
		return;
	}
}

//Sozluk olarak kullanacagimiz hash tablosunu olusturdugumuz fonksiyon
void createHashDict(char dictHash[MAX_DICT][MAX_WORD]){
	
	FILE *fp1 = fopen("save.txt", "r"); //Onceden olusturulmus kayit dosyasi varsa oradan okunarak table olusturulur
	if(fp1 == NULL){ // Yoksa sozluk dosyasi okunarak olusturulur
		printf("There is no save file!\n"); 
		
		FILE *fp = fopen("smallDictionary.txt","r");
		if(fp == NULL){ // O dosya da yoksa program calisamaz exit ile cikis yapilir
			printf("Dictionary File does not exist!");
			exit(1);
		}
		// Varsa oncelikle butun gozler NULL ile ilklendirilir
		int i;
		for(i = 0; i < MAX_DICT ; i++){
			strcpy(dictHash[i],"NULL"); // Hash tablosu 0'lar ile ilklendirilmistir
		}
		
		char word[MAX_WORD];
		unsigned long long int key;
		fscanf(fp,"%s",word);
		while(!feof(fp)){ 
			//dosya sonuna kadar kelime kelime okunarak horner ile key degerleri bulunur
			//ve o keyin gerektirdigi indexteki kelime sozluk hash'ine insert edilir.
			key = horner(word);
			insert(dictHash, key, word);
		
			fscanf(fp,"%s",word);
		}
		fclose(fp);
	}
	else{
		readFile(dictHash);
		printf("Hash table filled from save.txt file!\n");
		fclose(fp1);
	}
}

//Hash table'i yazdirmaya yarayan fonksiyon
void printTable(char hash_table[MAX_DICT][MAX_WORD]){
	printf("\nHASH TABLE:\n\n");
	int i;
	for(i = 0 ; i<MAX_DICT ; i++){
		printf("%d : %s\n",i,hash_table[i]);
	}
}

//Hatali yazilan kelimelerin tutulacagi struct tipinde hash tablosunu ilklendiren fonksiyon
void initializeMisspelled(struct misspell_node misspelled[MAX_DICT]){
	int i;
	for(i = 0 ; i < MAX_DICT ; i++){
		misspelled[i].key = 0;
	}
}

//iki hash icin de iki ayri search ve insert yapmamin sebebi hash tablolarinin tiplerinin farkli olmasi
//Hatali kelimeler icin oneri yazdigimiz tablo uzerinde arama yapmamizi saglayan fonksiyon
int searchMisspelled(struct misspell_node hash_table[MAX_DICT], unsigned long long key, char I_mean[MAX_WORD]){
	
	int i = 0, hash_index = key%MAX_DICT;
	int hash2 = key%(MAX_DICT - 1) + 1;
	
	if(hash_table[hash_index].key == 0){
		return 0;
	} 
	else{
		while(i<MAX_DICT && hash_table[hash_index].key != 0 && hash_table[hash_index].key != key){
			i++;
			hash_index = (key%MAX_DICT + i*hash2) % MAX_DICT;
		}
		if(i == MAX_DICT){
			return 0;
		}
		else if(hash_table[hash_index].key == key){
			strcpy(I_mean, hash_table[hash_index].word); 
			//Tabloda varsa I_mean degiskeninin icine atilir, bu degisken ekrana cumlenin
			//duzeltilmis halini yazarken kullanilir.											
			return 1;
		}
		else{
			return 0;
		}
	}
}

//Hatali kelimeler icin oneri yazdigimiz tabloya insert islemi yapmamizi saglayan fonksiyon
void insertMisspelled(struct misspell_node hash_table[MAX_DICT], unsigned long long int key, char word[MAX_WORD]){
	
	int i = 0, hash_index = key%MAX_DICT;
	int hash2 = key%(MAX_DICT - 1) + 1;
	
	if(hash_table[hash_index].key == 0){
		hash_table[hash_index].key = key;
		strcpy(hash_table[hash_index].word, word);
		printf("\n--> Insertion occured in Misspelled Hash: %d. index %llu suggestion : %s\n", hash_index, hash_table[hash_index].key, hash_table[hash_index].word);
	} 
	else{
		while(i<MAX_DICT && hash_table[hash_index].key != 0 && hash_table[hash_index].key != key){
			i++;
			hash_index = (key%MAX_DICT + i*hash2) % MAX_DICT;
		}
		if(i == MAX_DICT){
			printf("Table is full!\n");
		}
		else if(hash_table[hash_index].key == key){
			printf("Key already exists!\n");
		}
		else{
			hash_table[hash_index].key = key;
			strcpy(hash_table[hash_index].word, word);
			printf("\n--> Insertion occured in Misspelled Hash: %d. index %llu suggestion : %s\n", hash_index, hash_table[hash_index].key, hash_table[hash_index].word);
		}
	}
}

//Levernkusen edit distance algoritmasi ile iki string arasindaki fark miktari bulunur 
int editDistance(char wordX[MAX_WORD], char wordY[MAX_WORD]){
	int lenX = strlen(wordX);
	int lenY = strlen(wordY);
	//dinamik olarak matrix olusturulur
	int **matrix = (int**)calloc((lenX + 1),sizeof(int*));
	int i, j;
	for(i = 0 ; i < lenX + 1 ; i++){
		matrix[i] = (int*)calloc((lenY + 1), sizeof(int));
	}
	//Initialization -> ilk satir ve ilk sutun doldurulur
	for(i = 0 ; i < lenX + 1 ; i++){
		matrix[i][0] = i;
	}
	for(i = 0 ; i < lenY + 1 ; i++){
		matrix[0][i] = i;
	}
	//Dinamik olarak matrix uzerinde hesaplama yapilan kisim
	int x, y, z, cost;
	for(i = 1 ; i < (lenX + 1) ; i++){
		for(j = 1 ; j < (lenY + 1) ; j++){
			//Ayni harf ise cost 0 degilse 1
			if(wordX[i-1] == wordY[j-1]){
				cost = 0;
			}
			else{
				cost = 1;
			}
			
			x = matrix[i-1][j-1] + cost; //otherwise
			y = matrix[i-1][j] + 1; //deletion
			z = matrix[i][j-1] + 1; //insertion
			//Yukarida hesaplanan 3 durumdan hangisi en kucuk bulunur ve uzerinde bulunulan index'e o deger yazilir
			if(x < y){
				if(x < z){
					matrix[i][j] = x;
				}
				else{
					matrix[i][j] = z;
				}
			}
			else{
				if(y < z){
					matrix[i][j] = y;
				}
				else{
					matrix[i][j] = z;
				}
			}
		}
	}
	return matrix[lenX][lenY]; // algoritma sonunda distance degeri matrisin sol alt kosesinde tutulmaktadir
}

//Kullaniciya bilgilendirici outputlarin verildigi ve cumlenin kullanicidan alindigi kisim
void GUI(char dictHash[MAX_DICT][MAX_WORD], struct misspell_node misspelled[MAX_DICT]){
	
	char sentence[MAX_SENTENCE]; // Kullanicidan alinan cumleyi tutar
	unsigned long long int key; 
	int i, ctrl, ctrl_misspelled, flag; // ctrl -> kelime sozluk hash tablosunda varsa 1 yoksa 0
										// ctrl_misspelled -> kelime yazim hatalarinin tutuldugu tabloda varsa 1 yoksa 0
										// flag -> birkac farkli kontrolde switch olarak kullanilir
	char predictions[20][MAX_WORD], pred_i = 0; //predictions dizisi kullanicinin yazdigi kelimeye uygun uzakliktaki kelimeleri tutmak icin var
												//bu dizi kullanicinin onerilenler arasindan secip secmedigini kontrol eder
												//pred_i ise bu dizinin index bilgisini tutmakta
	char I_mean[MAX_WORD]; //Kullanicinin yanlis girdigi kelimenin gerekli islemlerden sonra dogrusunu tutan degisken
	char corrected_sentence[50][MAX_WORD]; //Cumlenin duzeltilmis halini tutan dizi
	int corrected_i = 0, is_any_correction = 0; //corrected_i -> bu dizinin index bilgisini tutmakta
												// is_any_correction -> duzeltme yapilip yapilmadigini tutar

	printf("\n\n---> WELCOME TO TYPEOWWWW! <---\n\n");
	printf("Type exit for exit the program!\n\n");
	do{
		printf("\nYour Sentence: ");
		gets(sentence);
		if(strcmp("exit", sentence)){

		   	char *tok = strtok(sentence, "\n");
			tok = strtok(sentence, " ");
			//Enter ve space karakterlerine gore cumle tokenize edilir
			while(tok != NULL){
				//tok degiskeni kelimeyi tutmaktadir, horner ile key degeri bulunur
				key = horner(tok);
				//Sozlukte var mi diye bakilir
				ctrl = search(dictHash, key, tok);
				if(ctrl == 0){
					//Yoksa onceden duzletilmis kelimeler arasinda var mi diye bakilir
					ctrl_misspelled = searchMisspelled(misspelled, key, I_mean);
					if(ctrl_misspelled){
						//Duzeltilmis hali zaten varsa duzeltilmis cumlenin tutuldugu string dizisine kopylanir	
						strcpy(corrected_sentence[corrected_i++], I_mean);
					}	
					else{
						//Onceden yapilmamis bir yazim hatasi ise uzaklik hesaplama islemleri yapilip kullaniciya oneri sunulur
						flag = 0; //1 uzaklikta kelime yoksa flag 0 kalir bir tane bile varsa flag 1 olur ki 2 uzakliktakiler yazdirilmasin
						printf("\nPredictions for '%s': \n", tok);
						for(i = 0 ; i < MAX_DICT ; i++){
							if(editDistance(dictHash[i], tok) == 1){
								printf("%s\n", dictHash[i]);
								strcpy(predictions[pred_i++], dictHash[i]);
								flag = 1;
							}	
						}
						//hic 1 uzaklikta yok ise 2 uzaklikta olanlar icin calisir
						if(flag == 0){
							for(i = 0 ; i < MAX_DICT ; i++){
								if(editDistance(dictHash[i], tok) == 2){
									printf("%s\n", dictHash[i]);
									strcpy(predictions[pred_i++], dictHash[i]);
								}	
							}
						}
						//pred_i degiskeninin 0 olmasi demek yanlis yazilan kelime icin sozlukte oneri bulunamamis demektir 
						if(pred_i != 0){
							//degilse kullaniciya secim yaptirilir ve sectigi kelime hatali kelimenin yerine yazilir
							//islem bitince kullanicidan aldigi duzeltmeyi hash tablosuna yazar ki bir sonraki ayni yazim hatasinda o tabloda bulabilsin
							do{
								printf("\nWhat did you mean: ");
								scanf("%s",I_mean); //Kelime kullanicidan alinir ve onerilenler arasinda mi diye kontrol edilir
								i = 0;
								flag = 0; //oneriler arasina yok ise flag 0 kalir varsa 1 olur
								while(i < pred_i && flag == 0){
									if(!strcmp(predictions[i], I_mean)){
										flag = 1;
									}
									i++;
								}
								if(flag == 0){
									printf("This is not in predictions!\n");
								}
							}while(flag == 0);
							strcpy(corrected_sentence[corrected_i++], I_mean);
							key = horner(tok);
							//kelime hash tablosuna yazilir ki bir sonraki ayni yazim hatasinda o tabloda bulunabilsin
							insertMisspelled(misspelled,key,I_mean);
							pred_i = 0;
						}
						else{
							printf("\nThere is no predictions!\n");
						}
					}
					is_any_correction = 1;
					//herhangi bir dogrulama yapilmis ise corrected sentence ekrana bastirilir
				}
				else{
					//kelime sozlukte var ise ilk kontrolden sonra program buraya atlar
					strcpy(corrected_sentence[corrected_i++], tok);
				}
	  			tok = strtok(NULL, " ");
			}
		}
		if(is_any_correction == 1){
			printf("\nCorrected Sentence :");
			for(i = 0 ; i < corrected_i ; i++){
				printf("%s ",corrected_sentence[i]);
			}
		}
		is_any_correction = 0;
		corrected_i = 0;
		getchar();
		printf("\n-----------------------------------------------------------\n");
	}while(strcmp("exit", sentence));	
}

//Main fonksiyon
int main(){
	
	char dictHash[MAX_DICT][MAX_WORD]; //Sozlukten okunan kelimelerin tutuldugu hash table
	
	createHashDict(dictHash); // Sozluk hash tablosu olusturulur
	printTable(dictHash); // Tablo bir kere yazdirilir.
	
	struct misspell_node misspelled[MAX_DICT]; // Hatali yazimlarin duzeltilmesinin ardindan tutuldugu hash table
	initializeMisspelled(misspelled); // Bu table'i 0 atayarak ilklendirir
	
	GUI(dictHash, misspelled); //Kullanici bilgilendirmeleri ile programi yuruten fonksiyon cagirilir
	
	writeFile(dictHash); //Programdan cikis yapmadan once sozluk hash'i dosyaya yazilir
	printf("\n-----------------------------------------------------------\n");
	printf("\n~~~~~~developed by sadi~~~~~~\n");
	
	return 0;
}



