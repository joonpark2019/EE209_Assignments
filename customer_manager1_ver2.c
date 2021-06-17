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
/*This function returns a pointer d to a struct DB structure*/
DB_T
CreateCustomerDB(void)
{
  // Uncomment and use the following implementation if you want 
  DB_T d;
  
  //Allocate memory for the DB:
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
/*The function frees all nodes in the structure pointed to by
 input pointer d*/
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  if (d == NULL)
    assert(0);

//Iterate through all elements of pArray
  for(int i=0; i < d->curArrSize; i++){
  //if pointers at name and id fields are not null, free memory:
    if(d->pArray[i].name != NULL){
      //printf("%s", (char *)(d->pArray[i].name)); fflush(stdout);
      free(d->pArray[i].name);
      d->pArray[i].name = NULL;
    }
    if(d->pArray[i].id != NULL){
      free(d->pArray[i].id);
      d->pArray[i].id = NULL;
    }
  }
  free(d->pArray);
  free(d);
  
  return;
}
/*--------------------------------------------------------------------*/
/*This function takes a name string, an id string, a purchase integer,
and a pre-existing struct DB structure d as inputs. 
It returns -1 (failure) if a node with the same id or name already exists 
in d. 
It returns 0 if a new node with the input name, id, and purchase 
value is successfully added. */
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{

  //Note: The function owns the id and name strings as buffers
  
  assert(d!=NULL); assert(id); assert(name);
  if(d==NULL || id == NULL || name == NULL || purchase <= 0)
    return (-1);
  
  //if we reach the end of pArray, we must expand pArray
  if(d->numItems == d->curArrSize){
    struct UserInfo *q = realloc(d->pArray, (d->curArrSize + UNIT_ARRAY_SIZE)*sizeof(struct UserInfo));
    //printf("%s", "Wrong"); fflush(stdout);
  
  //must check if realloc failed:
  if(q == NULL){
    fprintf(stderr, "Error: reallocation failed. Could not expand pArray\n");
    free(q); //memory allocation failed
  }else{
    d->pArray = q; //resized array of UserInfo pointers
    //initializing realloced additional struct elements to NULL
    for(int k= d->curArrSize; k< (d->curArrSize + UNIT_ARRAY_SIZE) ; k++){
        d->pArray[k].name = NULL; d->pArray[k].id = NULL;
        d->pArray[k].purchase = 0;
    }
    d->curArrSize += UNIT_ARRAY_SIZE;
  }
  }

  //reach the first non-NULL entry in the database's array:
  /*while traversing through pre-existing entries, check if there is already a
matching element. If there is, return as a failure */ 

//first create buffers to store name and id inputs that d will own
  char* name_buf = strdup((char *)name);
  char* id_buf =  strdup((char *)id);
  assert(name_buf); assert(id_buf);

//iterate through all entries of pArray:
  for(int i=0; i < d->curArrSize; i++)
  {
    //If the entry is an empty element: add data
    if(d->pArray[i].name==NULL || d->pArray[i].id==NULL){
        d->pArray[i].name=name_buf; d->pArray[i].id=id_buf;
        d->pArray[i].purchase = purchase;
        d->numItems++;
        return 0;
    }

    /*If there is a pre-existing element with a matching name or id, 
    then delete free the memory allocated for the buffers*/
    if( strcmp(d->pArray[i].name, name) == 0 || strcmp(d->pArray[i].id, id)==0){
      free(name_buf); free(id_buf);
      return -1;
    }
  
  }
  
  return 0;
  
}
/*--------------------------------------------------------------------*/
/*The function takes a pointer d to a pre-existing struct DB and an
 id string as inputs */
/*The function removes the first struct UserInfo entry with the same id
as the input. 
Return val. is -1(failure) if there is no struct UserInfo entry with
the matching id and 0 otherwise. */
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  assert(d); assert(id);
  if(d==NULL || id == NULL)
    return (-1);


//iterate through all entries of pArray:
  for(int i=0; (i < d->curArrSize) && (d->pArray[i].purchase > 0); i++){
    //if the entry of pArray is empty, skip it
    if(d->pArray[i].id==NULL)
        continue;
    //if the id field of the entry matches the input id, free the data
    if( strcmp(d->pArray[i].id, id)==0 ){
      free(d->pArray[i].id); free(d->pArray[i].name);
      d->pArray[i].id = NULL; d->pArray[i].name = NULL;
      return 0;
    }
  }
  
  //if no match was found, return failure
  return (-1);
}

/*--------------------------------------------------------------------*/
/*The function takes a pointer d to a pre-existing struct DB d 
and a name string as inputs */
/*The function removes the first struct UserInfo entry with the same name
as the input. 
Return val. is -1(failure) if there is no struct UserInfo entry with
the matching name and 0 otherwise. */
int
UnregisterCustomerByName(DB_T d, const char *name)
{
    /* fill out this function */
  assert(d); assert(name);
  if(d==NULL || name == NULL)
    return (-1);

//iterate through all entries of pArray:
  for(int i=0; (i < d->curArrSize) && (d->pArray[i].purchase > 0); i++){
    //if the entry of pArray is empty, skip it
    if(d->pArray[i].id==NULL)
        continue;
    //if the name field of the entry matches the input id, free the data
    if( strcmp(d->pArray[i].name, name)==0){
      free(d->pArray[i].name); free(d->pArray[i].id);
      d->pArray[i].name = NULL; d->pArray[i].id = NULL;
      return 0;
    }
  }

  //if no match was found, return failure
  return (-1);
}
/*--------------------------------------------------------------------*/
/*The function takes a pointer to a preexisting struct DB and an id string
as inputs. It searches for an entry with a matching id and returns the 
purchase value of the entry if a match is found. 
If no match is found, the function returns -1. */
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */
  assert(d); assert(id);
  if(d==NULL || id==NULL)
    return(-1);

//iterate through all entries of pArray:
  for(int i=0; (i < d->curArrSize) && (d->pArray[i].purchase > 0); i++){
    //skip entries with no data:
    if(d->pArray[i].id==NULL || d->pArray[i].name==NULL)
        continue;
    //if a match is found, return the purchase value
    if(strcmp(d->pArray[i].id, id)==0)
      return d->pArray[i].purchase;
  }

  //if no match was found, return -1
  return (-1);
}
/*--------------------------------------------------------------------*/
/*The function takes a pointer to a preexisting struct DB and a name string
as inputs. It searches for a node with a matching name and returns the 
purchase value of the node if a match is found. 
If no match is found, the function returns -1. */
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  assert(d); assert(name);
  if(d==NULL || name==NULL)
    return(-1);

  //iterate through all entries of pArray:
  /*Note: the end condition of the for loop includes d->pArray[i].purchase > 0:
  this is because the UnRegister functions never set the purchase value to zero
  even when an entry is cleared. Thus, we can iterate up to the first entry with a
  purchase value of zero and safely assume no entries exist beyond that point.*/
  for(int i=0; (i < d->curArrSize) && (d->pArray[i].purchase > 0); i++){
   //skip entries with no data:
    if(d->pArray[i].id==NULL || d->pArray[i].name==NULL)
        continue;
    //if a match is found, return the purchase value
    if(strcmp(d->pArray[i].name, name)==0)
      return d->pArray[i].purchase;
  }

  //if no match was found, return -1
  return (-1);
}
/*--------------------------------------------------------------------*/
/*The function takes a pointer to a function fp and a pointer to a 
pre-existing struct DB structure as inputs. The function applies the function
pointed to by fp on every entry and returns the sum of all of the return values
for the entries*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  assert(d); assert(fp);
  if(d==NULL || fp==NULL)
    return (-1);

//Integer value for the sum of the return values for all entries
  int result = 0;
//Traverse through all entries of pArray:
  for(int i=0; (i < d->curArrSize) && (d->pArray[i].purchase > 0); i++){
  //For an entry containing data, add the return value of fp to the result variable:
    if(d->pArray[i].name != NULL && d->pArray[i].id != NULL && d->pArray[i].purchase > 0)
      result += (*fp)(d->pArray[i].id, d->pArray[i].name, d->pArray[i].purchase);

  }

  return result;
}