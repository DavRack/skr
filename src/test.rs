use crate::test_helpers::{MockKbEvent, setup, output_events_to_mock};
use crate::keyboard_io::KeyState::{Pressed, Released};

#[test]
fn test_simple_remap(){
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
    let output_events = setup(mock_keyboard_events, config_file_string);
    assert_eq!(output_events_to_mock(output_events), spected_events)
}

#[test]
fn test_simple_layer_remap(){
    let config_file_string = "
        keyboardPath = \"/dev/null\"
        layer fn {
            blockKeys = true
            locklayer = true
            trigger = \"ALT\"
            remap {
              from = \"H\"
              to=\"LEFT\"
            }
        }
    ";
    let mock_keyboard_events: Vec<MockKbEvent> = vec![

        // test no remapped key remains equal
        MockKbEvent{key_code: 30, key_state: Pressed},
        MockKbEvent{key_code: 30, key_state: Released},

        // enter into layer fn with ALT key (56)
        MockKbEvent{key_code: 56, key_state: Pressed},
        MockKbEvent{key_code: 35, key_state: Pressed},
        MockKbEvent{key_code: 35, key_state: Released},
        MockKbEvent{key_code: 56, key_state: Released},

        // test keys function normaly after layer released
        MockKbEvent{key_code: 35, key_state: Pressed},
        MockKbEvent{key_code: 35, key_state: Released},
    ];
    let spected_events = vec![
        MockKbEvent{key_code: 30, key_state: Pressed},
        MockKbEvent{key_code: 30, key_state: Released},

        MockKbEvent{key_code: 105, key_state: Pressed},
        MockKbEvent{key_code: 105, key_state: Released},

        MockKbEvent{key_code: 35, key_state: Pressed},
        MockKbEvent{key_code: 35, key_state: Released},
    ];
    let output_events = setup(mock_keyboard_events, config_file_string);
    assert_eq!(spected_events, output_events_to_mock(output_events))
}

#[test]
fn test_simple_erlier_release_remap(){
    // when a layer is active and some action is active inside that layer
    // the user may release the layer trigger key before the action is
    // ended
    let config_file_string = "
        keyboardPath = \"/dev/null\"
        layer fn {
            blockKeys = true
            locklayer = true
            trigger = \"ALT\"
            remap {
              from = \"H\"
              to=\"LEFT\"
            }
        }
    ";
    let mock_keyboard_events: Vec<MockKbEvent> = vec![

        // test no remapped key remains equal
        MockKbEvent{key_code: 30, key_state: Pressed},
        MockKbEvent{key_code: 30, key_state: Released},

        // enter into layer fn with ALT key (56)
        MockKbEvent{key_code: 56, key_state: Pressed},
        MockKbEvent{key_code: 35, key_state: Pressed},
        // release the layer fn key
        MockKbEvent{key_code: 56, key_state: Released},
        MockKbEvent{key_code: 35, key_state: Released},

        // test keys function normaly after layer released
        MockKbEvent{key_code: 35, key_state: Pressed},
        MockKbEvent{key_code: 35, key_state: Released},
    ];
    let spected_events = vec![
        MockKbEvent{key_code: 30, key_state: Pressed},
        MockKbEvent{key_code: 30, key_state: Released},

        MockKbEvent{key_code: 105, key_state: Pressed},
        MockKbEvent{key_code: 105, key_state: Released},

        MockKbEvent{key_code: 35, key_state: Pressed},
        MockKbEvent{key_code: 35, key_state: Released},
    ];
    let output_events = setup(mock_keyboard_events, config_file_string);
    assert_eq!(spected_events, output_events_to_mock(output_events))
}
