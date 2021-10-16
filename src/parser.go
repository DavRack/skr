package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os/exec"
	"strings"
)

func parse() []layer {
	var layers []layer

	var arrowLayer = layer{
		layer_key: 0,
		onTap:     action{},
		actions: []action{
			{
				script_action: script{},
				remap_action: remap{
					trigger:      []uint16{30},
					action:       []uint16{29},
					blockKeys:    true,
					keyEmulation: true,
				},
			},
		},
	}
	layers = append(layers, arrowLayer)
	return layers
}

func get_keyboard_path_from_name(name string) (error, string) {

	command := "sudo sed -n '/" + name + "/,/Handlers/p' /proc/bus/input/devices | grep -o 'event.*' -m 1"

	out, _ := exec.Command("bash", "-c", command).Output()
	if len(out) == 0 {
		return errors.New("Keyboard not found"), ""
	}
	input_event := strings.TrimSpace(string(out))

	return nil, "/dev/input/" + input_event

}

func pressKey(keyCode KeyCode) {
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: pressed,
	}
	keyEvent.execute()
}

func releaseKey(keyCode KeyCode) {
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: released,
	}
	keyEvent.execute()
}
func (keyEvent KeyEvent) remapCode(fromKey KeyCode, toKey KeyCode) (executed bool) {
	if keyEvent.keyCode == fromKey {
		remapedKey := keyEvent
		remapedKey.keyCode = toKey
		remapedKey.execute()
		return true
	}
	return false
}

func (keyEvent KeyEvent) is(keyName string) bool {
	keyCode := keyCodes[strings.ToUpper(keyName)]
	return keyEvent.keyCode == keyCode
}

func (keyEvent KeyEvent) execute() {
	state := ""
	if keyEvent.keyState == pressed {
		state = "keyPressed"
	}
	if keyEvent.keyState == released {
		state = "keyReleased"
	}
	if keyEvent.keyState == held {
		state = "keyHeld"
	}

	fmt.Println("KeyCode:", keyEvent.keyCode, "Event type:", state)
}

func remap_as_script(currentKey KeyEvent, pressedKeys []uint16) (blockCurrentKey bool) {
	if currentKey.remapCode(30, 24) {
		blockCurrentKey = true
		return
	}
	return
}

func loop(keyboard Keyboard) {
	var raw_input InputEvent
	for keyboard_exist(keyboard.path) {
		// read event from keyboard
		binary.Read(keyboard.ioReader, binary.LittleEndian, &raw_input)

		if raw_input.Type == key_event {

			keyEvent := KeyEvent{
				time:     raw_input.Time,
				keyCode:  KeyCode(raw_input.Code),
				keyState: KeyState(raw_input.Value),
			}

			// if a key is pressed we ned to added to pressedKeys to perform
			// the needed action, but if a key is released we also need to
			// perform the key action, so we need to remove the key from
			// pressedKeys after we perform such action
			if raw_input.Value == keyPressed {
				keyboard.pressedKeys = keyboard.get_press_keys(keyEvent)
			}

			blockCurrentKey := remap_as_script(keyEvent, pressedKeys)

			if !blockCurrentKey {
				keyEvent.execute()
			}

			keyboard.pressedKeys = keyboard.get_press_keys(keyEvent)
		}
	}
}

var keyCodes = map[string]KeyCode{
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
	"SHIFT":            42,
	"CTRL":             29,
	"ALT":              56,
	"META":             125,
	"SUPER":            125,
}
