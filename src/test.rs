// use std::collections::LinkedList;
// use std::io::{BufReader, self};
// use std::process::{ChildStdout, ChildStdin, self};

// use crate::{config, nodes};
// use crate::config::KeyboardConfig;
// use crate::keyboard_io;
// use std::fs::{File, self};
// use std::io::Error;
// use std::os::unix::io::{FromRawFd, IntoRawFd};
// use std::os::unix::io::RawFd;

// // Function to create an in-memory pipe in Rust
// fn pipe() -> Result<(File, File), Error> {
//     let mut pipe_fds: [RawFd; 2] = [0; 2];

//     // Create File objects from the raw file descriptors
//     let read_file = unsafe { File::from_raw_fd(pipe_fds[0]) };
//     let write_file = unsafe { File::from_raw_fd(pipe_fds[1]) };

//     Ok((read_file, write_file))
// }

// fn setup() {
//     let config_file_string = "
//         keyboardPath = /dev/null
//         remap {
//             from = \"A\"
//             to=\"B\"
//         }
//         ";
//     let (r,w) = pipe().unwrap();
//     let keyboard_config = config::parse_config_file(String::from(config_file_string).replace("\n", ""));
//     let mut may_event_reader = BufReader::new(r);
//     let mut may_event_writer = std::os::fd::;


//     keyboard_loop(
//         keyboard_config,
//         may_event_reader,
//         may_event_writer,
//         0
//     );
// }
// fn keyboard_loop(keyboard_config: KeyboardConfig, event_reader: &mut ChildStdout, event_writer: &mut ChildStdin, event_count: u32) {
//     // get the node stack
//     let mut node_stack = vec![keyboard_config.node_tree];
//     let mut active_nodes = vec![];

//     let mut kb_state = keyboard_io::KeyboardState {
//         key_event_history: LinkedList::from([]),
//         current_pressed_keys: LinkedList::from([]),
//         event_reader,
//         event_writer,
//     };

//     for _ in 0..event_count {
//         let input_event = kb_state.read_input_event();

//         if !input_event.is_keyboard_event(){
//             continue;
//         }
//         let current_key = input_event.to_keyboad_event();
//         kb_state.add_keyboard_event_to_history(current_key);

//         (node_stack, active_nodes) = nodes::execute_node(node_stack, active_nodes, &mut kb_state);
//         kb_state.remove_from_current_presed_key(current_key);
//     }
// }

// #[test]
// fn test_1(){
//     assert!(true);
// }
