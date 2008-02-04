#define XAUTH_OK 0
#define XAUTH_ERROR 1
#define XAUTH_FAIL -1
#define IP_LENGHT 1024


#ifndef __XAUTH_H__

typedef struct ip_address {
  int data[4] ;
  int is_ip;
  char ipstr[IP_LENGHT];
} ip ;

#define  __XAUTH_H__
#endif


int gethostname(char *name, size_t len);
int setenv(const char *nombre, const char *valor, int sobrescribir);
void unsetenv(const char *nombre);
int handle_xauth( char *cookie , char *servername);
int snprintf(char *str, size_t size, const char *format, ...);
