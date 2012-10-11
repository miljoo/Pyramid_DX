#ifndef _KEYMAP
#define _KEYMAP

#define KEYDOWN( name, key ) ( name[key] & 0x80 )
#define BUTTONDOWN(device, key) ( device.rgbButtons[key] & 0x80 )

#endif