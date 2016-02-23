import sqlite3 as mydb
import sys


#Connects to database testTime.db, creates it if not existing
con = mydb.connect('testTime.db')
c = con.cursor()

#creates table inside database for use
c.execute('''CREATE TABLE dateTime (xdate TEXT, xtime TEXT);''')
con.commit()
