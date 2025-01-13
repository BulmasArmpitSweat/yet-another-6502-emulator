#![allow(dead_code)]
use crate::{err, include::{self}, warn};

const VERSION_INFO: &str = "memory sub-utility | Ver. 0.0.1";

#[allow(non_snake_case)]
pub fn Version() -> () {
    println!("{}", VERSION_INFO);
}

#[allow(non_snake_case)]
pub fn Usage() -> () {
    println!("Usage::\tmemory [start-end] [OPTIONS]");
    println!(""); // \n
}

#[allow(non_snake_case)]
pub fn Help() -> () {
    Usage();

    println!("Description::");
    println!("\tPrints most up-to-date contents of memory to stdout");
    println!(""); // \n

    println!("Options::");
    println!("\tstart-\t\t\t | The start of memory to list out | Omitting this defaults it to 0x0000");
    println!("\t-end  \t\t\t | The end of memory to list out | Omitting this defaults it to 0xFFFF");
    println!(""); // \n

    println!("Other Options::");
    println!("\t-h, --help    |\t\t\tPrints this help message and exits");
    println!("\t-v, --version |\t\t\tPrints version information and exits");
    println!("\t-p, --pc      |\t\t\tPrints 16-byte memory block with PC marker");
    println!(""); // \n
}

fn print_pc(cpu: &include::cpu_context) -> Result<(), std::fmt::Error> {
    let start: include::UShort = cpu.pc as include::UShort - ((cpu.pc % 16) as include::UShort);
    let end: include::UShort = start + 16;
    let mut pos: include::UShort = 0;

    print!("0x{:04x}", start);

    for i in start..end {
        print!("{:02x} ", cpu.mem[i as usize]);
        if i == cpu.pc {
            pos = (i as include::UShort % 16) * 3;
        }
    }
        
    for _ in 0..pos {
        print!(" ")
    }
    print!("^PC");
    
    return Ok(());
}

pub fn memory(argc: i8, argv: Vec<String>, cpu: &include::cpu_context) -> Result<(), std::fmt::Error> {
    let mut start: include::UShort = 0x0000;
    let mut end  : include::UShort = 0xFFFF;
    'a: {
        if argc == 1 {
            println!("No start or and parameters supplied. Defaulting to 0x0000-0xFFFF::");
            break 'a;
            // Get 16 byte block of memory around PC
        } else {
            for arg in argv.iter().skip(1) {
                if arg.starts_with("--") && (arg == "-p" || arg == "--pc") {
                    print_pc(&cpu).unwrap();
                    return Ok(());
                }
                if arg.ends_with("-") {
                    start = arg.trim_ascii_end().trim_start_matches("0x").parse::<include::UShort>().unwrap();
                
                } else if arg.starts_with("-") {
                    end = arg.trim_ascii_start().trim_start_matches("0x").parse::<include::UShort>().unwrap();
                
                } else if arg.contains("-") {
                    let parts: std::vec::Vec<&str> = arg.split('-').collect();
                    start = parts[0].parse::<include::UShort>().unwrap();
                    end = parts[1].parse::<include::UShort>().unwrap();
                } else {
                    // Requires more explicit handling than just .unwrap() in this case
                    match arg.trim_start_matches("0x").parse::<include::UShort>() {
                        Ok(val) => start = val,
                        _ => err!("Invalid start value. Cannot parse into integer literal"),
                    }
                }
            }
        }
    }

    if start > end {
        warn!("Start value is greater than end value. Defaulting to 0x0000-0xFFFF");
        start = 0x0000;
        end = 0xFFFF;
    }

    for i in start - (start % 16)..end + (16 - (end % 16)) % 16 {
        if i % 16 == 0 && i != start {
            println!("|");
        }
        let end_value: u16 = if i > end { end } else { i + 15 };
        print!("0x{:04x} (decimal: {:width$}) -> 0x{:04x} (decimal: {:width$}):: ", i, i, end_value, end_value, width = include::count_digits(&(start as i16)) as usize);
    
        if i < start || i > end {
            print!("-- ");
        } else {
            print!("{:02x} ", cpu.mem[i as usize]);
        }
    }
    println!("|");

    return Ok(());
}