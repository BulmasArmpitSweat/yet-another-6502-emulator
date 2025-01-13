// Shut up rustc warnings about code that is used not being used for some fucken stupid reason
#![allow(dead_code)]
use std::fmt::Error;
use crate::{include, warn};

const VERSION_INFO: &str = "registers sub-utility | Ver. 0.0.1";

const CARRY             : include::ByteRaw = 1 << 0;
const ZERO              : include::ByteRaw = 1 << 1;
const INTERRUPT_DISABLE : include::ByteRaw = 1 << 2;
const DECIMAL_MODE      : include::ByteRaw = 1 << 3;
const BRK_COMMAND       : include::ByteRaw = 1 << 4;
const OVERFLOW          : include::ByteRaw = 1 << 6;
const NEGATIVE          : include::ByteRaw = 1 << 7;

#[allow(non_snake_case)]
pub fn Version() -> () {
    println!("{}", VERSION_INFO);
}


#[allow(non_snake_case)]
pub fn Usage() -> () {
    println!("Usage::\tregisters [general | index | pointers | status]");
    println!(""); // \n
}

#[allow(non_snake_case)]
pub fn Help() -> () {
    Usage();
    
    println!("Description::");
    println!("\tPrints most up-to-date register values");
    println!(""); // \n
    
    println!("Options::");
    println!("\tgeneral       |\t\t\tPrints general purpose registers");
    println!("\tindex         |\t\t\tPrints index registers");
    println!("\tpointers      |\t\t\tPrints memory pointer registers");
    println!("\tstatus        |\t\t\tPrints status register");
    println!(""); // \n
    
    println!("Other Options::");
    println!("\t-h, --help    |\t\t\tPrints this help message and exits");
    println!("\t-v, --version |\t\t\tPrints version information and exits");
    println!(""); // \n

    println!("Examples::");
    println!("\tregisters general");
    println!("\tregisters index");
    println!("\tregisters pointers");
    println!("\tregisters status");
    println!(""); // \n

    println!("Notes::");
    println!("\tIf no argument is given, all registers will be printed");
    println!(""); // \n
}

fn test_flag(cpu: &include::cpu_context, flag: include::ByteRaw) -> bool {
    return (cpu.sr & flag) == flag;
}

fn general(cpu: &include::cpu_context) -> () {
    println!("General purpose registers::");
    println!("\tA (Accumulator): 0x{:02x}", cpu.a);
    println!(""); // \n
}

fn index(cpu: &include::cpu_context) -> () {
    println!("Index Registers::");
    println!("\tX (Index X): 0x{:02x}", cpu.x);
    println!("\tY (Index Y): 0x{:02x}", cpu.y);
    println!(""); // \n
}

fn pointers(cpu: &include::cpu_context) -> () {
    println!("Memory pointer registers::");
    println!("\tPC (Program Counter): 0x{:02x}", cpu.pc);
    println!("\tSP (Stack Pointer): 0x{:02x}", cpu.sp);
    println!(""); // \n
}

fn status(cpu: &include::cpu_context) -> () {
    println!("\tSR (Status Register): 0x{:02x}", cpu.sr);
    println!(""); // \n

    println!("Status Register Breakdown::");
    println!("\tFull binary representation: 0b{}", include::byte_to_binary(cpu.sr).iter().collect::<String>());
    println!("\tIndex 0: C (Carry)             : {} | {}", if test_flag(&cpu, CARRY)             { 0 } else { 1 }, if test_flag(&cpu, CARRY)             { "Set" } else { "Not Set" });
    println!("\tIndex 1: Z (Zero)              : {} | {}", if test_flag(&cpu, ZERO)              { 0 } else { 1 }, if test_flag(&cpu, ZERO)              { "Set" } else { "Not Set" });
    println!("\tIndex 2: I (Interrupt Disable) : {} | {}", if test_flag(&cpu, INTERRUPT_DISABLE) { 0 } else { 1 }, if test_flag(&cpu, INTERRUPT_DISABLE) { "Set" } else { "Not Set" });
    println!("\tIndex 3: D (Decimal Mode)      : {} | {}", if test_flag(&cpu, DECIMAL_MODE)      { 0 } else { 1 }, if test_flag(&cpu, DECIMAL_MODE)      { "Set" } else { "Not Set" });
    println!("\tIndex 4: B (Break Command)     : {} | {}", if test_flag(&cpu, BRK_COMMAND)       { 0 } else { 1 }, if test_flag(&cpu, BRK_COMMAND)       { "Set" } else { "Not Set" });
    println!("\tIndex 6: V (Overflow)          : {} | {}", if test_flag(&cpu, OVERFLOW)          { 0 } else { 1 }, if test_flag(&cpu, OVERFLOW)          { "Set" } else { "Not Set" });
    println!("\tIndex 7: N (Negative)          : {} | {}", if test_flag(&cpu, NEGATIVE)          { 0 } else { 1 }, if test_flag(&cpu, NEGATIVE)          { "Set" } else { "Not Set" });
}

pub fn registers(argc: i8, argv: Vec<String>, cpu: &include::cpu_context) -> Result<(), Error> {
    if argc < 1 {
        warn!("Expected arguments, but got none");
    }

    if argc == 1 {
        println!("Printing all registers::");

        general(&cpu);
        index(&cpu);
        pointers(&cpu);
        status(&cpu);
    } else {
        if argc == 2 {
            if argv[1] == "general" {
                general(&cpu);
            
            } else if argv[1] == "index" {
                index(&cpu);
            
            } else if argv[1] == "pointers" {
                pointers(&cpu);
            
            } else if argv[1] == "status" {
                status(&cpu);
            
            } else {
                warn!("Unknown argument");
            }
        }
    }
    return Ok(())
}