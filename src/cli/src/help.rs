#![allow(dead_code)]
use crate::{include, warn};

const VERSION_INFO: &str = "cli sub-utility | Ver. 0.0.1";

const REGISTERS_HELP: &str = "Usage::\tregisters [general | index | pointers | status]";
const DUMP_HELP: &str = "Usage::\tdump filepath  [OPTIONS]";
const MEMORY_HELP: &str = "Usage::\tmemory [start-end] [OPTIONS]";
const INSTRUCTION_HELP: &str = "Usage::\tinstruction <MNEMONIC>";

#[allow(non_snake_case)]
pub fn Version() -> () {
    println!("{}", VERSION_INFO);
}

#[allow(non_snake_case)]
pub fn Usage() -> () {
    println!("Usage::\thelp [command]");
    println!(""); // \n
}

#[allow(non_snake_case)]
pub fn Help() -> () {
    Usage();

    println!("You good bro?");
}


pub fn help(argc: i8, argv: Vec<String>, _cpu: &include::cpu_context) -> Result<(), std::fmt::Error> {
    if argc < 1 {
        warn!("Missing arguments");
        return Err(std::fmt::Error);
    }

    if argc == 1 {
        println!("No arguments supplied. Defaulting to all of them");
        println!("{}", REGISTERS_HELP);
        println!("{}", DUMP_HELP);
        println!("{}", MEMORY_HELP);
        println!("{}", INSTRUCTION_HELP);
        println!(""); // \n
    }

    else if argc > 2 {
        if argv[1] == "registers" {
            println!("{}", REGISTERS_HELP);
            println!(""); // \n
        
        } else if argv[1] == "dump" {
            println!("{}", DUMP_HELP);
            println!(""); // \n
        
        } else if argv[1] == "memory" {
            println!("{}", MEMORY_HELP);
            println!(""); // \n
        
        } else if argv[1] == "instruction" {
            println!("{}", INSTRUCTION_HELP);
            println!(""); // \n
        
        } else {
            warn!("Unknown command: '{}'", argv[1]);
            return Err(std::fmt::Error);
        }
    }
    
    println!("Note:: run \"<command> --help\" for more information");
    return Ok(())
}