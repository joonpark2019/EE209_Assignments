#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"


//Uncomment and use the following code if you want

#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
  
};



/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  // Uncomment and use the following implementation if you want 
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize, sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);   
    free(d);
    return NULL;
  }

  d->numItems = 0;
  return d;
  
  //return NULL;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  if (d == NULL)
    assert(0);

  for(int i=0; i < d->numItems; i++){
    if(d->pArray[i].name != NULL)
      //printf("%s", "Made it"); fflush(stdout);
      free(d->pArray[i].name);
    if(d->pArray[i].id != NULL)
      free(d->pArray[i].id);
  }

  //free(d->pArray);
  //free(d);
  
  return;
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* fill out this function */
  assert(d); assert(id); assert(name);
  int i;
  if(d==NULL || id == NULL || name == NULL || purchase <= 0)
    return (-1);


  //reach the first non-NULL entry in the database's array:
  /*while traversing through pre-existing entries, check if there is already a
matching element. If there is, modify existing element with no memory leak
and return as a failure */
  for(i=0; i < d->numItems; i++){
    if( strcmp(d->pArray[i].name, name) == 0 || strcmp(d->pArray[i].id, id)==0){
      d->pArray[i].purchase = purchase;
      return -1;
    }
  }
  //by end of for loop, i is incremented up to the first empty struct UserInfo

  struct UserInfo *q; //this is a temporary array variable
  //if we reach the end of pArray, we must expand pArray
  if(i == d->curArrSize)
    q = realloc(d->pArray, (d->curArrSize) + UNIT_ARRAY_SIZE);
  //must check if realloc failed:
  if(q == NULL){
    fprintf(stderr, "Error: reallocation failed. Could not expand pArray\n");
    free(q); //memory allocation failed
  }else{
    d->pArray = q; //resized array of UserInfo pointers
    d->curArrSize += UNIT_ARRAY_SIZE;
  }

//creating new UserInfo only after no matching elements found and enough space made:
// p is a pointer to new customer UserInfo entry
  char* name_buf = strdup((char *)name);
  char* id_buf =  strdup((char *)id);
  assert(name_buf); assert(id_buf);
  d->pArray[i].name = name_buf; d->pArray[i].id = id_buf; d->pArray[i].purchase = purchase;
  //printf("%s", d->pArray[i].name); fflush(stdout);
  d->numItems++;


  return 0;
  
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  assert(d); assert(id);
  if(d==NULL || id == NULL)
    return (-1);

  //char* id_buf = malloc(sizeof((char *)id));
  //strcpy(id_buf, (char* )id);

  for(int i=0; i < d->numItems; i++){
    if( strcmp(d->pArray[i].id, id)==0 ){
      free(d->pArray[i].id);
      d->pArray[i].id = NULL;
      d->numItems--;
      return 0;
    }
  }
  
  return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
    /* fill out this function */
  assert(d); assert(name);
  if(d==NULL || name == NULL)
    return (-1);

  for(int i=0; i < d->numItems; i++){
    if( strcmp(d->pArray[i].name, name)==0){
      free(d->pArray[i].name);
      d->pArray[i].name = NULL;
      d->numItems--;
      return 0;
    }
  }

  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */
  assert(d); assert(id);
  if(d==NULL || id==NULL)
    return(-1);

  for(int i=0; i < d->numItems; i++){
    if(strcmp(d->pArray[i].id, id)==0)
      return d->pArray[i].purchase;
  }

  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  assert(d); assert(name);
  if(d==NULL || name==NULL)
    return(-1);

  for(int i=0; i < d->numItems; i++){
    if(strcmp(d->pArray[i].name, name)==0)
      return d->pArray[i].purchase;
  }

  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  assert(d); assert(fp);
  if(d==NULL || fp==NULL)
    return (-1);

  int result = 0;
  for(int i=0; i < d->numItems; i++){
    if(d->pArray[i].name != NULL && d->pArray[i].id != NULL && d->pArray[i].purchase > 0)
      result += (*fp)(d->pArray[i].id, d->pArray[i].name, d->pArray[i].purchase);
    
  }

  return result;
}