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

    
    int c_main = 0; //the character read each time in while loop
    int c_temp1 = 0; //second character read in special cases
    int c_temp2 = 0;

    int in_comment = 0; //state of being in comment
    int in_string = 0; //turned on while in a quote and off while out of a quote
    int in_char = 0;

    //below are all of the counters needed:
    //counter for line number: incremented when /n is detected
    int line_count = 1; 
    
    int comment_line = 0; 

    while( (c_main=getchar()) != EOF)
    {

        if(in_comment)
        {
            if(c_main == '\n')
            {
                //putchar('n');
                putchar('\n');
                line_count++;
                continue;
            }
            
            //checks for end of comment:
            if(c_main == '*'){
                //get the next two characters to test for all test cases
                c_temp1 = getchar();
                c_temp2 = getchar();

                if(is_comment_end(c_main, c_temp1, c_temp2)){ 
                    in_comment = 0;
                    //the following two lines arose because of test0.c
                    /*the two lines are needed in case a comment ends right
                    at the end of the file
                    */
                    if(c_main == '*' && c_temp1 == '/' && c_temp2 != EOF){
                        if(c_temp2 == '\n'){
                            line_count++;
                            //putchar('n');
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
            else if(c_main == '/') //important: this could begin a comment
            {    //int c_temp2 = 0; //this reads the next character temporarily
                c_temp1 = getchar();
                
                /*this block checks if c_temp1 is  not '/' or '*' 
                such as an alphabet or a space */
                if(c_temp1 != '/' && c_temp1 != '*'){
                    putchar(c_main);
                    putchar(c_temp1);
                    //If the above is the case, the next iteration of the while loop is executed
                    continue;
                }

                //Do not touch this line: changing this causes issues
                c_temp2 = getchar();

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
                        comment_line = line_count;
                        // this is for case "//*"
                        if(c_temp1 == '/') putchar(c_main); 

                        putchar(' '); //output space since we are sure a comment begins
                        if(c_temp2 == '\n'){
                            //putchar('n'); 
                            putchar('\n');
                        }
                        continue;
                    }
                    else{ //if we are in a string or a character:
                            //then we print characters normally
                        putchar('/');
                        putchar(c_temp1);
                        putchar(c_temp2);
                    }
                }else{ //If not the start of a comment:
                    putchar('/');
                    putchar(c_temp1);
                    putchar(c_temp2);
                }
            }
            else{ //If character read is not a forward slash, print normally
                if(c_main=='\n'){
                    line_count++;
                    //putchar('n');
                }
                putchar(c_main);
            }
        }
    }
    
    //After while loop reaches EOF, state of comment is checked.
    /*If state of comment is still in after end of the while loop, then there is an
    unterminated comment and therefore an error.
    */ 
    if(in_comment == 1){
        fprintf(stderr, "Error: line %d: unterminated comment\n", comment_line);
    }

    return 0;

}