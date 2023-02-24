package keyboard

/*
Each triggerCondition is a cloujure wich returns a function that
*/

func KeyPressedCondition(keyCode KeyCode) TriggerCondition {
	return func(activeKey KeyEvent) bool {
		if activeKey.KeyCode == keyCode && activeKey.KeyState == KeyPressed {
			return true
		} else {
			return false
		}
	}
}

func KeyActiveCondition(keyCode KeyCode) TriggerCondition {
	return func(activeKey KeyEvent) bool {
		if activeKey.KeyCode == keyCode {
			return true
		} else {
			return false
		}
	}
}
