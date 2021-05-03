package main

import (
	"encoding/binary"
	"fmt"
	"log"
	"skr/logg"
)

func main() {
	log.SetFlags(log.Ltime | log.Lmicroseconds | log.Lshortfile)
	logs := logg.New(5)

	keyboardName := "AT Translated Set 2 keyboard"
	keyboardPath := get_keyboard_path_from_name(keyboardName)
	keyboardIn := keyboardReader(keyboardPath)

	layers := parse()

	fmt.Println("skr")
	logs.Print(1, "Keyboard path", keyboardPath)

	for keyboard_exist(keyboardPath) {
		// read rawevent from keyboard
		binary.Read(keyboardIn, binary.LittleEndian, &rawInput)

		if rawInput.Type == keyEvent {
			// if a key is pressed we ned to added to pressedKeys to perform
			// the needed action, but if a key is released we also need to
			// perform the key action, so we need to remove the key from
			// pressedKeys after we perform such action
			if rawInput.Value == keyPressed {
				pressedKeys = get_press_keys(rawInput, pressedKeys)
			}

			decided_action = decide_actions(pressedKeys, layers)
			rawKeys, rawScripts := get_raw_events(decided_action, rawInput)

			pressedKeys = get_press_keys(rawInput, pressedKeys)
			logs.Print(5, pressedKeys)

			execute_raw_keys(rawKeys)
			execute_raw_scripts(rawScripts)
		}
	}
}
