use crate::tests::common::{MockKbEvent, setup, output_events_to_mock};
use crate::keyboard_io::KeyState::{Pressed, Released};

#[test]
pub fn test_simple_remap(){
    let config_file_string = "
        keyboardPath = \"/dev/null\"
        remap {
            from = \"a\"
            to = \"b\"
        }
    ";
    let mock_keyboard_events: Vec<MockKbEvent> = vec![
        MockKbEvent{key_code: 30, key_state: Pressed},
        MockKbEvent{key_code: 30, key_state: Released},
        MockKbEvent{key_code: 31, key_state: Pressed},
        MockKbEvent{key_code: 31, key_state: Released},
    ];
    let spected_events = vec![
        MockKbEvent{key_code: 48, key_state: Pressed},
        MockKbEvent{key_code: 48, key_state: Released},
        MockKbEvent{key_code: 31, key_state: Pressed},
        MockKbEvent{key_code: 31, key_state: Released},
    ];
    let (output_events,_,_) = setup(mock_keyboard_events, config_file_string);
    assert_eq!(output_events_to_mock(output_events), spected_events)
}

#[test]
pub fn test_simple_swap(){
    let config_file_string = "
        keyboardPath = \"/dev/null\"
        swap {
            from = \"a\"
            to = \"b\"
        }
    ";
    let mock_keyboard_events: Vec<MockKbEvent> = vec![
        MockKbEvent{key_code: 30, key_state: Pressed},
        MockKbEvent{key_code: 30, key_state: Released},
        MockKbEvent{key_code: 48, key_state: Pressed},
        MockKbEvent{key_code: 48, key_state: Released},
    ];
    let spected_events = vec![
        MockKbEvent{key_code: 48, key_state: Pressed},
        MockKbEvent{key_code: 48, key_state: Released},
        MockKbEvent{key_code: 30, key_state: Pressed},
        MockKbEvent{key_code: 30, key_state: Released},
    ];
    let (output_events, _, _) = setup(mock_keyboard_events, config_file_string);
    assert_eq!(output_events_to_mock(output_events), spected_events)
}
