#!/usr/bin/python

#
#  Based on StudentControlPanel of Ubuntu
#
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
    print "for command line options use tcos-dbus --help"
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
        admin, passwd = a.split(":")


if  username and (type in ["mess", "exec", "kill", "killall"]) and text :
    response=dbus_iface.GotSignal([ [admin, passwd], [username] , [type] , [text] ])
    print "ok"
else:
    print "error: need more arguments..."

