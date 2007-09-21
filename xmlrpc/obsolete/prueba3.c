#include <stdio.h>
#include <string.h>

main()
{
  FILE *fp;
  char line[130];			/* line of data from unix command*/
  char *line2;
   
  fp = popen("pidof evolution", "r");		/* Issue the command.		*/

  fscanf(fp , "%s\n", line);
  printf("%s\n", line);

  sprintf(line2, "pidof evolution=%s", line);
  printf("%s\n", line2);
 
  if (line == " ")
    printf ("Not running!!\n");					/* Read a line			*/


//  while ( fgets( line, sizeof line, fp))
//  {
//    printf("OUT: %s", line);
//  }
  pclose(fp);
  return(0);
}

