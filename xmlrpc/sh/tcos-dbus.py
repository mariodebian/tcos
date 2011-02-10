#!/usr/bin/python
#
#  Based on StudentControlPanel of Ubuntu
#
# tcos-dbus.py 
#   => shell script t return some /proc info
# Copyright (C) 2006,2007 StudentControlPanel
# Copyright (C) 2006-2011  mariodebian at gmail
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#



import signal
import dbus
import dbus.service
import dbus.glib
import getopt
import sys

class TcosObject(dbus.service.Object):

    def __init__(self, bus_name, object_path="/TCOSObject"):
        dbus.service.Object.__init__(self, bus_name, object_path)

    @dbus.service.signal("com.consoltux.TcosMonitor.Comm", signature="aas")
    def GotSignal(self, message):
        pass


system_bus = dbus.SystemBus()
name = dbus.service.BusName("com.consoltux.TcosMonitor", bus=system_bus)
dbus_iface = TcosObject(name)

try:
    opts, args = getopt.getopt(sys.argv[1:], ":hd", ["help", "debug", "auth=", "username=", "type=", "text="])
except getopt.error, msg:
    print msg
    sys.exit(2)

username=None

# process options
for o, a in opts:
    if o in ("-d", "--debug"):
        debug = True
    if o == "--username":
        username = a
    if o == "--type":
        type = a
    if o == "--text":
        text = a
    if o == "--auth":
        try:
            admin, passwd = a.split(":")
        except:
            print "error: bad username or password"
            sys.exit(1)


if  username and (type in ["mess", "exec", "kill", "killall", "sendfiles"]) and text :
    response=dbus_iface.GotSignal([ [admin, passwd], [username] , [type] , [text] ])
    print "ok"
else:
    print "error: need more arguments..."

