#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include "tokenizer.h"
#include "hashtable.h"
#include "sorted-list.h"


typedef struct indexnode{
  char* filename;
  int count;


}indexnode;

typedef struct CLL
{
  int count;
  struct linknode* tail;
}CLL;
typedef struct linknode
{
  char data;
  struct linknode* next;
}linknode;    
typedef enum{

  word

} tokentype;

typedef struct TokenizerT {
  tokentype thistoken;
  char* token;
}TokenizerT;


void TokenFunction(TokenizerT* read, hashtable* index,char* filename,CompareFuncT cf,DestructFuncT df);
void  addEnd (char val,CLL* list);
char* CLLtoStr(CLL* list);
TokenizerT *TKCreate( char * ts );
void TKDestroy( TokenizerT * tk );
char *TKGetNextToken( char* here );
void TokenizeString(char* string,hashtable* index,char*filename,CompareFuncT cf, DestructFuncT df);
void TokenizeFile(char* filename,hashtable* index,CompareFuncT cf,DestructFuncT df);
void printIndeces(char* indexname,SortedList* indexList,hashtable* index);
#endif
