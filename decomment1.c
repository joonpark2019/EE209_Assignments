#include <stdio.h>

int is_comment_start(int c_main, int c_temp1, int c_temp2){

    if( (c_main == '/' && c_temp1 == '*') || (c_temp1 == '/' && c_temp2 == '*'))
    {
        return 1;
    }else{
        return 0;
    }
}

int is_comment_end(int c_main, int c_temp1, int c_temp2){
    if( (c_main == '*' && c_temp1 == '/') || (c_temp1 == '*' && c_temp2 == '/'))
    {
        return 1;
    }else{
        return 0;
    }
}


int main(void) {

    int in_comment = 0; //state of being in comment
    int c_main = 0; //the character read each time in while loop
    int c_temp1 = 0; //second character read in special cases
    int c_temp2 = 0;

    //below are all of the counters needed
    int c_count = 0; //incremented for each and every character read
    //int quote_start = 0; //counter value when a quote starts
    //int quote_end = 0;
    //int comment_start = 0; //counter value when a comment starts
    //int comment_end = 0; //counter value when a comment ends
    int line_count = 1; //counter for line number

    int in_string = 0; //turned on while in a quote and off while out of a quote
    int in_char = 0;

    int comment_count = 0; /*this is important because it allows 
    only one line to be printed when in the comment
    For more and more characters within a comment, the counter gets larger
    than one*/
    int comment_line = 0;

    while( (c_main=getchar()) != EOF)
    {
        c_count++; //first, just increment main counter

        if(in_comment)
        {
            comment_count++;

            if(comment_count == 1){//the first character read in a comment
                putchar(' ');
                //continue;
            }
            
            if(c_main == '\n')
            {
                putchar('n');
                putchar('\n');
                line_count++;
                continue;
            }
            
            //checks for end of comment:
            if(c_main == '*'){

                c_temp1 = getchar();
                c_temp2 = getchar();
                c_count = c_count + 2;
                comment_count = comment_count + 2;

                if(is_comment_end(c_main, c_temp1, c_temp2)){ 
                    in_comment = 0;
                    comment_count = 0;
                    //the following two lines arose because of test0.c
                    /*the two lines are needed in case a comment ends right
                    at the end of the file
                    */
                    if(c_main == '*' && c_temp1 == '/' && c_temp2 != EOF){
                        if(c_temp2 == '\n'){
                            line_count++;
                            putchar('n');
                        }
                        putchar(c_temp2);
                        
                    }
            
                    if(c_temp2 == EOF || c_temp1 == EOF) break;

                     
                }
                else if(!is_comment_end(c_main, c_temp1, c_temp2)){
                    if(c_temp1 == '\n') 
                    {
                        line_count++; 
                        putchar('\n');
                    }
                    if(c_temp2 == '\n'){
                        line_count++; 
                        putchar('\n');
                    } 
                }
            }    
        }

        else if(!in_comment){ //not in a comment
            if(c_main == '\'')
            {
                putchar('\'');
                
                if(in_char == 0)
                {
                    in_char = 1;
                }
                else if(in_char == 1)
                {
                    in_char = 0;
                }
            }
            else if(c_main == '\"'){
                putchar('\"');
                
                if(in_string == 0)
                {
                    in_string = 1;
                }
                else if(in_string == 1)
                {
                    in_string = 0;
                }
            }
            else if(c_main == '/') //important! this could begin a comment
            {    //int c_temp2 = 0; //this reads the next character temporarily
                c_temp1 = getchar();

                //this block exists solely to debug test0.c
                if(c_temp1 != '/' && c_temp1 != '*'){
                    putchar(c_main);
                    putchar(c_temp1);
                    continue;
                }

                c_temp2 = getchar();
                c_count = c_count + 2;

                if(c_temp1 == '\n'){
                    line_count++;
                }
                if(c_temp2 == '\n'){
                    line_count++;
                }
                
                if(is_comment_start(c_main, c_temp1, c_temp2) ) //THIS NEEDS TO BE CHANGED
                {
            
                    if(!in_string && !in_char)
                    {
                        in_comment = 1;
                        //comment_start = c_count;
                        //comment_count = 0;
                        comment_line = line_count;
                        if(c_temp1 == '/') putchar(c_main); // this is for case //*

                        if(c_temp2 == '\n'){
                            putchar('n'); 
                            putchar('\n');
                        }
                        continue;
                    }
                    else{ //if we are in a string or a character
                        putchar('/');
                        putchar(c_temp1);
                        putchar(c_temp2);
                    }
                }else{
                    putchar('/');
                    putchar(c_temp1);
                    putchar(c_temp2);
                }
                    //break;
                //default:
                    //putchar(c_main);
                    //break;
            }
            else{
                if(c_main=='\n'){
                    line_count++;
                    putchar('n');
                }
                putchar(c_main);
                //c_count++;
            }
        }
    }
    

    if(in_comment == 1){
        fprintf(stderr, "Error: line %d: unterminated comment\n", comment_line);
    }

    //putchar('\n');

    return 0;

}