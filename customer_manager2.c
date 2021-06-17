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
  struct UserInfo *nextId; //pointer to next Node for Id hash table
  struct UserInfo *nextName; //pointer to next node for Name hash table
};

//structure for DB structure:
//implement two hash tables (tables of pointers): pArrayId and pArrayName
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
  /*Declare DB and assign memory 
  (initialize all entries of hash table to NULL) using calloc*/
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
/*The function frees all nodes in the structure pointed to by
 input pointer d*/
void
DestroyCustomerDB(DB_T d)
{
  
  struct UserInfo *pId;
  struct UserInfo *nextpId;

//just use the ID hash table to eliminate all nodes:
//Iterate through all entries of pArrayId:
  for(int i=0; i < d->curArrSizeId; i++){
    /*If pArrayId[i] != NULL, use nextId pointer to go through 
  nodes at index i and free the nodes and the string literals*/
    for(pId = d->pArrayId[i]; pId != NULL; pId = nextpId){
      nextpId = pId -> nextId;
      free(pId->name); free(pId->id); free(pId);
    }
  }

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
  /* fill out this function */
  assert(d); assert(id); assert(name);
  if(d==NULL || id == NULL || name == NULL || purchase <= 0)
    return (-1);

  //declare struct UserInfo pointers to iterate through ID and Name nodes
  int hId = hash_function((char *)id, d->curArrSizeId); 
  int hName = hash_function((char *)name, d->curArrSizeName);

  
  struct UserInfo *pId, *pName;
  /*Initialize pId to pArrayId[hId]. Then use nextId to iterate
  through the nodes at hash index hId and check if any matches are
  already present.*/
  for(pId = d->pArrayId[hId]; pId !=NULL; pId = pId->nextId)
  {
      if(strcmp(pId->id, id)==0){
        return (-1);
      }
  }
//repeat the process above for pArrayName
  for(pName = d->pArrayName[hName]; pName != NULL; pName = pName->nextName )
  {
    if(strcmp(pName->name, name)==0){
        return (-1);
      }
  }
  
  //else: if no match was found:
//allocate memory for a new User node:
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
/*The function takes a pointer d to a pre-existing struct DB and an
 id string as inputs */
/*The function removes the first struct UserInfo node with the same id
as the input. 
Return val. is -1(failure) if there is no struct UserInfo node with
the matching id and 0 otherwise. */
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
  //declare pointer p to point to first node at hId:
  struct UserInfo *p = d->pArrayId[hId];

  //If there is no Node at hId, it was already removed:
  //Therefore, return failure
  if(p==NULL)
    return (-1);

  /*The hName is initialized to the corresponding hash index 
  for the name field of the node with the input id. q is 
  later made to point to p. */
  int hName;
  struct UserInfo *q;

  struct UserInfo *ptemp; //pointer to be used later for freeing data

  //if first node at pArrayId[hId] is the match
  if(strcmp(p->id, id) == 0){
    /*if p and q are pointing to the same node, then simply connect
    d->pArrayId[hId] and d->pArrayName[hName] to the next nodes in the Id
    and Name hash tables, respectively*/
    //This way, the linked list is not disconnected at p when p is freed

    hName = hash_function(p->name, d->curArrSizeName);
    q = d->pArrayName[hName];
    if(p==q){
        d->pArrayId[hId] = p->nextId;
        d->pArrayName[hName] = q->nextName;

        free(p->name); free(p->id);free(p);
        return 0;
    }else{
        /*If q!=p, we must travese the linked list at hName in pArrayName
    until q->nextName points to same node as p*/
        for(; q->nextName != p; q = q->nextName)
            ;
      //This is needed so that the nodes at hName remain connected after 
      //freeing p
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
        hName = hash_function(p->nextId->name, d->curArrSizeName);
        q = d->pArrayName[hName];
        /* 3 cases:
      Case 1: p->nextId and q->nextName point to the same node.
             Here, we connect p and q to their respective next, next nodes
             before freeing.
      Case 2: q points to the matching node p->nextId. 
             Here, assign q->nextName to the entry in pArrayName[hName]
      Case 3: q->nextName does not point to p->nextId. Must traverse
          nodes at pArrayName[hName] until q->nextName equals p->nextId*/
        if(p->nextId == q->nextName){ //case 1:
        //assign p->nextId to ptemp so that the matching node can be freed
            ptemp = p->nextId;
          //connect p and q to their next, next nodes
            p->nextId = p->nextId->nextId;
            q->nextName = q->nextName->nextName;
            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else if(p->nextId == q){ //case 2
            ptemp = p->nextId;
          //assign next node of q to pArrayName[hName]
            d->pArrayName[hName] = q->nextName;
            p->nextId = p->nextId->nextId;
            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else{ //case 3
        //traverse nodes until same scenario as case 1 is created:
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
/*The function takes a pointer d to a pre-existing struct DB d 
and a name string as inputs */
/*The function removes the first struct UserInfo node with the same name
as the input. 
Return val. is -1(failure) if there is no struct UserInfo node with
the matching name and 0 otherwise. */
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
  
  //If there is no Node at hId, it was already removed:
  //Therefore, return failure
  if(p==NULL)
    return (-1);
  
  /*The hId is initialized to the corresponding hash index 
  for the id field of the node with the input id. q is later 
  made to point to p. */
  int hId; 
  struct UserInfo *q;

  struct UserInfo *ptemp; //pointer to be used later for freeing data

  //if first node at pArrayName[hName] is the match
  if(strcmp(p->name, name) == 0){
    hId = hash_function(p->id, d->curArrSizeId);
    q = d->pArrayId[hId];
    if(p==q){
    /*if p and q are pointing to the same node, then simply connect
    d->pArrayName[hName] and d->pArrayId[hId] to the next nodes in the Id
    and Name hash tables, respectively*/
    //This way, the linked list is not disconnected at p when p is freed
      d->pArrayName[hName] = p->nextName;
      d->pArrayId[hId] = q->nextId;

      free(p->name); free(p->id); free(p);
      return 0;
    }else{
    /*If q!=p, we must travese the linked list at hId in pArrayId
    until q->nextId points to same node as p*/
        for(; q->nextId != p; q = q->nextId)
            ;
    //This is needed so that the nodes at hId remain connected after 
    //freeing p
        q->nextId = q->nextId->nextId;
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
        hId = hash_function(p->nextName->id, d->curArrSizeId);
        q = d->pArrayId[hId];
        /* 3 cases:
      Case 1: p->nextName and q->nextId point to the same node.
             Here, we connect p and q to their respective next, next nodes
             before freeing.
      Case 2: q points to the matching node p->nextName. 
             Here, assign q->nextId to the entry in pArrayId[hId]
      Case 3: q->nextId does not point to p->nextName. Must traverse
          nodes at pArrayId[hId] until q->nextId equals p->nextName*/
        if(p->nextName == q->nextId){ //case 1:
          //assign p->nextName to ptemp so that the matching node can be freed
            ptemp = p->nextName;
          //connect p and q to their next, next nodes
            p->nextName = p->nextName->nextName;
            q->nextId = q->nextId->nextId;

            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else if(p->nextName == q){ //case 2:
            ptemp = p->nextName;
            //assign next node of q to pArrayId[hId]
            d->pArrayId[hId] = q->nextId;
            p->nextName = p->nextName->nextName;

            free(ptemp->name); free(ptemp->id); 
            free(ptemp);
            return 0;
        }
        else{ //case 3:
        //traverse nodes until same scenario as case 1 is created:
            for(; q->nextId != p->nextName; q = q->nextId)
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
/*The function takes a pointer to a preexisting struct DB and an id string
as inputs. It searches for a node with a matching id and returns the 
purchase value of the node if a match is found. 
If no match is found, the function returns -1. */
int
GetPurchaseByID(DB_T d, const char* id)
{
  
  assert(d); assert(id);
  if(d==NULL || id==NULL)
    return(-1);
  
  //Find hash index in id hash table pArrayId for the input id
  int hId = hash_function((char *)id, d->curArrSizeId);
  //Declare pointer p that points to first element at hash index hId
  struct UserInfo *p = d->pArrayId[hId];

  //traverse the nodes at hId in pArrayId by shifting p to p->nextId
  for(; p!=NULL; p = p->nextId){
    if(p->id != NULL){
      //for each node, compare id to the input id:
      if(strcmp(p->id, id)==0){
        return p->purchase;
      }
    }
  }

  //if no matches were found after traversal:
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

  assert(d); assert(name);
  if(d==NULL || name==NULL)
    return(-1);
  
  //Find hash index in name hash table pArrayName for the input name
  int hName = hash_function((char *)name, d->curArrSizeName);
  //Declare pointer p that points to first element at hash index hName
  struct UserInfo *p = d->pArrayName[hName];

  //traverse the nodes at hName in pArrayName by shifting p to p->nextName
  for(; p!=NULL; p = p->nextName){
    if(strcmp(p->name, (char *)name)==0){
      return p->purchase;
    }
  }

  //if no matches were found after traversal:
  return (-1);
  
}
/*--------------------------------------------------------------------*/
/*The function takes a pointer to a function fp and a pointer to a 
pre-existing struct DB structure as inputs. The function applies the function
pointed to by fp on every node and returns the sum of all of the return values
for the nodes*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  assert(d); assert(fp);
  if(d==NULL || fp==NULL)
    return (-1);

  //Integer value for the sum of the return values for all nodes
  int result = 0;

  //Declare a pointer to point at the nodes:
  struct UserInfo *p;
  //use the ID hash table to iterate through all items:
  //traverse through all buckets:
  for(int i=0; i < d->curArrSizeId; i++){
    //traverse through all nodes at each bucket
      for(p=d->pArrayId[i]; p!=NULL; p=p->nextId){
        if(p->name != NULL && p->id != NULL && p->purchase > 0)
            result += (*fp)(p->id, p->name, p->purchase);
      }
  }


  return result;
}