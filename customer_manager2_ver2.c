#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"


#define UNIT_ARRAY_SIZE 1024
enum {HASH_MULTIPLIER = 65599};

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
  struct UserInfo *nextId;
  struct UserInfo *nextName;
};

//structure for hash table:
//implement two: pArray and t_name
struct DB {
  struct UserInfo *pArrayId[UNIT_ARRAY_SIZE]; //could cause error
  struct UserInfo *pArrayName[UNIT_ARRAY_SIZE];
  int curArrSizeId; int curArrSizeName;
  int numItems;
};

/*--------------------------------------------------------------------*/
//Hash function:
static int hash_function(const char *pcKey, int iBucketCount)

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash % (unsigned int)iBucketCount);
}

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSizeName = UNIT_ARRAY_SIZE; // start with 1024 buckets
  d->curArrSizeId = UNIT_ARRAY_SIZE;
  d->numItems = 0;

  return d;
  
}



/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  struct UserInfo *pId;
  struct UserInfo *nextpId;

//just use the ID hash table to eliminate all nodes
  for(int i=0; i < d->curArrSizeId; i++){
    for(pId = d->pArrayId[i]; pId != NULL; pId = nextpId){
      nextpId = pId -> nextId;
      free(pId->name); free(pId->id); free(pId);
    }
  }

  free(d);

  return;
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* fill out this function */
  assert(d); assert(id); assert(name);
  if(d==NULL || id == NULL || name == NULL || purchase <= 0)
    return (-1);

  //hash indices for both ID and Name hash tables:
  int hId = hash_function((char *)id, d->curArrSizeId); 
  int hName = hash_function((char *)name, d->curArrSizeName);


  //separate memory allocated for name and id strings:
  //this way, the hash tables own the string memory
  
  struct UserInfo *pId, *pName;
  for(pId = d->pArrayId[hId]; pId !=NULL; pId = pId->nextId)
  {
      if(strcmp(pId->id, id)==0){
        return (-1);
      }
  }

  for(pName = d->pArrayName[hName]; pName != NULL; pName = pName->nextName )
  {
    if(strcmp(pName->name, name)==0){
        return (-1);
      }
  }
  
  //else: if no match was found:

  //char* name_buf = strdup((char *)name);
  //char* id_buf =  strdup((char *)id);
  struct UserInfo *p= (struct UserInfo *)calloc(1, sizeof(struct UserInfo));
  
//Initializing and adding data to node:
  p->name = strdup((char *)name); p->id = strdup((char *)id);
  p->purchase = purchase;

  //adding node to the ID hash table:
  p->nextId = d->pArrayId[hId];
  d->pArrayId[hId] = p;

  //adding node to the Name hash table:
  p->nextName = d->pArrayName[hName];
  d->pArrayName[hName] = p;
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

  /*Initialize pointers p and q to first node to linked lists
  hashed at hId and hName*/
  int hId = hash_function((char *)id, d->curArrSizeId);
  struct UserInfo *p = d->pArrayId[hId];

  if(p==NULL)
    return (-1);

  int hName = hash_function(p->name, d->curArrSizeName);
  struct UserInfo *q = d->pArrayName[hName];

  struct UserInfo *ptemp;

  //if first node is the match
  if(strcmp(p->id, id) == 0){
    if(p==q){
        d->pArrayId[hId] = p->nextId;
        d->pArrayName[hName] = q->nextName;

        free(p->name); free(p->id);free(p);
        return 0;
    }else{
        //traverse Name linked list until q->nextName 
        //points to same Node as p
        for(; q->nextName != p; q = q->nextName)
            ;
        q->nextName = q->nextName->nextName;
        d->pArrayId[hId] = p->nextId;
        free(p->name); free(p->id);free(p);
        return 0;
    }
  }

//else, compare using next->id and next->name:
/*this is the only way you can reconnect a linked list
if a matching node is in the middle of the list*/
  for (; p->nextId!=NULL; p = p->nextId){
      //if a match is found
      if(strcmp(p->nextId->id, id) == 0){
        if(p->nextId == q->nextName){
            ptemp = p->nextId;
            p->nextId = p->nextId->nextId;
            q->nextName = q->nextName->nextName;
            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else if(p->nextId == q){
            ptemp = p->nextId;
            d->pArrayName[hName] = q->nextName;
            p->nextId = p->nextId->nextId;
            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else{
            for(; q->nextName != p->nextId; q = q->nextName)
            ;
            ptemp = p->nextId;
            p->nextId = p->nextId->nextId;
            q->nextName = q->nextName->nextName;
            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
    }
    }
  
  
  //if match is not found, then failure:
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

  /*Initialize pointers p and q to first node to linked lists
  hashed at hId and hName*/
  int hName = hash_function((char *)name, d->curArrSizeName);
  struct UserInfo *p = d->pArrayName[hName];

  if(p==NULL)
    return (-1);
  
  int hId = hash_function(p->id, d->curArrSizeId);
  struct UserInfo *q = d->pArrayId[hId];

  //temporary pointers for freeing:
  struct UserInfo *ptemp;

  //if first node is the match
  if(strcmp(p->name, name) == 0){
    if(p==q){

      d->pArrayName[hName] = p->nextName;
      d->pArrayId[hId] = q->nextId;

      free(p->name); free(p->id); free(p);
      return 0;
    }else{
        //traverse Name linked list until q->nextId
        //points to same Node as p
        for(; q->nextId != p && q->nextId != NULL; q = q->nextId)
            ;
        
        q->nextId = q->nextId->nextId; //ERROR HERE
        d->pArrayName[hName] = p->nextName;

        free(p->name); free(p->id); free(p);
        return 0;
    }
  }

//else, compare using next->id and next->name:
/*this is the only way you can reconnect a linked list
if a matching node is in the middle of the list*/
  for (; p->nextName!=NULL; p = p->nextName){
      //if a match is found
      if(strcmp(p->nextName->name, name) == 0){
        if(p->nextName == q->nextId){
            ptemp = p->nextName;
            p->nextName = p->nextName->nextName;
            q->nextId = q->nextId->nextId;

            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else if(p->nextName == q){
            ptemp = p->nextName;
            d->pArrayId[hId] = q->nextId;
            p->nextName = p->nextName->nextName;

            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else{
            for(; q->nextId != p->nextName && q!=NULL; q = q->nextId)
            ;
            ptemp = p->nextName;
            p->nextName = p->nextName->nextName;
            q->nextId = q->nextId->nextId; //ERROR HERE
            //printf("%s", p->nextName->name); fflush(stdout);

            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
    }
    }
  
  
  //if match is not found, then failure:
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  
  assert(d); assert(id);
  if(d==NULL || id==NULL)
    return(-1);
  
  int hId = hash_function((char *)id, d->curArrSizeId);
  struct UserInfo *p = d->pArrayId[hId];

  //if the hash index has no nodes at all

  for(; p!=NULL; p = p->nextId){
    if(p->id != NULL){
      if(strcmp(p->id, id)==0){
        return p->purchase;
      }
    }
  }
  return (-1);
  
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{

  assert(d); assert(name);
  if(d==NULL || name==NULL)
    return(-1);
  
  int hName = hash_function((char *)name, d->curArrSizeName);
  struct UserInfo *p = d->pArrayName[hName];

  for(; p!=NULL; p = p->nextName){
    if(strcmp(p->name, (char *)name)==0){
      return p->purchase;
    }
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

  struct UserInfo *p;
  //use the ID hash table to iterate through all items
  for(int i=0; i < d->curArrSizeId; i++){
      for(p=d->pArrayId[i]; p!=NULL; p=p->nextId){
        if(p->name != NULL && p->id != NULL && p->purchase > 0)
            result += (*fp)(p->id, p->name, p->purchase);
      }
  }


  return result;
}