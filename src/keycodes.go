package main

import (
	"strings"
)

func interfaceToKeyCode(key interface{}) (keyCode KeyCode, ok bool) {
	ok = true
	switch key.(type) {
	case string:
		keyCode = KeyName(key.(string)).keyCode()
		return
	case int:
		keyCode = KeyCode(key.(int))
		return
	default:
		ok = false
		return
	}
}

func (key KeyName) keyCode() KeyCode {
	keyName := strings.ToUpper(string(key))
	keyCode := keyCodesAlias[keyName]
	if keyCode != 0 {
		return keyCode
	}
	keyCode = systemKeyCodes[keyName]
	return keyCode
}

func (key KeyCode) keyName() KeyName {
	keyName, ok := mapkey(keyCodesAlias, key)
	if ok {
		return KeyName(keyName)
	}
	keyName, ok = mapkey(systemKeyCodes, key)
	if ok {
		return KeyName(keyName)
	}
	return ""
}

func mapkey(m map[string]KeyCode, value KeyCode) (key string, ok bool) {
	for k, v := range m {
		if v == value {
			key = k
			ok = true
			return
		}
	}
	return
}

var keyCodesAlias = map[string]KeyCode{
	"SHIFT": 42,
	"CTRL":  29,
	"ALT":   56,
	"META":  125,
	"SUPER": 125,
}
var systemKeyCodes = map[string]KeyCode{
	"RESERVED":         0,
	"ESC":              1,
	"1":                2,
	"2":                3,
	"3":                4,
	"4":                5,
	"5":                6,
	"6":                7,
	"7":                8,
	"8":                9,
	"9":                10,
	"0":                11,
	"MINUS":            12,
	"EQUAL":            13,
	"BACKSPACE":        14,
	"TAB":              15,
	"Q":                16,
	"W":                17,
	"E":                18,
	"R":                19,
	"T":                20,
	"Y":                21,
	"U":                22,
	"I":                23,
	"O":                24,
	"P":                25,
	"LEFTBRACE":        26,
	"RIGHTBRACE":       27,
	"ENTER":            28,
	"LEFTCTRL":         29,
	"A":                30,
	"S":                31,
	"D":                32,
	"F":                33,
	"G":                34,
	"H":                35,
	"J":                36,
	"K":                37,
	"L":                38,
	"SEMICOLON":        39,
	"APOSTROPHE":       40,
	"GRAVE":            41,
	"LEFTSHIFT":        42,
	"BACKSLASH":        43,
	"Z":                44,
	"X":                45,
	"C":                46,
	"V":                47,
	"B":                48,
	"N":                49,
	"M":                50,
	"COMMA":            51,
	"DOT":              52,
	"SLASH":            53,
	"RIGHTSHIFT":       54,
	"KPASTERISK":       55,
	"LEFTALT":          56,
	"SPACE":            57,
	"CAPSLOCK":         58,
	"F1":               59,
	"F2":               60,
	"F3":               61,
	"F4":               62,
	"F5":               63,
	"F6":               64,
	"F7":               65,
	"F8":               66,
	"F9":               67,
	"F10":              68,
	"NUMLOCK":          69,
	"SCROLLLOCK":       70,
	"KP7":              71,
	"KP8":              72,
	"KP9":              73,
	"KPMINUS":          74,
	"KP4":              75,
	"KP5":              76,
	"KP6":              77,
	"KPPLUS":           78,
	"KP1":              79,
	"KP2":              80,
	"KP3":              81,
	"KP0":              82,
	"KPDOT":            83,
	"ZENKAKUHANKAKU":   85,
	"102ND":            86,
	"F11":              87,
	"F12":              88,
	"RO":               89,
	"KATAKANA":         90,
	"HIRAGANA":         91,
	"HENKAN":           92,
	"KATAKANAHIRAGANA": 93,
	"MUHENKAN":         94,
	"KPJPCOMMA":        95,
	"KPENTER":          96,
	"RIGHTCTRL":        97,
	"KPSLASH":          98,
	"SYSRQ":            99,
	"RIGHTALT":         100,
	"LINEFEED":         101,
	"HOME":             102,
	"UP":               103,
	"PAGEUP":           104,
	"LEFT":             105,
	"RIGHT":            106,
	"END":              107,
	"DOWN":             108,
	"PAGEDOWN":         109,
	"INSERT":           110,
	"DELETE":           111,
	"MACRO":            112,
	"MUTE":             113,
	"VOLUMEDOWN":       114,
	"VOLUMEUP":         115,
	"POWER":            116,
	"KPEQUAL":          117,
	"KPPLUSMINUS":      118,
	"PAUSE":            119,
	"SCALE":            120,
	"KPCOMMA":          121,
	"HANGEUL":          122,
	"HANGUEL":          122,
	"HANJA":            123,
	"YEN":              124,
	"LEFTMETA":         125,
	"RIGHTMETA":        126,
	"COMPOSE":          127,
	"STOP":             128,
	"AGAIN":            129,
	"PROPS":            130,
	"UNDO":             131,
	"FRONT":            132,
	"COPY":             133,
	"OPEN":             134,
	"PASTE":            135,
	"FIND":             136,
	"CUT":              137,
	"HELP":             138,
	"MENU":             139,
	"CALC":             140,
	"SETUP":            141,
	"SLEEP":            142,
	"WAKEUP":           143,
	"FILE":             144,
	"SENDFILE":         145,
	"DELETEFILE":       146,
	"XFER":             147,
	"PROG1":            148,
	"PROG2":            149,
	"WWW":              150,
	"MSDOS":            151,
	"COFFEE":           152,
	"SCREENLOCK":       152,
	"ROTATE_DISPLAY":   153,
	"DIRECTION":        153,
	"CYCLEWINDOWS":     154,
	"MAIL":             155,
	"BOOKMARKS":        156,
	"COMPUTER":         157,
	"BACK":             158,
	"FORWARD":          159,
	"CLOSECD":          160,
	"EJECTCD":          161,
	"EJECTCLOSECD":     162,
	"NEXTSONG":         163,
	"PLAYPAUSE":        164,
	"PREVIOUSSONG":     165,
	"STOPCD":           166,
	"RECORD":           167,
	"REWIND":           168,
	"PHONE":            169,
	"ISO":              170,
	"CONFIG":           171,
	"HOMEPAGE":         172,
	"REFRESH":          173,
	"EXIT":             174,
	"MOVE":             175,
	"EDIT":             176,
	"SCROLLUP":         177,
	"SCROLLDOWN":       178,
	"KPLEFTPAREN":      179,
	"KPRIGHTPAREN":     180,
	"NEW":              181,
	"REDO":             182,
	"F13":              183,
	"F14":              184,
	"F15":              185,
	"F16":              186,
	"F17":              187,
	"F18":              188,
	"F19":              189,
	"F20":              190,
	"F21":              191,
	"F22":              192,
	"F23":              193,
	"F24":              194,
	"PLAYCD":           200,
	"PAUSECD":          201,
	"PROG3":            202,
	"PROG4":            203,
	"DASHBOARD":        204,
	"SUSPEND":          205,
	"CLOSE":            206,
	"PLAY":             207,
	"FASTFORWARD":      208,
	"BASSBOOST":        209,
	"PRINT":            210,
	"HP":               211,
	"CAMERA":           212,
	"SOUND":            213,
	"QUESTION":         214,
	"EMAIL":            215,
	"CHAT":             216,
	"SEARCH":           217,
	"CONNECT":          218,
	"FINANCE":          219,
	"SPORT":            220,
	"SHOP":             221,
	"ALTERASE":         222,
	"CANCEL":           223,
	"BRIGHTNESSDOWN":   224,
	"BRIGHTNESSUP":     225,
	"MEDIA":            226,
	"SWITCHVIDEOMODE":  227,
	"KBDILLUMTOGGLE":   228,
	"KBDILLUMDOWN":     229,
	"KBDILLUMUP":       230,
	"SEND":             231,
	"REPLY":            232,
	"FORWARDMAIL":      233,
	"SAVE":             234,
	"DOCUMENTS":        235,
	"BATTERY":          236,
	"BLUETOOTH":        237,
	"WLAN":             238,
	"UWB":              239,
	"UNKNOWN":          240,
	"VIDEO_NEXT":       241,
	"VIDEO_PREV":       242,
	"BRIGHTNESS_CYCLE": 243,
	"BRIGHTNESS_AUTO":  244,
	"BRIGHTNESS_ZERO":  244,
	"DISPLAY_OFF":      245,
	"WWAN":             246,
	"WIMAX":            246,
	"RFKILL":           247,
	"MICMUTE":          248,
	"MIN_INTERESTING":  113,
}

func combine(a map[string]KeyCode, b map[string]KeyCode) map[string]KeyCode {
	for k, v := range b {
		a[k] = v
	}
	return a
}

var keyCodes = combine(systemKeyCodes, keyCodesAlias)
