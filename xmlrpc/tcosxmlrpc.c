/*
* tcosxmlrpc.c standalone XML-RPC server written in C
* Copyright (C) 2006,2007,2008  mariodebian at gmail
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/





#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <xmlrpc.h>
#include <xmlrpc_abyss.h>

#define OLD_VERSION_MAYOR 0
#define OLD_VERSION_MINOR 920

#if XMLRPC_VERSION_MAYOR > OLD_VERSION_MAYOR || ( (XMLRPC_VERSION_MAYOR == OLD_VERSION_MAYOR) && (XMLRPC_VERSION_MINOR > OLD_VERSION_MINOR) )
    #define NEWAPI 1
    #include <xmlrpc_server.h>
#endif





#define BUFF_SIZE 1000
#define HAVE_MAIN

/* My TCOS includes */

#include "debug.c"
#include "common.c"

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
#include "lockcontroller.c"
#include "vnc.c"
#include "vlc.c"
#include "rtp.c"
#include "tnc.c"

#include "get_screenshot.c"
#include "reboot_poweroff.c"

#include "dpms.c"

#include "simple-methods.c"






int main (int argc, char **argv)
{
#ifdef NEWAPI
    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env envP;
#endif

    /* check command line config file */
    if (argc != 2) {
	fprintf(stderr, "Usage:\n\t tcosxmlrpc /path/of/abyss.conf\n");
	return(1);
    }

    /* initialize server */

#ifdef NEWAPI
    xmlrpc_env_init(&envP);
    registryP = xmlrpc_registry_new(&envP);
#else
    xmlrpc_server_abyss_init(XMLRPC_SERVER_ABYSS_NO_FLAGS, argv[1]);
#endif


    /* add methods */


#ifdef NEWAPI
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.version", &tcos_version, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.echo", &tcos_echo, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.status", &tcos_status, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.login", &tcos_login, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.logout", &tcos_logout, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.xauth", &tcos_xauth, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.standalone", &tcos_standalone, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.dbus", &tcos_dbus, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.pci", &tcos_pci, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.exe", &tcos_exe, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.kill", &tcos_kill, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.screenshot", &tcos_screenshot, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.xorg", &tcos_xorg, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.sound", &tcos_sound, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.devices", &tcos_devices, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.lockscreen", &tcos_lockscreen, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.unlockscreen", &tcos_unlockscreen, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.lockcontroller", &tcos_lockcontroller, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.unlockcontroller", &tcos_unlockcontroller, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.info", &tcos_info, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.vnc", &tcos_vnc, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.getscreenshot", &tcos_get_screenshot, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.vlc", &tcos_vlc, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.rtp", &tcos_rtp, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.tnc", &tcos_tnc, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.rebootpoweroff", &tcos_reboot_poweroff, NULL);
    xmlrpc_registry_add_method(&envP, registryP, NULL, "tcos.dpms", &tcos_dpms, NULL);
#else
    xmlrpc_server_abyss_add_method_w_doc("tcos.version", &tcos_version, NULL,
    ":s", "Tcos, Returns tcosxmlrpc version. (no auth needed)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.echo", &tcos_echo, NULL,
    "s:s", "Tcos, debug class, returns string passed. (no auth needed)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.status", &tcos_status, NULL,
    "s:s", "Tcos, returns 1 if app running 0 if not or error string. (no auth needed)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.login", &tcos_login, NULL,
    "ss:s", "Tcos, Basic auth system.");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.logout", &tcos_logout, NULL,
    ":s", "Tcos, Basic auth system.");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.xauth", &tcos_xauth, NULL,
    "ss:s", "Tcos, authenticate with X cookies.");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.standalone", &tcos_standalone, NULL,
    "ss:s", "Tcos, Standalone. Return standalone values (no auth needed)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.dbus", &tcos_dbus, NULL,
    "s:s", "Tcos, DBus. Exe with a wrapper some dbus events (auth needed)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.pci", &tcos_pci, NULL,
    "s:s", "Tcos, PCI data stuff. Send pci_all to get pci bus ids. (no auth needed)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.exe", &tcos_exe, NULL,
    "sss:s", "Tcos, Exec a command passed as string. (need login first)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.kill", &tcos_kill, NULL,
    "sss:s", "Tcos, killall command passed as string. (need login first)");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.screenshot", &tcos_screenshot, NULL,
    "ss:s", "Tcos, make a screenshot and return files. (need login first)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.xorg", &tcos_xorg, NULL,
    "ssss:s", "Tcos, configure, change or get xorg.conf settings. (need login first)");

    xmlrpc_server_abyss_add_method_w_doc("tcos.sound", &tcos_sound, NULL,
    "ssss:s", "Tcos, configure, change or get sound settings.");
    
    xmlrpc_server_abyss_add_method_w_doc("tcos.devices", &tcos_devices, NULL,
    "ssss:s", "Tcos, configure, change or get devices settings.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.lockscreen", &tcos_lockscreen, NULL,
    "ss:s", "Tcos, exec lockscreen to block thin client.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.unlockscreen", &tcos_unlockscreen, NULL,
    "ss:s", "Tcos, kill lockscreen to unblock thin client.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.lockcontroller", &tcos_lockcontroller, NULL,
    "sss:s", "Tcos, exec lockvnc or lockvlc to block thin client.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.unlockcontroller", &tcos_unlockcontroller, NULL,
    "sss:s", "Tcos, kill lockvlc or lockvnc to unblock thin client.");

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

    xmlrpc_server_abyss_add_method_w_doc("tcos.vnc", &tcos_vnc, NULL,
    "ssss:s", "Tcos, manage vnc server and client.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.getscreenshot", &tcos_get_screenshot, NULL,
    "ssss:ss", "Tcos, make screenshot and return in base64.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.vlc", &tcos_vlc, NULL,
    "ssss:s", "Tcos, manage vlc params.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.rtp", &tcos_rtp, NULL,
    "ssss:s", "Tcos, manage rtp params.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.tnc", &tcos_rtp, NULL,
    "sssss:s", "Tcos, net controller params.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.rebootpoweroff", &tcos_reboot_poweroff, NULL,
    "ssss:s", "Tcos, Reboot or Poweroff using Xorg cookie.");

    xmlrpc_server_abyss_add_method_w_doc("tcos.dpms", &tcos_dpms, NULL,
    "sss:s", "Tcos, manage DPMS Monitor options.");
#endif

    /*  end of add methods */


    dbgtcos("tcosxmlrpc::main() switching to background.\n");


#ifdef NEWAPI
    serverparm.config_file_name = argv[1];
    serverparm.registryP = registryP;
    xmlrpc_server_abyss(&envP, &serverparm, XMLRPC_APSIZE(registryP));
#else
    xmlrpc_server_abyss_run();
#endif

    /* We never reach this point. */
    return 0;
}
