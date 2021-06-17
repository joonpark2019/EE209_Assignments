#include <stdio.h>
#include <assert.h>

//The functions needed in main function are defined first:

/* is_comment_start() takes 3 consecutive characters from stdin 
(char1 char2 char3 in this order) as inputs. 
It returns 1 if a comment beginning is detected in it and 
returns 0 otherwise */
int is_comment_start(int char1, int char2, int char3){
    /*checks if first character is '/' and second is '*' or if 
    second character is '/' and third character is '*'
    */
    if((char1 == '/' && char2 == '*') || (char2 == '/' && char3 == '*'))
        return 1;
    else
        return 0;
}

/* is_comment_start() takes 3 consecutive characters from stdin 
(char1 char2 char3 in this order) as inputs. 
It returns 1 if a comment beginning is detected in it and 
returns 0 otherwise */
int is_comment_end(int char1, int char2, int char3){
    /*checks if first character is '*' and second is '/' or if 
    second character is '*' and third character is '/'
    */
    if((char1 == '*' && char2 == '/') || (char2 == '*' && char3 == '/'))
        return 1;
    else
        return 0;
}

//Define two possible states IN and OUT for DFA states
enum DFAState {IN, OUT};

/* In the main function, 
a while loop reads a character (char1) until a forward slash, backslash, or asterix is
reached. Only when a forward slash, backslash, or asterix is read are additional characters 
(char2 char3) read from standard input to verify if there is the 
beginning/end of a comment or a peculiar case.
*/
int main(void) {

//All variable declarations:
    //the character read each time in while loop:
    int char1 = 0;
    //additional characters read if char1 == '/' or '*' or '\\':
    int char2, char3 = 0; 
    //state of being in a comment: 
    enum DFAState in_comment = OUT;
    //state of being in a string: 
    enum DFAState in_string = OUT;
    //state of being in a character: 
    enum DFAState in_char = OUT; 

    /*in_comment, in_string, in_char are all initialized as OUT, 
    which corresponds to State1 of my DFA diagram */

    //counter for line number: incremented when newline is detected
    int line_count = 1; 
    
    /*comment_line is the line number of the beginning of a comment: it is 
    equated to the line number if the beginning of a comment is detected*/
    int comment_line = 0; 

    //The main while loop that reads a character until the end of file:
    while( (char1=getchar()) != EOF)
    {
        switch(in_comment) {
            //If current state of in_comment is IN (ie. char1 is in a comment)
            case IN: //This corresponds to State2 of my DFA diagram
                //Comments should preserve line numbers, so newline is printed
                if(char1 == '\n')
                {
                    putchar('\n');
                    line_count++; //increment line_count to keep track of line number
                    continue;
                }

                /* '*' may signal the end of a comment, so more characters 
                are read to make sure */
                if(char1 == '*'){
                    //get the next two characters to test if this is end of comment
                    char2 = getchar(); char3 = getchar();
                    
                    //If '*' marks the end of a comment:
                    if(is_comment_end(char1, char2, char3)){
                        in_comment = OUT;
                        /*the following two if-blocks are needed in case 
                        a comment ends right at the end of the file: 
                        if char3 is not EOF, then print it to standard output as normal
                        */
                        if(char1 == '*' && char2 == '/' && char3 != EOF){
                            if(char3 == '\n'){
                                line_count++;
                            }
                            putchar(char3);
                        }
                        /*break the main while loop if EOF is read 
                        since this means file has ended */
                        if(char3 == EOF || char2 == EOF) break;       
                    }
                    // if char1 is '*' but not actually end of comment:
                    else{
                    /*Comments should preserve line numbers, so newline is printed
                    and line_count is incremented*/
                        if(char2 == '\n') 
                        {
                            line_count++; 
                            putchar('\n');
                        }
                        if(char3 == '\n'){
                            line_count++; 
                            putchar('\n');
                        } 
                    }
                }
                break;

            /*If current state of in_comment is OUT (ie. char1 is not in a comment)
            This corresponds to States 1, 3, 4 (all being outside comment) of my DFA 
             */
            case OUT: 
                //If char1 is a single quote:
                if(char1 == '\'')
                {
                    putchar('\'');
                    
                    switch (in_char) {
                    /*If state of in_char is IN when a single quote is read, 
                    then change state to OUT (same vice versa)*/
                        case OUT:
                        /*Corresponds to State4 (outside comment, inside character)
                        in my DFA diagram */
                            in_char = IN;
                            putchar('N');
                            break;
                        case IN:
                        /*switch from State4(outside comment, inside character)
                        to State2(outside comment, outside character)*/
                            in_char = OUT;
                            putchar('O');
                            break;
                        default:
                            assert(0);
                            break;
                    }
                }
                //If char1 is a double quote:
                else if(char1 == '\"'){
                    putchar('\"');
                    switch (in_string) {
                    /*If state of in_string is IN when a single quote is read, 
                    change state to OUT (same vice versa)*/
                        case OUT:
                        /*Corresponds to State3(outside comment, inside string)
                         in my DFA diagram */
                            in_string = IN;
                            putchar('N');
                            break;
                        case IN:
                        /*switch from State3(outside comment, inside string) 
                        to State2(outside comment, outside character)*/
                            in_string = OUT;
                            putchar('O');
                            break;
                        default:
                            assert(0);
                            break;
                    }
                }
                /*If char1 is a forward slash, it may start the beginning of a comment, 
                so additional character char2 is read*/
                else if(char1 == '/') 
                {   
                    char2 = getchar();
                    
                    /*if char2 is  not '/' or '*', then this is 
                    definitely not the beginning of a comment 
                    so char1 and char2 are just printed to stdout and 
                    next iteration of while loop is executed */
                    if(char2 != '/' && char2 != '*'){
                        putchar(char1);
                        putchar(char2);
                        continue;
                    }

                    /*if the previous block is not exectuted, 
                    another character is read to check if a comment begins*/
                    char3 = getchar();

                    if(char2 == '\n')
                        line_count++;
                    if(char3 == '\n')
                        line_count++;
                    
                    if(char2 == '\'' ){
                        switch (in_char) {
                            case OUT:
                                in_char = IN;
                                putchar('N');
                                break;
                            case IN:
                                in_char = OUT;
                                putchar('O');
                                break;
                            default:
                                assert(0);
                                break;
                        }
                    }
                    if(char2 == '\"'){
                        switch (in_string) {
                            case OUT:
                                in_string = IN;
                                putchar('N');
                                break;
                            case IN:
                                in_string = OUT;
                                putchar('O');
                                break;
                            default:
                                assert(0);
                                break;
                        }
                    }

                    
                    //The following block checks if "/*" is in the sequence char1char2char3
                    //If it is, char1 char2 char3 contains the start of a comment
                    if(is_comment_start(char1, char2, char3) ) 
                    {
                        /* The sequence of characters "forward-slash asterix" only starts a comment if 
                        char1, char2, char3 are not within a string or a character 
                        (ie. we are in state1 of DFA), 
                        so states of in_string and in_char are first checked*/
                        if(in_string == OUT && in_char == OUT) //Verify we are not in states 3 or 4 of DFA
                        {
                            /* If char1, char2, char3 are not in a 
                            string or character, then in_comment is changed to IN*/
                            in_comment = IN;
                            //the line number of the start of a comment is recorded
                            comment_line = line_count;
                            /* this if-statement is for the case char1=='/' char2=='/' char3=='*', 
                            where the first '/' of the sequence must be printed */
                            if(char2 == '/')
                                putchar(char1); 
                            //a space is printed to mark start of a comment (rule 1 of assignment)
                            putchar(' '); 
                            
                            /*A comment may start with char1 and char2 
                            while char3 is a newline. In this case, a newline must 
                            be printed to maintain total number of lines.   
                            */
                            if(char3 == '\n')
                                putchar('\n');

                            continue;
                        }
                        else{ //if we are in a string or a character (ie. inside state 3 or 4)
                                //then we print characters normally
                            putchar(char1); putchar(char2); putchar(char3);
                        }
                    }else{ //If not the start of a comment:
                        putchar(char1); putchar(char2); putchar(char3);
                    }
                }
                //The following block is for rule 6 of the assignment specifications:
                //If we are inside a character or string and we read a backslash:
                else if((char1 == '\\') && (in_char == IN || in_string == IN) ){
                    putchar('\\');
                /*Read extra character after backslash: this way, we don't give 
                the program the chance to switch in_string or in_char to OUT
                even if ' or " follows the backslash*/
                    char2 = getchar();
                    putchar(char2);
                    if(char2 == '\n') line_count++; //as always, check for newline
                         
                }
                /*If char1 is not a forward slash, single/double quote, then there are no
                state changes to account for so print char1 to standard output as per normal*/
                else{ 
                    if(char1=='\n') line_count++; //as always, check for newline
                    putchar(char1);
                }
                break;
            default:
                assert(0);
                break;
            }
    //End of main while loop
    }

    /* If the state of in_comment is IN after while loop terminates,
    then that means that the while loop terminated before in_comment was turned OUT. 
    Therefore, this is an unterminated comment and an error*/
    if(in_comment == IN)
    fprintf(stderr, "Error: line %d: unterminated comment\n", comment_line);

    return 0;
}