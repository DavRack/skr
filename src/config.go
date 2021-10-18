package main

func initConfig() (keyboard Keyboard) {
	keyboard.useConfigFile = false
	keyboard.name = "ASUSTeK Computer Inc. N-KEY Device"
	return
}

func skrConfig(keyboard *Keyboard) (blockCurrentKey bool) {
	baseLayer := keyboard.createLayer()
	numbersLayer := keyboard.createLayer(58)
	keyboard.blockLayerKeys()

	if keyboard.isActiveLayer(numbersLayer) {
		keyboard.keyRemap(35, 105)
		keyboard.keyRemap(36, 108)
		keyboard.keyRemap(37, 103)
		keyboard.keyRemap(38, 106)
	}
	if keyboard.isActiveLayer(baseLayer) {
		keyboard.keyRemap(15, 1)
	}

	return
}
