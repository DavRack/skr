package keyboard

import (
	"os"
	"os/exec"
)

// Exist godoc
func Exist(keyboardPath string) bool {
	_, err := os.Stat(keyboardPath)
	return err == nil
}

// PathFromName godoc
func PathFromName(name string) string {
	command := "cat /proc/bus/input/devices | awk '$1==\"N:\" || $1==\"H:\" {print $0}' | grep \"" + name + "\" -A1 | tail -1 | tr \" \" \"\n\" | grep event"

	out, _ := exec.Command("bash", "-c", command).Output()
	event := string(out)[:len(out)-1]

	return "/dev/input/" + event

}

// KeyIsPress godoc
func (kb State) KeyIsPress() bool {
	return kb.KeyState() == Press
}

// IsKeyEvent godoc
func (rawInput InputEvent) IsKeyEvent() bool {
	return rawInput.Type == KeyEvent
}

// KeyState godoc
func (kb State) KeyState() int32 {
	return kb.RawInput.Value
}

// KeyCode godoc
func (kb State) KeyCode() Key {
	return Key(kb.RawInput.Code)
}

// DeleteKeyAt godoc
func (pressKeys Keys) DeleteKeyAt(index int) Keys {
	if index >= 0 && index < len(pressKeys) {
		return append(pressKeys[:index], pressKeys[index+1:]...)
	}
	return pressKeys
}

// DeleteKey godoc
func (pressKeys Keys) DeleteKey(key Key) Keys {
	for i, pressKey := range pressKeys {
		if pressKey == key {
			return pressKeys.DeleteKeyAt(i)
		}
	}
	return pressKeys
}

// GetPressKeys godoc
func (kb State) GetPressKeys() Keys {
	pressKeys := kb.PressKeys
	if kb.KeyState() == Press && !kb.PressKeys.Contains(kb.KeyCode()) {
		return append(pressKeys, kb.KeyCode())
	}
	if kb.KeyState() == Released {
		return kb.PressKeys.DeleteKey(kb.KeyCode())

	}
	return pressKeys
}

// Contains godoc
func (pressKeys Keys) Contains(key Key) bool {
	for _, pressKey := range pressKeys {
		if pressKey == key {
			return true
		}
	}
	return false
}
