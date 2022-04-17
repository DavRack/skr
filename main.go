package main

import (
	"skr/keyboard"
)

func main() {

	user_keyboard := InitConfig()
	user_keyboard.IO = keyboard.InitStdinIO(user_keyboard.Name)

	if user_keyboard.UseConfigFile {
	} else {
		loop(&user_keyboard, Config)
	}
}

func loop(kb *keyboard.Keyboard, skrConfig func(*keyboard.Keyboard) bool) {
	for kb.Exist() {
		// read event from keyboard
		raw_input, err := kb.IO.Read()

		if err != nil {
			break
		}

		if raw_input.Type == keyboard.KeyEventType {

			keyEvent := keyboard.KeyEvent{
				Time:     raw_input.Time,
				KeyCode:  keyboard.KeyCode(raw_input.Code),
				KeyState: keyboard.KeyState(raw_input.Value),
			}

			// if a key is pressed we ned to added to pressedKeys to perform
			// the needed action, but if a key is released we also need to
			// perform the key action, so we need to remove the key from
			// pressedKeys after we perform such action
			if raw_input.Value == keyboard.KeyPressed {
				kb.PressedKeys = kb.GetPressKeys(keyEvent)
			}

			// reset kb state for each new kb event
			kb.SomeActionExecuted = false
			kb.ExecuteDefaulAction = true
			kb.LastKey = keyEvent

			skrConfig(kb)

			kb.PressedKeys = kb.GetPressKeys(keyEvent)
		}
	}
}
