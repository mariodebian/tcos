#define XAUTH_OK 0
#define XAUTH_ERROR 1
#define XAUTH_FAIL -1


int gethostname(char *name, size_t len);
int setenv(const char *nombre, const char *valor, int sobrescribir);
int handle_xauth( char *cookie , char *servername);
