use std::collections::LinkedList;
use std::process::{Command, Stdio, ChildStdout, ChildStdin};
use std::io::{Read, Write};
use std::time::UNIX_EPOCH;

const INPUT_EVENT_SIZE: usize = std::mem::size_of::<KeyboardEvent>();
const KEYBOARD_EVENT_ID: u16 = 1;
const MAX_HISTORY_SIZE: usize = 256;

pub type KeyCode = u16;

# [repr(C)]
# [derive(PartialEq,Debug,Copy,Clone)]
pub struct InputEvent {
    pub time: Timeval,
    pub event_type: u16,
    pub code: KeyCode,
    pub value: i32,
}
# [derive(Debug, Clone, Copy)]
pub struct KeyboardEvent {
    pub time: Timeval,
    pub code: u16,
    pub state: KeyState,
}

# [derive(PartialEq,Debug,Copy,Clone)]
pub struct Timeval {
    sec: i64,
    usec: i64,
}

pub struct KeyboardState {
    pub key_event_history: LinkedList<KeyboardEvent>,
    pub current_pressed_keys: LinkedList<KeyboardEvent>,
    pub event_reader: EventReader,
    pub event_writer: EventWriter,
}

type EventReader = Box<dyn Read>;
type EventWriter = Box<dyn Write>;

impl std::fmt::Debug for KeyboardState {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "key_event_history: {:?}\n current_pressed_keys: {:?}",self.key_event_history, self.current_pressed_keys)
    }
}

# [derive(PartialEq,Debug,Copy,Clone)]
pub enum KeyState{
    Pressed = 1,
    Released = 0,
    HeldDown = 2,
}
impl KeyboardEvent {
    pub fn to_input_event(&self) -> InputEvent{
        let input_event = InputEvent{
            time: self.time,
            event_type: KEYBOARD_EVENT_ID,
            code: self.code,
            value: self.state as i32,
        };
        return input_event;
    }
}

impl KeyboardState{
    pub fn add_keyboard_event_to_history(&mut self, new_kb_event: KeyboardEvent){
        // first node is the most recent one
        self.key_event_history.push_front(new_kb_event);
        if self.key_event_history.len() > MAX_HISTORY_SIZE {
            self.key_event_history.pop_back();
        }
        if new_kb_event.state == KeyState::Pressed {
            self.current_pressed_keys.push_front(new_kb_event);
        }
    }
    pub fn remove_from_current_presed_key(&mut self, new_kb_event: KeyboardEvent){
        if new_kb_event.state != KeyState::Released {
            return
        }
        let mut pressed_keys: LinkedList<KeyboardEvent> = LinkedList::new();
        for key in self.current_pressed_keys.iter(){
            if &key.code != &new_kb_event.code {
                pressed_keys.push_back(*key);
            }
        }
        self.current_pressed_keys = pressed_keys;
    }
    pub fn send_key_stroke(&mut self, key_code: &u16, key_state: &KeyState){
        let system_time = std::time::SystemTime::now().duration_since(UNIX_EPOCH).unwrap();
        let system_timeval = Timeval{
            sec: system_time.as_secs() as i64,
            usec: system_time.subsec_nanos() as i64,
        };
        let prev_evt_wrap = InputEvent{
            time: system_timeval,
            event_type: 4,
            code: 4,
            value: 4,
        };
        let post_evt_wrap = InputEvent{
            time: system_timeval,
            event_type: 0,
            code: 0,
            value: 0,
        };
        let key_stroke_event = InputEvent{
            time: system_timeval,
            event_type: KEYBOARD_EVENT_ID,
            code: *key_code,
            value: *key_state as i32,
        };

        self.write_input_event(prev_evt_wrap);
        self.write_input_event(key_stroke_event);
        self.write_input_event(post_evt_wrap);
        let result = self.event_writer.flush(); 
        match result {
            Err(_) => panic!("error writing keyboard events"),
            _ => (),
        }
    }
    pub fn write_input_event(&mut self, input_event: InputEvent){
        let write_data = unsafe {
            any_as_u8_slice(&input_event)
        };
        let result = self.event_writer.write(write_data);
        match result {
            Err(_) => panic!("error while writing to keyboard stdio"),
            _ => (),
        }
    }
    pub fn read_input_event(&mut self) -> InputEvent{
        let mut buffer = [0; INPUT_EVENT_SIZE];
        let result = self.event_reader.read_exact(&mut buffer);
        match result {
            Err(_) => panic!("error while reading keyboard stdio"),
            _ => (),
        }
        let (_,input_event,_) = unsafe {
            buffer.align_to::<InputEvent>()
        };
        return input_event[0];
    }
}

impl InputEvent {
    pub fn is_keyboard_event(&self) -> bool{
        return self.event_type == KEYBOARD_EVENT_ID;
    }

    pub fn to_keyboad_event(&self) -> KeyboardEvent{
        let key_state: KeyState = match self.value {
            0 => KeyState::Released,
            1 => KeyState::Pressed,
            2 => KeyState::HeldDown,
            _ => panic!("Invalid keyboard event state value")
        };
        return KeyboardEvent { 
            time: self.time,
            code: self.code,
            state: key_state
        };
    }
}

unsafe fn any_as_u8_slice<T: Sized>(p: &T) -> &[u8] {
    ::core::slice::from_raw_parts(
        (p as *const T) as *const u8,
        ::core::mem::size_of::<T>(),
    )
}

pub fn create_kb_event_writer(kb_path: &String) -> impl Write {
    return Command::new("uinput")
        .arg("-d")
        .arg(kb_path)
        .stdin(Stdio::piped())
        .spawn()
        .expect("failed to open keyboard")
        .stdin.unwrap();
}

pub fn create_kb_event_reader(kb_path: &String) -> impl Read {
    return Command::new("intercept")
        .arg("-g")
        .arg(kb_path)
        .stdout(Stdio::piped())
        .spawn()
        .expect("failed to open keyboard")
        .stdout.unwrap();
}

pub fn get_key_code(key_name: &str) -> KeyCode {
    let key_code = match key_name {
    // aliases
    "CTRL" => 29,
    "SHIFT" => 42,
    "ALT" => 56,
    // linux key codes
    "RESERVED" => 0,
    "ESC" => 1,
    "1" => 2,
    "2" => 3,
    "3" => 4,
    "4" => 5,
    "5" => 6,
    "6" => 7,
    "7" => 8,
    "8" => 9,
    "9" => 10,
    "0" => 11,
    "MINUS" => 12,
    "EQUAL" => 13,
    "BACKSPACE" => 14,
    "TAB" => 15,
    "Q" => 16,
    "W" => 17,
    "E" => 18,
    "R" => 19,
    "T" => 20,
    "Y" => 21,
    "U" => 22,
    "I" => 23,
    "O" => 24,
    "P" => 25,
    "LEFTBRACE" => 26,
    "RIGHTBRACE" => 27,
    "ENTER" => 28,
    "LEFTCTRL" => 29,
    "A" => 30,
    "S" => 31,
    "D" => 32,
    "F" => 33,
    "G" => 34,
    "H" => 35,
    "J" => 36,
    "K" => 37,
    "L" => 38,
    "SEMICOLON" => 39,
    "APOSTROPHE" => 40,
    "GRAVE" => 41,
    "LEFTSHIFT" => 42,
    "BACKSLASH" => 43,
    "Z" => 44,
    "X" => 45,
    "C" => 46,
    "V" => 47,
    "B" => 48,
    "N" => 49,
    "M" => 50,
    "COMMA" => 51,
    "DOT" => 52,
    "SLASH" => 53,
    "RIGHTSHIFT" => 54,
    "KPASTERISK" => 55,
    "LEFTALT" => 56,
    "SPACE" => 57,
    "CAPSLOCK" => 58,
    "F1" => 59,
    "F2" => 60,
    "F3" => 61,
    "F4" => 62,
    "F5" => 63,
    "F6" => 64,
    "F7" => 65,
    "F8" => 66,
    "F9" => 67,
    "F10" => 68,
    "NUMLOCK" => 69,
    "SCROLLLOCK" => 70,
    "KP7" => 71,
    "KP8" => 72,
    "KP9" => 73,
    "KPMINUS" => 74,
    "KP4" => 75,
    "KP5" => 76,
    "KP6" => 77,
    "KPPLUS" => 78,
    "KP1" => 79,
    "KP2" => 80,
    "KP3" => 81,
    "KP0" => 82,
    "KPDOT" => 83,
    "ZENKAKUHANKAKU" => 85,
    "102ND" => 86,
    "F11" => 87,
    "F12" => 88,
    "RO" => 89,
    "KATAKANA" => 90,
    "HIRAGANA" => 91,
    "HENKAN" => 92,
    "KATAKANAHIRAGANA" => 93,
    "MUHENKAN" => 94,
    "KPJPCOMMA" => 95,
    "KPENTER" => 96,
    "RIGHTCTRL" => 97,
    "KPSLASH" => 98,
    "SYSRQ" => 99,
    "RIGHTALT" => 100,
    "LINEFEED" => 101,
    "HOME" => 102,
    "UP" => 103,
    "PAGEUP" => 104,
    "LEFT" => 105,
    "RIGHT" => 106,
    "END" => 107,
    "DOWN" => 108,
    "PAGEDOWN" => 109,
    "INSERT" => 110,
    "DELETE" => 111,
    "MACRO" => 112,
    "MUTE" => 113,
    "VOLUMEDOWN" => 114,
    "VOLUMEUP" => 115,
    "POWER" => 116,
    "KPEQUAL" => 117,
    "KPPLUSMINUS" => 118,
    "PAUSE" => 119,
    "SCALE" => 120,
    "KPCOMMA" => 121,
    "HANGEUL" => 122,
    "HANGUEL" => 122,
    "HANJA" => 123,
    "YEN" => 124,
    "LEFTMETA" => 125,
    "RIGHTMETA" => 126,
    "COMPOSE" => 127,
    "STOP" => 128,
    "AGAIN" => 129,
    "PROPS" => 130,
    "UNDO" => 131,
    "FRONT" => 132,
    "COPY" => 133,
    "OPEN" => 134,
    "PASTE" => 135,
    "FIND" => 136,
    "CUT" => 137,
    "HELP" => 138,
    "MENU" => 139,
    "CALC" => 140,
    "SETUP" => 141,
    "SLEEP" => 142,
    "WAKEUP" => 143,
    "FILE" => 144,
    "SENDFILE" => 145,
    "DELETEFILE" => 146,
    "XFER" => 147,
    "PROG1" => 148,
    "PROG2" => 149,
    "WWW" => 150,
    "MSDOS" => 151,
    "COFFEE" => 152,
    "SCREENLOCK" => 152,
    "ROTATE_DISPLAY" => 153,
    "DIRECTION" => 153,
    "CYCLEWINDOWS" => 154,
    "MAIL" => 155,
    "BOOKMARKS" => 156,
    "COMPUTER" => 157,
    "BACK" => 158,
    "FORWARD" => 159,
    "CLOSECD" => 160,
    "EJECTCD" => 161,
    "EJECTCLOSECD" => 162,
    "NEXTSONG" => 163,
    "PLAYPAUSE" => 164,
    "PREVIOUSSONG" => 165,
    "STOPCD" => 166,
    "RECORD" => 167,
    "REWIND" => 168,
    "PHONE" => 169,
    "ISO" => 170,
    "CONFIG" => 171,
    "HOMEPAGE" => 172,
    "REFRESH" => 173,
    "EXIT" => 174,
    "MOVE" => 175,
    "EDIT" => 176,
    "SCROLLUP" => 177,
    "SCROLLDOWN" => 178,
    "KPLEFTPAREN" => 179,
    "KPRIGHTPAREN" => 180,
    "NEW" => 181,
    "REDO" => 182,
    "F13" => 183,
    "F14" => 184,
    "F15" => 185,
    "F16" => 186,
    "F17" => 187,
    "F18" => 188,
    "F19" => 189,
    "F20" => 190,
    "F21" => 191,
    "F22" => 192,
    "F23" => 193,
    "F24" => 194,
    "PLAYCD" => 200,
    "PAUSECD" => 201,
    "PROG3" => 202,
    "PROG4" => 203,
    "DASHBOARD" => 204,
    "SUSPEND" => 205,
    "CLOSE" => 206,
    "PLAY" => 207,
    "FASTFORWARD" => 208,
    "BASSBOOST" => 209,
    "PRINT" => 210,
    "HP" => 211,
    "CAMERA" => 212,
    "SOUND" => 213,
    "QUESTION" => 214,
    "EMAIL" => 215,
    "CHAT" => 216,
    "SEARCH" => 217,
    "CONNECT" => 218,
    "FINANCE" => 219,
    "SPORT" => 220,
    "SHOP" => 221,
    "ALTERASE" => 222,
    "CANCEL" => 223,
    "BRIGHTNESSDOWN" => 224,
    "BRIGHTNESSUP" => 225,
    "MEDIA" => 226,
    "SWITCHVIDEOMODE" => 227,
    "KBDILLUMTOGGLE" => 228,
    "KBDILLUMDOWN" => 229,
    "KBDILLUMUP" => 230,
    "SEND" => 231,
    "REPLY" => 232,
    "FORWARDMAIL" => 233,
    "SAVE" => 234,
    "DOCUMENTS" => 235,
    "BATTERY" => 236,
    "BLUETOOTH" => 237,
    "WLAN" => 238,
    "UWB" => 239,
    "UNKNOWN" => 240,
    "VIDEO_NEXT" => 241,
    "VIDEO_PREV" => 242,
    "BRIGHTNESS_CYCLE" => 243,
    "BRIGHTNESS_AUTO" => 244,
    "BRIGHTNESS_ZERO" => 244,
    "DISPLAY_OFF" => 245,
    "WWAN" => 246,
    "WIMAX" => 246,
    "RFKILL" => 247,
    "MICMUTE" => 248,
    "MIN_INTERESTING" => 113,
    _ => panic!("keyname: '{}' not found", key_name),
    };
    return key_code;
}
