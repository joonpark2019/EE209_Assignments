#include <stdio.h>
/*

//after first reading a slash, this checks if the slash marks the beginning of a comment
int is_comment_start(void) {
    char c = 0;
    c = getchar();
    if(c == "*"){
        return 1;
    } else{
        return 0;
    }
}



int is_comment_end(void){
    char c1, c2 = 0;
    c1 = getchar();
    c2 = getchar();
    if(c1 == "*" && c2 == "/"){
        return 1;
    } else{
        return 0;
    }
}
*/

int main(void){

    char c_main = 0; //this is the reader for the main while loop

    //counters
    int c_main_count = 0;

    int quote_start_count = 0;
    int comment_start_count = 0;

    //int quote_end_count = 0;
    //int end_file_count = 0;

    //main while loop

    //
    c_main = getchar();

    while(c_main != EOF){

        //this part checks quotation marks
        if(c_main == '\'' || c_main == '\"')
        {
            quote_start_count = c_main_count;
            putchar(c_main);
        }


        //this part checks if a quote begins
        if(c_main == '/')
        {
            //putchar(c_main);
            char c_temp1 = 0; //read two new variables
            //char c_temp2 = 0;
            c_temp1 = getchar();
            //c_temp2 = getchar();

            if( c_temp1 == '*') //check if a comment starts
            {
                comment_start_count = c_main_count; //update the counter

                //while a comment is still going GIVEN there was no quote previously

                if(comment_start_count < quote_start_count)
                {
                    putchar(' '); // first just print a space
                    char c, d = 0;
                    while( ((c=getchar()) != '*' && (d=getchar()) != '/') && (c != EOF) && (d != EOF) )
                    {
                        //we should print what is in a comment if it is a space or newline
                        if( c == ' ' || c =='\n')
                        {
                            putchar(c);
                        }
                        if( d == ' ' || d == '\n')
                        {
                            putchar(d);
                        }

                        //if we reach end of file before a comment ends, raise error and break
                        if(c == EOF || d == EOF) 
                        {
                            fprintf(stderr, "Error: line X string\n");
                            break;
                        }
                    
                    }

                    
                }
                
            }

        }else if(c_main == '\n'){
            putchar('\n');
        }
        
        else{
            //output c_main if it passes the previous tests
            putchar(c_main);
        }
        
        
        

        c_main = getchar();

        //increment the main character counter
        c_main_count++;

    putchar('\n');

    return 0;
}