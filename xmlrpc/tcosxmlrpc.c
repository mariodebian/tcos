/*# tcosxmlrpc.c standalone XML-RPC server written in C 2006-09-09 14:22:40 mariodebian $
#
# This file is part of tcosxmlrpc.
#
# tcosxmlrpc is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# tcosxmlrpc is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with tcosxmlrpc; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
*/

/* 

Incompatible pointer types error
http://www.elrincondelc.com/portal/modules.php?name=Forums&file=viewtopic&p=20325

*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xmlrpc.h>

#ifdef XMLRPC_INC
  #include XMLRPC_INC
#endif

#include <unistd.h>

/*#define VERSION "0.0.15"*/
#define BUFF_SIZE 1000
#define HAVE_MAIN

/* My TCOS includes */

#include "debug.c"

#include "login.c"
#include "xauth.c"
#include "info.c"
#include "standalone.c"
#include "pci.c"
#include "exe.c"
#include "screenshot.c"
#include "xorg.c"
#include "sound.c"
#include "devices.c"
#include "lockscreen.c"

static xmlrpc_value *
tcos_version(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {

    dbgtcos("tcosxmlrpc::tcos_version() %s\n", VERSION);
    return xmlrpc_build_value(env, "s", VERSION);
 }

static xmlrpc_value *
tcos_echo(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
   char *s;
   size_t *len;

   
   xmlrpc_parse_value(env, in, "(s#)", &s, &len);

   dbgtcos("tcosxmlrpc::tcos_echo() %s\n", s);

   return xmlrpc_build_value(env, "s", s);
 }



static xmlrpc_value *
tcos_status (xmlrpc_env *env, xmlrpc_value *param_array, void *user_data)
{
   FILE *fp=NULL;
   char *app=NULL;
   /*char *login_ok;
   char *user;
   char *pass;*/
   char cmd[BUFF_SIZE];
   char ret[BUFF_SIZE];


   dbgtcos("tcosxmlrpc::tcos_status() Init \n");

   /* Parse app string */
   xmlrpc_parse_value(env, param_array, "(s)", &app);

/*   xmlrpc_parse_value(env, param_array, "(sss)", &app, &user, &pass); */
   if (env->fault_occurred)
	return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  /* login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );
  */

   dbgtcos("tcosxmlrpc::tcos_status() pidof %s\n", app);
   

   snprintf( (char*) &cmd, BUFF_SIZE, "pidof %s| grep [1234567890] | wc -l" ,app);

   dbgtcos("tcosxmlrpc::tcos_status() exec cmd=\"%s\"\n", cmd);

   fp=(FILE*)popen(cmd, "r");
   if (env->fault_occurred)
	return xmlrpc_build_value(env, "s", "exec error");;

   dbgtcos("tcosxmlrpc::tcos_status() reading from fp pointer\n");

   fscanf(fp, "%s", ret);

   dbgtcos( "tcosxmlrpc::tcos_status() ret value=%s\n", ret);
   
   pclose(fp);

   if (ret != NULL)   
       return xmlrpc_build_value(env, "s", ret);
   else
       return xmlrpc_build_value(env, "s", "error");

}


int main (int argc, char **argv)
{
    /* check command line config file */
    if (argc != 2) {
	fprintf(stderr, "Usage:\n\t tcosxmlrpc /path/of/abyss.conf\n");
	return(1);
    }


    xmlrpc_server_abyss_init(XMLRPC_SERVER_ABYSS_NO_FLAGS, argv[1]);

    /* add methods */

    xmlrpc_server_abyss_add_method_w_doc("tcos.login", &tcos_login, NULL,
    "ss:s", "Tcos, Basic auth system.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.logout", &tcos_logout, NULL,
    ":s", "Tcos, Basic auth system.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.version", &tcos_version, NULL,
    ":s", "Tcos, Returns tcosxmlrpc version. (no auth needed)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.exe", &tcos_exe, NULL,
    "sss:s", "Tcos, Exec a command passed as string. (need login first)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.kill", &tcos_kill, NULL,
    "sss:s", "Tcos, killall command passed as string. (need login first)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.status", &tcos_status, NULL,
    "s:s", "Tcos, returns 1 if app running 0 if not or error string. (no auth needed)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.screenshot", &tcos_screenshot, NULL,
    "ss:s", "Tcos, make a screenshot and return files. (need login first)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.xorg", &tcos_xorg, NULL,
    "ssss:s", "Tcos, configure, change or get xorg.conf settings. (need login first)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.sound", &tcos_sound, NULL,
    "ssss:s", "Tcos, configure, change or get sound settings.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.devices", &tcos_devices, NULL,
    "ssss:s", "Tcos, configure, change or get devices settings.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.xauth", &tcos_xauth, NULL,
    "ss:s", "Tcos, authenticate with X cookies.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.lockscreen", &tcos_lockscreen, NULL,
    "ss:s", "Tcos, exec lockscreen to block thin client.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.unlockscreen", &tcos_unlockscreen, NULL,
    "ss:s", "Tcos, kill lockscreen to unblock thin client.");

/* DEPRECATED METHOD
    xmlrpc_server_abyss_add_method_w_doc("tcos.update_system_info", &tcos_update_system_info, NULL,
    ":i", "Exec \"WEBSERVER_SCRIPT update\" (need login first).");
*/
    xmlrpc_server_abyss_add_method_w_doc("tcos.echo", &tcos_echo, NULL,
    "s:s", "Tcos, debug class, returns string passed. (no auth needed)");

    /* Info method */
    xmlrpc_server_abyss_add_method_w_doc("tcos.info", &tcos_info, NULL,
    "s:s", "Tcos, return passed info string. (no auth needed)\n\
Info methods:\n\
* cpu_model\n\
* cpu_speed\n\
* cpu_vendor\n\
* ram_total\n\
* ram_active\n\
* ram_free\n\
* ram_used\n\
* swap_avalaible\n\
* swap_total\n\
* swap_used\n\
* swap_free\n\
* tcos_date\n\
* tcos_generation_date\n\
* tcos_version\n\
* network_hostname\n\
* network_ip\n\
* network_mak\n\
* network_mask\n\
* network_rx\n\
* network_tx\n\
* modules_loaded");

    xmlrpc_server_abyss_add_method_w_doc("tcos.standalone", &tcos_standalone, NULL,
    "s:s", "Tcos, Standalone. Return standalone values (no auth needed)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.dbus", &tcos_dbus, NULL,
    "s:s", "Tcos, DBus. Exe with a wrapper some dbus events (auth needed)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.pci", &tcos_pci, NULL,
    "s:s", "Tcos, PCI data stuff. Send pci_all to get pci bus ids. (no auth needed)");

    dbgtcos("tcosxmlrpc::main() switching to background.\n");

    xmlrpc_server_abyss_run();

    /* We never reach this point. */
    return 0;
}
