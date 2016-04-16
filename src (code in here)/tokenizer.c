#include "tokenizer.h"

void TokenFunction(TokenizerT* read, hashtable* index,char*filename,CompareFuncT cf,DestructFuncT df){
  char* str = read->token;
  
  SortedList* filelist = get(str,index);
  
  SortedListIterator * iter;
  indexnode* new;
  if(filelist == NULL){
    filelist = SLCreate(cf,df);
    new = (indexnode*)malloc(1*sizeof(indexnode));
    new->filename = malloc(strlen(filename)+1);
    strcpy(new->filename,filename);
    new->count = 1;
    SLInsert(filelist,new);
    set(str,filelist,index);
  }
  else{
    iter = SLCreateIterator(filelist); 
    while((new=SLNextItem(iter))){
	if(strcmp(new->filename,filename)==0){
	  new->count++;
	  break;
	}
      }
    if (new == NULL){
      new = (indexnode*)malloc(1*sizeof(indexnode));
      new->filename = malloc(strlen(filename)+1);
      strcpy(new->filename,filename);
      new->count = 1;
      SLInsert(filelist,new);
    }
  }
}
void  addEnd (char val,CLL* list)
{
  linknode* tmp = NULL;
  linknode* new = NULL;
  new = (linknode*)malloc(1*sizeof(linknode));
  new -> data = val;
  new -> next = NULL;
  if (list->tail == NULL)
    {
      list->tail = new;                                     //insert into linked list
      list-> tail->next = NULL;
      list->count++;
      return;
    }
  else if (list->tail->next == NULL)
    {
      tmp =list->tail;
      list-> tail = new;
      list-> tail->next = tmp;
      tmp->next =list-> tail;
      list->count++;
      return;
    }
  else
    {
      tmp = list ->tail;
      list-> tail = new;
      list-> tail->next = tmp ->next;
      tmp->next = list->tail;
      list->count++;
      return;
    }
}
char* CLLtoStr(CLL* list)
{
  linknode* point;
  int count = list->count;
  char* str = NULL;
  str = (char *) calloc(count,sizeof(char));
  int ptr;
  if(list->tail == NULL)
    {
      free(str);
      str = NULL;
      return NULL;
    }
  else if(list->tail->next == NULL)
    {
      point = list->tail;
    }
  else                                                       //turn linked list into a string
    {
      point = list->tail->next;
    }
  for(ptr = 0; ptr<count;ptr++)
    {
      str[ptr] = point->data;
      point = point->next;
    }
  return str;
}
void deleteCLL (CLL* list)
{
  if(list->tail == NULL)
    {
      return;
    }
  linknode* ptr = list->tail->next;                          //delete a linked list
  linknode* tmp;
  tmp = ptr;
  int count = list->count;
  if(ptr == NULL)
    {
      free(list->tail);
      list->tail = NULL;
      list->count = 0;
      return;
    }
  while (count!=1)
    {

      ptr = ptr->next;
      free(tmp);
      tmp = ptr;
      count--;
    }
  free(ptr);
  ptr = NULL;
  tmp = NULL;
  list->tail = NULL;
  list->count = 0;
  return;
}




TokenizerT *TKCreate( char * ts ) {
  CLL* list = (CLL*) malloc(sizeof(CLL));
  list->tail = NULL;
  list->count = 0;
  TokenizerT* new = (TokenizerT*)malloc(sizeof(TokenizerT));
  char * ptr= ts;
  char* temp;
  int i;
  if(ptr!=NULL&&isalpha(*ptr)){
    while(isalpha(*ptr)||isdigit(*ptr)){
      addEnd(*ptr,list);
      ptr++;
    }
    addEnd('\0',list);
    temp = CLLtoStr(list);
    for(i = 0; temp[i]; i++){
      temp[i] = tolower(temp[i]);
    }

    new->token = temp;
    new->thistoken = word;
    deleteCLL(list);
    return new;  
  
  }
  else{
    free(new);
    return NULL;
  }
}


void TKDestroy( TokenizerT * tk ) {
  free(tk->token);
  free(tk);
}


char *TKGetNextToken(char*here ) {
  char*  ptr = here;
  CLL* list = (CLL*) malloc(sizeof(CLL));                                        
  list->count = 0;
  while(*ptr != '\0'&&!isalpha(*ptr)){ //Find next token
    ptr++;
  }
  if(isalpha(*ptr)){  //if found, return pointer to beginning
    return ptr;
  }
  else{      //else all tokens found, return null
    return NULL;
  }

}
void TokenizeString(char* string,hashtable* index,char*filename,CompareFuncT cf, DestructFuncT df){
  char* ptr;
  TokenizerT* new;
  ptr = string;
  while(ptr!= NULL &&*ptr!='\0'){
    ptr = TKGetNextToken(ptr);  //find token
    new = TKCreate(ptr);       //use token
    if(new!=NULL){            //if token was found do this
      TokenFunction(new,index,filename,cf,df);
      TKDestroy(new);
      new = NULL;
    }  
    while(ptr!=NULL&&*ptr != '\0'&&(isalpha(*ptr)||isdigit(*ptr))){  //get to end of token
      ptr++;
    }

  }
  free(filename);
  
}
void TokenizeFile(char* filename,hashtable* index,CompareFuncT cf,DestructFuncT df){
  char buffer[5000];
  if( access( filename, F_OK ) == -1 ) {
    printf("nonexistant directory or file\n");
    return;
  }
   
  FILE* fp = fopen(filename,"r");
  if(fp == NULL) 
    {
      
      return;
    }
  char* base;  
  while(fscanf(fp,"%4999s",buffer)!=EOF){
    base = malloc(strlen(filename)+1);
    strcpy(base,basename(filename));
    TokenizeString(buffer,index,base,cf,df);
  }
  free(filename);
  fclose(fp);
  return;
}
void printIndeces(char* indexname,SortedList* indexList,hashtable* index){
  SortedListIterator* printptr;
  SortedListIterator* filelister;
  indexnode* printindex;
  hashnode* tmpr;
  char inp;
  FILE* fp;
  if( access( indexname, F_OK ) != -1 ) {
    printf("write file exists, overwrite?: y/n \n");
    scanf("%c",&inp);
    if(inp == 'n'){
      return;
    }
    else if(inp == 'y'){
     
    }
  }
  if(index->items==0){
    return;
  }
  fp = fopen(indexname,"w");
  if (fp == NULL){
    printf("error opening indexfile\n");
    return;
  }
  printptr = SLCreateIterator(indexList);
  fprintf(fp,"{\"list\":[ \n");
  while(printptr!=NULL){
    tmpr = SLNextItem(printptr);
    if(tmpr !=NULL){

      fprintf(fp,"   {\"%s\":[ \n",tmpr->key);
      filelister = SLCreateIterator(tmpr->value);

      while(filelister!=NULL){
        printindex = SLNextItem(filelister);
        if(printindex!=NULL){
          fprintf(fp,"      {\"%s\": %d}\n",printindex->filename,printindex->count);
        }
        else{
	  fprintf(fp,"   ]}\n");
          break;
        }
      }
    }
    else{
      fprintf(fp,"]}\n");
      break;
    }
  }

}
