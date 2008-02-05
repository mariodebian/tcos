/*
* pci.c part of tcosxmlrpc
*   => return all pci buses info
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


#include "pci.h"

#if NEWAPI
static xmlrpc_value *tcos_pci(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_pci(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BSIZE];
  char *pci;
  char allpci[BSIZE];
  char pci_cmd[BSIZE];
  size_t *len;

  /* put error msg into line var */
  strncpy(line, PCI_ERROR, BSIZE);


  /* read what info search */
  xmlrpc_parse_value(env, in, "(s#)", &pci, &len);

  dbgtcos("tcosxmlrpc::tcos_pci() searching for pci=\"%s\"\n", pci);


  if (strcmp(pci, "") == 0 )
  {
	return xmlrpc_build_value(env, "s", PCI_NEED_ID );
  }
  /* list all pci ids */
  else if ( strcmp(pci, "pci_all") == 0 )
  {
      fp=(FILE*)popen(PCI_ALL, "r");
      dbgtcos("tcosxmlrpc::tcos_pci() reading pipe\n");

       if (fp == NULL)
	return xmlrpc_build_value(env, "s", PCI_FP_ERROR );

      fgets( line, sizeof line, fp);
      dbgtcos("tcosxmlrpc::tcos_pci() line=\"%s\"\n", line);

      pclose(fp);
      return xmlrpc_build_value(env, "s", line );
  }
  /* default method */
  else
  {
      /* read all PCI ids and store in allpci */
      fp=(FILE*)popen(PCI_ALL, "r");
      fgets( allpci, sizeof allpci, fp);
      pclose(fp);
      /* search pci in allpci */

      dbgtcos("tcosxmlrpc::tcos_pci() compare=\"%d\"\n", strstr( allpci, pci));

      if ( strstr( allpci, pci ) == 0 ) {
	return xmlrpc_build_value(env, "s", PCI_UNKNOW );
      }
      else {
        /* return info about pci bus id */
	snprintf ( (char*) pci_cmd, BSIZE, "lspci |grep \"%s\" | sed s/\"%s \"//g", pci, pci);

        dbgtcos("tcosxmlrpc::tcos_pci() pci_cmd=\"%s\"\n", pci_cmd);

        fp=(FILE*)popen(pci_cmd, "r");
	fgets( line, sizeof line, fp);
        pclose(fp);
        return xmlrpc_build_value(env, "s", line );
      }
  }

  /* never here */
  return xmlrpc_build_value(env, "s", PCI_UNKNOW );
}

