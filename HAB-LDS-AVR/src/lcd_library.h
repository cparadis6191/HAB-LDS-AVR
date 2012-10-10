#ifndef __LCD_LIBRARY_H_
#define __LCD_LIBRARY_H_

// Initialization Sequences
#define UART_READY 0xA3	// Received from the LCD after Power_ON to indicate that UART is ready for commands
#define SLCD_INIT_ACK 0xA5	// Send this to initialize the LCD. Initialization takes at least 150ms
#define SLCD_INIT_DONE 0xAA	// Wait for this response from SLCD_INIT_ACK
#define SLCD_CONTROL_HEADER 0x9F	// Send this before sending any control commands

// Working Commands - Header
#define SLCD_CONTROL_HEADER 0x9F
#define SLCD_CHAR_HEADER 0xFE	// Send this before sending any characters
#define SLCD_CURSOR_HEADER 0xFF
#define SLCD_CURSOR_ACK 0x5A

// Power and Display Commands
#define SLCD_BACKLIGHT_OFF 0x80
#define SLCD_BACKLIGHT_ON 0x81
#define SLCD_POWER_OFF 0x82
#define SLCD_POWER_ON 0x83
#define SLCD_DISPLAY_OFF 0x63	// Turns off the display, screen data is still stored in memory
#define SLCD_DISPLAY_ON 0x64	// Turns on the display and restores the text
#define SLCD_CLEAR_DISPLAY 0x65	// Clear the display, also returns home

// Cursor/Movement Commands
#define SLCD_RETURN_HOME 0x61	// Return the cursor to the top-left corner
#define SLCD_SCROLL_LEFT 0x6C	// Scroll the contents left by one character
#define SLCD_SCROLL_RIGHT 0x72	// Scroll the contents left by one character

#define SLCD_CURSOR_OFF 0x66
#define SLCD_CURSOR_ON 0x67	// Set cursor to an underscore

#define SLCD_BLINK_OFF 0x68
#define SLCD_BLINK_ON 0x69	// Set cursor to a blinking block

// Misc Commands
#define SLCD_NO_AUTO_SCROLL 0x6A
#define SLCD_AUTO_SCROLL 0x6D
#define SLCD_LEFT_TO_RIGHT 0x70
#define SLCD_RIGHT_TO_LEFT 0x71

// Error
#define SLCD_INVALIDCOMMAND 0x46

#endif