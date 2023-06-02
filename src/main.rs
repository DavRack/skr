use std::collections::LinkedList;
use std::fs;
use std::io::Read;
use std::io::Write;

use log::debug;
use env_logger;

use self::config::KeyboardConfig;
use self::innit::innit_app;

mod keyboard_io;
mod nodes;
mod config;
mod actions;
mod in_conditions;
mod innit;

use clap::Parser;

/// Simple Key Remap
#[derive(Parser, Debug)]
#[command(author, about, long_about = None)]
struct Args {
    /// Path to the config file
    #[arg(short, long, conflicts_with = "innit", default_value = "", id = "/absolute/path/to/config/file.hcl")]
    config_file: String,

    /// Initialize config file
    #[arg(long, name = "innit")]
    innit: bool,

    /// Initialize config file
    #[arg(long, default_value = "off", id = "off|error|warn|info|debug|trace")]
    log_level: log::LevelFilter
}

#[cfg(test)]
mod tests;

fn main(){
    let args = Args::parse();

    let mut log_builder = env_logger::Builder::new();
    log_builder
        .filter_level(args.log_level)
        .init();

    if args.innit {
    println!("{:?}",args);
        innit_app();
    }else{
        run_remap(args);
    }
}

fn run_remap(args: Args){

    let config_file_string = fs::read_to_string(args.config_file)
        .expect("Should have been able to read the file");
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
