/*
Pawan Raj Adhikari

UTA ID:: 1001669750

*/


#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 100    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // msh shell only supports ten arguments



int main()
{

  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
	int i;
	int pid_count = 0;
	char history[50][MAX_COMMAND_SIZE];
	pid_t pids[15];
	int history_count=0;
  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];



    int   token_count = 0;


    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;

    char *working_str  = strdup( cmd_str );

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

	if(!token[0])
	{
		continue;
	}
	strcpy(history[history_count],cmd_str);
	history_count++;

   
	 if(cmd_str[0]=='!')
	{
		int hist_index;
		//int tok_count=0;
		hist_index = atoi(&cmd_str[1]);
	 
	token_count = 0;
	working_str  = strdup( history[hist_index] );
	 while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    
	}
	}
	
	if(strcmp(token[0],"cd")==0)
	{
		chdir(token[1]);
		continue;

	}
	
	if(strcmp(token[0],"exit")==0 ||strcmp(token[0],"quit")==0)
	{
			exit(0);
	}
	if(strcmp(token[0],"history")==0)
	{
		for(i=0;i<history_count;i++)
				printf("%d. %s\n",i,history[i]);
			
	}
	
	else
	{
		pid_t pid = fork();
		if(pid ==0)
		{
			if(strcmp(token[0],"showpids")==0)
			{
				for(i=0;i<pid_count;i++)
				{
					printf("%d. %d\n",i,pids[i]);
					
				}
				fflush(NULL);
				exit(EXIT_SUCCESS);
			}
			else
			{
				int returnValue;
				returnValue = execvp(token[0],token);
				if(returnValue==-1)
				{
					printf("%s: command not found.\n",token[0]);
				}
					fflush(NULL);
					exit(EXIT_SUCCESS);
			}
		}
		else
		{

			//pid_t cpid = getpid();
			pids[pid_count]=pid;


			if(pid_count>15)
			{
				pid_count=0;
			}
			int stat;
			wait(&stat);
		}

	}
	pid_count++;
    free( working_root );

  }
  return 0;
}
