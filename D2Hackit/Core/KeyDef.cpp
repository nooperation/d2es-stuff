#include "KeyDef.h"
#include <string.h>
#include <stdio.h>

DWORD D2GetKeyName(BYTE iVKCode, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = '\0';
	if (dwMaxChars == 0 || iVKCode > 0x80)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);

	switch (iVKCode)
	{
	case 0x08:

		strncpy(lpszBuffer, "BACK SPACE", dwMaxChars);
		break;
		
	case 0x09:
	
		strncpy(lpszBuffer, "TAB", dwMaxChars);
		break;

	case 0x0c:
	
		strncpy(lpszBuffer, "NUMPAD 5", dwMaxChars);
		break;
		
	case 0x0d:
	
		strncpy(lpszBuffer, "RETURN", dwMaxChars);
		break;

	case 0x10:
	
		strncpy(lpszBuffer, "SHIFT", dwMaxChars);
		break;

	case 0x11:
	
		strncpy(lpszBuffer, "CTRL", dwMaxChars);
		break;

	case 0x12:
	
		strncpy(lpszBuffer, "ALT", dwMaxChars);
		break;

	case 0x13:
	
		strncpy(lpszBuffer, "PAUSE", dwMaxChars);
		break;

	case 0x14:
	
		strncpy(lpszBuffer, "CAPS LOCK", dwMaxChars);
		break;

	case 0x1b:
	
		strncpy(lpszBuffer, "ESC", dwMaxChars);
		break;

	case 0x20:
	
		strncpy(lpszBuffer, "SPACE", dwMaxChars);
		break;

	case 0x21:
	
		strncpy(lpszBuffer, "PAGE UP", dwMaxChars);
		break;
		
	case 0x22:
	
		strncpy(lpszBuffer, "PAGE DOWN", dwMaxChars);
		break;

	case 0x23:
	
		strncpy(lpszBuffer, "END", dwMaxChars);
		break;
	case 0x24:
	
		strncpy(lpszBuffer, "HOME", dwMaxChars);
		break;

	case 0x25:
	
		strncpy(lpszBuffer, "LEFT", dwMaxChars);
		break;

	case 0x26:
	
		strncpy(lpszBuffer, "UP", dwMaxChars);
		break;

	case 0x27:
	
		strncpy(lpszBuffer, "RIGHT", dwMaxChars);
		break;

	case 0x28:
	
		strncpy(lpszBuffer, "DOWN", dwMaxChars);
		break;

	case 0x2d:
	
		strncpy(lpszBuffer, "INSERT", dwMaxChars);
		break;
		
	case 0x2e:
	
		strncpy(lpszBuffer, "DELETE", dwMaxChars);
		break;
		
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	
		sprintf(lpszBuffer, "%d", iVKCode - 0x30);
		break;

	case 0x41: // a
	case 0x42: // b
	case 0x43: // c
	case 0x44: // d
	case 0x45: // e
	case 0x46: // f
	case 0x47: // g
	case 0x48: // h
	case 0x49: // i
	case 0x4a: // j
	case 0x4b: // k
	case 0x4c: // l
	case 0x4d: // m
	case 0x4e: // n
	case 0x4f: // o
	case 0x50: // p
	case 0x51: // q
	case 0x52: // r
	case 0x53: // s
	case 0x54: // t
	case 0x55: // u
	case 0x56: // v
	case 0x57: // w
	case 0x58: // x
	case 0x59: // y
	case 0x5a: // z
	
		sprintf(lpszBuffer, "%C", iVKCode - 0x41 + 'A');
		break;

	case 0x60:
	case 0x61:
	case 0x62:
	case 0x63:
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	
		sprintf(lpszBuffer, "NUMPAD %d", iVKCode - 0x60);
		break;

	case 0x6a:
	
		strncpy(lpszBuffer, "NUMPAD *", dwMaxChars);
		break;

	case 0x6b:
	
		strncpy(lpszBuffer, "NUMPAD +", dwMaxChars);
		break;
	case 0x6d:
	
		strncpy(lpszBuffer, "NUMPAD -", dwMaxChars);
		break;

	case 0x6e:
	
		strncpy(lpszBuffer, "NUMPAD .", dwMaxChars);
		break;

	case 0x6f:
	
		strncpy(lpszBuffer, "NUMPAD /", dwMaxChars);
		break;

	case VK_F1:
	case VK_F2:
	case VK_F3:
	case VK_F4:
	case VK_F5:
	case VK_F6:
	case VK_F7:
	case VK_F8:
	case VK_F9:
	case VK_F10:
	case VK_F11:
	case VK_F12:
	
		sprintf(lpszBuffer, "F%d", iVKCode - VK_F1 + 1);
		break;

	case 0x90:
	
		strncpy(lpszBuffer, "NUM LOCK", dwMaxChars);
		break;
	case 0x91:
	
		strncpy(lpszBuffer, "SCROLL LOCK", dwMaxChars);
		break;

	case 0xba:
	
		strncpy(lpszBuffer, ";", dwMaxChars);
		break;

	case 0xbb:
	
		strncpy(lpszBuffer, "=", dwMaxChars);
		break;

	case 0xbc:
	
		strncpy(lpszBuffer, ",", dwMaxChars);
		break;

	case 0xbd:
	
		strncpy(lpszBuffer, "-", dwMaxChars);
		break;
	case 0xbe:
	
		strncpy(lpszBuffer, ".", dwMaxChars);
		break;

	case 0xbf:
	
		strncpy(lpszBuffer, "/", dwMaxChars);
		break;
	case 0xc0:
	
		strncpy(lpszBuffer, "`", dwMaxChars);
		break;

	case 0xdb:
	
		strncpy(lpszBuffer, "[", dwMaxChars);
		break;
	case 0xdc:
	
		strncpy(lpszBuffer, "\\", dwMaxChars);
		break;

	case 0xdd:
	
		strncpy(lpszBuffer, "]", dwMaxChars);
		break;
	case 0xde:
	
		strncpy(lpszBuffer, "'", dwMaxChars);
		break;

	default:
		return 0;
		break;	
	}

	return strlen(lpszBuffer);
}