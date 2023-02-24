package keyboard

func sendCurrentKeyAction(kb Keyboard) {
	currentKey := kb.LastKey
	kb.Execute(currentKey)
}
