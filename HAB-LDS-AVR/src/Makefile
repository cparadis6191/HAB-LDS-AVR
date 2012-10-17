CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atxmega32a4u
OBJ2HEX=avr-objcopy
#UISP=/usr/local/bin/uisp
TARGET=HAB-LDS-AVR
ADFLAGS=-p x32a4 -c dragon_pdi -P usb
# Added this
OBJS=HAB-LDS-AVR.o lcd.o sd.o adc.o pc_interface.o

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

HAB-LDS-AVR.o : HAB-LDS-AVR.h
lcd.o : lcd.h
lcd.o : lcd_library.h
adc.o : adc.h
sd.o : sd.h
pc_interface.o : pc_interface.h

erase :
	avrdude ${ADFLAGS} -E noreset -e
clean :
	rm -f *.hex *.obj *.o *eeprom
