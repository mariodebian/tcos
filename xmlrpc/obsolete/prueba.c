#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct info {
  char line[150];
  char *user;
  char *pass;
  int logged;
} login ;



void split_login( struct info *login)
{
int i;
char *p;

  p = strtok (login->line,":");
  i=0;
  while (p != NULL)
  {
    if(i==0){
      login->user=p;
      i++;
    }
    else if(i==1){
      login->pass=p;
      i++;
    }
    p = strtok (NULL, "");
  }
}


int main() {

struct info *login;

strcpy(login->line, "usuario:password");
split_login(login);
    
   printf("usuario=\"%s\"\n", login->user);
   printf("   pass=\"%s\"\n", login->pass);
  return 0;
}
