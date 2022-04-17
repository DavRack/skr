package main

import (
	"skr/keyboard"
)

func InitConfig() keyboard.Keyboard {
	var keyboard keyboard.Keyboard
	keyboard.UseConfigFile = false
	keyboard.Name = "Asus Keyboard"
	return keyboard
}

var spaceExecuted bool = false

func Config(keyboard *keyboard.Keyboard) (blockCurrentKey bool) {
	return
}

// func skrConfig(keyboard *keyboard.Keyboard) (blockCurrentKey bool) {
// 	baseLayer := keyboard.newLayer("base")
// 	numbersLayer := keyboard.newLayer("numbersLayer", 58)
// 	spaceLayer := keyboard.newLayer("spaceLayer", "space")
// 	keyboard.blockLayerKeys()

// 	if keyboard.isActiveLayer(numbersLayer) {
// 		blockCurrentKey = true

// 		keyboard.keyRemap("U", "7")
// 		keyboard.keyRemap("I", "8")
// 		keyboard.keyRemap("O", "9")
// 		keyboard.keyRemap("J", "4")
// 		keyboard.keyRemap("K", "5")
// 		keyboard.keyRemap("L", "6")
// 		keyboard.keyRemap("M", "1")
// 		keyboard.keyRemap("COMMA", "2")
// 		keyboard.keyRemap("DOT", "3")
// 		keyboard.keyRemap("space", "0")

// 		keyboard.keyRemap("n", "DOT")
// 		keyboard.keyRemap("p", "KPMINUS")
// 		keyboard.keyRemap("SEMICOLON", "KPPLUS")
// 	}

// 	if keyboard.isActiveLayer(spaceLayer) {
// 		if keyboard.lastKey.is("space") && keyboard.lastKey.keyState != keyReleased {
// 			spaceExecuted = false
// 			return
// 		}

// 		keyboard.keyRemap("CAPSLOCK", "TAB")

// 		keyboard.keyRemap("H", "LEFT")
// 		keyboard.keyRemap("J", "DOWN")
// 		keyboard.keyRemap("K", "UP")
// 		keyboard.keyRemap("L", "RIGHT")

// 		if keyboard.lastKey.is("space") && keyboard.lastKey.keyState == keyReleased {
// 			if spaceExecuted == false {
// 				keyboard.pressKey("space")
// 				time.Sleep(5 * time.Millisecond)
// 				keyboard.releaseKey("space")
// 			}
// 		}

// 		if keyboard.executeDefaulAction == false {
// 			spaceExecuted = true
// 		}
// 	}
// 	if keyboard.isActiveLayer(baseLayer) {
// 		keyboard.keyRemap(15, 1)
// 		keyboard.keyRemap(56, 125)
// 		keyboard.keyRemap(125, 56)
// 	}

// 	return
// }
