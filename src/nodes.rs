use crate::keyboard_io::KeyboardState;
use std::any::type_name;
use std::fmt::Debug;
use log::{debug};

fn get_type<T>(_: &T) -> String{
    let complete_type_name = String::from(type_name::<T>());
    let parts = complete_type_name.split("::");
    let short_type_name = parts.last().unwrap();
    return String::from(short_type_name);
}

#[derive(Clone)]
pub struct Node{
    pub id: u32,
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

fn add_to_active_nodes(node: Node, mut active_nodes: Vec<Node>) -> Vec<Node>{
    for n in active_nodes.clone() {
        if node.id == n.id {
            return active_nodes;
        }
    }
    active_nodes.push(node);
    return active_nodes;
}

pub fn execute_node<'a>(mut node_stack: Vec<Node>, mut active_nodes: Vec<Node>, kb_state: &mut KeyboardState) -> (Vec<Node>, Vec<Node>){
    let current_node = node_stack.last().unwrap();

    // test if we have to remove the current layer node
    let current_node_action = &current_node.action;
    if current_node_action.out_condition(kb_state) {
        debug!("out condition true");
        node_stack.pop();
        return (node_stack,active_nodes);
    }

    // test if we have to add some node to node stack
    let node_childs = current_node.childs.to_vec();
    for child_node in node_childs {
        if child_node.childs.len() == 0 {
            // if child_node has no child_nodes, then isn't a layer
            continue;
        }
        if child_node.action.in_condition(kb_state) {
            node_stack.push(child_node);
            return (node_stack,active_nodes);
        }
    }

    // check if some action node is activated and then add thad node to active_nodes
    let node_childs = current_node.childs.to_vec();
    for child_node in node_childs {
        if child_node.childs.len() > 0 {
            // if child_node has child_nodes, then its a layer, not an action node
            continue;
        }

        if child_node.action.in_condition(kb_state) {
            active_nodes = add_to_active_nodes(child_node, active_nodes);
            break;
        }
    }

    // execute active nodes
    for (i, active_node) in active_nodes.clone().iter().enumerate() {
        let in_condition = active_node.action.in_condition(kb_state);
        let out_condition = active_node.action.out_condition(kb_state);

        if out_condition {
            active_nodes.remove(i);
            active_node.action.exec(kb_state);
            return (node_stack,active_nodes);
        }

        if in_condition {
            active_node.action.exec(kb_state);
            return (node_stack,active_nodes);
        }
    }

    return (node_stack,active_nodes);
}
