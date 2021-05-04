package main

import (
	"encoding/binary"
	"fmt"
	"log"
	"skr/keyboard"
	"skr/logg"
)

func main() {
	log.SetFlags(log.Ltime | log.Lmicroseconds | log.Lshortfile)
	logs := logg.New(5)

	keyboardName := "AT Translated Set 2 keyboard"
	keyboardPath := keyboard.PathFromName(keyboardName)
	keyboardIn := keyboard.EventReader(keyboardPath)

	kb := keyboard.State{}

	fmt.Println("skr")
	logs.Print(1, "Keyboard path", keyboardPath)

	for keyboard.Exist(keyboardPath) {
		// read rawevent from keyboard
		binary.Read(keyboardIn, binary.LittleEndian, &kb.RawInput)

		if kb.RawInput.IsKeyEvent() {
			// if a key is pressed we ned to added to pressedKeys to perform
			// the needed action, but if a key is released we also need to
			// perform the key action, so we need to remove the key from
			// pressedKeys after we perform such action
			if kb.KeyIsPress() {
				kb.PressKeys = kb.GetPressKeys()
			}

			kb.PressKeys = kb.GetPressKeys()
			logs.Print(5, kb.PressKeys)

		}
	}
}
