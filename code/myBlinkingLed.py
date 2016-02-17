import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(17,GPIO.OUT)

def Blink(blinks):
  for i in range(0,blinks):
	GPIO.output(17,True)
	time.sleep(.25)
	GPIO.output(17,False)
	time.sleep(.25)

while 1:
  Blink(3)
  time.sleep(5)
  Blink(4)
  time.sleep(5)	
	
