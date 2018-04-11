#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// List element for Dictionary lists.
typedef struct ListNode {
	struct ListNode *next;
	struct ListNode *prev;
	char *key;
	char *value;
	int frequency;
} ListNode;

// Dictionary structure that includes the lists of elements and their number.
typedef struct Dictionary {
	ListNode **lists;		// lists of elements.
	int N;					// number of lists.
} Dictionary;
// Returns a new Dictionary with reversed lists of the input structure.


Dictionary* createDictionary(int N) {

	Dictionary *table = calloc( 1, sizeof( Dictionary ) );
	if( !table ) {free (table); return NULL;}

	table->lists = calloc(N,sizeof(ListNode*) );
	if(!( table->lists )) { free (table->lists); free (table); return NULL;}
	int i;
	for(  i = 0 ; i < N ; i++ ) table->lists[i] = NULL;

	table->N = N;

	return table;

}
int hash_function(char *key,int d){

	int i, suma = 0;
	for(  i = 0 ; i < strlen(key) ; i++ )
		suma += (int)key[i];
	return suma % d ;

}

void removeElement(Dictionary *dictionary, char *key, char *value) {
	int index = hash_function(key,dictionary->N);
	int found = 0;
	ListNode **start = &dictionary->lists[index];
		if (*start == NULL)
        return;
    ListNode *temp = *start;
		if(temp->next == *start && temp->prev == *start){
			temp->next = NULL;
			temp->prev = NULL;
			free(temp->key);
			free(temp->value);
			free(temp);
			temp = NULL;
			*start = NULL;
			return;
		}
    while (temp->next != *start){
	     if( ( strcmp(key,temp->key)==0 ) && (strcmp(value,temp->value)==0 ) && ( temp == *start) ){
	     	*start = temp->next;
	    	temp->prev->next = *start;
	    	temp->next->prev = temp->prev;
				free(temp->key);
				free(temp->value);
				free(temp);
				temp = NULL;
	    	return;
	     }
	     if( ( strcmp(key,temp->key)==0 ) && (strcmp(value,temp->value)==0 )  ){
	     	temp->prev->next = temp->next;
	     	temp->next->prev = temp->prev;
				free(temp->key);
				free(temp->value);
				free(temp);
				temp = NULL;
	     	return;
	     }
	    temp = temp->next;
	 }

		if( ( strcmp(key,temp->key)==0 ) && (strcmp(value,temp->value)==0 ) ){
	     	temp->prev->next = *start;
	    	temp->next->prev = temp->prev;
				free(temp->key);
				free(temp->value);
	    	free(temp);
				temp = NULL;
	    	return;
	     }
}
int list_size(ListNode *start){
	struct ListNode *temp = start;
	int i;
	i=0;
	if (temp==NULL)
		return i;
	if(temp->next == start){
		return 1;
	}
	else{
		while (temp->next != start){
	        i++;
	        temp = temp->next;
	    }
	    i++;
    }
    return i;
}
int find_in_list(ListNode *start, char *key, char *value){
ListNode *temp = start;
int found = 0;
	if(!temp) return 0;

		while (temp->next != start)
			{

				if(!strcmp(key,temp->key) && !strcmp(value,temp->value))
					 found = 1;

			 temp = temp->next;
			}
			if(!strcmp(key,temp->key) && !strcmp(value,temp->value))
				 found = 1;
	return found;
}
void addElement(Dictionary *dictionary, char *key, char *value, int frequency) {
	int index = hash_function(key,dictionary->N);
	ListNode **start = &dictionary->lists[index];
	ListNode *iter = dictionary->lists[index];
	struct ListNode* end = NULL;
  int newfreq, old_freq;
  struct ListNode* new_node = calloc( 1, sizeof( ListNode ) );
  new_node->frequency = frequency;
  new_node->key = calloc(1,strlen(key));
  strcpy(new_node->key,key);
  new_node->value = calloc(1,strlen(value));
  strcpy(new_node->value,value);
  new_node->next = new_node;
  new_node->prev = new_node;
  int nr_list;
  int nr_total_list;
  nr_list = 0;
  nr_list = list_size(dictionary->lists[index]);
	int found;
	found = find_in_list(dictionary->lists[index],key, value);
	if(found == 0){
	  	if(nr_list == dictionary->N){
	  		removeElement(dictionary,(*start)->prev->key,(*start)->prev->value);

	  	}
			nr_list = 0;
			int i;
			for(i=0;i<dictionary->N;i++){
				nr_list += list_size(dictionary->lists[i]);
			}

			if(nr_list>2*dictionary->N){
				for(i=0;i<dictionary->N;i++){
					ListNode **start2 = &dictionary->lists[i];
					if(*start2 == NULL) continue;
					//ListNode **last = &dictionary->lists[index]->prev;
					removeElement(dictionary,(*start2)->prev->key,(*start2)->prev->value);
				}
			}
	}
  ListNode *current = dictionary->lists[index];
  if(*start != NULL){
	   if (((*start)->next == *start) &&
  		(!(strcmp(key,((*start)->key))) &&
  			!(strcmp(value,((*start)->value))) ) ||
  			(!(strcmp(key,((*start)->key))) &&
  				!(strcmp(value,((*start)->value)))))
  		{
	    	old_freq = (*start)->frequency;
	    	(*start)->frequency+= frequency;
	    	newfreq = (*start)->frequency;
	    	removeElement(dictionary,key, value);
		    addElement(dictionary,key,value,newfreq);
	    	return;
	    }
  	while (current->next != *start){
      	if(!(strcmp(key,((current->next)->key))) &&
			!(strcmp(value,((current->next)->value))) )
		{
	       	old_freq = current->next->frequency;
	       	current->next->frequency+= frequency;
	       	newfreq = (current->next)->frequency;
  				removeElement(dictionary,key, value);
    			addElement(dictionary,key,value,newfreq);
        	return;
    }
        	current = current->next;
    	}
	}
	  if(*start == NULL){
	      *start = new_node;
	      end = new_node;
	      return;
	  }
	  else if ( (new_node->frequency > (*start)->frequency) ||
    (  (new_node->frequency == (*start)->frequency) &&
    	(strcmp(new_node->key,(*start)->key)<0) ) ||
    ( (new_node->frequency == (*start)->frequency) &&
    	(strcmp(new_node->key,(*start)->key)==0) &&
    	(strcmp(new_node->value,(*start)->value)<0) ) )
    {
	  	end = (*start)->prev;
	    new_node->next = *start;
	    new_node->prev = end;
	    (*start)->prev = new_node;
	    end->next = new_node;
	    *start = new_node;
	    return;
	  }
	    else{
	    	current = *start;
	      int i;
	      i=1;
	      struct ListNode* temp = calloc( 1, sizeof( ListNode ) );
	      while(current->next != *start )
	      {
	          if ( (new_node->frequency > (current->next)->frequency) ||
            (  (new_node->frequency == (current->next)->frequency) &&
            	(strcmp(new_node->key,(current->next)->key)<0) ) ||
            ( (new_node->frequency == (current->next)->frequency) &&
            	(strcmp(new_node->key,(current->next)->key)==0) &&
            	(strcmp(new_node->value,(current->next)->value)<0) ) )
            	{
	              temp = current->next;
	              if(current->next->next == *start){
	              	temp->next = *start;
	              	(*start)->prev = temp;
	              }
	              current->next = new_node;
	              new_node->prev = current;
	              new_node->next = temp;
	              temp->prev = new_node;
	              return;
	            }
	            current = current->next;
	        }
	        current->next = new_node;
	        new_node->prev = current;
	        (*start)->prev = new_node;
	        new_node->next = (*start);
	        return;
	    }
}


void printDictionary(FILE *f,Dictionary *dictionary) {
	ListNode *p = NULL;

	int i;
	int j;
	j=0;
	for(i=0;i<dictionary->N;i++){
		p = dictionary->lists[i];
		struct ListNode *temp = p;
		if(p==NULL) {
			fprintf(f,"List %d: \n",i);
		}
		else{
			fprintf(f,"List %d: ",i);
			while (temp->next != p){
	        	j=j+1;
	    			fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
	        	temp = temp->next;
    	}
    	j=j+1;
			fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
			fprintf(f,"\n");
		}
	}
}
void insert_to_Begin(struct ListNode** start, char *key, char *value, int frequency)
{
  struct ListNode *last;
  struct ListNode* new_node = calloc( 1, sizeof( ListNode ) );
	new_node->frequency = frequency;   // Inserting the data
	new_node->key = calloc(1,strlen(key));
	strcpy(new_node->key,key);
	new_node->value = calloc(1,strlen(value));
	strcpy(new_node->value,value);
	new_node->next = new_node;
  new_node->prev = new_node;
  if(*start == NULL){
	     *start = new_node;
	     last = new_node;
	     return;
	}
	else{
	    last = (*start)->prev;
		  new_node->next = *start;
	    new_node->prev = last;
  		last->next = new_node;
			(*start)->prev = new_node;
			*start = new_node;
	}
}
void insert_to_End(struct ListNode** start, char *key, char *value, int frequency)
{
    struct ListNode *last;
    struct ListNode* new_node = calloc( 1, sizeof( ListNode ) );
		new_node->frequency = frequency;   // Inserting the data
		new_node->key = calloc(1,strlen(key));
		strcpy(new_node->key,key);
		new_node->value = calloc(1,strlen(value));
		strcpy(new_node->value,value);
		new_node->next = new_node;
    new_node->prev = new_node;
    if (*start == NULL)
    {
        last = new_node;
        *start = new_node;
				last->next = (*start);
				(*start)->prev = last;
        return;
    }
    last = (*start)->prev;
    new_node->next = *start;
    (*start)->prev = new_node;
    new_node->prev = last;
    last->next = new_node;
}
Dictionary* reverseLists(Dictionary *dictionary) {
	Dictionary *dictionaryreversed = createDictionary(dictionary->N);
	int i;
	struct ListNode *start=NULL, *temp=NULL;
	for(i=0;i<dictionary->N;i++){
		start = dictionary->lists[i];
		temp = start;
		if(!temp) continue;
		while (temp->next != start){
	      insert_to_Begin(&(dictionaryreversed->lists[i]),temp->key, temp->value, temp->frequency);
	      temp = temp->next;
	  }
		insert_to_Begin(&(dictionaryreversed->lists[i]),temp->key, temp->value, temp->frequency);
	}
	return dictionaryreversed;
}
void freeList(ListNode *start){
	ListNode *temp = start;
	if (start == NULL){
		return;
	}
	if((start)->next == start){
		(start)->next = NULL;
		(start)->prev = NULL;
		free((start)->key);
		free((start)->value);
		free((start));
		(start) = NULL;
		return;
	}
	else{
		start = temp->next;
		temp->prev->next = start;
		temp->next->prev = temp->prev;
		free(temp->key);
		free(temp->value);
		temp->next = NULL;
		temp->prev = NULL;
		free(temp);
		freeList(start);
	}
}
void freeDictionary(Dictionary **table){
	int i;
	ListNode *current;
	for(i=0;i<(*table)->N;i++){
		current = (*table)->lists[i];
		freeList(current);
	}
	free((*table)->lists);
	free(*table);
	*table = NULL;
}
void printFrequency(FILE *f,Dictionary *dictionary , int frequency){
	struct ListNode *start=NULL, *temp=NULL;
	int i;
	for(i=0;i<dictionary->N;i++){
		start = dictionary->lists[i];
		temp = start;
		if(!temp) continue;
		while (temp->next != start)
	    {
	     if(frequency == temp->frequency)
	    	fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
	      temp = temp->next;
	    }

		if(frequency == temp->frequency){
				fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
	}
	fprintf(f,"\n");
}
void printValue(FILE *f,Dictionary *dictionary , char *value){
	struct ListNode *start=NULL, *temp=NULL;
	int i;
	for(i=0;i<dictionary->N;i++){
	start = dictionary->lists[i];
	temp = start;
	if(!temp) continue;
	while (temp->next != start){
		if(!strcmp(value,temp->value))
	  fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
	  temp = temp->next;
	}
	if(!strcmp(value,temp->value))
				fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
	}
	fprintf(f,"\n");
}
ListNode* get(Dictionary *dictionary, char *key) {
	int index = hash_function(key,dictionary->N);
	ListNode *start = dictionary->lists[index];
	ListNode *temp = start;
	struct ListNode* listget = NULL;
	if(!temp) return NULL;
	int freq = 0;
	char *key1 = calloc(1,strlen(key));
	strcpy(key1,key);
	while (temp->next != start){
		if(!strcmp(key,temp->key)){
			insert_to_End(&listget,temp->key,temp->value,temp->frequency);
			char *value =calloc(1,strlen(temp->value));
			strcpy(value,temp->value);
			freq= temp->frequency+1;
			removeElement(dictionary,key1, value);
     	addElement(dictionary,key,value,freq);
		}
			 temp = temp->next;
	}
	if(!strcmp(key,temp->key)){
		insert_to_End(&listget,temp->key,temp->value,temp->frequency);
		char *value =calloc(1,strlen(temp->value));
		strcpy(value,temp->value);
		freq= temp->frequency+1;
		removeElement(dictionary,key1, value);
	 addElement(dictionary,key,value,freq);
	}
	return listget;
}
void printList(FILE *f,ListNode *start) {
	struct ListNode *temp = start;
 	if(!(start)){
		fprintf(f,"\n");
		return;
	}
  while (temp->next != start){
				fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
        temp = temp->next;
    }
		fprintf(f,"(%s, %s, %d) ", temp->key, temp->value, temp->frequency);
  	fprintf(f,"\n");
}

ListNode* unionValues(Dictionary *dictionary, char *value){
	struct ListNode *start=NULL, *temp=NULL;
	struct ListNode *listunionvalue = NULL;
	int i;
	for(i=0;i<dictionary->N;i++){
		start = dictionary->lists[i];
		temp = start;
		if(!temp) continue;
		while (temp->next != start){
			if(!strcmp(value,temp->value))
		  	insert_to_End(&listunionvalue,temp->key,temp->value,temp->frequency);
		    temp = temp->next;
		  }
			if(!strcmp(value,temp->value))
		     	insert_to_End(&listunionvalue,temp->key,temp->value,temp->frequency);
	}
	return listunionvalue;
}

ListNode* unionMaxFrequencies(Dictionary *dictionary){
	struct ListNode *start=NULL, *temp=NULL;
	struct ListNode *listunionMaxFrequencies = NULL;
	int i;
	int maxfreq;
	for(i=0;i<dictionary->N;i++){
		start = dictionary->lists[i];
		temp = start;
		if(!temp) continue;
		maxfreq = dictionary->lists[i]->frequency;
		while (temp->next != start){
	     if(temp->frequency == maxfreq)
	     	insert_to_End(&listunionMaxFrequencies,temp->key,temp->value,temp->frequency);
	     temp = temp->next;
	  }
		if(temp->frequency == maxfreq)
	     	insert_to_End(&listunionMaxFrequencies,temp->key,temp->value,temp->frequency);
	}
	return listunionMaxFrequencies;
}

void easy_0() {
	FILE *f = fopen("test_easy_0.txt", "w");
	if (f == NULL) {
		printf("Error opening file test_easy_0!\n");
		return;
	}

	Dictionary *dictionary = createDictionary(4);
	printDictionary(f, dictionary);

	Dictionary *reversedDictionary = reverseLists(dictionary);
	printDictionary(f, reversedDictionary);

	freeDictionary(&reversedDictionary);
	freeDictionary(&dictionary);
	fclose(f);
}

void easy_1() {
	FILE *f = fopen("test_easy_1.txt", "w");
	if (f == NULL) {
		printf("Error opening file test_easy_1!\n");
		return;
	}

	Dictionary *dictionary = createDictionary(4);
	addElement(dictionary, "key1", "value1", 20);
	addElement(dictionary, "key2", "value2", 30);
	addElement(dictionary, "key3", "value3", 15);
	addElement(dictionary, "key4", "value4", 20);
	printDictionary(f, dictionary);
	printFrequency(f, dictionary , 20);
	printValue(f, dictionary , "value4");

	if (dictionary && dictionary->lists && dictionary->lists[0]) {
		fprintf(f, "%d\n", dictionary->lists[0] == dictionary->lists[0]->prev);
	}

	Dictionary *reversedDictionary = reverseLists(dictionary);
	printDictionary(f, reversedDictionary);

	freeDictionary(&reversedDictionary);
	freeDictionary(&dictionary);
	fclose(f);
}

void easy_2() {
	FILE *f = fopen("test_easy_2.txt", "w");
	if (f == NULL) {
		printf("Error opening file test_easy_1!\n");
		return;
	}

	Dictionary *dictionary = createDictionary(4);
	addElement(dictionary, "key1", "value1", 20);
	addElement(dictionary, "key2", "value2", 30);
	addElement(dictionary, "key3", "value3", 15);
	addElement(dictionary, "key4", "value4", 20);
	printDictionary(f, dictionary);
	printValue(f, dictionary , "value4");

	removeElement(dictionary, "key3", "value3");
	addElement(dictionary, "key3", "value4", 50);
	printValue(f, dictionary , "value4");

	Dictionary *reversedDictionary = reverseLists(dictionary);
	printDictionary(f, reversedDictionary);

	freeDictionary(&reversedDictionary);
	freeDictionary(&dictionary);
	fclose(f);
}

void medium_1() {
	FILE *f = fopen("test_medium_1.txt", "w");
	if (f == NULL) {
		printf("Error opening file test_medium_1!\n");
		return;
	}

	Dictionary *dictionary = createDictionary(4);
	addElement(dictionary, "key1", "value1", 20);
	addElement(dictionary, "key1", "value2", 20);
	addElement(dictionary, "key1", "value3", 20);
	addElement(dictionary, "key1", "value4", 20);
	addElement(dictionary, "key1", "value5", 20);
	printDictionary(f, dictionary);
	removeElement(dictionary, "key1", "value0");
	removeElement(dictionary, "key1", "value2");
	removeElement(dictionary, "key1", "value5");
	addElement(dictionary, "key1", "value0", 20);
	printDictionary(f, dictionary);
	printValue(f, dictionary , "value4");

	ListNode *list = get(dictionary, "key1");
	printList(f, list);
	freeList(list);
	list = unionValues(dictionary, "value0");
	printList(f, list);
	freeList(list);
	list = unionValues(dictionary, "value6");
	printList(f, list);
	freeList(list);
	list = unionMaxFrequencies(dictionary);
	printList(f, list);
	freeList(list);

	addElement(dictionary, "key1", "value1", 20);

	list = unionMaxFrequencies(dictionary);
	printList(f, list);
	freeList(list);
	printDictionary(f, dictionary);

	Dictionary *reversedDictionary = reverseLists(dictionary);
	printDictionary(f, reversedDictionary);

	freeDictionary(&reversedDictionary);
	freeDictionary(&dictionary);
	fclose(f);
}

void medium_2() {
	FILE *f = fopen("test_medium_2.txt", "w");
	if (f == NULL) {
		printf("Error opening file test_medium_2!\n");
		return;
	}

	Dictionary *dictionary = createDictionary(4);
	addElement(dictionary, "key1", "value1", 20);
	addElement(dictionary, "key1", "value2", 20);
	addElement(dictionary, "key1", "value3", 20);
	addElement(dictionary, "key1", "value4", 20);
	addElement(dictionary, "key1", "value5", 20);
	removeElement(dictionary, "key1", "value2");
	removeElement(dictionary, "key1", "value5");
	addElement(dictionary, "key1", "value0", 20);

	ListNode *list = get(dictionary, "key2");
	printList(f, list);
	freeList(list);
	printFrequency(f, dictionary , 21);
	addElement(dictionary, "key1", "value0", 20);
	printDictionary(f, dictionary);
	printFrequency(f, dictionary , 21);

	Dictionary *reversedDictionary = reverseLists(dictionary);
	printDictionary(f, reversedDictionary);

	freeDictionary(&reversedDictionary);
	freeDictionary(&dictionary);

	dictionary = createDictionary(4);
	addElement(dictionary, "key", "value1", 20);
	addElement(dictionary, "key", "value0", 20);
	addElement(dictionary, "key", "value2", 30);
	addElement(dictionary, "key", "value3", 15);
	addElement(dictionary, "key4", "value1", 20);
	addElement(dictionary, "key1", "value1", 20);
	addElement(dictionary, "key1", "value0", 20);
	addElement(dictionary, "key1", "value2", 30);
	addElement(dictionary, "key1", "value3", 15);
	addElement(dictionary, "key14", "value1", 20);
	addElement(dictionary, "key", "value", 30);
	printDictionary(f, dictionary);
	list = get(dictionary, "key");
	printList(f, list);
	freeList(list);
	list = get(dictionary, "key14");
	printList(f, list);
	freeList(list);
	printDictionary(f, dictionary);
	list = get(dictionary, "key1");
	printList(f, list);
	freeList(list);
	list = get(dictionary, "key1");
	printList(f, list);
	freeList(list);
	list = unionMaxFrequencies(dictionary);
	printList(f, list);
	freeList(list);
	printDictionary(f, dictionary);
	printFrequency(f, dictionary, 77);
	printFrequency(f, dictionary, 21);
	printValue(f, dictionary, "value1");
	printValue(f, dictionary, "value4");
	list = unionValues(dictionary, "value777");
	printList(f, list);
	freeList(list);
	list = unionValues(dictionary, "value0");
	printList(f, list);
	freeList(list);
	freeDictionary(&dictionary);

	fclose(f);
}

void complex_1() {
	FILE *f = fopen("test_complex_1.txt", "w");
	if (f == NULL) {
		printf("Error opening file test_complex_1!\n");
		return;
	}

	Dictionary *dictionary = createDictionary(9);
	addElement(dictionary, "key333", "value333", 33);
	addElement(dictionary, "key333", "value303", 22);
	addElement(dictionary, "key333", "value330", 44);
	addElement(dictionary, "key333", "value331", 22);
	addElement(dictionary, "key333", "value033", 22);
	addElement(dictionary, "key396", "value033", 22);
	addElement(dictionary, "key000", "value033", 22);
	addElement(dictionary, "key333", "value306", 44);
	addElement(dictionary, "key333", "value336", 44);
	addElement(dictionary, "key000", "value336", 44);
	addElement(dictionary, "key396", "value336", 44);
	printDictionary(f, dictionary);
	freeDictionary(&dictionary);

	dictionary = createDictionary(1);
	addElement(dictionary, "key", "value", 3);
	addElement(dictionary, "key", "value", 2);
	addElement(dictionary, "key", "value", 1);
	printDictionary(f, dictionary);
	addElement(dictionary, "key", "value1", 1);
	printDictionary(f, dictionary);
	freeDictionary(&dictionary);

	dictionary = createDictionary(5);
	addElement(dictionary, "key555", "value555", 55);
	addElement(dictionary, "key555", "value555", 77);
	addElement(dictionary, "key555", "value555", 44);
	addElement(dictionary, "key555", "value555", 60);
	addElement(dictionary, "key555", "value555", 50);
	addElement(dictionary, "key555", "value444", 44);
	addElement(dictionary, "key555", "value777", 44);
	addElement(dictionary, "key555", "value333", 77);
	addElement(dictionary, "key555", "value777", 77);
	addElement(dictionary, "key555", "value", 55);
	addElement(dictionary, "key", "value", 55);
	addElement(dictionary, "key444", "value444", 44);
	addElement(dictionary, "key777", "value444", 44);
	addElement(dictionary, "key444", "value000", 44);
	addElement(dictionary, "key444", "value777", 77);
	addElement(dictionary, "key777", "value777", 77);
	addElement(dictionary, "key555", "value888", 77);
	addElement(dictionary, "key444", "value000", 44);
	addElement(dictionary, "key222", "value000", 77);
	printDictionary(f, dictionary);

	ListNode *list = unionMaxFrequencies(dictionary);
	printList(f, list);
	freeList(list);
	printFrequency(f, dictionary, 77);
	list = get(dictionary, "key777");
	printList(f, list);
	freeList(list);
	printDictionary(f, dictionary);
	list = get(dictionary, "key222");
	printList(f, list);
	freeList(list);
	printDictionary(f, dictionary);
	list = get(dictionary, "key2");
	printList(f, list);
	freeList(list);
	printFrequency(f, dictionary, 122);
	printValue(f, dictionary, "value777");
	list = unionValues(dictionary, "value777");
	printList(f, list);
	freeList(list);
	list = unionValues(dictionary, "value27");
	printList(f, list);
	freeList(list);
	printValue(f, dictionary, "value23");
	printDictionary(f, dictionary);

	Dictionary *reversedDictionary = reverseLists(dictionary);
	printDictionary(f, reversedDictionary);

	Dictionary *identicalDictionary = reverseLists(reversedDictionary);
	printDictionary(f, identicalDictionary);
	freeDictionary(&reversedDictionary);

	removeElement(dictionary, "key555", "value555");
	removeElement(dictionary, "key555", "value555");
	removeElement(dictionary, "key555", "value555");
	removeElement(dictionary, "key555", "value555");
	removeElement(dictionary, "key555", "value555");
	removeElement(dictionary, "key555", "value444");
	removeElement(dictionary, "key555", "value333");
	removeElement(dictionary, "key555", "value777");
	removeElement(dictionary, "key555", "value");
	removeElement(dictionary, "key", "value");
	removeElement(dictionary, "key444", "value444");
	removeElement(dictionary, "key777", "value444");
	removeElement(dictionary, "key444", "value000");
	removeElement(dictionary, "key444", "value777");
	removeElement(dictionary, "key777", "value777");
	removeElement(dictionary, "key555", "value888");
	removeElement(dictionary, "key444", "value000");
	removeElement(dictionary, "key222", "value000");
	printDictionary(f, dictionary);

	reversedDictionary = reverseLists(dictionary);
	printDictionary(f, reversedDictionary);

	freeDictionary(&reversedDictionary);
	freeDictionary(&identicalDictionary);
	freeDictionary(&dictionary);
	fclose(f);
}

void complex_2() {
	FILE *f = fopen("test_complex_2.txt", "w");
	if (f == NULL) {
		printf("Error opening file test_complex_2!\n");
		return;
	}

	Dictionary *dictionary = createDictionary(4);
	addElement(dictionary, "key0", "value1", 20);
	addElement(dictionary, "key0", "value2", 20);
	removeElement(dictionary, "key0", "value1");
	removeElement(dictionary, "key0", "value2");
	addElement(dictionary, "key0", "value2", 20);
	addElement(dictionary, "key0", "value1", 20);
	removeElement(dictionary, "key0", "value1");
	removeElement(dictionary, "key0", "value2");
	addElement(dictionary, "key0", "value1", 20);
	addElement(dictionary, "key0", "value2", 20);
	addElement(dictionary, "key1", "value3", 20);
	addElement(dictionary, "key1", "value4", 20);
	addElement(dictionary, "key2", "value5", 20);
	addElement(dictionary, "key2", "value6", 20);
	addElement(dictionary, "key3", "value7", 20);
	addElement(dictionary, "key3", "value8", 20);
	printDictionary(f, dictionary);
	removeElement(dictionary, "key2", "value6");
	removeElement(dictionary, "key3", "value8");
	addElement(dictionary, "key4", "value1", 20);
	addElement(dictionary, "key8", "value1", 20);
	addElement(dictionary, "key1", "value3", 20);
	addElement(dictionary, "key9", "value3", 20);
	printDictionary(f, dictionary);
	freeList((get(dictionary, "key8")));
	printDictionary(f, dictionary);
	freeList(get(dictionary, "key0"));
	printDictionary(f, dictionary);
	freeList(get(dictionary, "key4"));
	printDictionary(f, dictionary);
	freeList(get(dictionary, "key1"));
	printDictionary(f, dictionary);
	freeList(get(dictionary, "key1"));
	printDictionary(f, dictionary);
	ListNode *list = unionMaxFrequencies(dictionary);
	printList(f, list);
	freeList(list);
	addElement(dictionary, "key9", "value3", 22);
	printDictionary(f, dictionary);
	addElement(dictionary, "key9", "value3", 2);
	addElement(dictionary, "key1", "value3", 1);
	printDictionary(f, dictionary);
	freeList(get(dictionary, "key1"));
	printDictionary(f, dictionary);
	freeList(get(dictionary, "key1"));
	printDictionary(f, dictionary);
	list = unionValues(dictionary, "value3");
	printList(f, list);
	freeList(list);
	printValue(f, dictionary, "value3");
	addElement(dictionary, "key4", "value1", 3);
	printFrequency(f, dictionary, 24);
	addElement(dictionary, "key0", "value0", 3);
	printDictionary(f, dictionary);
	freeList(get(dictionary, "key0"));
	printDictionary(f, dictionary);
	freeDictionary(&dictionary);
	fclose(f);
}

int main(int argc, char *argv[]) {
	//easy_0();
	//easy_1();

	//easy_2();
	//medium_1();
	//medium_2();
	//complex_1();
	//complex_2();

	return 0;
}
