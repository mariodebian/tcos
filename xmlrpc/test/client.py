#!/usr/bin/env python2.4
import xmlrpclib
from time import time, sleep
import sys
import threading
import base64
import popen2
import urllib2

# Create an object to represent our server.
#server_url = 'http://192.168.0.10:8998/RPC2';

#server_url = 'http://192.168.0.11:8998/RPC2';
#server_url = 'http://192.168.0.6:8998/RPC2';
#server_url = 'http://localhost:8998/RPC2';
server_url = 'http://192.168.0.21:8998/RPC2';

server = xmlrpclib.ServerProxy(server_url, verbose=0);


result = server.tcos.version("")
print "PYTHON::version is=%s" %(result)

#result = server.tcos.info("get_client")
#print "PYTHON::get_client is=%s" %(result)

#result = server.tcos.standalone("get_server")
#print "PYTHON::get_server is=%s" %(result)

#result = server.tcos.info("get_user")
#print "PYTHON::get_user is=%s" %(result)


#result = server.tcos.standalone("get_process")
#print "PYTHON::get_process is=%s" %(result)


#result = server.tcos.dbus("--auth='root:root' --type='exec' --text='xterm' --username='mario'", "root", "root")
#print "PYTHON::dbus is=%s" %(result)

#result=server.tcos.exe("uname", "root", "root")
#print "PYTHON::exe uname is=%s" %(result)

#print "PYTHON devicesctl.sh =%s" %(server.tcos.sound("soundctl.sh --showcontrols", "root", "root"))
#print server.tcos.screenshot("10", "root", "root")

def get_url():
  start=time()
  result=server.tcos.screenshot("65", "root", "root")
  #print "\nPYTHON::exe screenshot is=%s" %(result)
  # download with urllib2
  mem = urllib2.urlopen("http://192.168.0.21:8081/capture-thumb.jpg").read()
  f=open("image-urllib.jpg", "w")
  f.write(mem)
  f.close()
  #print "Take screenshot using screenshot() and urllib2 %f secs"%(time() - start)
  return "%.4f" %(time() - start)

def get_base64():
  start=time()
  result=server.tcos.getscreenshot("65", "root", "root")
  #print "PYTHON::exe getscreenshot is=%s" %(result[0])
  f=open("image-base64.jpg", "w")
  f.write(base64.decodestring(result[1]))
  f.close()
  #print "Take screenshot using base64 and getscreenshot() %f secs"%(time() - start)
  return "%.4f" %(time() - start)

itime1=[]
itime2=[]
if len(sys.argv) > 1:
  iters=int(sys.argv[1])
else:
  iters=20

print "Taking %d screenshots...."%iters
for i in range(iters):
  if i % 2 == 0:
    itime1.append (get_url())
  else:
    itime2.append (get_base64())

print "using urllib ", itime1
print "using base64 ", itime2

print "Comparing..."
better1=0
better2=0
for i in range(iters/2):
  if float(itime1[i]) < float(itime2[i]):
    better1+=1
  else:
    better2+=1

print "better with urllib2 %d"%better1
print "better with base64  %d"%better2

#p = popen2.Popen3("display image.jpg")


sys.exit(0)



