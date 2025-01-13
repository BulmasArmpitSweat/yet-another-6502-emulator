#![allow(dead_code)]
use crate::{include, warn};

const VERSION_INFO: &str = "addr_modes sub-utility | Ver. 0.0.1";

const ADDR_MODES: [&str; 12] = [
    "ACCUMULATOR (XXX A) | Instruction takes the accumulator as input. As a result, the input isn't hardcoded into the bytecode.\n",
    "IMPLIED (XXX) | Instruction takes no arguments. Instruction's functionality is \"implied\".\n",
    "IMMEDIATE (XXX #$nn) | Instruction takes a single byte as input, and is directly passed to the instruction.\n",
    "ABSOLUTE (XXX $nnnn) | Instruction parameter is at the memory location pointed at by the two-byte operand passed directly to the instruction.\n",
    "X_INDEXED_ABSOLUTE (XXX $nnnn,X) | Instruction parameter is located at the memory location pointed at by the two-byte operand, plus the value in the index register X.\n",
    "Y_INDEXED_ABSOLUTE (XXX $nnnn,Y) | Instruction parameter is located at the memory location pointed at by the two-byte operand, plus the value in the index register Y.\n",
    "ABSOLUTE_INDIRECT (XXX ($nnnn)) | Instruction parameter is at the memory location at the memory location pointed at by the included two-byte operand.\n",
    "ZERO_PAGE (XXX $nn) | Instruction parameter is located in the first 256 byte page of memory. Address is directly given as an operand. Like Absolute, but faster, as it only has to fetch one byte for the address, instead of two\n",
    "X_INDEXED_ZERO_PAGE (XXX $nn,X) | Instruction parameter is located in the first page of memory, and is the address directly given as an operand, plus the value in the index register X. Like X_INDEXED_ABSOLUTE, but faster, as it only has to fetch one byte for the address, instead of two\n",
    "Y_INDEXED_ZERO_PAGE (XXX $nn,Y) | Instruction parameter is located in the first page of memory, and is the address directly given as an operand, plus the value in the index register Y. Like Y_INDEXED_ABSOLUTE, but faster, as it only has to fetch one byte for the address, instead of two\n",
    "X_INDEXED_ZERO_PAGE_INDIRECT (XXX ($nn,X)) | Instruction parameter is located at the memory location, in the first 256 byte page of memory, plus the value in the index register X, pointed at by the value in memory pointed at by the one-byte operand.\n",
    "ZERO_PAGE_INDIRECT_Y_INDEXED (XXX ($nn),Y)) | Instruction parameter is located at the memory location, in the first 256 byte page of memory, pointed at by the value in memory pointed at by the two-byte operand, plus the value in the Index register Y.\n",
];

#[allow(non_snake_case)]
pub fn Version() {
    println!("{}", VERSION_INFO);
}

#[allow(non_snake_case)]
pub fn Usage() {
    println!("Usage::\taddr_modes [addr_mode]");
    println!("");
}

#[allow(non_snake_case)]
pub fn Help() {
    Usage();

    println!("Description::");
    println!("\tPrints out basic information about different address modes");
    println!("");

    println!("Options::");
    println!("\taddr_mode |\t\t\tThe address mode to print information about");
    println!("");

    println!("Other Options::");
    println!("\t-h, --help    |\t\t\tPrints this help message and exits");
    println!("\t-v, --version |\t\t\tPrints version information and exits");
    println!("\t-a, --all     |\t\t\tPrints all address modes.");
    println!("");
}

fn addr_to_idx(addr_mode: &str) -> u8 {
    let addr_mode_to_idx: std::collections::HashMap<&str, u8> = std::collections::HashMap::from([
        ("ACCUMULATOR", 0),
        ("IMPLIED", 1),
        ("IMMEDIATE", 2),
        ("ABSOLUTE", 3),
        ("X_INDEXED_ABSOLUTE", 4),
        ("Y_INDEXED_ABSOLUTE", 5),
        ("ABSOLUTE_INDIRECT", 6),
        ("ZERO_PAGE", 7),
        ("X_INDEXED_ZERO_PAGE", 8),
        ("Y_INDEXED_ZERO_PAGE", 9),
        ("X_INDEXED_ZERO_PAGE_INDIRECT", 10),
        ("ZERO_PAGE_INDIRECT_Y_INDEXED", 11),
    ]);
        if ! addr_mode_to_idx.contains_key(addr_mode) {
            return 255;
        }
    return addr_mode_to_idx[addr_mode];
}

pub fn addr_modes(argc: i8, argv: Vec<String>, _cpu: &include::cpu_context) -> Result<(), std::fmt::Error> {
    if argc < 1 {
        warn!("Expected arguments, but got none");
        return Err(std::fmt::Error);
    }

    if argc == 1 {
        warn!("Expected arguments, but got none");
        return Err(std::fmt::Error);
    }

    for arg in argv.iter().skip(1) {
        if argv[1] == "--all" || argv[1] == "-a" {
            for i in 0..ADDR_MODES.len() {
                println!("{}", ADDR_MODES[i]);
            }
            return Ok(());
        } else {
            let idx = addr_to_idx(&argv[1]);
            if idx == 255 {
                warn!("Unknown addressing mode \"{}\"", arg);
                return Err(std::fmt::Error);
            }
            println!("{}", ADDR_MODES[idx as usize]);
            return Ok(());
        }
    }
    return Ok(());
}