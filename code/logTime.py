import time
import sqlite3 as mydb
import sys

def readTime():
  xdate = time.strftime("%Y-%m-%d")
  xtime = time.strftime("%H-%M-%S")
  return [xdate,xtime]

 
con = mydb.connect('testTime.db')
c = con.cursor()
currDateTime = readTime()
newRow = [currDateTime[0],currDateTime[1]]
c.execute('INSERT INTO dateTime VALUES (?,?)',newRow)

con.commit()
con.close()
