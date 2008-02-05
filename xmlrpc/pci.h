/*
* pci.h part of tcosxmlrpc
*   => common headers of pci.c
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


/* xmlrpc methods to export thin client info */

/* extract pci info */
#define PCI_ALL "for id in $(lspci| awk '{print $1}'); do echo -n \"$id \"; done; echo"
#define READ_PCI_ID "lspci |grep \"00:08.0\" | sed s/\"00:08.0 \"//g"

/* messages */
#define PCI_UNKNOW "error: Unknow pci id"
#define PCI_FP_ERROR "error: Error opening pointer"
#define PCI_NEED_ID "error: Need PCI id (example 00:10.1)"
#define PCI_ERROR "error: pci command failure"
