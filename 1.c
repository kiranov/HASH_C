#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

const int p = 53;
const int q = 1;

struct Man{
	char * name;
	char * surname;
	uint64_t tel;
} typedef Man;

struct Data{
	int size;
	void * data;
}typedef Data;

struct Hash{
	size_t size;
	Data * arr;
	int collision;
}typedef Hash;


//хеш функция для строки и int
uint64_t h_func1(Man man){
	char string[100];
	strcpy(string, man.name);
	strcpy(string, man.surname);
	int len = strlen(string);
	uint64_t h_sum = 0;
	int pow = 1;
	for(int i = 0; i < len; i++){
		if(string[i] >= 65 && string[i] <= 90){
			h_sum += (string[i] - 'A' + 1) * pow;
			pow = p * pow;
		}
		else{
			h_sum += (string[i] - 'a' + 1) * pow;
			pow = p * pow;
		}
	}
	h_sum = h_sum + (man.tel%p);
	return h_sum;
}


int h_hash2(Data * data, size_t size){
	int result = 0;
	int * arr;
	arr = (int*)data->data;
	for(size_t i = 0; i < data->size; i++){
		result += arr[i];
	} 
	result = result % size;
	return result;
}

void scan1(Data * data){
	scanf("%d", &(data->size));
	char arr[data->size];
	for(int i = 0; i < data->size; i++){
		scanf("%c",  &arr[i]);
	}
	data->data = (char*)data->data;
	data->data = arr;
}
/*
void scan2(Data * data, int size){
	printf("What is size of the one item?\n");
	int e_size;
	scanf("%d", &e_size);
	for(int i = 0; i < size; i++){
		data[i].size = e_size;
	}
	int arr[e_size];
	for(int j = 0; j < size; j++){
		printf("data %d\n", j);
		for(int i = 0; i < e_size; i++){
			printf("	item %d:\n", i);
			scanf("%d",  &arr[i]);
		}
		//data[j].data = data[j].(int*)data;
		data[j].data = arr;
		//printf("%с\n", data[j].data[0]);
	}
}
*/


void h_create(size_t size, Hash * hash){
	hash->size = size;
	hash->arr = (Data*)malloc(sizeof(Data)*size);
	Data nill;
	nill.size = -1;
	for(size_t i = 0; i < size; i++){
		hash->arr[i] = nill;
	}
	hash->collision = 0;
}

void print(Data data){
	int * a = (int*)data.data;
	for(int i = 0; i < data.size; i++){
		printf("%d ", a[i]);
	}
}

int h_find(Hash hash, Data * data, int (*h_func)(Data *, size_t)){
	int counter = 0;
	int * a = (int*)data->data;
	int start = h_func(data, hash.size);
	for(int i = start; i < hash.size; i += q){
		int * b = (int*)hash.arr[i].data;
		for(int j = 0 ; j < data->size; j++){
			if((hash.arr[i].size == data->size) && (b[j] == a[j])){
				counter++;
			}
		}
		if(counter == data->size){
			return i;
		}
	}
	return -1;
}

void h_insert(Hash * hash, Data data, int (*h_func)(Data *, size_t)){
		int current = h_func(&data, hash->size);
		printf("%d ", current);
		if(hash->arr[current].size != -1){
			hash->collision += 1;
		}
		while((hash->arr[current].size != -1)){
			current += q;
			if(current > hash->size){
				printf("Can't insert: no free place for");
				print(data);
				printf("element\n");
				break;
			}
		}
		printf("%d ", current);
		hash->arr[current] = data;
}


void h_print(Hash hash, void(*print)(Data)){
	printf("h_print\n");
	for(size_t i = 0; i < hash.size; i++){
		if(hash.arr[i].size != -1){
			print(hash.arr[i]);
			printf("\n");
		}
	}
}
 

void delete_1(Hash * hash, Data data, int (*h_func)(Data *, size_t)){
	int n = h_find(*hash, &data, h_func);
	printf("%d\n", n);
	int value = h_func(&data, hash->size);
	int i = n;
	while(hash->arr[i].size != -1){
		if(i > hash->size){
			break;
		}
		printf("i+q = %d , value = %d  hash_size = %d\n", i+q, value, hash->size);
		//(h_func(&(hash->arr[i+q]), hash->size) == value)
		if((i+q < hash->size) && (hash->arr[i+q].size != -1)){
			printf("BOOM");
			if((h_func(&(hash->arr[i+q]), hash->size) == value)){
				hash->arr[i] = hash->arr[i+q];
				i += q;
				printf("done");
			}
			else if((i+2*q)<=hash->size){
				i+=2*q;
			}
			 
		}
		else if(hash->arr[i+q].size == -1){
				free(hash->arr[i].data);
				hash->arr[i].size = -1;
				break;
			}
		else if(i+q >= hash->size){
			//printf("ABA");
			hash->arr[i].size = -1;
			free(hash->arr[i].data);
			break;
		}
	}
}

void delete(Hash * hash, Data data, int (*h_func)(Data *, size_t)){
	int n = h_find(*hash, &data, h_func);
	int value = h_func(&data, hash->size);
	int i = n, j = n;
	while(hash->arr[i].size != -1){
		if(i+q >= hash->size){
			free(hash->arr[j].data);
			hash->arr[j].size = -1;
			break;
		}
		if(hash->arr[i+q].size != -1){
			if(h_func(&(hash->arr[i+q]),hash->size) == value){
				//printf("AAA");
				//free(hash->arr[j].data);
				hash->arr[j] = hash->arr[i+q];
				//print(hash->arr[j]);
				//printf("\n");
				i += q;
				j = i;
				if(i >= hash->size){
					free(hash->arr[i+q].data);
					hash->arr[i+q].size = -1;
					break;
				}
				
			}
			else{
				i += q;
				if(i >= hash->size){
					free(hash->arr[i+q].data);
					hash->arr[i+q].size = -1;
					break;
				}
			}
		}
		else{
			free(hash->arr[j].data);
			hash->arr[j].size = -1;
			break;
		}
	}
}

int main(int argc, char **argv)
{

	size_t size;//, num;
	printf("What is the size of the table?\n");
	scanf("%d", &size);

	int a1[] = {1, 2, 3};
	Data A;
	A.size = 3;
	A.data = a1;
	
	int a12[] = {1, 2, 3};
	Data A1;
	A1.size = 3;
	A1.data = a12;
	
	int a13[] = {1, 2, 3};
	Data A2;
	A2.size = 3;
	A2.data = a13;
	
	int a14[] = {1, 2, 3};
	Data A3;
	A3.size = 3;
	A3.data = a14;
	
	int a15[] = {1, 2, 3};
	Data A4;
	A4.size = 3;
	A4.data = a15;
	
	int a2[] = {2, 3, 4};
	Data B;
	B.size = 3;
	B.data = a2;
	
	int a3[] = {12, 2, 17, 28};
	Data C;
	C.size = 4;
	C.data = a3;
	
	int a4[] = {4, 5};
	Data D;
	D.size = 2;
	D.data = a4;
	
	int a5[] = {1, 5, 7};
	Data E;
	E.size = 3;
	E.data = a5;
	
	int a6[] = {1, 2, 3};
	Data F;
	F.size = 3;
	F.data = a6;
	
	int a7[] = {2, 3, 4};
	Data G;
	G.size = 3;
	G.data = a7;
	
	Hash hash;
	h_create(size, &hash);
	//print(A);
	printf("insert:\n");
	h_insert(&hash, A, h_hash2);
	h_insert(&hash, B, h_hash2);
	h_insert(&hash, F, h_hash2);
	h_insert(&hash, D, h_hash2);
	h_insert(&hash, E, h_hash2);
	h_insert(&hash, C, h_hash2);
	h_insert(&hash, G, h_hash2);
	h_insert(&hash, A1, h_hash2);
	h_insert(&hash, A2, h_hash2);
	h_insert(&hash, A3, h_hash2);
	h_insert(&hash, A4, h_hash2);
	printf("\n %d\n", hash.collision);
	printf("\nfind\n");
	int find = h_find(hash, &G, h_hash2);
	printf("%d\n", find);
	//h_print(hash, print);
	printf("A\n");
	delete(&hash, A, h_hash2);
	//h_print(hash, print);
	printf("B\n");
	delete(&hash, B, h_hash2);
	//h_print(hash, print);
	printf("C\n");
	delete(&hash, C, h_hash2);
	//h_print(hash, print);
	printf("D\n");
	delete(&hash, D, h_hash2);
	delete(&hash, E, h_hash2);
	delete(&hash, F, h_hash2);
	delete(&hash, G, h_hash2);
	delete(&hash, A1, h_hash2);
	delete(&hash, A2, h_hash2);
	delete(&hash, A3, h_hash2);
	delete(&hash, A4, h_hash2);
	//printf("\n");
	h_print(hash, print);
	return 0;
}

