use std::fs::{read_dir, OpenOptions};
use std::io::{stdin, BufReader, BufRead};
use std::os::unix::prelude::OpenOptionsExt;

pub fn innit_app(){
    println!("Welcome to SKR!\n");
    println!("To use skr we need a config file configured with your keyboard name");

    println!("{:?}",get_keyboard_event_path());
}

const O_NONBLOCK: i32 = 2048;
fn get_keyboard_event_path() -> Result<String, &'static str>{
    // get all input events of /dev/input
    let mut input_event_paths = Vec::new(); 
    for entry in read_dir("/dev/input/").unwrap() {
        let file = entry.unwrap();
        if !file.metadata().unwrap().is_dir() {
            input_event_paths.push(file.path());
        }
    }

    let mut input_event_file_readers = Vec::new();
    for input_event_path in &input_event_paths {
        // open all input event files in a non-blocking manner
        let input_event_file = OpenOptions::new()
            .custom_flags(O_NONBLOCK)
            .read(true)
            .open(input_event_path)
            .unwrap()
        ;
        let input_event_reader = BufReader::new(input_event_file);

        input_event_file_readers.push((input_event_path.to_str().unwrap(), input_event_reader));
    }

    // promt the user to press enter, this will all some information
    // to the input event file asociated with te user keyboard
    println!("Press ENTER on the keyboard you wish to remap:");
    let mut buf = String::new();
    stdin().read_line(&mut buf).unwrap();

    // try to read each file, if the read succeeds then we have
    // find the file asociated to the user keyboard
    for (file_path, file_reader) in input_event_file_readers {
        let mut _buf: Vec<u8> = vec![];
        let mut mut_file_reader = file_reader;
        if mut_file_reader.read_until(0,&mut _buf).is_ok() {
            return Ok(file_path.to_owned());
        }
    }
    return Err("cant find the keyboard path")
}
