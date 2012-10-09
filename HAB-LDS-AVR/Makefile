CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atxmega32a4
OBJ2HEX=avr-objcopy
#UISP=/usr/local/bin/uisp
TARGET=light_direction_sensor
ADFLAGS=-p m8 -c dragon_pdi -P usb
# Added this
OBJS=light_direction_sensor.o lcd.o sd.o adc.o bluetooth.o

.PHONY: fuses prog erase clean

prog : ${TARGET}.hex ${TARGET}.eeprom
	avrdude ${ADFLAGS} -V -U flash:w:${TARGET}.hex:i
#	avrdude ${ADFLAGS} -U eeprom:w:${TARGET}.eeprom:i

${TARGET}.obj : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${TARGET}.obj

#${TARGET} : ${OBJS}
	#${CC} -o ${TARGET} ${CFLAGS} ${OBJS}

%.hex : %.obj
	${OBJ2HEX} -R .eeprom -O ihex $< $@

%.eeprom : %.obj
	${OBJ2HEX} -j .eeprom -O ihex $< $@

light_direction_sensor.o : light_direction_sensor.h
lcd.o : lcd.h
adc.o : adc.h
sd.o : sd.h
bluetooth.o : bluetooth.h

erase :
	avrdude ${ADFLAGS} -E noreset -e
clean :
	rm -f *.hex *.obj *.o *eeprom

fuses:
	avrdude ${ADFLAGS} -U lfuse:w:0xe1:m #http://www.engbedded.com/cgi-bin/fc.cgi 
	avrdude ${ADFLAGS} -U hfuse:w:0xd9:m
