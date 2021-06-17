//Assignment 2. File name: sgrep.c
//Joon Park 20190829

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

/*
 * Fill out your own functions here (If you need) 
 */


//takes a pointer to a string and an integer flag value (0 or 1)
//is called in an if statement to check if string has ended
int end_of(const char *str, int in_q){
  //in_q == 1 means that the pattern is enclosed in double quotes
  if(in_q) //if in_q == 1, then double quote can mean end of string
    return (*str == '\0' || *str == '\"');
  else // pattern not enclosed in quote
    return *str == '\0';
}

/* match_found serves as a helper function to recursive function 
pattern_search. It takes pointer pattern to a pattern to be found
in the string at pointer buf. It returns 1 if pattern is found in buf
and 0 otherwise. match_found is recursive */
int match_found(const char *buf, const char *pattern, int in_quote)
{
/*approach: recursively increment pointers until the end of either pattern 
or buf is reached */

  //base condition 1: end of buf is reached but end of pattern not reached
  //this means that a match was not found, so return 0
    if (end_of(buf, in_quote) && !end_of(pattern, in_quote))
        return 0;
  //base condition 2: end of pattern is reached or an asterix in the pattern is reached
/*this means that the pattern is found or a subsection 
of the pattern before an asterix is found */
    if (end_of(pattern, in_quote) || *pattern == '*')
        return 1; //the ultimate condition for a match
    
    //recursive call:
    if ( *buf == *pattern)
        return match_found(buf + 1, pattern + 1, in_quote);
        
    return 0;
}

/* pattern_search uses recursion to see if the string at pattern is found
in the string at buf, even with asterix stars in it.  */
int pattern_search(const char *buf, const char *pattern, int in_quote){
  char*ptr;

//base condition: end of the line is reached but no matches found yet
  if(end_of(buf, in_quote))
    return 0;
  
  //search for subsection of pattern until next asterix in the pattern
  if(*pattern == '*'){ 
        if(pattern_search(buf, pattern+1, in_quote)){ 
        //if subsection of pattern has match in buf:
          ptr = StrFindChr(pattern+1, '*');
          if(ptr) //check if another star is found further down the string
            //call pattern_search using pattern from the next star found
            return pattern_search(buf, ptr, in_quote);
          return 1; //if we have no more stars further down, we have found a match
        }
        else
            return 0;
    }
  
  // *pattern == *buf is the condition for match_found being called
  if(*pattern == *buf){ 
    ptr = StrFindChr(pattern, '*');
    //if we have more stars down the line
    if(ptr){
        if(match_found(buf, pattern, in_quote))//substring before next star has a match
            return pattern_search(buf, ptr, in_quote);
    }
    //evaluates at the very end where no more stars are left in pattern to recurse
    if(match_found(buf, pattern, in_quote)) //this part may cause some lines to evaluate true
        return 1;
    else
        return pattern_search(buf+1, pattern, in_quote);
  }

  return pattern_search(buf+1, pattern, in_quote);
  
}


/*--------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                     */
/*--------------------------------------------------------------------*/
void 
PrintUsage(const char* argv0) 
{
  const static char *fmt = 
	  "Simple Grep (sgrep) Usage:\n"
	  "%s pattern [stdin]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* SearchPattern()
   Your task:
   1. Do argument validation 
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long, 
   print out "Error: argument is too long"
   
   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes, 
   print out "Error: input line is too long" 
   - Error message should be printed out to standard error (stderr)
   
   3. Check & print out the line contains a given string (search-string)
      
   Tips:
   - fgets() is an useful function to read characters from file. Note 
   that the fget() reads until newline or the end-of-file is reached. 
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
SearchPattern(const char *patt)
{
  char buf[MAX_STR_LEN + 2]; 
  int len; //stores the length of the line read from stdin
  int len_pattern; //stores the length of the pattern
  int flag_null = 0; //flag for checking if pattern is empty(null) (ie. "")
  int flag_star = 0; //flag for checking if asterix star is in pattern
  int in_quote = 0; //flag for checking if pattern is enclosed in quotes
  int is_match = 0; //flag for checking if pattern has a match in a line
  const char *pattern = patt;
  /* 
   *  TODO: check if pattern is too long
   */
  len = StrGetLength(pattern)*sizeof(char); //possible error here:
                                
  if(len > MAX_STR_LEN){
    fprintf(stderr, "Error: pattern is too long\n");
    return FALSE;
  }

  //Removing double quotes from pattern:
  if(*pattern == '\"')
  {
    ++pattern; //ignore the first quote for now by skipping to next character
    len_pattern = StrGetLength(pattern);

    if(*pattern == '\"' && len_pattern == 1)
      flag_null = 1; //we have "" as pattern(empty pattern), so every line is printed
    else if( pattern[len_pattern - 1] == '\"'){
      //if the last character of the pattern is a double quote,
      //then our pattern is enclosed in double quotes, so activate in_quote flag
      in_quote = 1;
    }else
      --pattern; //else decrment pointer back to first character
  }

  //check if our pattern has stars in it: activate flag_star if true
  if(StrFindChr(pattern, '*'))
    flag_star = 1;
  
  /* Read one line at a time from stdin, and process each line */
  while (fgets(buf, sizeof(buf), stdin)) {
	  
    /* check the length of an input line */
    if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
	
    /* TODO: fill out this function */
    //part2:
    if(flag_star){ //if we have stars in our pattern
      //call pattern_search to check if pattern is detected in line
      is_match = pattern_search(buf, pattern, in_quote);
      if(is_match) //if the line buf contains the pattern, print the line
        printf("%s", buf);
    }
    //if there are no stars in our pattern
    else{
      if(flag_null){ //our pattern is "", so all lines are printed
        printf("%s", buf);
        continue;
      }
      else{
        if(pattern_search(buf, pattern, in_quote))
          printf("%s", buf);
      }
    }
  
  }
   
  return TRUE;
}
/*-------------------------------------------------------------------*/
int 
main(const int argc, const char *argv[]) 
{
  /* Do argument check and parsing */
  if (argc < 2) {
	  fprintf(stderr, "Error: argument parsing error\n");
	  PrintUsage(argv[0]);
	  return (EXIT_FAILURE);
  }

  return SearchPattern(argv[1]) ? EXIT_SUCCESS:EXIT_FAILURE;
}