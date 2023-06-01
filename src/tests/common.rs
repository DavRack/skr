use std::collections::LinkedList;
use std::io::BufReader;
use std::io::Read;
use std::io::Write;

use crate::nodes::Node;
use crate::{config, nodes};
use crate::config::KeyboardConfig;
use crate::keyboard_io;
use crate::keyboard_io::{InputEvent, KeyboardState};


# [derive(Debug, PartialEq)]
pub struct MockKbEvent {
    pub key_code: keyboard_io::KeyCode,
    pub key_state: keyboard_io::KeyState
}

fn create_mock_events(mock_keyboard_events: &Vec<MockKbEvent>, mut kb: KeyboardState){
    for kb_event in mock_keyboard_events{
        kb.send_key_stroke(&kb_event.key_code, &kb_event.key_state);
    }
}
pub fn output_events_to_mock(output_events: Vec<u8>) -> Vec<MockKbEvent>{

    // convert into input events
    let (_,input_events,_) = unsafe {
        output_events.align_to::<InputEvent>()
    };

    // remove padding events
    let keyboard_events: Vec<MockKbEvent> = input_events
        .into_iter()
        .filter(|e| e.is_keyboard_event())
        .map(|e| e.to_keyboad_event())
        .map(|e| MockKbEvent{key_code: e.code, key_state: e.state})
        .collect();
    return keyboard_events;
}
fn keyboard_loop(keyboard_config: KeyboardConfig, event_reader: Box<dyn Read + '_>, event_writer: Box<dyn Write + '_>, event_count: usize) -> (Vec<Node>, Vec<Node>){
    // get the node stack
    let mut node_stack = vec![keyboard_config.node_tree];
    let mut active_nodes = vec![];

    let mut kb_state = keyboard_io::KeyboardState {
        key_event_history: LinkedList::from([]),
        current_pressed_keys: LinkedList::from([]),
        event_reader,
        event_writer,
    };

    for _ in 0..event_count {
        let input_event = kb_state.read_input_event();

        if !input_event.is_keyboard_event(){
            continue;
        }
        let current_key = input_event.to_keyboad_event();
        kb_state.add_keyboard_event_to_history(current_key);

        (node_stack, active_nodes) = nodes::execute_node(node_stack, active_nodes, &mut kb_state);
        kb_state.remove_from_current_presed_key(current_key);
    }

    return (node_stack, active_nodes);
}
pub fn setup(mock_keyboard_events: Vec<MockKbEvent>, config_file_string: &str) -> (Vec<u8>, Vec<Node>, Vec<Node>){
    let keyboard_config = config::parse_config_file(String::from(config_file_string));
    let dummy_input_events = Vec::new();
    let mut input_events = Vec::new();
    let input_events_write_reference = input_events.by_ref();

    let kb_state_creator = keyboard_io::KeyboardState {
        key_event_history: LinkedList::from([]),
        current_pressed_keys: LinkedList::from([]),
        event_reader: Box::new(BufReader::new(dummy_input_events.as_slice())),
        event_writer: Box::new(input_events_write_reference),
    };

    create_mock_events(&mock_keyboard_events, kb_state_creator);


    let mut output_events = Vec::new();
    let may_event_reader = BufReader::new(input_events.as_slice());
    let may_event_writer = output_events.by_ref();


    let (node_stack, active_nodes) = keyboard_loop(
        keyboard_config,
        Box::new(may_event_reader),
        Box::new(may_event_writer),
        mock_keyboard_events.len()*3, // 3 events per key stroke
    );
    return (output_events, node_stack, active_nodes);
}
