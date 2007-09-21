/*# info.c method that get system info based on /proc 2006-09-09 14:22:40 mariodebian $
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

#include "info.h"


static xmlrpc_value *
tcos_info(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
  FILE *fp;
  char line[BIG_BUFFER];
  char *info;
  size_t *len;

  /* read what info search */
  xmlrpc_parse_value(env, in, "(s#)", &info, &len);

  dbgtcos("tcosxmlrpc::tcos_info() searching for info=\"%s\"\n", info);

  if ( strcmp(info, "get_client") == 0 )
      fp=(FILE*)popen(GET_CLIENT, "r");
  else if ( strcmp(info, "get_process") == 0 )
      fp=(FILE*)popen(GET_PROCESS, "r");

  /* CPU METHODS */
  else if ( strcmp(info, "cpu_model") == 0 )
      fp=(FILE*)popen(CPU_MODEL, "r");
  else if ( strcmp(info, "cpu_speed" ) == 0)
      fp=(FILE*)popen(CPU_SPEED, "r");
  else if ( strcmp(info, "cpu_vendor" ) == 0)
      fp=(FILE*)popen(CPU_VENDOR, "r");

  /* RAM METHODS */
  else if ( strcmp(info, "ram_total" ) == 0)
      fp=(FILE*)popen(RAM_TOTAL, "r");
  else if ( strcmp(info, "ram_active" ) == 0)
      fp=(FILE*)popen(RAM_ACTIVE, "r");
  else if ( strcmp(info, "ram_free" ) == 0)
      fp=(FILE*)popen(RAM_FREE, "r");
  else if ( strcmp(info, "ram_used" ) == 0)
      fp=(FILE*)popen(RAM_USED, "r");

  /* SWAP METHODS */
  else if ( strcmp(info, "swap_avalaible" ) == 0)
      fp=(FILE*)popen(SWAP_AVALAIBLE, "r");
  else if ( strcmp(info, "swap_total" ) == 0)
      fp=(FILE*)popen(SWAP_TOTAL, "r");
  else if ( strcmp(info, "swap_used" ) == 0)
      fp=(FILE*)popen(SWAP_USED, "r");
  else if ( strcmp(info, "swap_free" ) == 0)
      fp=(FILE*)popen(SWAP_FREE, "r");

  /* TCOS METHODS */
  else if ( strcmp(info, "tcos_date" ) == 0)
      fp=(FILE*)popen(TCOS_DATE, "r");
  else if ( strcmp(info, "tcos_generation_date" ) == 0)
      fp=(FILE*)popen(TCOS_GENERATION_DATE, "r");
  else if ( strcmp(info, "tcos_version" ) == 0)
      fp=(FILE*)popen(TCOS_VERSION, "r");
  else if ( strcmp(info, "tcos_uptime" ) == 0)
      fp=(FILE*)popen(TCOS_UPTIME, "r");

  /* KERNEL METHODS*/
  else if ( strcmp(info, "kernel_version" ) == 0)
      fp=(FILE*)popen(KERNEL_VERSION, "r");
  else if ( strcmp(info, "kernel_complete_version" ) == 0)
      fp=(FILE*)popen(KERNEL_COMPLETE_VERSION, "r");

  /* NETWORK METHODS */  
  else if ( strcmp(info, "network_hostname" ) == 0)
      fp=(FILE*)popen(NETWORK_HOSTNAME, "r");
  else if ( strcmp(info, "network_ip" ) == 0)
      fp=(FILE*)popen(NETWORK_IP, "r");
  else if ( strcmp(info, "network_mac" ) == 0)
      fp=(FILE*)popen(NETWORK_MAC, "r");
  else if ( strcmp(info, "network_mask" ) == 0)
      fp=(FILE*)popen(NETWORK_MASK, "r");
  else if ( strcmp(info, "network_rx" ) == 0)
      fp=(FILE*)popen(NETWORK_RX, "r");
  else if ( strcmp(info, "network_tx" ) == 0)
      fp=(FILE*)popen(NETWORK_TX, "r");

  /* MODULES METHODS */  
  else if ( strcmp(info, "modules_loaded" ) == 0)
      fp=(FILE*)popen(MODULES_LOADED, "r");
  else if ( strcmp(info, "modules_notfound" ) == 0)
      fp=(FILE*)popen(MODULES_NOTFOUND, "r");

  /* default method = error */
  else
      return xmlrpc_build_value(env, "s", INFO_UNKNOW );

  if (fp == NULL)
	return xmlrpc_build_value(env, "s", INFO_UNKNOW );

  /* put error into line var */
  strncpy(line, INFO_ERROR, BSIZE);

  fgets( line, sizeof line, fp);

  dbgtcos("tcosxmlrpc::tcos_info() line=\"%s\"\n", line);

  pclose(fp);
  return xmlrpc_build_value(env, "s", line );  
}




