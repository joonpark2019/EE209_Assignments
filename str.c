//Assignment 2. File name: str.c
//Joon Park 20190829

#include <assert.h> /* to use assert() */
#include <stdio.h>
#include <stdlib.h> /* for strtol() */
#include <string.h>
#include <ctype.h>
#include "str.h"

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/

//takes pointer to a string
//returns size_t value corresponding to length of input
size_t StrGetLength(const char* pcSrc)
{
  const char* pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc; //copy pointer (repeated for all functions)

  size_t n = 0;
  /* Use for loop to increment pcEnd with the condition
  that the char element pointed to by pcEnd is not null terminator*/
  for(n=0; *pcEnd != '\0'; pcEnd++){
      n++;
  }
    
  return n;
}

/*------------------------------------------------------------------------*/

//takes a pointer to a string pcSrc and a pointer to a buffer pcDest
//returns a pointer to the destination string
char *StrCopy(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  
  char *dest, *return_pointer; const char *src;
  //size_t i = 0;
  assert(pcDest); assert(pcSrc);
  //copy pointers
  dest = pcDest; src = pcSrc;
  return_pointer = dest;

  /* for loop increments src which points to the input string.
  Each character pointed to by src is copied to character pointed to
  by dest given that string has not ended. */
  while(*src != '\0'){
    *dest = *src;
    dest++;
    src++;
  }
  
  //add null terminator to the end: possible error here
  *dest = '\0';

  return return_pointer;
}

/*------------------------------------------------------------------------*/
/*takes pointers to 2 strings, pcS1 and pcS2, as inputs
returns negative, zero, or positive if pcS1 is less than, equal to, 
or greater than pcS2
*/
int StrCompare(const char* pcS1, const char* pcS2)
{
  /* TODO: fill this function */
  const char *s1, *s2; //possible error from declaration
  assert(pcS1); assert(pcS2);
  //copy inputs to pointers s1 and s2
  s1 = pcS1; s2 = pcS2;
  
  /* while loop tests if character pointed to s1 and s2 are equal and
  increments pointers to parse through both strings.
  If s1 and s2 both end together, then strings are completely equal.*/
  while(*s1 == *s2)
  {
    if(*s1 == '\0')
        return 0;
    s1++; s2++;
  }

  /* If s1 is greater, positive value is outputed.
  If s1 is less, negative value is outputed*/
  return *s1 - *s2;
  
}
/*------------------------------------------------------------------------*/
//takes a pointer pcHaystack to a string and a character int c as inputs
//returns the character at which c is found in the string
//returns NULL(0) if c is not found
char *StrFindChr(const char* pcHaystack, int c)
{
  /* TODO: fill this function */
  const char* haystack;
  assert(pcHaystack);
  haystack = pcHaystack;

  if(c == '\0')
    return (char*)(haystack + strlen(haystack));

  //increment haystack pointer until it reaches first match in string
  while( (*haystack != c) && (*haystack != '\0')){
    haystack++;
  }
  
  //case that there was no match until string ended
  if (*haystack == '\0') 
      return 0;
  else
      return (char*)haystack; // possible error

}
/*------------------------------------------------------------------------*/
/* StrFindStr takes the pointers to 2 strings, pcHaystack and pcNeedle
 as inputs. It checks if the string at pcNeedle is a substring of the
 string at pcHaystack. It returns the value of the pointer to the first 
 character in haystack that the substring matches. */

char *StrFindStr(const char* pcHaystack, const char *pcNeedle)
{
  /* TODO: fill this function */
  const char* haystack; const char* needle;
  assert(pcHaystack); assert(pcNeedle);
  //copy pointers: pcHaystack copies to haystack and pcNeedle to needle
  haystack = pcHaystack; needle = pcNeedle;

  //additional pointers are declared to traverse through haystack and needle
  const char *ptr_haystack, *ptr_needle;

/*Use a nested while loop structure: while incrementing the haystack pointer, 
check if there is a match further down the string at each character*/
  while(*haystack != '\0')
  {
    ptr_haystack = haystack; 
    ptr_needle = needle;
    /*Use while loop to increment pointers if the characters
    are equal and the end of neither arrays has been reached*/
    while((*ptr_needle != '\0') && (*haystack !='\0') && (*ptr_needle == *haystack))
    {
      haystack++; ptr_needle++;
    }
    /*if the character at ptr_needle is the null terminator, previous while loop
    reached the end of the needle string: there was a match */
    if(*ptr_needle == '\0')
      return (char *)ptr_haystack;
    //increment haystack for outer while loop
    haystack = ptr_haystack + 1;
  }
 //if no match is found, return 0(NULL)
  return 0;



}
/*------------------------------------------------------------------------*/
/* StrConcat takes the character pointers pcDest and pcSrc as inputs.
It copies the characters of the string at pcSrc to the end of the string at 
pcDest and returns the pointer dest which points at pcDest.  */
char *StrConcat(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  char *dest; char *src;
  assert(pcDest); assert(pcSrc);
  //copy pointers
  dest = pcDest; src = (char *)pcSrc;
  
  //i and j are indices used to traverse the character arrays at pointers
  size_t i, j;
  i = j = 0;
  /* find end of s by incrementing index i until null terminator is reached*/
  while (dest[i] != '\0') 
    i++;
  /* assign character at src[j] to dest[i] until end of array src is reached*/
  while ((dest[i++] = src[j++]) != '\0') /* copy t */
  ;

  return (char*)dest;
  
}

/*------------------------------------------------------------------------*/
/*StrToLong takes a pointer to a string nptr and returns an integer value
that is the equivalent of the numeric string in the base that is
inputted. It also takes endptr, a pointer to a character pointer, as an input. If
the end pointer is not null, the function stores the address of the first invalid
character at endptr. */
long int StrToLong(const char *nptr, char **endptr, int base)
{
  /* handle only when base is 10 */
  if (base != 10) return 0;

  /* TODO: fill this function */
  assert(nptr); assert(base);
  int sign = 1; //sign variable is the sign of the string
  const char *str = nptr; //copy input pointer to str
  int chr; //character of the string read
  unsigned long int result = 0; //value of the result

  /*The flag variable can take 3 possible values: -1, 0, 1. 
  The flag is initialized to 1: if the flag remains 1 at the end of the
  program, we know that the program did nothing, so an empty string was passed.
  If flag == -1, then the program ran successfully. If flag == 0, then overflow
  occurred*/
  int flag = 1;
  
//max_val is the maximum value that can be stored without overflow given the sign
  unsigned long int max_val;

/*max_lim is the value that, if added to the current return value, 
will cause overflow of the return value*/
  int max_lim;

//skip all white space: increment str up to a non-space character
  while(*str && isspace(*str))
    str++;

    //detect the sign
   if(*str == '+'){
     sign = 1; str++;
   }
    else if(*str == '-'){
      sign = -1; str++;
    }


    //setting cutoff for overflow/underflow:
    if(sign < 0){
        max_val = -(unsigned long)LONG_MIN;
    }else{
        max_val = LONG_MAX;
    }
    /* max_lim is found as the remainder when max_val is divided by the base:
    adding max_lim to the current return value creates overflow */
    max_lim = max_val % base;
    max_val = max_val / base;
  
  //increment through the string input:
  while(*str)
    {
      //copy the pointer to current character to chr
      chr = *str;
      if(isdigit(chr)){
        //using ascii value of chr, find integer value of chr
            chr = chr - '0';
        }else{ //if chr is not a digit, it is invalid
            break;
        }
        if(chr >= base){ //if chr is greater than the givenbase, it is invalid
            break;
        }
        if(flag == 0 || result > max_val || (result == max_val && chr > max_lim)){
            flag = 0; //this flag value indicates an overflow or underflow happened
        }
        else{
            result = result*base + chr;
            flag = -1;
      }
      str++;
    }

//If flag == 0, overflow occurred somewhere in the program
    if(flag == 0){
        if(sign < 0)
            return LONG_MIN;
        else
            return LONG_MAX;
    }
    if(endptr != NULL){
        if(flag != 1) //there was an invalid character in string
            *endptr = (char *) str;
        else if(flag == 1) //empty string: initial value was never changed
            *endptr = (char *)nptr;
    }

    return sign*result;

}