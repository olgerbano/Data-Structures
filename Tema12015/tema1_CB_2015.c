#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Element{
	char *KEY;
	char *IP;
}Element;


typedef struct node{
	void *INFO;
	struct node *next;
}node;


typedef struct hashtable{
	int M;
	node **V;
}hashtable;

int hash_function(char *key,int d){

	int i, suma = 0;
	for(  i = 0 ; i < strlen(key) ; i++ )
		suma += (int)key[i];
	return suma % d ;

}

int compareKeys(char *key1, char *key2)
{
	return strcmp(key1, key2);
}

hashtable *init_hashtable(int M){

	hashtable *table = calloc( 1, sizeof( hashtable ) );
	if( !table ) return NULL;

	table->V = calloc(M,sizeof(node*) );
	if(!( table->V )) {free (table); return NULL;}
	int i;
	for(  i = 0 ; i < M ; i++ ) table->V[i] = NULL;

	table->M = M;

	return table;

}
void insert_in_hashtable(hashtable *table, Element *e){
	int index = hash_function(e->KEY,table->M);
	node **head_ref = &table->V[index];
	node *current = table->V[index];
	while(current){
		if(strcmp(((Element*)(current->INFO))->KEY,e->KEY)==0)
			return;
		else current=current->next;
	}

	if(*head_ref == NULL || strcmp( ( (Element*)(*head_ref)->INFO)->KEY,e->KEY)>0){
		node *aux = calloc(1,sizeof(node));
		aux->next = *head_ref;
		aux->INFO = e;
		*head_ref = aux;
	}
	else{
		current = *head_ref;
		while(current->next !=NULL && strcmp( ( (Element*)( (current->next)->INFO) )->KEY,e->KEY)<0){
			current = current->next;
		}
		node *aux = calloc(1,sizeof(node));
		aux->next = current->next;
		aux->INFO = e;
		current->next = aux;
	}


}
char *find_in_hashtable_and_return(hashtable *table,Element e){
	int i;
	i = hash_function(e.KEY,table->M);
	e.IP = NULL;
	node *p = table->V[i];
	while(p){
		if(strcmp(((Element*)(p->INFO))->KEY,e.KEY) == 0){
			e.IP = (char*)malloc(strlen(((Element*)(p->INFO))->IP) * sizeof(char*) + 1 );
			strcpy(e.IP,((Element*)(p->INFO))->IP);
		}
		p = p->next;
	}
	return e.IP;
}
int find_in_hashtable(hashtable *table , char *e){
	int index;
	index = hash_function(e,table->M);
	node *p = table->V[index];
	while(p){
		if(strcmp(((Element *)(p->INFO))->KEY, e) == 0){
			return 1;
		}
		else p = p->next;
	}
	return 0;
}


void remove_from_hashtable(hashtable *table, char *key){
	int index = hash_function(key, table->M);
	node **p = &table->V[index];

	while(*p){
		if(strcmp(((Element*)((*p)->INFO))->KEY,key) == 0){
			node *aux = *p;
			if(!aux) return;
			*p = (*p)->next;
			free(aux->INFO);
			free(aux);
		}
		else
			p = &(*p)->next;
	}
}
void hashtable_destroy(hashtable **table){
	int i;
	node *current;
	for(i=0;i<(*table)->M;i++){
		current = (*table)->V[i];
		if(!current) continue;
		else{
			node *aux = current;
			current = aux->next;
			free(aux->INFO);
			free(aux->next);
			free(aux);
		}
	}

	free((*table)->V);
	free(*table);
	*table = NULL;
}
int main(int argc, char*argv[]){
	char *key = (char*)malloc(255*sizeof(char));
	char *ip = (char*)malloc(20*sizeof(char));
	char *command = (char*)malloc(15*sizeof(char));
	if(argc < 4) return -1;

	int dim = atoi(argv[1]);
	hashtable *tabela = init_hashtable(dim);
	freopen(argv[2], "r", stdin);
	//freopen(argv[3], "w", stdout);
	FILE *g = fopen( argv[3], "w" );
	while(fscanf(stdin,"%s",command) !=EOF){
		if(strcmp(command,"put")==0){
			Element *aux = calloc(1,sizeof(Element));
			fscanf(stdin,"%s",key);
			fscanf(stdin,"%s",ip);
			aux->KEY = calloc(1, strlen(key));
			if (!aux->KEY){
				free(aux);return -1;
			}
			strcpy(aux->KEY,key);
			aux->IP = calloc(1, strlen(ip));
			if (!aux->IP){
				free(aux->KEY);
				free(aux);
				return -1;
			}
			strcpy(aux->IP,ip);

			insert_in_hashtable(tabela,aux);
		}
		if(strcmp(command,"print")==0){
			node *p = NULL;
			int i;
			for(i=0;i<dim;i++){
				p = tabela->V[i];
				if(p==NULL) continue;
				printf("%d: ",i);
				fprintf(g, "%d: ",i );
				for(;p!=NULL;p=p->next){
					printf("%s ",((Element*)(p->INFO))->IP );
					fprintf(g,"%s ",((Element*)(p->INFO))->IP );
				}
				printf("\n");
				fprintf(g,"\n");

			}
		}
		if(strcmp(command,"get")==0){
			fscanf(stdin,"%s",key);
			Element aux;
			aux.KEY = (char*)malloc(strlen(key)*sizeof(char)+1);
			strcpy(aux.KEY,key);
			char *val = find_in_hashtable_and_return(tabela,aux);
			if(val==NULL){ printf("NULL\n"); fprintf(g,"NULL\n");}
			else {printf("%s\n",val); fprintf(g,"%s\n",val);}

		}
		if(strcmp(command,"print_bucket")==0){
			int bucket;
			fscanf(stdin,"%d",&bucket);
			if(bucket>=dim) continue;
			else{
				node *p = tabela->V[bucket];
				if(!p){
					printf("VIDA");
					fprintf(g,"VIDA");
				}
				while(p){
					fprintf(g,"%s ",((Element*)(p->INFO))->IP);
					printf("%s ",((Element*)(p->INFO))->IP);
					p = p->next;
				}
			}
			printf("\n");
			fprintf(g,"\n");

		}
		if(strcmp(command,"find")==0){
			fscanf(stdin, "%s", key);
			if(find_in_hashtable(tabela,key)==1){
				printf("True\n");
				fprintf(g,"True\n");
			}
			else{ printf("False\n"); fprintf(g,"False\n");}

		}

		if(strcmp(command,"remove")==0){
			fscanf(stdin, "%s",key );
			remove_from_hashtable(tabela,key);
		}
	}
	hashtable_destroy(&tabela);
	return 0;

}
