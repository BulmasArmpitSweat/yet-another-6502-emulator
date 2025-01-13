#![allow(dead_code)]
use std::io::Write;
use crate::{err, include, warn};

const VERSION_INFO: &str = "dump sub-utility | Ver. 0.0.1";

#[allow(non_snake_case)]
pub fn Version() -> () {
    println!("{}", VERSION_INFO);
}

#[allow(non_snake_case)]
pub fn Usage() -> () {
    println!("Usage::\tdump filepath  [OPTIONS]");
}

#[allow(non_snake_case)]
pub fn Help() -> () {
    Usage();
    println!(""); // \n

    println!("Description::");
    println!("\tDumps the most up-to-date contents of memory to external file");
    println!(""); // \n

    println!("Options::");
    println!("\t-s, --simple |\t\t\tPrints memory without formatting");
}

pub fn dump(argc: i8, argv: Vec<String>, cpu: &include::cpu_context) -> Result<(), std::fmt::Error> {
    let mut simple: bool = false;
    
    // command_name, filepath, option
    if argc == 1 {
        warn!("Expected filepath, got none");
    }

    // only one option can be passed, so handling for it can be hardcoded
    if argc == 3 && argv[2] == "-s" || argv[2] == "--simple" {
        simple = true;
    }

    let filepath = &argv[1];
    let mut output: std::fs::File;

    match std::fs::File::open(filepath) {
        Ok(file) => output = file,
        Err(_e) => {
            err!("Failed to open file {}", filepath);
        }
    }
    
    let mut num_zeros: include::UShort = 0;
    for pos in (0..include::U16_MAX).step_by(1) {
        if simple == false {
            if pos % 16 == 0 {
                write!(output, "|\n").unwrap(); // Lazy way of avoiding lots of match case blocks to handle Result<...>
                write!(output, "0x{:04x} -> 0x{:04x}:: ", pos, if pos + 15 < include::U16_MAX { pos + 15 } else { include::U16_MAX - 1 }).unwrap();
                write!(output, "{:02x} ", cpu.mem[pos as usize]).unwrap();
                if cpu.mem[pos as usize] == 0 {
                    num_zeros += 1;
                }
            }
        } else {
            write!(output, "{:02x} ", cpu.mem[pos as usize]).unwrap();
        }
    }
    if simple == false {
        write!(output, "{} bytes ({}%) of the internal memory are assigned values", include::U16_MAX - num_zeros, ((include::U16_MAX - num_zeros) / include::U16_MAX) as f64 * 100.0 as f64).unwrap();
    }
    return Ok(());
}