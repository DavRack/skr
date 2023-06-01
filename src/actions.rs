use log::debug;

use crate::keyboard_io::{KeyboardState, KeyCode, KeyState};
use crate::nodes::Action;

fn key_chord_is_active(trigger: Vec<KeyCode>, kb_state: &KeyboardState) -> bool{
    if kb_state.current_pressed_keys.len() < trigger.len() {return false}
    let mut pressed_keys = kb_state.current_pressed_keys.iter();
    debug!("pressed_keys: {pressed_keys:?}");
    for key_event in &trigger {
        if key_event != &pressed_keys.next().unwrap().code{
            return false
        }
    }
    return true
}
#[derive(Clone,Debug)]
pub struct SwapAction {
    pub from: Vec<KeyCode>,
    pub to: Vec<KeyCode>,
}

impl Action for SwapAction {
    fn exec(&self, kb_state: &mut KeyboardState) {
        // decide if we have to send a key press or key release
        let last_key_event = kb_state.key_event_history.front().unwrap();
        debug!("Exec remap");
        debug!("Last key event: {:?}", last_key_event);
        let keys_to_send = {
            if key_chord_is_active((&self.from).to_vec(), kb_state){
                &self.to[..]
            }else if key_chord_is_active((&self.to).to_vec(), kb_state){
                &self.from[..]
            }else{
                panic!("no key_chord_is_active")
            }
        };
        match last_key_event.state {
            KeyState::Pressed => {
                for key_code in keys_to_send{
                    debug!("send key: {key_code} {:?}", KeyState::Pressed);
                    kb_state.send_key_stroke(key_code, &KeyState::Pressed);
                }
            },
            KeyState::Released => {
                for key_code in keys_to_send.iter().rev(){
                    debug!("send key: {key_code} {:?}", KeyState::Released);
                    kb_state.send_key_stroke(key_code, &KeyState::Released);
                }
            }
            _ => (),

        }
    }
    fn in_condition(&self, kb_state: &mut KeyboardState) -> bool {
        return {
            key_chord_is_active((&self.from).to_vec(), kb_state) ||
            key_chord_is_active((&self.to).to_vec(), kb_state)
        }

    }
    fn out_condition(&self, kb_state: &mut KeyboardState) -> bool {
        debug!("kb_state: {:?}",kb_state.current_pressed_keys);
        let last_key_event = kb_state.key_event_history.front().unwrap();
        return {
            last_key_event.state == KeyState::Released && self.from.contains(&last_key_event.code) ||
            last_key_event.state == KeyState::Released && self.to.contains(&last_key_event.code)
        }
    }
    fn clone_box(&self) -> Box<dyn Action> {
        let a = self.clone();
        Box::new(a)
    }
}

#[derive(Clone,Debug)]
pub struct LayerAction {
    pub trigger: Vec<KeyCode>,
}

impl Action for LayerAction {
    fn in_condition(&self, kb_state: &mut KeyboardState) -> bool {
        let trigger = (&self.trigger).to_vec();
        return key_chord_is_active(trigger, kb_state);
    }
    fn out_condition(&self, kb_state: &mut KeyboardState) -> bool {
        let last_key_event = kb_state.key_event_history.front().unwrap();
        if last_key_event.state == KeyState::Released && self.trigger.contains(&last_key_event.code){
            return true
        }
        return false
    }
    fn clone_box(&self) -> Box<dyn Action> {
        let a = self.clone();
        Box::new(a)
    }
}

#[derive(Clone,Debug)]
pub struct RemapAction {
    pub from: Vec<KeyCode>,
    pub to: Vec<KeyCode>,
}

impl Action for RemapAction {
    fn exec(&self, kb_state: &mut KeyboardState) {
        // decide if we have to send a key press or key release
        let last_key_event = kb_state.key_event_history.front().unwrap();
        debug!("Exec remap");
        debug!("Last key event: {:?}", last_key_event);
        match last_key_event.state {
            KeyState::Pressed => {
                for key_code in &self.to[..]{
                    debug!("send key: {key_code} {:?}", KeyState::Pressed);
                    kb_state.send_key_stroke(key_code, &KeyState::Pressed);
                }
            },
            KeyState::Released => {
                for key_code in self.to[..].iter().rev(){
                    debug!("send key: {key_code} {:?}", KeyState::Released);
                    kb_state.send_key_stroke(key_code, &KeyState::Released);
                }
            }
            _ => (),

        }
    }
    fn in_condition(&self, kb_state: &mut KeyboardState) -> bool {
        let trigger = (&self.from).to_vec();
        return key_chord_is_active(trigger, kb_state);
    }
    fn out_condition(&self, kb_state: &mut KeyboardState) -> bool {
        debug!("kb_state: {:?}",kb_state.current_pressed_keys);
        let last_key_event = kb_state.key_event_history.front().unwrap();
        if last_key_event.state == KeyState::Released && self.from.contains(&last_key_event.code){
            return true
        }
        return false
    }
    fn clone_box(&self) -> Box<dyn Action> {
        let a = self.clone();
        Box::new(a)
    }
}

#[derive(Clone)]
pub struct DefaultKeyAction {
}

impl Action for DefaultKeyAction {
    fn exec(&self, kb_state: &mut KeyboardState){
        let current_key = kb_state.key_event_history.front().unwrap();
        kb_state.send_key_stroke(&current_key.code.clone(), &current_key.state.clone());
    }
    fn clone_box(&self) -> Box<dyn Action> {
        let a = self.clone();
        Box::new(a)
    }
}

#[derive(Clone)]
pub struct NoAction {
}

impl Action for NoAction {
    fn clone_box(&self) -> Box<dyn Action> {
        let a = self.clone();
        Box::new(a)
    }
    fn in_condition(&self, _kb_state: &mut KeyboardState) -> bool {
        false
    }
    fn out_condition(&self, _kb_state: &mut KeyboardState) -> bool {
        false
    }
}
