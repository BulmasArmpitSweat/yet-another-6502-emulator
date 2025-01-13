#![allow(dead_code)]

use crate::{include::{self}, warn};

const VERSION_INFO: &str = "instruction sub-utility | Ver. 0.0.1";

#[allow(non_snake_case)]
pub fn Version() {
    println!("{}", VERSION_INFO);
}

#[allow(non_snake_case)]
pub fn Usage() {
    println!("Usage::\tinstruction <MNEMONIC>");
    println!("");
}

#[allow(non_snake_case)]
pub fn Help() {
    Usage();

    println!("Description::");
    println!("\tPrints out basic information about an instruction");
    println!("");

    println!("Options::");
    println!("\tMNEMONIC |\t\t\tThe mnemonic of the instruction to print information about");
    println!("");

    println!("Other Options::");
    println!("\t-h, --help    |\t\t\tPrints this help message and exits");
    println!("\t-v, --version |\t\t\tPrints version information and exits");
    println!("");
}

fn instruction_help(instr: &include::help_message) -> () {
    println!("Mnemonic:  {}", instr.mnemonic.to_uppercase());
    println!("Summary:   {}", instr.operation_summary.to_uppercase());
    println!("Category:  {}", instr.category.to_uppercase());
    println!("Operation: {}", instr.operation_string);
    println!("| C | Z | I | D | B | _ | V | N |");
    print!("|");
    for i in 0..8 {
        #[allow(unused_assignments)]
        let mut ch: u32 = ' ' as u32;
        match instr.affected_flags[i] {
            include::Flag::AFFECTED     => ch = "✓".as_ptr() as u32,
            include::Flag::NOT_AFFECTED => ch = "-".as_ptr() as u32,
            include::Flag::RESET        => ch = "0".as_ptr() as u32,
            include::Flag::INITIALIZED  => ch = "1".as_ptr() as u32,
        }
        print!(" ");
        include::print_unicode(ch);
        print!(" |")
    }

    // Separator
    print!("\n\n");
    print!("______________________________________________________________________\n\n");

    print!("{}\n\n", instr.help_message);

    print!("Expected addressing modes: ({} {})", instr.addr_mode_info_len, if instr.addr_mode_info_len == 1 { "mode" } else { "modes" });

    for i in 0..instr.addr_mode_info_len {
        let mnemonic: &str = &instr.mnemonic;
        let bytecode: u8 = instr.addressing_modes[i as usize].bytecode;
        let undoc_instr_ch: char = if instr.addressing_modes[i as usize].undocumented_instruction == true { '*' } else { ' ' };

        match &instr.addressing_modes.iter().collect::<std::vec::Vec<&include::addr_mode_info<>>>()[i as usize].addr_mode {
            include::AddressModes::IMPLIED => //
                println!("IMPLIED (0x{:02x}){}                      | {}", bytecode, undoc_instr_ch, mnemonic),
            
            include::AddressModes::ACCUMULATOR => //
                println!("ACCUMULATOR (0x{:02x}){}                  | {} A", bytecode, undoc_instr_ch, mnemonic),
            
            include::AddressModes::IMMEDIATE => //
                println!("IMMEDIATE (0x{:02x}){}                    | {} #$HHLL", bytecode, undoc_instr_ch, mnemonic),

            include::AddressModes::ABSOLUTE => //
                println!("ABSOLUTE (0x{:02x}){}                     | {} $HHLL", bytecode, undoc_instr_ch, mnemonic),
        
            include::AddressModes::X_INDEXED_ABSOLUTE => //
                println!("X_INDEXED_ABSOLUTE (0x{:02x}){}           | {} $HHLL,X", bytecode, undoc_instr_ch, mnemonic),
        
            include::AddressModes::Y_INDEXED_ABSOLUTE => //
                println!("Y_INDEXED_ABSOLUTE (0x{:02x}){}           | {} $HHLL,Y", bytecode, undoc_instr_ch, mnemonic),
        
            include::AddressModes::ABSOLUTE_INDIRECT => //
                println!("ABSOLUTE_INDIRECT (0x{:02x}){}            | {} ($HHLL)", bytecode, undoc_instr_ch, mnemonic),

            include::AddressModes::ZERO_PAGE => //
                println!("ZERO_PAGE (0x{:02x}){}                    | {} $HH", bytecode, undoc_instr_ch, mnemonic),

            include::AddressModes::X_INDEXED_ZERO_PAGE => //
                println!("X_INDEXED_ZERO_PAGE (0x{:02x}){}          | {} $HH,X", bytecode, undoc_instr_ch, mnemonic),

            include::AddressModes::Y_INDEXED_ZERO_PAGE => //
                println!("Y_INDEXED_ZERO_PAGE (0x{:02x}){}          | {} $HH,Y", bytecode, undoc_instr_ch, mnemonic),
            
            include::AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT => //
                println!("X_INDEXED_ZERO_PAGE_INDIRECT (0x{:02x}){} | {} ($HH,X)", bytecode, undoc_instr_ch, mnemonic),
            
            include::AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED => //
                println!("ZERO_PAGE_INDIRECT_Y_INDEXED (0x{:02x}){} | {} ($HH),Y", bytecode, undoc_instr_ch, mnemonic),

            include::AddressModes::RELATIVE => //
                println!("RELATIVE (0x{:02x}){}                     | {} $HHLL", bytecode, undoc_instr_ch, mnemonic),
        
            include::AddressModes::NOTHING => //
                println!("NOTHING (0x{:02x}){}                      | {} N/A", bytecode, undoc_instr_ch, mnemonic),
        }
        println!("") // \n
    }
    return ();
}

pub fn instruction(argc: i8, argv: &Vec<String>, _cpu: &include::cpu_context) -> Result<(), std::fmt::Error> {
    if argc < 2 {
        warn!("Expected arguments, but got none");
        return Err(std::fmt::Error);
    }
    include::set_instructions(); // Shit's fucked

    // Fugly, but makes the red squiggles go away
    Ok::<(), std::fmt::Error>(
        for arg in argv.iter().skip(1) {
            if let Some(instructions) = include::INSTRUCTIONS.get() {
                for instruction in instructions {
                    if instruction.mnemonic == arg {
                        instruction_help(instruction);
                        println!("{}", instruction.mnemonic);
                        return Ok(());
                    } else {
                        continue;
                    }
                }
                return Err(std::fmt::Error);
            }
        }).unwrap();
    return Ok(())
}