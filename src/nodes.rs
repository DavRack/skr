use crate::keyboard_io::KeyboardState;
use std::any::type_name;
use std::fmt::Debug;
use log::{info, debug};

fn get_type<T>(_: &T) -> String{
    let complete_type_name = String::from(type_name::<T>());
    let parts = complete_type_name.split("::");
    let short_type_name = parts.last().unwrap();
    return String::from(short_type_name);
}

#[derive(Clone)]
pub struct Node{
    pub action: Box<dyn Action>,
    pub childs: Vec<Node>

}

impl Clone for Box<dyn Action> {
    fn clone(&self) -> Self {
        self.as_ref().clone_box()
    }
}

impl Debug for Node {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}",self.action.get_display_name())
    }
}

pub trait Action {
    fn get_display_name(&self) -> String{
        return get_type(&self);
    }
    fn clone_box(&self) -> Box<dyn Action>;
    fn exec(&self, _kb_state: &mut KeyboardState){
        // info!("exec {}",get_type(&self))
    }
    fn in_condition(&self, _kb_state: &mut KeyboardState) -> bool{
        return true;
    }
    fn out_condition(&self, _kb_state: &mut KeyboardState) -> bool{
        return true;
    }

}

pub fn execute_node<'a>(mut node_stack: Vec<Node>, mut active_nodes: Vec<Node>, kb_state: &mut KeyboardState) -> (Vec<Node>, Vec<Node>){
    for (i, active_node) in active_nodes.iter().enumerate() {
        if active_node.action.in_condition(kb_state){
            active_node.action.exec(kb_state);
            return (node_stack,active_nodes);
        }
        if active_node.action.out_condition(kb_state){
            debug!("######################### remove ############################");
            active_nodes.remove(i);
            return (node_stack,active_nodes);
        }
    }

    let current_node = node_stack.last().unwrap();

    let current_node_action = &current_node.action;
    current_node_action.exec(kb_state);

    if current_node_action.out_condition(kb_state) {
        node_stack.pop();
        return (node_stack,active_nodes);
    }

    let node_childs = current_node.childs.to_vec();
    for child_node in node_childs {
        if child_node.action.in_condition(kb_state) {
            let node_action = &child_node.action;
            node_action.exec(kb_state);

            if child_node.action.out_condition(kb_state){
                // dont add the node to stack if the out condition is satisfied
                return (node_stack,active_nodes);
            }

            if child_node.childs.len() > 0 {
                node_stack.push(child_node);
            }else{
                active_nodes.push(child_node);
            }
            return (node_stack,active_nodes);
        }
    }

    return (node_stack,active_nodes);
}
