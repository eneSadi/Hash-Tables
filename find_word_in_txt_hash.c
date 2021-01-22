#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define MAX_DOC 50 // Maksimum dosya uzunlugu 
#define MAX_WORD 50 // Maksimum kelime uzunlugu
#define TABLE_SIZE 997 // Hash table boyutu

struct doc_node{ // Her bir dokuman icin olusturulacak struct
	
	char doc_name[MAX_DOC];
	struct doc_node *next;
};

struct node{ // Hash Table'in her bir index'i bir tane bu struct'tan olusmaktadir
	 		 // ve dokumanlari linkli liste halinde tutabilmek icin bir head bulundurmaktadir.
	unsigned long long int key;
	struct doc_node *head;
};

struct node* insert(struct node *hash_table,unsigned long long int key, char doc_name[MAX_DOC], int *numberof_words){
	//Hash Table'a kelime eklemek icin kullanilan fonksiyondur.
	int i = 0;
	int hash_index = key%TABLE_SIZE; // Tablodaki indexini belirlemek icin uygulanan division metodu
	int hash2 = key%(TABLE_SIZE - 1) + 1; // Double hashing icin uygulanan hash2 fonksiyonu
	
	if(hash_table[hash_index].key == 0){ // 0 ise boþ olacagindan o index'e eklenir.
		(*numberof_words)++; // bu degisken kelime eklenen her yerde artirilmaktadir ki loadFactor hesaplanabilsin.
		hash_table[hash_index].key = key; 
		
		struct doc_node *tmp = (struct doc_node*)malloc(sizeof(struct doc_node));
		strcpy(tmp->doc_name, doc_name); // Eklenecek kelimenin hangi dokumana ait oldugu bilgisi de index'e eklenir.
		tmp->next = NULL;
		
		hash_table[hash_index].head = tmp;
		
		printf("Insertion occured : %d. index Key: %llu\n", hash_index, hash_table[hash_index].key);
	} 
	else{ // 0 degilse double hashing metodu ile yeni index bulunur.
		while(i<TABLE_SIZE && hash_table[hash_index].key != 0 && hash_table[hash_index].key != key){
			//Bos index veya key'in aynisini bulana kadar veyahut da tablonun dolu oldugunu anlayana 
			//kadar double hashing ile tabloda gezilir.
			i++; 
			hash_index = (key%TABLE_SIZE + i*hash2) % TABLE_SIZE;
		}
		if(i == TABLE_SIZE){ //Dolu ise kelimeyi eklemez ve fonksiyondan cikar.
			printf("Table is full!\n");
		}
		else if(hash_table[hash_index].key == key){ // Key zaten var ise iki ihtimal var
		
			// 1 -> ayni dokumanda o kelime iki defa gecmistir, o zaman documents listesine bir ekleme yapilmaz
			// 2 -> farkli bir dokumanda gecmistir, o dokuman listeye eklenir. 
			
			if(strcmp(hash_table[hash_index].head->doc_name, doc_name) == 0){ //listenin basinda o dokuman var ise 
				printf("This doc already has this word!\n");				  //ekleme yapmadan cikilir.
			}
			else{ // Basinda degil ise ortalarda veya sonra olabilir.
				struct doc_node *tmp = hash_table[hash_index].head;
				//Burada tmp bir degisken ile listede o dokumanin ismi aranir.
				while(tmp != NULL && strcmp(tmp->doc_name, doc_name) != 0){
					tmp = tmp->next;
				}
				if(tmp != NULL){ //Sonuna kadar gelemediyse dokumanin adini bulmus demektir, eklemeye gerek yok
					printf("This doc already has this word!\n");
				}
				else{ 
					// Bulamadiysa dokuman listesine ekleme islemi icin yeni bir doc_node olusturulup gerekli atamalar yapilir
					// ve listenin basina eklenir.
					struct doc_node *tmp2 = (struct doc_node*)malloc(sizeof(struct doc_node));	
					strcpy(tmp2->doc_name, doc_name);
					
					tmp2->next = hash_table[hash_index].head;
					hash_table[hash_index].head = tmp2;

					printf("Key already exists but doc insertion occured index : %d. index Key: %llu\n", hash_index, hash_table[hash_index].key);
				}
			}
		}
		else{ // dolu degildir ama key ile ayni olan gozden once bos bir goz bulmusuzdur
		      // o zaman da bulunan index'e direkt kontrol yapmadan ekleme yapilir.
			(*numberof_words)++;
			hash_table[hash_index].key = key;
			
			struct doc_node *tmp = (struct doc_node*)malloc(sizeof(struct doc_node));
			strcpy(tmp->doc_name, doc_name);
			tmp->next = NULL;
		
			hash_table[hash_index].head = tmp;
			
			printf("Insertion occured : %d. index Key: %llu\n", hash_index, hash_table[hash_index].key);
		}
	}
	return hash_table;
}

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

struct node* insertDocs(struct node *hash_table, int *numberof_words){
	//Kullanicidan dokuman isimlerinin alinip, dokumanlarin kelime kelime ayrildigi
	//ve bunlar icin tek tek insert fonksiyonunun cagirildigi fonksiyon
	float loadFactor;
	
	char name_of_doc[MAX_WORD];
	char word[MAX_WORD];
	FILE *fp;
	unsigned long long int key;
	
	printf("Type exit for exit!\n\n");
	do{
		printf("Name of the doc wanted to add (without .txt): ");
		gets(name_of_doc); //dosya ismi kullanicidan alinip sonuna .txt eklenir
		fp = fopen(strcat(name_of_doc,".txt"),"r");
		
		if(fp == NULL){
			printf("File not exists!\n"); //Dosya acilamadi uyarisi
		}
		else{
			//Dosyanin kendi ismi .txt ile birlikte horner metoduna gonderilip key'e donusturulur.
			key = horner(name_of_doc);
			hash_table = insert(hash_table, key, name_of_doc, numberof_words); //ve tabloya eklenir.
			//Burada eklenen dokuman adiyla birlikte loadFactor hesaplanir ve bastirilir.
			loadFactor = (float)(*numberof_words)/(float)TABLE_SIZE;
			printf("Load Factor : %f\n",loadFactor);
			
			//Burada dosyanin sonuna kadar kelime kelime okundugu ve tek tek insert edildigi yerdir.
			while(fscanf(fp,"%s",word)){ //Buraya feof yazarak kontrol yapinca son kelimeyi okumuyor.
				int i = 0;
				while(word[i]){
					word[i] = tolower(word[i]);
					i++;
				}
				//Ekleme isleminden önce loadFactor'un 1 olmadigindan emin olunur.
				if(loadFactor != 1){
					printf("\nWord : %s\n",word);
					key = horner(word);
					hash_table = insert(hash_table, key, name_of_doc, numberof_words);
					// Ekleme isleminin ardindan loadFactor yeniden hesaplanir.
					loadFactor = (float)(*numberof_words)/(float)TABLE_SIZE;
				
					printf("Load Factor : %f\n",loadFactor);
					if(loadFactor > 0.8){ // Henüz 1 olamdiysa fakat 0.8i gectiyse uyari yazdirilir.
						printf("TABLE HAS OVER %80 LOADED\n");
					}
					//fscanf(fp,"%s",word);
				}
				else{ // 1 oldugunda tablomuz dolmus demektir ve ekleme yapilamayan kelimeler ekrana yazdirilir.
					printf("\nWord is NOT added TABLE : %s\n",word);
					//fscanf(fp,"%s",word);
					printf("Load Factor : %f\n",loadFactor);
				}
				if(feof(fp)){ //feof kontrolunu mecburen burada yapmaliyiz yukaridaki sebepten
					break;
				}
			}
		}
		
		printf("\n----------------------------------------------------\n");
	}while(strcmp("exit.txt", name_of_doc)); //Kullanici exit girdiginde dosya ekleme islemi biter.
	
	return hash_table;
}

//Arama asamasina gectigimizde kullanici tarafindan girilen kelimenin key'e donusmus halini
//table'da arayan fonksiyon.
void search(struct node* hash_table, unsigned long long int key, char word[MAX_WORD]){
	//Arama isleminin kac adimda bittigini tutan degisken
	int counter = 1;
	int i = 0, hash_index = key%TABLE_SIZE; //Yukaridaki gibi hash1 ve has2 hesaplanir.
	int hash2 = key%(TABLE_SIZE-1) + 1;
	
	if(hash_table[hash_index].key == 0){ //direkt bakilan index bos ise zaten kelime yoktur.
		printf("Key %s is not in table! Search takes %d step.\n",word,counter);
	} 
	else{
		while(i<TABLE_SIZE && hash_table[hash_index].key != 0 && hash_table[hash_index].key != key){
			//Bos index veya key'in aynisini bulana kadar veyahut da tablonun dolu oldugunu anlayana 
			//kadar double hashing ile tabloda gezilir.
			i++;
			hash_index = (key%TABLE_SIZE + i*hash2) % TABLE_SIZE;
			// Her ilerleme bir kontrol oldugundan arama isleminin adim sayisi artar.
			counter++;
		}
		if(i == TABLE_SIZE){ //Tablo doluysa ve tamaminda bulamadiysak 
			printf("Table is full and key is not in table! Search takes %d step.\n",counter);
		}
		else if(hash_table[hash_index].key == key){
			//Key'e esit olan index'i buldugumuz kisim burasidir.
			// Adim sayisi ile birlikte, head uzerinden erisilen dokumanlar listesi yazdirilir.
			printf("Key %s is in %dth index! Search takes %d step and these docs have this word:\n",word, hash_index, counter);
			struct doc_node *tmp = hash_table[hash_index].head;
			while(tmp->next != NULL){
				printf("%s ,",tmp->doc_name);
				tmp = tmp->next;
			}
			printf("%s\n",tmp->doc_name);
		}
		else{ //Bos bir index'e geldigimiz halde key yoksa, tabloda yok demektir.
			printf("Key %s is not in table! Search takes %d step.\n",word,counter);
		}
	}
}

//Tablonun ekrana yazdirilmasi icin kullanilan fonksiyon
void printTable(struct node *hash_table){
	
	int i;
	printf("\n\nHASH TABLE:\n\n");
	for(i = 0 ; i<TABLE_SIZE ; i++){
		if(hash_table[i].key != 0){
			printf("%d : %llu  --> ",i,hash_table[i].key);	
			
			struct doc_node *tmp = hash_table[i].head;
			
			while(tmp->next != NULL){
				printf("%s ,",tmp->doc_name);
				tmp = tmp->next;
			}
			printf(" %s\n",tmp->doc_name);
		}
	}
}

//Dosyaya yazma isleminin yapildigi fonksiyon
void writeFile(struct node *hash_table, int numberof_words){
	
	FILE *fp = fopen("save.txt","w");
	int i;
	//tekrar acildiginda loadFactor hesaplamasi yapabilmek icin tablodaki kelime sayisi dosyaya yazilir
	fprintf(fp,"%d\n",numberof_words);
	for(i = 0 ; i<TABLE_SIZE ; i++){
		if(hash_table[i].key != 0){
			//index ve key bilgisi yazilir.
			fprintf(fp,"%d %llu ",i,hash_table[i].key);	
			
			struct doc_node *tmp = hash_table[i].head;
			//o key'in icinde gectigi dokumanlar yan yana yazilir.
			while(tmp->next != NULL){
				fprintf(fp,"%s ",tmp->doc_name);
				tmp = tmp->next;
			}
			fprintf(fp,"%s\n",tmp->doc_name);
		}
	}
	fclose(fp);
}

//Program calismaya baslar baslamaz eger varsa dosyadan doldurur
struct node* readFile(struct node *hash_table, int *numof_words){
	
	FILE *fp = fopen("save.txt","r");
	//Tablo ilklendirmeleri yapilir
	int i;
	for(i = 0 ; i<TABLE_SIZE ; i++){ 
		hash_table[i].key = 0;
		hash_table[i].head = NULL;
	}
	//Okurken icine okuyacagim degiskenler
	int index;
	unsigned long long int key;
	char docs[500];
	
	//Eger icine daha onceden kaydedilmis bir dosya yoksa okumadan bos tablo dondurur.
	if(fp == NULL){
		printf("There is no saved file! Hash table returned empty.\n");
		fclose(fp);
		return hash_table;
	}
	else{
		//eger var ise hash table'i dosyaya gore doldurur
		fscanf(fp,"%d\n",numof_words);
		while(!feof(fp)){
			//index ve key bilgisi satirin ilk iki elemani olarak okunur.
			fscanf(fp,"%d ",&index);
			fscanf(fp,"%llu ",&key);
			hash_table[index].key = key;	
			
			//bu kontrolun sebebi feof fonksiyonunun yapisindan kaynaklanan bir hatadir.
			//dosyanin son satirini iki defa okumasina sebep olur.
			if(!fgets(docs,500,fp)){
				break; //dosya sonuna gelindiyse cikis yapilir
			}
   			
   			char *tok = strtok(docs, "\n");
   			tok = strtok(docs, " ");
   			//Enter ve space karakterlerine gore tokenize edilen satirin elemanlari
   			//ayni zamanda o satirdaki indexe ait dokumanlar oldugundan
   			//sirasiyla ilgili dokuman listesine eklenir
			
			while(tok != NULL){
    			struct doc_node *tmp = (struct doc_node*)malloc(sizeof(struct doc_node));
				strcpy(tmp->doc_name, tok);
				tmp->next = NULL;		
    			tmp->next = hash_table[index].head;
				hash_table[index].head = tmp;
    			
      			tok = strtok(NULL, " ");
   			}
		}
		printf("Hash table filled from save file!\n");
		fclose(fp);
		printTable(hash_table);
		return hash_table;
	}
}

//Kullaiciya kolaylik saglayan aciklamalar ile fonksiyonlarin sirasiyla cagirildigi fonsiyon
void runGUI(struct node *hash_table){
	
	char word[MAX_WORD];
	unsigned long long int key;
	int numberof_words = 0;
	
	printf("------------WELCOME TO THE HASH TABLE------------\n\n");
	//Baslamadan once varsa save dosyasindan hash table doldurulur.
	hash_table = readFile(hash_table, &numberof_words);
	
	printf("\n------------ADDING DOCUMENTS---------------------\n\n");
	//Once istenen butun dosyalar tabloya eklenir.
	hash_table = insertDocs(hash_table, &numberof_words);
	//Tablo ekrana bastirilir.
	printTable(hash_table);
	
	printf("------------SEARCHING----------------------------\n\n");
	printf("Type exit for exit!\n\n");
	//Kullanici exit girene kadar alinan her kelime key'e donusturulup tabloda aranir.
	do{
		printf("\n\nType the word: ");
		gets(word);
		printf("\n");
		if(strcmp("exit", word) != 0){
		
			key = horner(word);
			search(hash_table, key, word);
			
		}
	}while(strcmp("exit", word) != 0);
	
	//Dosyaya yazma islemi tamamlanir ve program bitirilir.
	writeFile(hash_table,numberof_words);
	printf("--------HASH TABLE IS WRITTEN TO THE SAVE FILE---\n\n");
	
	printf("--------developed by Sadi------------------------\n");	
}

//Main fonksiyon icinde Tablo olusturulur ve GUI fonksiyonu cagirilir.
int main(){
	
	struct node *hash_table = (struct node*)malloc(TABLE_SIZE*sizeof(struct node));

	runGUI(hash_table);
	
	return 0;
}
