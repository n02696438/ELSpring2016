import RPi.GPIO as GPIO
import time

# set mode of GPIO and set up pin 17 as gpio output
GPIO.setmode(GPIO.BCM)
GPIO.setup(17,GPIO.OUT)

#blinks pin 17 led rapidly argument "blinks" number of times
def Blink(blinks):
  for i in range(0,blinks):
	GPIO.output(17,True)
	time.sleep(.25)
	GPIO.output(17,False)
	time.sleep(.25)

#blinks LED 3 times, sleeps for 5 seconds then 4 times. Repeats.
while 1:
  Blink(3)
  time.sleep(5)
  Blink(4)
  time.sleep(5)	
	
