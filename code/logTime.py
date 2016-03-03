import time
import sqlite3 as mydb
import sys

#returns current date and time as list
def readTime():
  xdate = time.strftime("%Y-%m-%d")
  xtime = time.strftime("%H-%M-%S")
  return [xdate,xtime]


#Connects to database testTime.db (created with createDB.py)
con = mydb.connect('testTime.db')
c = con.cursor()
#reads current date/time
currDateTime = readTime()
#converts currDateTime to array for insertion
newRow = [currDateTime[0],currDateTime[1]]
#inserts newRow into database testTime.db
c.execute('INSERT INTO dateTime VALUES (?,?)',newRow)

#commits and closes database
con.commit()
con.close()
