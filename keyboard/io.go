package keyboard

import (
	"bufio"
	"encoding/binary"
	"errors"
	"fmt"
	"os"
	"os/exec"
	"strings"
	"syscall"
	"time"
)

type EventType uint16
type KeyCode uint16
type KeyCodeList []KeyCode
type KeyName string
type KeyState int32

type KeyboardEvent struct {
	Time  syscall.Timeval // time in seconds since epoch
	Type  EventType       // event type
	Code  KeyCode         // keycode
	Value KeyState        // key state: press, held or released
}

// values from evdev
const KeyEventType EventType = 1 // event.type
const KeyPressed KeyState = 1    // key state
const KeyReleased KeyState = 0   // key state

type Key interface {
	keyCode() KeyCode
	keyName() KeyName
}

type KeyEvent struct {
	Time     syscall.Timeval
	KeyCode  KeyCode
	KeyState KeyState // press, held, released
}

// values for KeyEvent
var pressed KeyState = 1
var held KeyState = 2
var released KeyState = 0

type Keyboard struct {
	SomeActionExecuted  bool
	ExecuteDefaulAction bool
	IO                  KeyboardIO
	LastKey             KeyEvent
	Name                string
	Path                string
	PressedKeys         KeyCodeList
	BlockedEvents       map[string][]KeyEvent
	ToBlockEvents       map[string][]KeyEvent
	UseConfigFile       bool
}

type KeyboardIO struct {
	IoReader *bufio.Reader
	IoWriter *bufio.Writer
}

var prev_evt_wrap KeyboardEvent = KeyboardEvent{syscall.Timeval{}, 4, 4, 4}
var post_evt_wrap KeyboardEvent = KeyboardEvent{syscall.Timeval{}, 0, 0, 0}

func (keyboard *Keyboard) SendKey(keyCode KeyCode, state KeyState) (ok bool) {
	keyEvent := KeyEvent{
		KeyCode:  keyCode,
		KeyState: state,
	}
	keyboard.Execute(keyEvent)
	ok = true
	return
}

func (keyboard *Keyboard) PressKey(key interface{}) (ok bool) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		KeyCode:  keyCode,
		KeyState: pressed,
	}
	keyboard.Execute(keyEvent)
	ok = true
	return
}

func (keyboard *Keyboard) ReleaseKey(key interface{}) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		KeyCode:  keyCode,
		KeyState: released,
	}
	keyboard.Execute(keyEvent)
}

func (user_keyboard Keyboard) Exist() bool {
	_, err := os.Stat(user_keyboard.Path)
	return err == nil
}

func (keyboard *Keyboard) Execute(keyEvent KeyEvent) {
	inputEvent := keyEvent.ToInputEvent()

	t := time.Now()
	sec := t.Unix()
	usec := int64(t.Nanosecond())

	inputEvent.Time.Sec = sec
	inputEvent.Time.Usec = usec

	err := keyboard.IO.Write(prev_evt_wrap)
	if err != nil {
		fmt.Println(err)
	}
	err = keyboard.IO.Write(inputEvent)
	if err != nil {
		fmt.Println(err)
	}
	err = keyboard.IO.Write(post_evt_wrap)
	if err != nil {
		fmt.Println(err)
	}
	keyboard.IO.Flush()
}

func (keyEvent KeyEvent) ToInputEvent() (inputEvent KeyboardEvent) {
	inputEvent = KeyboardEvent{
		Time:  keyEvent.Time,
		Type:  KeyEventType,
		Code:  keyEvent.KeyCode,
		Value: keyEvent.KeyState,
	}
	return
}

func GetKeyboardPathFromName(name string) (error, string) {

	command := "sudo sed -n '/" + name + "/,/Handlers/p' /proc/bus/input/devices | grep -o 'event.*' -m 1"

	out, _ := exec.Command("bash", "-c", command).Output()
	if len(out) == 0 {
		return errors.New("Keyboard not found"), ""
	}
	input_event := strings.TrimSpace(string(out))

	return nil, "/dev/input/" + input_event

}

func (keyboard KeyboardIO) Read() (KeyboardEvent, error) {
	var raw_input KeyboardEvent
	err := binary.Read(keyboard.IoReader, binary.LittleEndian, &raw_input)
	if raw_input.Type == 6 {
		err = errors.New("Invalid Keyboard event")
	}
	return raw_input, err
}

func (keyboard KeyboardIO) Write(outputKeyEvent KeyboardEvent) error {
	err := binary.Write(keyboard.IoWriter, binary.LittleEndian, &outputKeyEvent)
	return err
}

func (keyboard KeyboardIO) Flush() {
	keyboard.IoWriter.Flush()
}

func InitStdinIO(keyboardName string) KeyboardIO {
	var keyboard KeyboardIO
	// create a process to read raw input data from interception tools
	_, keyboardPath := GetKeyboardPathFromName(keyboardName)

	write_cmd := exec.Command("uinput", "-d", keyboardPath)
	write_pipe, _ := write_cmd.StdinPipe()
	write_cmd.Start()
	defer write_cmd.Wait()
	ioWriter := bufio.NewWriter(write_pipe)

	read_cmd := exec.Command("intercept", "-g", keyboardPath)
	read_pipe, _ := read_cmd.StdoutPipe()
	read_cmd.Start()
	defer read_cmd.Wait()
	ioReader := bufio.NewReader(read_pipe)

	keyboard.IoReader = ioReader
	keyboard.IoWriter = ioWriter

	return keyboard
}

func (event KeyboardEvent) ToString() string {
	code := event.Code
	state := ""
	if event.Value == KeyPressed {
		state = "Key pressed"
	}
	if event.Value == KeyReleased {
		state = "Key released"
	}
	resoult := "Code: " + fmt.Sprint(code) + ", State: " + state
	return resoult
}
