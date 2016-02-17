import sqlite3 as mydb
import sys



con = mydb.connect('testTime.db')
c = con.cursor()

c.execute('''CREATE TABLE dateTime (xdate TEXT, xtime TEXT);''')
con.commit()
