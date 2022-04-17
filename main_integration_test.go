package main

import (
	"bufio"
	"encoding/binary"
	"os"
	"skr/keyboard"
	"syscall"
	"testing"

	"github.com/stretchr/testify/assert"
)

func mock_keyEvent(code int, state keyboard.KeyState) keyboard.KeyboardEvent {
	return keyboard.KeyboardEvent{syscall.Timeval{}, keyboard.KeyEventType, keyboard.KeyCode(code), state}
}

func keyboardEventsToSimpleList(espectedEvents []keyboard.KeyboardEvent) []string {
	simpleEvents := []string{}
	for _, event := range espectedEvents {
		simpleEvents = append(simpleEvents, event.ToString())
	}
	return simpleEvents
}

func keyboardInToSimpleList(kb keyboard.Keyboard) []string {
	simpleEvents := []string{}
	for {
		event, err := kb.IO.Read()
		if err != nil {
			break
		}
		if event.Type != keyboard.KeyEventType {
			continue
		}
		simpleEvents = append(simpleEvents, event.ToString())
	}
	return simpleEvents
}

func runTest(inputEvents []keyboard.KeyboardEvent, config_function func(*keyboard.Keyboard) bool) keyboard.Keyboard {
	keyboard := setup(inputEvents)
	loop(&keyboard, config_function)
	setdown(keyboard)
	return keyboard
}

func setup(inputEvents []keyboard.KeyboardEvent) keyboard.Keyboard {
	keyboardIn, keyboardOut, _ := os.Pipe()
	keyboardStdin := keyboard.KeyboardIO{}
	keyboardStdin.IoReader = bufio.NewReader(keyboardIn)
	keyboardStdin.IoWriter = bufio.NewWriter(keyboardOut)

	kb := InitConfig()
	kb.Path = "/dev/null"
	kb.BlockedEvents = make(map[string][]keyboard.KeyEvent)
	kb.ToBlockEvents = make(map[string][]keyboard.KeyEvent)
	KeyboardIO := keyboardStdin
	kb.IO = KeyboardIO
	for _, inputEvent := range inputEvents {
		binary.Write(KeyboardIO.IoWriter, binary.LittleEndian, &inputEvent)
	}
	nonKeyboardEvent := keyboard.KeyboardEvent{}
	nonKeyboardEvent.Type = 6
	binary.Write(KeyboardIO.IoWriter, binary.LittleEndian, &nonKeyboardEvent)
	KeyboardIO.Flush()

	return kb
}

func setdown(kb keyboard.Keyboard) {
	nonKeyboardEvent := keyboard.KeyboardEvent{}
	nonKeyboardEvent.Type = 6
	kb.IO.Write(nonKeyboardEvent)
	kb.IO.Flush()
}

func transparent_config(keyboard *keyboard.Keyboard) (blockCurrentKey bool) {
	currentKeyEvent := keyboard.LastKey
	keyboard.Execute(currentKeyEvent)
	return
}

func Test_no_custom_actions(t *testing.T) {
	inputEvents := []keyboard.KeyboardEvent{
		mock_keyEvent(30, keyboard.KeyPressed),
		mock_keyEvent(30, keyboard.KeyReleased),
	}

	espectedOutput := []keyboard.KeyboardEvent{
		mock_keyEvent(30, keyboard.KeyPressed),
		mock_keyEvent(30, keyboard.KeyReleased),
	}
	keyboard := runTest(inputEvents, transparent_config)

	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))
}
