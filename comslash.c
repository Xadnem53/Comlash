/********************************************************************************
 *										*
 *      CHANGE THE FORM OF THE COMMENTS FROM "/*" TO "//" OR VICE VERSA 	*
 *										*
 *      USE: 									*
 *           ./comslash < file name > s     ( from "/*" to "//" )               *
 *           ./comslash < file name > a     ( from "//" to "/*" )               *
 *										*
 ********************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLINES 100000 /* Maximum of lines that the file to be changed can have  */

/* Function to insert spaces at the beginning of a line   */
void InsertSpaces(char* line, int linelength, int spaces);

/* char * array to contain the file lines */
char * lines [MAXLINES];  


int main(int argc, char * argv[] )
{    	
 /* Initialize the array of pointers to pointers to char  */
     int i;
     for(i = 0; i < MAXLINES;i++)
     {
	lines[i] = (char*)malloc(500);
     }
  /* Counters for chars, lines and spaces  */
   int countchars, contlines,contspaces; 
   countchars = contspaces = 0;
   contlines = 1;
   char c ; /* For the current char read */
   int incomment = 0; /* To register when it is in a comment */
   int commented = 0; /* To register when it is in a "/*" and do nothing */

/* Parameter checking and file opening if correct   */
   if(argc != 3 || (argv[2][0] != 'a' && argv[2][0] != 's') )
   {
    puts("\nUse to change comments from type /* ... */ to type // : comslash < file path > a \nUse to change comments from type // to type /* ... */ : comslash < file path > s ");
    return 1;
   }
   
   FILE * file = fopen(argv[1],"rw");
   if(file == NULL)
   {       
     printf("\n The file : %s couldn't been opened",argv[1]);
     return 1;
   } 
/* To change from // to "/*"  */
  if(argv[2][0] == 'a')
  {
    while( (c = fgetc(file)) != EOF)
    {
      //To count the spaces at the beginning of the line in order to insert them later   
      if(countchars== 0 && c ==' ')
      {
	  contspaces++;
	  continue;    
      }
      countchars++;
     //To check if it is into a prior comment and, if so, put commented flag to 1
      if(countchars > 1 && c == '*' && lines[contlines-1][countchars-2] == '/')
	      commented = 1;
      //To check if it have exited from a prior comment and, if so, put commented flag to 0
      else if(countchars > 1 && c == '/' && lines[contlines-1][countchars-2] == '*')
	     commented = 0;
    
      //  If a / character is readed and the prior one was / also, change them
      if(commented == 0 && c ==  '/' && countchars > 1 && lines[contlines-1][countchars-2] == '/' & incomment == 0)
      {
	incomment = 1;      
	lines[contlines-1][countchars-2] = '/';
	lines[contlines-1][countchars-1] = '*';
      }
//If it is a line start and it is into a comment, to check if the comment continues or have ended.
      else if( commented == 0 && countchars == 1 && incomment && c != '/' )
      {
	  //To add the readed character to the current line.
	  lines[contlines-1][countchars-1] = c;
	  //To obtain the prior line and to close the comment
	  char * lin = lines[contlines-2];
	  int cont = 0;
	  while(lin[cont] != '\n')
	    cont++;
	  lines[contlines-2][cont] = '*';
	  lines[contlines-2][cont+1] = '/';
	  lines[contlines-2][cont+2] = '\n';
	  lines[contlines-2][cont+3] = '\0';
	  incomment = 0;
      }
      //If the comment continues, to eliminate the slashes at the beginning of the line.
      else if( commented == 0 && c == '/' && countchars == 2 && incomment )
      {
         lines[contlines-1][0]=' ';
	 lines[contlines-1][1]=' ';
      }
      else if( c == '\n') //If it is at the line end
      {
	lines[contlines-1][countchars-1] = c;
	lines[contlines-1][countchars] ='\0';
	//To insert the spaces at the line beginning if they exist.
	if(contspaces > 0)
	  InsertSpaces(lines[contlines-1],countchars+1,contspaces);
	contspaces = 0;
	contlines++;
	countchars = 0;
      }
      else
	lines[contlines-1][countchars-1] = c;
    }//End of the while
    /* If it is in a comment, close it */
    if(incomment)
    {
       lines[contlines-1][countchars] = '*';
       lines[contlines-1][countchars+1]='/';
       lines[contlines-1][countchars+2]='\0';
    }
    //If the last line is empty, dispose it
    if(countchars == 0)
	contlines--;
 }//End of the option for change from // to '/*'

 /* OPTION FOR TO CHANGE COMMENTS FROM "/*" TO //  */
 else if(argv[2][0] == 's') 
 {
   while((c = fgetc(file)) != EOF)
   {
     countchars++;
     // If it is in a comment and is the first character of the line
     if(incomment && countchars == 1)
     {
       lines[contlines-1][countchars-1] = '/';
       countchars++;
       lines[contlines-1][countchars-1] = '/';
       countchars++;
       lines[contlines-1][countchars-1] = c;
     }
     //If a asterix is readed, check if it is a comment start
     else if( !incomment && countchars > 1 && 
	 c == '*' && lines[contlines-1][countchars-2] == '/')
     {
       lines[contlines-1][countchars-1] = '/';
       incomment = 1;
     }

     //If a / is found and the prior character was a *, exit from the comment
     else if(incomment && countchars > 1 && c == '/' &&
		     lines[contlines-1][countchars-2] == '*')
     {
	lines[contlines-1][countchars-1] =' ';
	lines[contlines-1][countchars-2] =' ';
	incomment = 0;
     }
     // If the line end is found
     else if(c == '\n')
     {
        lines[contlines-1][countchars-1] = c;
	lines[contlines-1][countchars] = '\0';
	contlines++;
	countchars = 0;
     }
     else if(incomment == 1 && countchars > 2 && c == '/'
		     && lines[contlines-1][countchars-2] == '/')
     {
	     lines[contlines-1][countchars-1] = c;
	     lines[contlines-1][0] =' ';
	     lines[contlines-1][1] =' ';
     }
     else
     {
	lines[contlines-1][countchars-1] = c;
     }

   }/* End of the while */
 }/* End of the option to change from "/*" to //  */

 //Replace the lines in the file by the ones in the array of lines
  fclose(file);
  int length = strlen(argv[1]);
  char * nfile = (char*)malloc(length+2);
  for(i = 0; i < length;i++)
    nfile[i] = argv[1][i];
  nfile[i]='r';
  nfile[i+1]='\0';
  FILE * replaced = fopen(nfile,"w");
  for(i = 0; i < contlines;i++)
    fprintf(replaced,"%s",lines[i]);
  fclose(replaced);
  return 0;  

}//End of main

////////////////////////////////////////
//
//	INSERTS SPACES AT THE BEGINING OF A LINE
//
//////////////////////////////////////////////////////
//
 void InsertSpaces(char * line, int linelength, int spaces)
 {
    //Make gap for the spaces
    int i;
    for(i = linelength; i > 0; i--)
      line[i+spaces-1] = line[i-1];
    //Add the spaces at the beginning of the line
    for(i = 0; i < spaces; i++)
      line[i] = ' ';
 }
