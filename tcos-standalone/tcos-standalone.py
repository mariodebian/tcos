#!/usr/bin/env python
# -*- coding: UTF-8 -*-
##########################################################################
# TcosStandalone writen by MarioDebian <mariodebian@gmail.com>
#
#    TcosStandalone version __VERSION__
#
# Copyright (c) 2007 Mario Izquierdo <mariodebian@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
###########################################################################

import sys
import os

import pygtk
pygtk.require('2.0')
from gtk import *
import gtk.glade

from time import time, sleep
import getopt
from gettext import gettext as _
from gettext import bindtextdomain, textdomain
from locale import setlocale, LC_ALL

from subprocess import Popen, PIPE, STDOUT
import popen2
from threading import Thread

#import threading
gtk.gdk.threads_init()

import gobject

debug=False
PACKAGE="tcos-standalone"

# if exec from svn or sources dir
if os.path.isdir('../debian'):
    LOCALE_DIR = "./po/"
    GLADE_DIR = "./"
    IMG_DIR = "./images/"
    tcos_standalone_file="../debian/tcos-standalone.default"
    tcos_standalone_pass="/etc/tcospasswd"
    daemon_cmd="sh ../debian/tcos-standalone.init"
    print "exec in sources dir"
else:
    tcos_standalone_file="/etc/default/tcos-standalone"
    tcos_standalone_pass="/etc/tcospasswd"
    daemon_cmd="/etc/init.d/tcos-standalone"
    GLADE_DIR = "/usr/share/tcos-standalone/"
    IMG_DIR = "/usr/share/tcos-standalone/images/"
    LOCALE_DIR = "/usr/share/locale"

def print_debug(txt):
    if debug:
        print "%s::%s" %("tcos-standalone", txt)
    return

def usage():
    print "tcos-standalone help:"
    print ""
    print "   tcos-standalone -d [--debug]  (write debug data to stdout)"
    print "   tcos-standalone -h [--help]   (this help)"


try:
    opts, args = getopt.getopt(sys.argv[1:], ":hd", ["help", "debug"])
except getopt.error, msg:
    print msg
    print "for command line options use tcosconfig --help"
    sys.exit(2)

# process options
for o, a in opts:
    if o in ("-d", "--debug"):
        print "DEBUG ACTIVE"
        debug = True
    if o in ("-h", "--help"):
        usage()
        sys.exit()


class TcosStandalone:
    def __init__(self):
        print_debug("__init__()")
        
        # vars
        self.v={}
        self.v["daemon_enabled"]=None
        self.v["daemon_running"]={}
        self.v["username"]=None
        self.v["password"]=None
        self.v["newconfig"]=False
        self.v["newpass"]=False
        
        gtk.glade.bindtextdomain(PACKAGE, LOCALE_DIR)
        gtk.glade.textdomain(PACKAGE)

        # gettext support
        setlocale( LC_ALL )
        bindtextdomain( PACKAGE, LOCALE_DIR )
        textdomain( PACKAGE )
        
        # Widgets
        self.ui = gtk.glade.XML(GLADE_DIR + 'tcos-standalone.glade')
        self.mainwindow = self.ui.get_widget('mainwindow')
        self.mainwindow.set_icon_from_file(IMG_DIR +'tcos-icon-32x32.png')
        
        # close windows signals
        self.mainwindow.connect('destroy', self.quitapp )
        self.mainwindow.connect("delete_event", self.quitapp)
        
        # read vars
        self.v["daemon_enabled"]=self.read_conf(tcos_standalone_file, "TCOS_STANDALONE_START", 1, '=')
        self.v["username"]=self.read_conf(tcos_standalone_pass, "root", 0, ':')
        daemons=self.exe_cmd(daemon_cmd + " status")
        for daemon in daemons:
            self.v["daemon_running"][daemon.split(' ')[0]]=daemon.split(' ')[1]
        
        # widgets
        self.w={}
        self.w["txt_username"] = self.ui.get_widget('txt_username')
        self.w["txt_password"] = self.ui.get_widget('txt_password')
        self.w["lb_status"] = self.ui.get_widget('lb_status')
        self.w["ck_standalone_start"] = self.ui.get_widget('ck_standalone_start')
        self.w["lb_tcosxmlrpc"] = self.ui.get_widget('lb_tcosxmlrpc')
        self.w["lb_busybox"] = self.ui.get_widget('lb_busybox')
        self.w["img_tcosxmlrpc"] = self.ui.get_widget('img_tcosxmlrpc')
        self.w["img_busybox"] = self.ui.get_widget('img_busybox')
        self.w["bt_apply"] = self.ui.get_widget('bt_apply')
        self.w["bt_stop"] = self.ui.get_widget('bt_stop')
        self.w["bt_start"] = self.ui.get_widget('bt_start')
        
        self.w["ck_standalone_start"].connect('toggled', self.ck_change )
        
        if self.v["daemon_enabled"] == '1':
            self.w["ck_standalone_start"].set_active(True)
        else:
            self.w["ck_standalone_start"].set_active(False)
        
        
        
        self.w["txt_username"].connect('changed', self.pass_change )
        self.w["txt_password"].connect('changed', self.pass_change )
        
        if not self.v["username"]:
            self.w["lb_status"].set_text( _("Not user and password configured.\nUsing root system user.") )
        else:
            self.w["txt_username"].set_text(self.v["username"])
        
        
        self.w["bt_apply"].connect('clicked', self.apply_changes)
        
        self.w["bt_stop"].connect('clicked', self.stop_daemon)
        self.w["bt_start"].connect('clicked', self.start_daemon)

        print_debug ( self.v )

        self.v["newconfig"]=False
        self.v["newpass"]=False
        self.update_images()
        self.refresh_buttons()
        

    def ck_change(self, *args):
        self.v["newconfig"]=True
        self.refresh_buttons()

    def pass_change(self, *args):
        self.v["newpass"]=True
        self.refresh_buttons()


    def refresh_buttons(self):
        print_debug("refresh_buttons() newconfig=%s newpass=%s" %(self.v["newconfig"], self.v["newpass"]))
        if self.v["newconfig"] or self.v["newpass"]:
            # configure buttons
            self.w["bt_apply"].set_sensitive(True)
        else:
            # configure buttons
            self.w["bt_apply"].set_sensitive(False)
            

    def apply_changes(self, *args):
        if self.v["newconfig"]:
            print_debug("apply_changes() newconfig")
            # daemon enabled when open app
            if self.v["daemon_enabled"] != '0':
                if self.w["ck_standalone_start"].get_active() != 1:
                    # disable daemon
                    self.exe_cmd(daemon_cmd + " stop")
                    self.save_conf(tcos_standalone_file, "TCOS_STANDALONE_START", 0, '=')
                    self.v["daemon_enabled"]='0'
            # daemon disabled when open app
            else:
                if self.w["ck_standalone_start"].get_active() == 1:
                    # enable daemon
                    self.save_conf(tcos_standalone_file, "TCOS_STANDALONE_START", 1, '=')
                    self.exe_cmd(daemon_cmd + " stop")
                    self.exe_cmd(daemon_cmd + " start")
                    self.v["daemon_enabled"]='1'
            self.v["newconfig"]=False
        
        if self.v["newpass"]:
            tmp=self.exe_cmd("/usr/sbin/tcospasswd %s" %(self.w["txt_password"].get_text() ) )
            fw=file(tcos_standalone_pass, 'w')
            fw.write("%s:%s\n" %(self.w["txt_username"].get_text(), tmp) )
            fw.close()
            self.w["lb_status"].set_text( _("New username and password used.\n") )
            self.v["newpass"]=False
        
        self.refresh_buttons()
        self.update_images()

    def stop_daemon(self, *args):
        self.exe_cmd(daemon_cmd + " stop")
        self.update_images()

    def start_daemon(self, *args):
        self.exe_cmd(daemon_cmd + " start")
        self.update_images()
        

    def update_images(self):
        daemons=self.exe_cmd(daemon_cmd + " status")
        for daemon in daemons:
            self.v["daemon_running"][daemon.split(' ')[0]]=daemon.split(' ')[1]
            
        if self.v["daemon_running"]["tcosxmlrpc"] == "running":
            self.w["img_tcosxmlrpc"].set_from_stock(gtk.STOCK_YES, 4)
            self.w["lb_tcosxmlrpc"].set_text( _("tcosxmlrpc running") )
            self.w["bt_stop"].set_sensitive(True)
            self.w["bt_start"].set_sensitive(False)
        else:
            self.w["img_tcosxmlrpc"].set_from_stock(gtk.STOCK_NO, 4)
            self.w["lb_tcosxmlrpc"].set_text( _("tcosxmlrpc stopped") )
            self.w["bt_stop"].set_sensitive(False)
            self.w["bt_start"].set_sensitive(True)
        
        if self.v["daemon_running"]["busybox"] == "running":
            self.w["img_busybox"].set_from_stock(gtk.STOCK_YES, 4)
            self.w["lb_busybox"].set_text( _("busybox running") )
        else:
            self.w["img_busybox"].set_from_stock(gtk.STOCK_NO, 4)
            self.w["lb_busybox"].set_text( _("busybox stopped") )


    def read_conf(self, fname, fvar, item=0, sep='='):
        if not os.path.isfile(fname): return None
        f=open(fname, 'r')
        data=f.readlines()
        f.close()
        for line in data:
            if line.startswith(fvar):
                return line.replace('\n','').split(sep)[item]

    def save_conf(self, fname, fvar, value, sep='='):
        if not os.path.isfile(fname): return None
        f=open(fname, 'r')
        data=f.readlines()
        f.close()
        
        fw=file(fname, 'w')
        for i in range(len(data)):
            if data[i].startswith(fvar):
                print_debug("save_conf() line=%s%s%s" %(fvar, sep, value) )
                fw.write( "%s%s%s\n" %(fvar, sep, value) )
            else:
                fw.write(data[i])
                print_debug("save_conf() line=%s" %(data[i].replace('\n','')) )
        fw.close()
        
    def exe_cmd(self, cmd, verbose=1):
        print_debug("exe_cmd() cmd=%s" %cmd)
        output=[]
        (stdout, stdin) = popen2.popen2(cmd)
        stdin.close()
        for line in stdout:
            if line != '\n':
                line=line.replace('\n', '')
                output.append(line)
        if len(output) == 1:
            return output[0]
        elif len(output) > 1:
            if verbose==1:
                print_debug ( "exe_cmd(%s) %s" %(cmd, output) )
            return output
        else:
            if verbose == 1:
                print_debug ( "exe_cmd(%s)=None" %(cmd) )
            return []


    def quitapp(self,*args):
        print_debug ( "Exiting" )
        self.mainloop.quit()

    def run (self):
        self.mainloop = gobject.MainLoop()
        try:
            self.mainloop.run()
        except KeyboardInterrupt: # Press Ctrl+C
            self.quitapp()
   


if __name__ == '__main__':
    app = TcosStandalone ()
    # Run app
    app.run ()
