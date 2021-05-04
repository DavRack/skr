package config

import "skr/data"

var config = data.SkrConfig{
	KeyboardName: "AT Translated Set 2 keyboard",
	LogLevel:     1,
}

func script(rawInput data.InputEvent, pressedKeys []uint16) {
	// all your behaviour goes here
}
