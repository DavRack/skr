package main

import (
	"fmt"
	"syscall"
	"testing"

	"github.com/stretchr/testify/assert"
)

func mock_keyEvent(code int, state KeyState) KeyboardEvent {
	return KeyboardEvent{syscall.Timeval{}, key_event, KeyCode(code), keyPressed}
}

func keyboardEventsAreEquals(espectedEvents, realEvents []KeyboardEvent) bool {
	if len(espectedEvents) != len(realEvents) {
		return false
	}
	for i, espectedEvent := range espectedEvents {
		realEvent := realEvents[i]

		fmt.Println(espectedEvent, realEvent)

		if realEvent.Code != espectedEvent.Code ||
			realEvent.Type != espectedEvent.Type {
			return false
		}
	}
	return true
}

func _Test_skr(t *testing.T) {
	keyboard := initConfig()
	keyboard.path = "/dev/null"
	keyboardEventsIO := initKeyboardEventIO()

	keyboardEventsIO.inputKeyboardEvents = []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
	}

	keyboard.IO = keyboardEventsIO

	fmt.Println(keyboardEventsIO.outputKeyboardEvents, espectedOutput)

	loop(&keyboard, skrConfig)
	assert.True(t, keyboardEventsAreEquals(keyboardEventsIO.outputKeyboardEvents, espectedOutput))

}
