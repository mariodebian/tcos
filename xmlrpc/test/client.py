#!/usr/bin/env python2.4
import xmlrpclib
from time import time, sleep
import sys
import threading


# Create an object to represent our server.
#server_url = 'http://192.168.0.10:8080/RPC2';

#server_url = 'http://192.168.0.11:8080/RPC2';
#server_url = 'http://192.168.0.6:8080/RPC2';
server_url = 'http://localhost:8080/RPC2';
#server_url = 'http://192.168.0.100:8080/RPC2';

server = xmlrpclib.Server(server_url);


result = server.tcos.version("")
print "PYTHON::version is=%s" %(result)

#result = server.tcos.info("get_client")
#print "PYTHON::get_client is=%s" %(result)

result = server.tcos.standalone("get_server")
print "PYTHON::get_server is=%s" %(result)

result = server.tcos.info("get_user")
print "PYTHON::get_user is=%s" %(result)


result = server.tcos.standalone("get_process")
print "PYTHON::get_process is=%s" %(result)


result = server.tcos.dbus("--auth='root:root' --type='exec' --text='xterm' --username='mario'", "root", "root")
print "PYTHON::dbus is=%s" %(result)

#result=server.tcos.exe("uname", "root", "root")
#print "PYTHON::exe uname is=%s" %(result)

#print "PYTHON devicesctl.sh =%s" %(server.tcos.sound("soundctl.sh --showcontrols", "root", "root"))
#print server.tcos.screenshot("10", "root", "root")

sys.exit(0)



