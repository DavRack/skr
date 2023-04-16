use std::collections::LinkedList;
use std::env;
use log::{error, debug};
use log;
use env_logger;

mod keyboard_io;
mod nodes;
mod config;
mod actions;
mod in_conditions;

fn main(){
    let mut args: Vec<String> = env::args().collect();
    println!("Starting SKR {:?}", args);

    let log_level = log::LevelFilter::Debug;

    let mut log_builder = env_logger::Builder::new();
    log_builder
        .filter_level(log_level)
        .init();
    test(args);
}

fn test(mut args: Vec<String>) {
    // get the node stack
    let keyboard_config = config::parse_config_file(args.pop().unwrap());
    let mut node_stack = vec![keyboard_config.node_tree];
    let mut active_nodes = vec![];

    let mut may_event_reader = keyboard_io::create_kb_event_reader(&keyboard_config.path);
    let mut may_event_writer = keyboard_io::create_kb_event_writer(&keyboard_config.path);

    let mut kb_state = keyboard_io::KeyboardState {
        key_event_history: LinkedList::from([]),
        current_pressed_keys: LinkedList::from([]),
        event_reader: may_event_reader.as_mut().unwrap(),
        event_writer: may_event_writer.as_mut().unwrap(),
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
