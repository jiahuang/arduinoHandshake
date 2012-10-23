# sends a bmp over serial to the arduino for writing

import serial
from datetime import datetime
import time
import os
#arduino1 = serial.Serial('/dev/tty.usbmodem1411', 9600)
arduino2 = serial.Serial('/dev/tty.usbserial-A800eHxm', 9600)

# cmd = "curl -F 'access_token=AAAHZA5yyJhbMBAHZCK6ZC01RxKU1jfahYZAkipH58hrVdAlyH4G9FH17AMOelXdk6ZCFR2vdi1JVPKHsHChrEQQDtt8yqp6QxdsDTrSmfdAZDZD' \
#       -F 'profile=http://thawing-shore-8520.herokuapp.com/hit/' \
#       'https://graph.facebook.com/me/scope_kendo:hit'"

#os.system(cmd)
#one = []
two = []

dataWrite = open("data2.txt", "a")

while 1:
  data2 = arduino2.readline()
  #data2 = arduino2.readline()
  print data2
  now = time.time()
  if data2: #== "Handshake"
    two.append(now)
    print "date 1", now
    dataWrite.write(str(now)+"\n")

