use crate::keyboard_io::{KeyCode, get_key_code};
use crate::nodes::Node;
use crate::actions::{LayerAction,RemapAction,DefaultKeyAction,NoAction};
use hcl;
use log::info;

pub struct KeyboardConfig{
    pub path: String,
    pub node_tree: Node,
}

pub fn parse_config_file(config_file_string: String) -> KeyboardConfig {
    let config: hcl::Value = hcl::from_str(&config_file_string)
        .expect("The config file has some errors");

    let config = config.as_object().expect("The config file has some errors");

    let keyboad_path = config.clone().remove("keyboardPath")
        .expect( "A path for the keyboard is espected")
        .as_str()
        .expect("The keyboard path should be an string")
        .to_string();

    info!("{:?}",keyboad_path);
    info!("{:?}", config);
    let mut child_nodes = recursive_parse_layers(config, String::from("ROOT"));

    child_nodes.push(Node{
        action: Box::new(DefaultKeyAction{}),
        childs: vec![],
    });

    let kb_config = KeyboardConfig{
        path: String::from(keyboad_path),
        node_tree: Node{
            action: Box::new(NoAction{}),
            childs: child_nodes,
        }
    };
    return kb_config;
}

fn recursive_parse_layers(layer_config: &hcl::Map<String,hcl::Value>, _layer_name: String) -> Vec<Node>{
    let mut layer_actions = vec![];

    let mut remaps = match layer_config.get("remap") {
        Some(remaps) => parse_remaps(remaps),
        _ => vec![]
    };
    let mut layers = match layer_config.get("layer") {
        Some(layers) => parse_layers(layers),
        _ => vec![]
    };

    layer_actions.append(&mut remaps);
    layer_actions.append(&mut layers);
    return layer_actions
}

fn parse_layers(remaps: &hcl::Value) -> Vec<Node>{
    let mut layer_nodes = vec![];
    let layers = remaps.as_object().expect("Config syntax error: all layers must have a unique name");

    for (layer_name,layer) in layers {
        let layer_config = layer.as_object().unwrap();
        let layer_action = LayerAction{
            trigger: field_to_key_chord(layer_config,"trigger")
        };
        layer_nodes.push(Node{
            action: Box::new(layer_action),
            childs: recursive_parse_layers(layer_config, layer_name.to_string()),
        });
    }
    return layer_nodes;
}

fn parse_remaps(remaps: &hcl::Value) -> Vec<Node>{
    let remap_array = normalize_to_array(remaps);
    let mut actions:Vec<Node> = vec![];
    for remap in remap_array{
        actions.push(Node{
            action: Box::new(create_remap_action(remap)),
            childs: vec![],
        });
    }
    return actions;
}

fn create_remap_action(remap: hcl::Value) -> RemapAction{
    let remap_values = remap.as_object().expect("Cant open remap object");
    let action = RemapAction{
        from: field_to_key_chord(remap_values, "from"),
        to: field_to_key_chord(remap_values, "to")
    };
    return action;
}

fn field_to_key_chord(remap: &hcl::Map<String, hcl::Value>, field: &str) -> Vec<u16>{
    let string_key_chord = remap.get(field)
        .expect(&format!("expected to find a value for: {}", field))
        .as_str()
        .expect("not str")
        .to_string();
    let keys: Vec<KeyCode> = string_key_chord.split("+").map(|key_name| get_key_code(&key_name.to_uppercase())).collect();
    return keys;
}

fn normalize_to_array(value: &hcl::Value) -> Vec<hcl::Value>{
    let value_array: Vec<hcl::Value>;
    let aux = value.to_owned();

    if value.is_object() {
        value_array = vec![aux];
    }else if value.is_array() {
        value_array = value.as_array().unwrap().to_owned();
    }else{
        panic!("error reading remaps");
    }
    return value_array;
}
