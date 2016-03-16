#!/usr/bin/python
import os
import time
import sqlite3 as mydb
import sys

""" Log Current Time, Temperature in Celsius and Fahrenheit
 Returns a list [Date, time, tempC, tempF] """
def readTemp():
	tempfile = open("/sys/bus/w1/devices/28-0000069616c4/w1_slave")
	tempfile_text = tempfile.read()
	currDate = time.strftime("%Y-%m-%d")
	currentTime=time.strftime('%x %X %Z')
	tempfile.close()
	tempC=float(tempfile_text.split("\n")[1].split("t=")[1])/1000
	tempF=tempC*9.0/5.0+32.0
	return [currDate, currentTime, tempC, tempF]

con = mydb.connect('/home/pi/Documents/ELSpring2016/code/tempSensorTests/myTempTime.db')
###con = mydb.connect('myTempTime.db')
c = con.cursor()
try:
	c.execute('''CREATE TABLE tempLog (tempDate TEXT, tempTime TEXT, tempC INT, tempF INT);''')
except mydb.OperationalError:
	print "Database file already exists, table exists"
currTemp = readTemp()
newRow = [currTemp[0],currTemp[1],currTemp[2],currTemp[3]]
c.execute('INSERT INTO tempLog VALUES (?,?,?,?)',newRow)

con.commit()
con.close()

###print readTemp() 
