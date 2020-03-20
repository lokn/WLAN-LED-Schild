# Python
import urllib.request
import time

host = "http://IP-Adresse/";

import random
r = lambda: random.randint(0,255)
g = lambda: random.randint(0,255)
b = lambda: random.randint(0,255)

secs = 0.000
LED_NUM = 22

#for i in range(22):
#	html_quelltext = urllib.request.urlopen(host+'?'+'l'+ str(i) + '=%23' + 'FFFFFF')
#	time.sleep(1)

all_leds = [0,1,2,3,4,5,6,7,13,12,11,10,9,8,14,15,16,17,19,18,20,21]

#row_1 = [0,1,2,3,4,5,6,7]
#row_2 = [13,12,11,10,9,8]
#row_3 = [14,15,16,17]
#row_4 = [19,18,20,21]

row = [[0,1,2,3,4,5,6,7],[13,12,11,10,9,8],[14,15,16,17],[19,18,20,21]]


def set_led(paramsstr, number, color):
	return paramsstr + "l" + str(number) + "=%23" + color + "&"
	
	
"""
for j in range(20):
	color = '%02X%02X%02X' % (r(),g(),b())
	#color = '%02X%02X%02X' % (j*10,j*10,j*10)

	for i in range(len(all_leds)):
		color = '%02X%02X%02X' % (r(),g(),b())
		print(i)
		urllib.request.urlopen(host+'?'+'l'+ str(all_leds[i]) + '=%23' + color)
		time.sleep(secs)
"""

for j in range(80):	
	color = '%02X%02X%02X' % (r(),g(),b())
	i = random.randint(0,21)
	print(i)
	urllib.request.urlopen(host+'?'+'l'+ str(all_leds[i]) + '=%23' + color)
	time.sleep(secs)

def set_leds(led_color, led_list):
	paramsstr = ""
	color = led_color
	for num_led in range(len(led_list)):
		paramsstr = set_led(paramsstr, led_list[num_led], color)
	urllib.request.urlopen(host+'?'+paramsstr)

for blink in range(10):
	freq = 1
	
	set_leds('FF0000', all_leds)
	time.sleep(freq)
	
	paramsstr = ""
	color='000000'
	#color = '%02X%02X%02X' % (r(),g(),b())
	print(color)
	for led_num in range(LED_NUM):
		paramsstr = set_led(paramsstr, led_num, color)
	urllib.request.urlopen(host+'?'+paramsstr)
	time.sleep(freq)


freq=0.5
color="DF013A"
set_leds(color, row[3])
time.sleep(freq)
set_leds(color, row[2])
time.sleep(freq)
set_leds(color, row[1])
time.sleep(freq)
set_leds(color, row[0])
time.sleep(freq)
set_leds('000000', all_leds)

def strength (color, percent):
	verteilung=[20,50,75,90]
	
	#color = '%02X%02X%02X' % (r(),g(),b())
	
	for i in range(4):
		if (percent >= verteilung[i] ):
			set_leds(color, row[3-i])

for j in range(11):
	strength(color, j*10)
	print(j*10)
	time.sleep(1)

set_leds('DF013A', all_leds)
set_leds('DF013A', row[0])
set_leds('DF013A', row[2])
set_leds('DF013A', row[1])
set_leds('FFBF00', row[3]) #gelb

