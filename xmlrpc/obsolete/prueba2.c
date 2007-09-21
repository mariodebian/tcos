#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#define MSIZE 250

struct output {
   long stype;
   char *scmd;
   char *stext;
} ;


void exec_cmd(struct output *salida)
{
  /*
  cmd=command
  mode is r or w (read from stdin or write)
  */
  FILE *fp;
  
#ifdef DEBUG
  fprintf(stderr, "tcosxmlrpc::exec_cmd() exec \"%s\"\n", salida.scmd);
#endif

  fp = popen(*salida.scmd, "r");	
  //fprintf(salida.stext, "%s\n", fp);
  
  //fflush(value);
  //fgets( value, sizeof value, fp);

  fgets (*salida.stext, 250, fp);
	/*while (!feof (fp))
	{
		//printf ("%s", value);
		fgets (salida.stext, 1000, fp);
	}*/

  pclose(fp);

#ifdef DEBUG
  fprintf(stderr, "tcosxmlrpc::exec_cmd() returning \"%s\"\n", *salida.stext);
#endif

}


main() {

  struct output salida;
  salida.stype=NULL;
  salida.scmd=NULL;
  salida.stext=NULL;
  strcpy(salida.scmd, "ps aux|grep evolution|grep -v grep|wc -l"); 
  
  exec_cmd(&salida);
  
  printf("Salida:\n%s\n", salida.stext);
  /*char *value;
  char *cmd;
  cmd="ps aux|grep evolution|grep -v grep|wc -l";
  
  value=exec_cmd(cmd);
  printf("value=%s\n", value);
  if (value == "4")
      printf ("value es 4\n");
*/
}
