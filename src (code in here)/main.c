#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include "hashtable.h"

void hashnodeDestroy(hashnode* trash){
  free(trash->key);
  SLDestroy(trash->value);
  free(trash);

}

int hashnodeCompare(hashnode* first, hashnode* second){
  if(strcmp (first->key,second->key)==0){
    return 0;
  }
  else if(strcmp (first->key,second->key)<0){
    return 1;
  }
  else{
    return -1;
  }

}
void IndexNodeDestroy(indexnode* trash){
  free(trash->filename);
  free(trash);
}

int IndexCompare(indexnode* first, indexnode* second){
  if(first->count!=second->count){
    if(first->count>second->count){
      return 1;
    }
    else{
      return -1;
    }
  }
  else{
    if(strcmp(first->filename,second->filename)<0){
      return 1;
    }
    else if((strcmp(first->filename,second->filename)>0)){
	return -1;
      }
      else{
	return 0;
      }
  }

}
void dirhandler(char *name,hashtable* index, CompareFuncT cf,DestructFuncT df)
{
  DIR *dir;
  struct dirent *entry;
  char* path;
  if (!(dir = opendir(name))){
    TokenizeFile(name,index,cf,df);
    return;
  }
  if (!(entry = readdir(dir))){
    return;
  }
  do {

    path = malloc(strlen(name)+strlen(entry->d_name)+2);
    strcpy(path,name);
    strcat(path,"/");
    strcat(path,entry->d_name);

    if (entry->d_type == DT_DIR) {
     
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
        free(path);
	continue;
      }
      
      dirhandler(path,index,cf,df);
    }
    else{
      TokenizeFile(path,index,cf,df);
    }
  } while ((entry = readdir(dir)));
  free(name);
  closedir(dir);
}


int main(int argc,char** argv){
  CompareFuncT indexC = (CompareFuncT) IndexCompare;
  DestructFuncT indexD = (DestructFuncT) IndexNodeDestroy;
  CompareFuncT hashnodeC = (CompareFuncT) hashnodeCompare;
  DestructFuncT hashnodeD = (DestructFuncT) hashnodeDestroy;
   int i;
  hashnode* hold;
  char* s;
  char* indexname ;
  char* initial;
  if(argc!=3){
    printf("not enough args\n");
    return 0;
  }
  indexname = argv[1];
  s = argv[2];
  initial = malloc(strlen(s)+1);
  strcpy(initial,s);
  hashtable* index = constructor(80000);
  
  dirhandler(initial,index,indexC,indexD);


  SortedList* indexList;
  indexList = SLCreate(hashnodeC,hashnodeD);
  if(index->size>0){
    for(i=0;i<index->size;i++){
      hold = index->table[i];
      while(hold!=NULL){
	SLSort(hold->value);
	SLInsert(indexList,hold);
	hold = hold->next;
      }
    }
    printIndeces(indexname,indexList,index);
    SLDestroy(indexList);
  }
  free(initial);
  return 0;

}
