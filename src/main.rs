use std::collections::LinkedList;
use std::env;
use std::fs;
use std::io::Read;
use std::io::Write;

use log::debug;
use env_logger;

use self::config::KeyboardConfig;

mod keyboard_io;
mod nodes;
mod config;
mod actions;
mod in_conditions;
mod test;

fn main(){
    let mut args: Vec<String> = env::args().collect();
    println!("Starting SKR {:?}", args);

    let log_level = log::LevelFilter::Debug;

    let mut log_builder = env_logger::Builder::new();
    log_builder
        .filter_level(log_level)
        .init();

    let config_file_string = fs::read_to_string(args.pop().unwrap())
        .expect("Should have been able to read the file");
    println!("{}",config_file_string);
    let keyboard_config = config::parse_config_file(config_file_string);
    let may_event_reader = keyboard_io::create_kb_event_reader(&keyboard_config.path);
    let may_event_writer = keyboard_io::create_kb_event_writer(&keyboard_config.path);
    keyboard_loop(
        keyboard_config,
        Box::new(may_event_reader),
        Box::new(may_event_writer),
    );
}

pub fn keyboard_loop(keyboard_config: KeyboardConfig, event_reader: Box<dyn Read>, event_writer: Box<dyn Write>) {
    // get the node stack
    let mut node_stack = vec![keyboard_config.node_tree];
    let mut active_nodes = vec![];

    let mut kb_state = keyboard_io::KeyboardState {
        key_event_history: LinkedList::from([]),
        current_pressed_keys: LinkedList::from([]),
        event_reader,
        event_writer,
    };

    loop {
        let input_event = kb_state.read_input_event();

        if !input_event.is_keyboard_event(){
            continue;
        }

        debug!("");

        let current_key = input_event.to_keyboad_event();
        debug!("Current key event: {current_key:?}");
        kb_state.add_keyboard_event_to_history(current_key);

        debug!("Before Node stack: {node_stack:?}");
        debug!("Before active_nodes: {active_nodes:?}");
        (node_stack, active_nodes) = nodes::execute_node(node_stack, active_nodes, &mut kb_state);
        debug!("After  Node stack: {node_stack:?}");
        debug!("After active_nodes: {active_nodes:?}");
        kb_state.remove_from_current_presed_key(current_key);
    }
}
