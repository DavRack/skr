package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os/exec"
	"strings"
	"syscall"
	"time"
)

var prev_evt_wrap InputEvent = InputEvent{syscall.Timeval{}, 4, 4, 4}
var post_evt_wrap InputEvent = InputEvent{syscall.Timeval{}, 0, 0, 0}

func (keyboard *Keyboard) pressKey(key interface{}) (ok bool) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: pressed,
	}
	keyboard.execute(keyEvent)
	ok = true
	return
}

func (keyboard *Keyboard) releaseKey(key interface{}) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: released,
	}
	keyboard.execute(keyEvent)
}

func (keyboard *Keyboard) execute(keyEvent KeyEvent) {
	inputEvent := keyEvent.toInputEvent()
	// state := ""
	// if keyEvent.keyState == pressed
	// 	state = "keyPressed"
	// }
	// if keyEvent.keyState == released {
	// 	state = "keyReleased"
	// }
	// if keyEvent.keyState == held {
	// 	state = "keyHeld"
	// }

	// fmt.Println("KeyCode:", keyEvent.keyCode, "Event type:", state)
	// var bytes bytes.Buffer
	// enc := gob.NewEncoder(&bytes)

	// fmt.Println("New Write")

	// enc.Encode(pre_evt_wrap)
	// keyboard.ioWriter.Write(bytes.Bytes())

	// enc.Encode(inputEvent)
	// keyboard.ioWriter.Write(bytes.Bytes())

	// enc.Encode(post_evt_wrap)
	// keyboard.ioWriter.Write(bytes.Bytes())
	t := time.Now()
	sec := t.Unix()
	usec := int64(t.Nanosecond())

	inputEvent.Time.Sec = sec
	inputEvent.Time.Usec = usec

	err := binary.Write(keyboard.ioWriter, binary.LittleEndian, &prev_evt_wrap)
	if err != nil {
		fmt.Println(err)
	}
	err = binary.Write(keyboard.ioWriter, binary.LittleEndian, &inputEvent)
	if err != nil {
		fmt.Println(err)
	}
	err = binary.Write(keyboard.ioWriter, binary.LittleEndian, &post_evt_wrap)
	if err != nil {
		fmt.Println(err)
	}
	keyboard.ioWriter.Flush()
}

func (keyEvent KeyEvent) toInputEvent() (inputEvent InputEvent) {
	inputEvent = InputEvent{
		Time:  keyEvent.time,
		Type:  key_event,
		Code:  keyEvent.keyCode,
		Value: keyEvent.keyState,
	}
	return
}

func get_keyboard_path_from_name(name string) (error, string) {

	command := "sudo sed -n '/" + name + "/,/Handlers/p' /proc/bus/input/devices | grep -o 'event.*' -m 1"

	out, _ := exec.Command("bash", "-c", command).Output()
	if len(out) == 0 {
		return errors.New("Keyboard not found"), ""
	}
	input_event := strings.TrimSpace(string(out))

	return nil, "/dev/input/" + input_event

}
