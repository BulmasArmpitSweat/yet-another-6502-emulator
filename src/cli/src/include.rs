#![allow(dead_code)]

#[macro_export]
macro_rules! wontreturn {
    () => {
        std::process::exit(-1);
    };
}

#[macro_export]
macro_rules! err {
    ($($arg:tt)*) => ({
        eprintln!($($arg)*);
        std::process::exit(-1);  
    })
}

#[macro_export]
macro_rules! warn {
    ($($arg:tt)*) => ({
        eprintln!($($arg)*);
    })
}

pub type ByteRaw = i8;
pub type UByte = u8;

pub type USeconds = u32;
pub type UInt = u32;
pub type UShort = u16;
pub type ULong = u64;

pub type CBool = u8;

pub const I8_MAX: i8 = i8::MAX;
pub const I8_MIN: i8 = i8::MIN;

pub const U8_MAX: u8 = u8::MAX;
pub const U8_MIN: u8 = u8::MIN;

pub const U16_MAX: u16 = u16::MAX;
pub const U16_MIN: u16 = u16::MIN;

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub enum AddressModes {
    IMPLIED,                           // N/A
    ACCUMULATOR,                       // A
    IMMEDIATE,                         // #$nn
    ABSOLUTE,                          // $nnnn
    X_INDEXED_ABSOLUTE,                // $nnnn,X
    Y_INDEXED_ABSOLUTE,                // $nnnn,Y
    ABSOLUTE_INDIRECT,                 // ($nnnn)
    ZERO_PAGE,                         // $nn
    X_INDEXED_ZERO_PAGE,               // $nn,X
    Y_INDEXED_ZERO_PAGE,               // $nn,Y
    X_INDEXED_ZERO_PAGE_INDIRECT,      // ($nn,X)
    ZERO_PAGE_INDIRECT_Y_INDEXED,      // (&nn),Y
    RELATIVE,                          // $nnnn
    NOTHING,                           // N/A
}

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub enum Flag {
    AFFECTED,
    NOT_AFFECTED,
    RESET,
    INITIALIZED,
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct cpu_context {
    pub sr: ByteRaw,
    pub a: UByte,
    pub x: UByte,
    pub y: UByte,
    pub sp: UByte,
    pub pc: UShort,
    pub mem: [ByteRaw; U16_MAX as usize],
    pub f_stack: [ByteRaw; 5],
    pub fsp: ByteRaw,
    pub halt: CBool,
    pub last_accessed_memory_location: UShort,
    pub nanoseconds_per_cycle: ULong,
    pub configured_hertz: std::ffi::c_char,
}

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub struct addr_mode_info<'a> {
    pub addr_mode: AddressModes,
    pub bytecode: UByte,
    pub num_bytes: ByteRaw,
    pub num_cycles: &'a str,
    pub undocumented_instruction: bool,
}

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub struct help_message<'a> {
    pub mnemonic: &'static str,
    pub operation_summary: &'static str,
    pub operation_string: &'static str,
    pub category: &'static str,
    pub affected_flags: [Flag; 8],
    pub help_message: &'static str,
    pub addr_mode_info_len: ByteRaw,
    pub addressing_modes: std::vec::Vec<addr_mode_info<'a>>,
}

pub fn byte_to_binary(byte: ByteRaw) -> [char; 8] {
    [
        if byte & 0x80u8 as i8 != 0 { '1' } else { '0' },
        if byte & 0x40u8 as i8 != 0 { '1' } else { '0' },
        if byte & 0x20u8 as i8 != 0 { '1' } else { '0' },
        if byte & 0x10u8 as i8 != 0 { '1' } else { '0' },
        if byte & 0x08u8 as i8 != 0 { '1' } else { '0' },
        if byte & 0x04u8 as i8 != 0 { '1' } else { '0' },
        if byte & 0x02u8 as i8 != 0 { '1' } else { '0' },
        if byte & 0x01u8 as i8 != 0 { '1' } else { '0' },
    ]
}

pub fn count_digits(number: &i16) -> i16 {
    let mut count: i16 = 0;
    let mut temp: i16 = *number;
    if temp == 0 {
        return 1;
    }

    if temp < 0 {
        temp = -temp;
    }

    while temp != 0 {
        temp /= 10;
        count += 1;
    }
    return count;
}

pub fn print_unicode(ch: u32) -> () {
    println!(
        "{}",
        match std::char::from_u32(ch) {
            Some(c) => c,
            None => '�',
        }
    );
}

pub const INSTRUCTIONS: std::sync::OnceLock<[help_message; 75]> = std::sync::OnceLock::new();

pub fn set_instructions() -> () {
    INSTRUCTIONS.set(
        [
    help_message { mnemonic: "LAS",
        operation_summary: "\"AND\" memory with stack pointer",
        operation_string: "M & SP -> A X SP",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This undocumented instruction performs a bit-by-bit \"AND\" operation of the stack pointer and memory and\n\
        stores the result back in the accumulator, the index register X and the stack pointer.\n\n\
        \
        The LAS instruction does not affect the carry or overflow flags. It sets N if the bit 7 of the result is on,\n\
        otherwise it is reset. If the result is zero, then the Z flag is set, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE, bytecode: 0xBB, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "LAX",
        operation_summary: "Load accumulator and index register X from memory",
        operation_string: "M -> A & X",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The undocumented LAX instruction loads the accumulator and the index register X from memory.\n\n\
        \
        LAX does not affect the C or V flags; sets Z if the value loaded was zero, otherwise resets it; sets N if the\n\
        value loaded in bit 7 is a 1; otherwise N is reset, and affects only the X register.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0xAB, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0xAF, num_bytes: 3, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0xBF, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0xA7, num_bytes: 2, num_cycles: "3", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ZERO_PAGE         , bytecode: 0xB7, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0xA3, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0xB3, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "LDA",
        operation_summary: "Load Accumulator from memory",
        operation_string: "M -> A",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "When instruction LDA is executed by the microprocessor, data is transferred from memory to the\n\
        accumulator and stored in the accumulator.\n\n\
        \
        LDA affects the contents of the accumulator, does not affect the carry or overflow flags; sets the zero\n\
        flag if the accumulator is zero as a result of the LDA, otherwise resets the zero flag; sets the negative\n\
        flag if bit 7 of the accumulator is a 1, otherwise resets the negative flag.",
        addr_mode_info_len: 8,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0xA9, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0xAD, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0xBD, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0xB9, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0xA5, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0xB5, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0xA1, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0xB1, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "LDX",
        operation_summary: "Load index X register from memory",
        operation_string: "M -> X",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "Load the index register X from memory\n\n\
        \
        LDX does not affect the C or V flags; sets Z if the value loaded was zero, otherwise resets it; sets N if\n\
        the value loaded in bit 7 is a 1; otherwise N is reset, and only affects the X register.",
        addr_mode_info_len: 5,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE          , bytecode: 0xA2, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0xAE, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE , bytecode: 0xBE, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0xA6, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ZERO_PAGE, bytecode: 0xB6, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "LDY",
        operation_summary: "Load index Y register from memory",
        operation_string: "M -> Y",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "Load the index register Y from memory\n\n\
        \
        LDX does not affect the C or V flags; sets Z if the value loaded was zero, otherwise resets it; sets N if\n\
        the value loaded in bit 7 is a 1; otherwise N is reset, and only affects the Y register.",
        addr_mode_info_len: 5,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE          , bytecode: 0xA0, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0xAC, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE , bytecode: 0xBC, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0xA4, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ZERO_PAGE, bytecode: 0xB4, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "SAX",
        operation_summary: "Store accumulator \"AND\" index register X in memory",
        operation_string: "A & X -> M",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "The undocumented SAX instruction performs a bit-by-bit AND operation of the value of the accumulator and\n\
        the value of the index register X and stores the result in memory.\n\n\
        \
        No flags or registers in the microprocessor are affected by the store operation.",
        addr_mode_info_len: 4,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x8F, num_bytes: 3, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x87, num_bytes: 2, num_cycles: "3", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ZERO_PAGE         , bytecode: 0x97, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x83, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "SHA",
        operation_summary: "Store accumulator \"AND\" index register X \"AND\" value",
        operation_string: "A & X & V -> M",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "The undocumented SHA instruction performs a bit-by-bit AND operation of the following three operands:\n\
        The first two are the accumulator and the index register X.\n\n\
        \
        The third operand depends on the addressing mode. In the zero page indirect Y-indexed case, the third\n\
        operand is the data in memory at the given zero page address (ignoring the addressing mode's Y offset)\n\
        plus 1. In the Y-indexed absolute case, it is the upper 8 bits of the given address (ignoring the addressing\n\
        mode's Y offset), plus 1.\n\n\
        \
        It then transfers the result to the addressed memory location.\n\n\
        \
        No flags or registers in the microprocessor are affected by the store operation.",
        addr_mode_info_len: 2,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x9F, num_bytes: 3, num_cycles: "5", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x93, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "SHX",
        operation_summary: "Store index register X \"AND\" value",
        operation_string: "X & (H + 1) -> M",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "The undocumented SHX instruction performs a bit-by-bit AND operation of the index register X and the\n\
        upper 8 bits of the given address (ignoring the addressing mode's Y offset), plus 1. It then transfers the\n\
        result to the addressed memory location.\n\n\
        \
        No flags or registers in the microprocessor are affected by the store operation.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE, bytecode: 0x9E, num_bytes: 3, num_cycles: "5", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "SHY",
        operation_summary: "Store index register Y \"AND\" value",
        operation_string: "Y & (H + 1) -> M",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "The undocumented SHY instruction performs a bit-by-bit AND operation of the index register Y and the\n\
        upper 8 bits of the given address (ignoring the addressing mode's X offset), plus 1. It then transfers the\n\
        result to the addressed memory location.\n\n\
        \
        No flags or registers in the microprocessor are affected by the store operation.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE, bytecode: 0x9C, num_bytes: 3, num_cycles: "5", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "STA",
        operation_summary: "Store accumulator in memory",
        operation_string: "A -> M",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction transfers the contents of the accumulator to memory\n\n\
        \
        This instruction affects none of the flags in the processor status register and does not affect the\n\
        accumulator.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x8D, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x9D, num_bytes: 3, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x99, num_bytes: 3, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x85, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x95, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x81, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x91, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "STX",
        operation_summary: "Store index X register in memory",
        operation_string: "X -> M",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction transfers the contents of the X index register to memory\n\n\
        \
        This instruction affects none of the flags in the processor status register and does not affect the\n\
        accumulator.",
        addr_mode_info_len: 3,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0x8E, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x86, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ZERO_PAGE, bytecode: 0x96, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "STY",
        operation_summary: "Store index X register in memory",
        operation_string: "Y -> M",
        category: "Load",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction transfers the contents of the Y index register to memory\n\n\
        \
        This instruction affects none of the flags in the processor status register and does not affect the\n\
        accumulator.",
        addr_mode_info_len: 3,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0x8C, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x84, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ZERO_PAGE, bytecode: 0x94, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "SHS",
        operation_summary: "Transfer Accumulator \"AND\" Index Register X to Stack Pointer then Store Stack Pointer \"AND\" Hi-Byte In Memory",
        operation_string: "A & X -> SP, SP & (H + 1) -> M",
        category: "Trans",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "The undocumented SHS instruction performs a bit-by-bit AND operation of the value of the accumulator and\n\
        the value of the index register X and stores the result in the stack pointer. It then performs a bit-by-bit AND\n\
        operation of the resulting stack pointer and the upper 8 bits of the given address (ignoring the addressing\n\
        mode's Y offset), plus 1, and transfers the result to the addressed memory location.\n\n\
        \
        No flags or registers in the microprocessor are affected by the store operation.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE, bytecode: 0x9B, num_bytes: 3, num_cycles: "5", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "TAX",
        operation_summary: "Transfer accumulator to index X",
        operation_string: "A -> X",
        category: "Trans",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction takes the value from the accumulator and transfers or loads it into the index register X\n\
        without disturbing the contents of the accumulator.\n\n\
        \
        TAX only affects the index register X, and does not affect the C or V flags. The N flag is set if the\n\
        resultant value in the index register X has bit 7 on, otherwise it is reset. The Z flag is set if the contents\n\
        of the index register X is 0 after the operation, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xAA, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "TAY",
        operation_summary: "Transfer accumulator to index register Y",
        operation_string: "A -> Y",
        category: "Trans",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction takes the value from the accumulator and transfers or loads it into the index register Y\n\
        without disturbing the contents of the accumulator.\n\n\
        \
        TAY only affects the index register Y, and does not affect the C or V flags. The N flag is set if the\n\
        resultant value in the index register Y has bit 7 on, otherwise it is reset. The Z flag is set if the contents\n\
        of the index register Y is 0 after the operation, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xA8, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "TSX",
        operation_summary: "Transfer stack pointer to index register X",
        operation_string: "SP -> X",
        category: "Trans",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction takes the value from the stack pointer and transfers or loads it into the index register X\n\
        without disturbing the contents of the accumulator.\n\n\
        \
        TAX only affects the index register X, and does not affect the C or V flags. The N flag is set if the\n\
        resultant value in the index register X has bit 7 on, otherwise it is reset. The Z flag is set if the contents\n\
        of the index register X is 0 after the operation, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xBA, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "TXA",
        operation_summary: "Transfer index register X to accumulator",
        operation_string: "X -> A",
        category: "Trans",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction takes the value from the index register X and transfers or loads it into the accumulator\n\
        without disturbing the contents of the accumulator.\n\n\
        \
        TAX only affects the accumulator, and does not affect the C or V flags. The N flag is set if the\n\
        resultant value in the accumulator has bit 7 on, otherwise it is reset. The Z flag is set if the contents\n\
        of the accumulator is 0 after the operation, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x8A, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "TXS",
        operation_summary: "Transfer index register X to stack pointer",
        operation_string: "X -> A",
        category: "Trans",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction takes the value from the index register X and transfers or loads it into the stack pointer\n\
        without disturbing the contents of the accumulator.\n\n\
        \
        TAX only affects the stack pointer, and does not affect the C or V flags. The N flag is set if the\n\
        resultant value in the stack pointer has bit 7 on, otherwise it is reset. The Z flag is set if the contents\n\
        of the stack pointer is 0 after the operation, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x9A, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "TYA",
        operation_summary: "Transfer index register Y to accumulator",
        operation_string: "Y -> A",
        category: "Trans",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction takes the value from the index register Y and transfers or loads it into the accumulator\n\
        without disturbing the contents of the accumulator.\n\n\
        \
        TAX only affects the accumulator, and does not affect the C or V flags. The N flag is set if the\n\
        resultant value in the accumulator has bit 7 on, otherwise it is reset. The Z flag is set if the contents\n\
        of the accumulator is 0 after the operation, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x98, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "PHA",
        operation_summary: "Push accumulator on stack",
        operation_string: "A|v ST, SP--",
        category: "Stack",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction transfers the current value of the accumulator to the next location on the stack,\n\
        automatically decrementing the stack pointer to point to the next vacant location.\n\n\
        \
        The PHA instruction only affects the stack pointer register which is decremented by 1 as a result of\n\
        the operation. It does not affect any flags.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x48, num_bytes: 1, num_cycles: "3", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "PHP",
        operation_summary: "Push processor status register on stack",
        operation_string: "SP|v ST, SP--",
        category: "Stack",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction transfers the current value of the processor status register to the next location on the stack,\n\
        automatically decrementing the stack pointer to point to the next vacant location.\n\n\
        \
        The PHA instruction only affects the stack pointer register which is decremented by 1 as a result of\n\
        the operation. It does not affect any flags.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x08, num_bytes: 1, num_cycles: "3", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "PLA",
        operation_summary: "Pull accumulator from stack",
        operation_string: "SP++, A|^ ST",
        category: "Stack",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction increments the stack pointer then pulls the top item\n\
        from the stack, and transfers it to the accumulator.\n\n\
        \
        The PLA instruction does not affect the C or V flags. It sets N if bit 7 in\n\
        the accumulator is on, otherwise resets it. It also sets the Z flag if\n\
        the accumulator is equal to 0, otherwise resets it. The PLA instruction changes the content\n\
        of the accumulator to the contents of the memory location at the stack register plus 1, and\n\
        also increments the stack register.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x68, num_bytes: 1, num_cycles: "4", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "PLP",
        operation_summary: "Pull processor status from stack",
        operation_string: "SP++, SP|^ ST",
        category: "Stack",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction increments the stack pointer then pulls the top item\n\
        from the stack, and transfers it to the processor status register.\n\n\
        \
        Because the instruction affects the processor status register, it\n\
        affects all of the processor flags, with the exception of the B\n\
        flag, which is never transferred over and is always reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x28, num_bytes: 1, num_cycles: "4", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "ASL",
        operation_summary: "Arithmetic shift left",
        operation_string: "C <- /M7...M0/ <- 0",
        category: "Shift",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The shift left instruction shifts either the accumulator or the address memory location 1 bit to the left,\n\
        with the bit 0 always being set to 0 and the input bit 7 being stored in the carry flag. ASL either shifts\n\
        the accumulator left 1 bit or is a read/modify/write instruction that affects only memory.\n\n\
        \
        The instruction does not affect the overflow bit, sets N equal to the result bit 7 (bit 6 in the input), sets\n\
        Z flag if the result is equal to 0, otherwise resets Z and stores the input bit 7 in the carry flag.\n\n\
        \
        NOTE: Revision A -> C of the 6502 had a fault with the ROR instruction, where it behaved more like this\n\
        instruction. Revision D fixed the issue. (see: help ROR)",
        addr_mode_info_len: 5,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ACCUMULATOR        , bytecode: 0x0A, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0x0E, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x1E, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x06, num_bytes: 2, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x16, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "LSR",
        operation_summary: "Logical shift right",
        operation_string: "0 -> /M7...M0/ -> C",
        category: "Shift",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::RESET,
        ],
        help_message: "This instruction shifts either the accumulator or a specified memory location 1 bit to the right, with\n\
        the higher bit of the result always being set to 0, and the low bit which is shifted out of the field being\n\
        stored in the carry flag.\n\n\
        \
        The shift right instruction either affects the accumulator by shift­ing it right 1 or is a read/modify/write\n\
        instruction which changes a specified memory location but does not affect any internal registers. The\n\
        shift right does not affect the overflow flag. The N flag is always reset. The Z flag is set if the result of\n\
        the shift is 0 and reset otherwise. The carry is set equal to bit 0 of the input.",
        addr_mode_info_len: 5,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ACCUMULATOR        , bytecode: 0x4A, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0x4E, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x5E, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x46, num_bytes: 2, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x56, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "ROL",
        operation_summary: "Rotate left",
        operation_string: "C <- /M7...M0/ <- C",
        category: "Shift",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The rotate left instruction shifts either the accumulator or addressed memory left 1 bit, with the input\n\
        carry being stored in bit 0 and with the input bit 7 being stored in the carry flags.\n\n\
        \
        The ROL instruction either shifts the accumulator left 1 bit and stores the carry in accumulator bit 0 or\n\
        does not affect the internal registers at all. The ROL instruction sets carry equal to the input bit 7, sets\n\
        N equal to the input bit 6 , sets the Z flag if the result of the ro­ tate is 0, otherwise it resets Z and does\n\
        not affect the overflow flag at all.",
        addr_mode_info_len: 5,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ACCUMULATOR        , bytecode: 0x2A, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0x2E, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x3E, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x26, num_bytes: 2, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x36, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "ROR",
        operation_summary: "Rotate right",
        operation_string: "C -> /M7...M0/ -> C",
        category: "Shift",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The rotate right instruction shifts either the accumulator or addressed memory right 1 bit with bit 0 shifted\n\
        into the carry and carry shifted into bit 7.\n\n\
        \
        The ROR instruction either shifts the accumulator right 1 bit and stores the carry in accumulator bit 7 or\n\
        does not affect the internal registers at all. The ROR instruction sets carry equal to input bit 0, sets N equal\n\
        to the input carry and sets the Z flag if the result of the rotate is 0; otherwise it resets Z and does not affect\n\
        the overflow flag at all.\n\
        \
        (Available on Microprocessors after June, 1976)\n\
        NOTE: Compiling with the INCLUDE_ROR_BUG flag enabled will compile the CPU with the aforementioned bug enabled.\n\n\
        \
        For context, in early revisions of the MOS 6502 (revisions before June 1976) had a bug with this instruction. It\n\
        behaved more or less like the ASL instruction (see: help ASL). The bug was finally patched in the revision D of\n\
        the chip. (see https://www.pagetable.com/?p=406 for more info.)",
        addr_mode_info_len: 5,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ACCUMULATOR        , bytecode: 0x6A, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0x6E, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x7E, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x66, num_bytes: 2, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x76, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "AND",
        operation_summary: "\"AND\" memory with accumulator",
        operation_string: "A & M -> A",
        category: "Logic",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The AND instruction transfer the accumulator and memory to the adder which performs a bit-by-bit AND\n\
        operation and stores the result back in the accumulator.\n\n\
        \
        This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise\n\
        resets the zero flag; sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the\n\
        negative flag.",
        addr_mode_info_len: 8,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0x29, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x2D, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x3D, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x39, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x25, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x35, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x21, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x31, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BIT",
        operation_summary: "Test bits in memory with accumulator",
        operation_string: "A & M, M7 -> N, M6 -> V",
        category: "Logic",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction performs an AND between a memory location and the accumulator but does not store the\n\
        result of the AND into the accumulator.\n\n\
        \
        The bit instruction affects the N flag with N being set to the value of bit 7 of the memory being tested, the V\n\
        flag with V being set equal to bit 6 of the memory being tested and Z being set by the result of the AND\n\
        operation between the accumulator and the memory if the result is Zero, Z is reset otherwise. It does not\n\
        affect the accumulator.",
        addr_mode_info_len: 2,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE , bytecode: 0x2C, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE, bytecode: 0x24, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "EOR",
        operation_summary: "\"Exclusive OR\" memory with accumulator",
        operation_string: "A ^ M -> A",
        category: "Logic",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The EOR instruction transfers the memory and the accumulator to the adder which performs a binary\n\
        \"EXCLUSIVE OR\" on a bit-by-bit basis and stores the result in the accumulator.\n\n\
        \
        This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise\n\
        resets the zero flag sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the\n\
        negative flag.",
        addr_mode_info_len: 8,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0x49, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x4D, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x5D, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x59, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x45, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x55, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x41, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x51, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "ORA",
        operation_summary: "\"OR\" memory with accumulator",
        operation_string: "A | M -> A",
        category: "Logic",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The ORA instruction transfers the memory and the accumulator to the adder which performs a binary \"OR\"\n\
        on a bit-by-bit basis and stores the result in the accumulator.\n\n\
        \
        This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise\n\
        resets the zero flag; sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the\n\
        negative flag.",
        addr_mode_info_len: 8,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0x09, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x0D, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x1D, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x19, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x05, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x15, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x01, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x11, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "ADC",
        operation_summary: "Add memory to accumulator with carry",
        operation_string: "A + M + C -> A, C",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction adds the value of memory and carry from the previous operation to the value of the\n\
        accumulator and stores the result in the accumulator.\n\n\
        \
        This instruction affects the accumulator; sets the carry flag when the sum of a binary add exceeds 255 or\n\
        when the sum of a decimal add exceeds 99, otherwise carry is reset. The overflow flag is set when the sign\n\
        or bit 7 is changed due to the result exceeding +127 or -128, otherwise overflow is reset. The negative flag\n\
        is set if the accumulator result contains bit 7 on, otherwise the negative flag is reset. The zero flag is set if\n\
        the accumulator result is 0, otherwise the zero flag is reset.\n\
        NOTE: In decimal mode, the N, V and Z flags are not consistent with the decimal result.",
        addr_mode_info_len: 8,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0x69, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x6D, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x7D, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x79, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x65, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x75, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x61, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x71, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "ANC",
        operation_summary: "\"AND\" memory with accumulator then move negative flag to carry flag",
        operation_string: "A & M -> A, N -> C",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The undocumented ANC instruction performs a bit-by-bit AND operation of the accumulator and memory\n\
        and stores the result back in the accumulator.\n\n\
        \
        This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise\n\
        resets the zero flag; sets the negative flag and the carry flag if the result in the accumulator has bit 7 on,\n\
        otherwise resets the negative flag and the carry flag.",
        addr_mode_info_len: 2,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0x0B, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0x2B, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "ARR",
        operation_summary: "\"AND\" accumulator then rotate right",
        operation_string: "(A & M) / 2 -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The undocumented ARR instruction performs a bit-by-bit \"AND\" operation of the accumulator and memory,\n\
        then shifts the result right 1 bit with bit 0 shifted into the carry and carry shifted into bit 7. It then stores the\n\
        result back in the accumulator.\n\n\
        \
        If bit 7 of the result is on, then the N flag is set, otherwise it is reset. The instruction sets the Z flag if the\n\
        result is 0; otherwise it resets Z.\n\n\
        \
        The V and C flags depends on the Decimal Mode Flag:\n\n\
        \
        In decimal mode, the V flag is set if bit 6 is different than the original data's bit 6, otherwise the V flag is\n\
        reset. The C flag is set if (operand & 0xF0) + (operand & 0x10) is greater than 0x50, otherwise the C flag is\n\
        reset.\n\n\
        \
        In binary mode, the V flag is set if bit 6 of the result is different than bit 5 of the result, otherwise the V flag is\n\
        reset. The C flag is set if the result in the accumulator has bit 6 on, otherwise it is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0x6B, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "ASR",
        operation_summary: "\"AND\" then logical shift right",
        operation_string: "(A & M) / 2 -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::RESET,
        ],
        help_message: "The undocumented ASR instruction performs a bit-by-bit AND operation of the accumulator and memory,\n\
        then shifts the accumulator 1 bit to the right, with the higher bit of the result always being set to 0, and the\n\
        low bit which is shifted out of the field being stored in the carry flag.\n\n\
        \
        This instruction affects the accumulator. It does not affect the overflow flag. The N flag is always reset. The\n\
        Z flag is set if the result of the shift is 0 and reset otherwise. The carry is set equal to bit 0 of the result of the\n\
        \"AND\" operation",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0x4B, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "CMP",
        operation_summary: "Compare memory and accumulator",
        operation_string: "A - M",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction subtracts the contents of memory from the contents of the accumulator.\n\n\
        \
        The use of the CMP affects the following flags: Z flag is set on an equal comparison, reset otherwise; the N\n\n\
        flag is set or reset by the result bit 7, the carry flag is set when the value in memory is less than or equal to\n\
        the accumulator, reset when it is greater than the accumulator. The accumulator is not affected.",
        addr_mode_info_len: 8,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0xC9, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0xCD, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0xDD, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0xD9, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0xC5, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0xD5, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0xC1, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0xD1, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "CPX",
        operation_summary: "Compare index register X to memory",
        operation_string: "X - M",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction subtracts the value of the addressed memory location from the content of index register X\n\
        using the adder but does not store the result; therefore, its only use is to set the N, Z and C flags to allow for\n\
        comparison between the index register X and the value in memory.\n\n\
        \
        The CPX instruction does not affect any register in the machine; it also does not affect the overflow flag. It\n\
        causes the carry to be set on if the absolute value of the index register X is equal to or greater than the data\n\
        from memory. If the value of the memory is greater than the content of the index register X, carry is reset. If\n\
        the results of the subtraction contain a bit 7, then the N flag is set, if not, it is reset. If the value in memory is\n\
        equal to the value in index register X, the Z flag is set, otherwise it is reset.",
        addr_mode_info_len: 3,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0xE0, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE , bytecode: 0xEC, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE, bytecode: 0xE4, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "CPY",
        operation_summary: "Compare index register Y to memory",
        operation_string: "Y - M",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction performs a two's complement subtraction between the index register Y and the specified\n\
        memory location. The results of the subtraction are not stored anywhere. The instruction is strictly used to\n\
        set the flags\n\n\
        \
        CPY affects no registers in the microprocessor and also does not affect the overflow flag. If the value in the\n\
        index register Y is equal to or greater than the value in the memory, the carry flag will be set, otherwise it will\n\
        be cleared. If the results of the subtraction contain bit 7 on the N bit will be set, otherwise it will be cleared.\n\
        If the value in the index register Y and the value in the memory are equal, the zero flag will be set, otherwise\n\
        it will be cleared",
        addr_mode_info_len: 3,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0xC0, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE , bytecode: 0xCC, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE, bytecode: 0xC4, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "DCP",
        operation_summary: "Decrement memory by one then compare with accumulator",
        operation_string: "M - 1 ->, A - M",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This undocumented instruction subtracts 1, in two's complement, from the contents of the addressed\n\
        memory location. It then subtracts the contents of memory from the contents of the accumulator.\n\n\
        \
        The DCP instruction does not affect any internal register in the microprocessor. It does not affect the\n\
        overflow flag. Z flag is set on an equal comparison, reset otherwise; the N flag is set or reset by the result bit\n\
        7, the carry flag is set when the result in memory is less than or equal to the accumulator, reset when it is\n\
        greater than the accumulator.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0xCF, num_bytes: 3, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0xDF, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0xDB, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0xC7, num_bytes: 2, num_cycles: "5", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0xD7, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0xC3, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0xD3, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "ISC",
        operation_summary: "Increment Memory By One then SBC then Subtract Memory from Accumulator with Borrow",
        operation_string: "M + 1 -> M, A - M -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This undocumented instruction adds 1 to the contents of the addressed memory location. It then subtracts\n\
        the value of the result in memory and borrow from the value of the accumulator, using two's complement\n\
        arithmetic, and stores the result in the accumulator.\n\n\
        \
        This instruction affects the accumulator. The carry flag is set if the result is greater than or equal to 0. The\n\
        carry flag is reset when the result is less than 0, indicating a borrow. The overflow flag is set when the result\n\
        exceeds +127 or -127, otherwise it is reset. The negative flag is set if the result in the accumulator has bit 7\n\
        on, otherwise it is reset. The Z flag is set if the result in the accumulator is 0, otherwise it is reset.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0xEF, num_bytes: 3, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0xFF, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0xFB, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0xE7, num_bytes: 2, num_cycles: "5", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0xF7, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0xE3, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0xF3, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "RLA",
        operation_summary: "Rotate left then \"AND\" with accumulator",
        operation_string: "C <- /M7...M0/ <- C, A & M -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The undocumented RLA instruction shifts the addressed memory left 1 bit, with the input carry being stored\n\
        in bit 0 and with the input bit 7 being stored in the carry flags. It then performs a bit-by-bit AND operation of\n\
        the result and the value of the accumulator and stores the result back in the accumulator.\n\n\
        \
        This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise\n\
        resets the zero flag; sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the\n\
        negative flag.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x2F, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x3F, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x3B, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x27, num_bytes: 2, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x37, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x23, num_bytes: 2, num_cycles: "8", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x33, num_bytes: 2, num_cycles: "8", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "RRA",
        operation_summary: "Rotate right then add memory with accumulator",
        operation_string: "C -> /M7...M0/ -> C, A + M + C -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The undocumented RRA instruction shifts the addressed memory right 1 bit with bit 0 shifted into the carry\n\
        and carry shifted into bit 7. It then adds the result and generated carry to the value of the accumulator and\n\
        stores the result in the accumulator.\n\n\
        \
        This instruction affects the accumulator; sets the carry flag when the sum of a binary add exceeds 255 or\n\
        when the sum of a decimal add exceeds 99, otherwise carry is reset. The overflow flag is set when the sign\n\
        or bit 7 is changed due to the result exceeding +127 or -128, otherwise overflow is reset. The negative flag\n\
        is set if the accumulator result contains bit 7 on, otherwise the negative flag is reset. The zero flag is set if\n\
        the accumulator result is 0, otherwise the zero flag is reset.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x6F, num_bytes: 3, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x7F, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x7B, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x67, num_bytes: 2, num_cycles: "5", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x77, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x63, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x73, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "SBC",
        operation_summary: "Subtract memory from accumulator with borrow",
        operation_string: "A - M - ~C -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction subtracts the value of memory and borrow from the value of the accumulator, using two's\n\
        complement arithmetic, and stores the result in the accumulator. Borrow is defined as the carry flag\n\
        complemented; therefore, a resultant carry flag indicates that a borrow has not occurred.\n\n\
        \
        This instruction affects the accumulator. The carry flag is set if the result is greater than or equal to 0. The\n\
        carry flag is reset when the result is less than 0, indicating a borrow. The overflow flag is set when the result\n\
        exceeds +127 or -127, otherwise it is reset. The negative flag is set if the result in the accumulator has bit 7\n\
        on, otherwise it is reset. The Z flag is set if the result in the accumulator is 0, otherwise it is reset.\n\
        NOTE: In decimal mode, the N, V and Z flags are not consistent with the decimal result.",
        addr_mode_info_len: 9,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0xE9, num_bytes: 2, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE                   , bytecode: 0xEB, num_bytes: 2, num_cycles: "2", undocumented_instruction:  true},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0xED, num_bytes: 3, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0xFD, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0xF9, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0xE5, num_bytes: 2, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0xF5, num_bytes: 2, num_cycles: "4", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0xE1, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0xF1, num_bytes: 2, num_cycles: "5+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "SBX",
        operation_summary: "Subtract memory from accumulator \"AND\" index register X",
        operation_string: "(A & X) - M -> X",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This undocumented instruction performs a bit-by-bit \"AND\" of the value of the accumulator and the index\n\
        register X and subtracts the value of memory from this result, using two's complement arithmetic, and stores\n\
        the result in the index register X.\n\n\
        \
        This instruction affects the index register X. The carry flag is set if the result is greater than or equal to 0.\n\
        The carry flag is reset when the result is less than 0, indicating a borrow. The negative flag is set if the result\n\
        in index register X has bit 7 on, otherwise it is reset. The Z flag is set if the result in index register X is 0,\n\
        otherwise it is reset. The overflow flag not affected at all.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0xCB, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "SLO",
        operation_summary: "Arithmetic shift left then \"OR\" memory with accumulator",
        operation_string: "M * 2 -> M, A | M -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The undocumented SLO instruction shifts the address memory location 1 bit to the left, with the bit 0 always\n\
        being set to 0 and the bit 7 output always being contained in the carry flag. It then performs a bit-by-bit \"OR\"\n\
        operation on the result and the accumulator and stores the result in the accumulator.\n\n\
        \
        The negative flag is set if the accumulator result contains bit 7 on, otherwise the negative flag is reset. It sets\n\
        Z flag if the result is equal to 0, otherwise resets Z and stores the input bit 7 in the carry flag.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x0F, num_bytes: 3, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x1F, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x1B, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x07, num_bytes: 2, num_cycles: "5", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x17, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x03, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x13, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "SRE",
        operation_summary: "Logical shift right then \"Exclusive OR\" memory with accumulator",
        operation_string: "M / 2 -> M, A ^ M -> A",
        category: "Arith",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The undocumented SRE instruction shifts the specified memory location 1 bit to the right, with the higher bit\n\
        of the result always being set to 0, and the low bit which is shifted out of the field being stored in the carry\n\
        flag. It then performs a bit-by-bit \"EXCLUSIVE OR\" of the result and the value of the accumulator and stores\n\
        the result in the accumulator.\n\n\
        \
        This instruction affects the accumulator. It does not affect the overflow flag. The negative flag is set if the\n\
        accumulator result contains bit 7 on, otherwise the negative flag is reset. The Z flag is set if the result is 0\n\
        and reset otherwise. The carry is set equal to input bit 0.",
        addr_mode_info_len: 7,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE                    , bytecode: 0x0F, num_bytes: 3, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE          , bytecode: 0x1F, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::Y_INDEXED_ABSOLUTE          , bytecode: 0x1B, num_bytes: 3, num_cycles: "7", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE                   , bytecode: 0x07, num_bytes: 2, num_cycles: "5", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE         , bytecode: 0x17, num_bytes: 2, num_cycles: "6", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE_INDIRECT, bytecode: 0x03, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE_INDIRECT_Y_INDEXED, bytecode: 0x13, num_bytes: 2, num_cycles: "8", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "XXA",
        operation_summary: "Non-deterministic operation of accumulator, index register X, memory, and bus contents",
        operation_string: "(A & Q) & X & M -> A",
        category: "Arith",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "The operation of the undocumented XAA instruction depends on the individual microprocessor. On most\n\
        machines, it performs a bit-by-bit AND operation of the following three operands: The first two are the index\n\
        register X and memory.\n\n\
        \
        The third operand is the result of a bit-by-bit AND operation of the accumulator and a magic component.\n\
        This magic component depends on the individual microprocessor and is usually one of $00, $EE, $EF, $FE\n\
        and $FF, and may be influenced by the RDY pin, leftover contents of the data bus, the temperature of the\n\
        microprocessor, the supplied voltage, and other factors.\n\n\
        \
        On some machines, additional bits of the result may be set or reset depending on non-deterministic factors.\n\n\
        \
        It then transfers the result to the accumulator.\n\n\
        \
        XAA does not affect the C or V flags; sets Z if the value loaded was zero, otherwise resets it; sets N if the\n\
        result in bit 7 is a 1; otherwise N is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE, bytecode: 0x8B, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "DEC",
        operation_summary: "Decrement memory by one",
        operation_string: "M - 1 -> M",
        category: "Inc",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction subtracts 1, in two's complement, from the contents of the addressed memory location.\n\n\
        \
        The decrement instruction does not affect any internal register in the microprocessor. It does not affect the\n\
        carry or overflow flags. If bit 7 is on as a result of the decrement, then the N flag is set, otherwise it is reset.\n\
        If the result of the decrement is 0, the Z flag is set, otherwise it is reset.",
        addr_mode_info_len: 4,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0xCE, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0xDE, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0xC6, num_bytes: 2, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0xD6, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "DEX",
        operation_summary: "Decrement index register X by one",
        operation_string: "X - 1 -> X",
        category: "Inc",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction subtracts one from the current value of the index register X and stores the result in the index\n\
        register X\n\n\
        \
        DEX does not affect the carry or overflow flag, it sets the N flag if it has bit 7 on as a result of the decrement,\n\
        otherwise it resets the N flag; sets the Z flag if X is a 0 as a result of the decrement, otherwise it resets the Z\n\
        flag.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xCA, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "DEY",
        operation_summary: "Decrement index register Y by one",
        operation_string: "Y - 1 -> Y",
        category: "Inc",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction subtracts one from the current value in the index register Y and stores the result into the\n\
        index register Y. The result does not affect or consider carry so that the value in the index register Y is\n\
        decremented to 0 and then through 0 to FF.\n\n\
        \
        Decrement Y does not affect the carry or overflow flags; if the Y register contains bit 7 on as a result of the\n\
        decrement the N flag is set, otherwise the N flag is reset. If the Y register is 0 as a result of the decrement,\n\
        the Z flag is set otherwise the Z flag is reset. This instruction only affects the index register Y.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x88, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "INC",
        operation_summary: "Increment memory by one",
        operation_string: "M + 1 -> M",
        category: "Inc",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction adds 1 to the contents of the addressed memory location.\n\n\
        \
        The increment memory instruction does not affect any internal registers and does not affect the carry or\n\
        overflow flags. If bit 7 is on as the result of the increment,N is set, otherwise it is reset; if the increment\n\
        causes the result to become 0, the Z flag is set on, otherwise it is reset.",
        addr_mode_info_len: 4,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0xEE, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0xFE, num_bytes: 3, num_cycles: "7", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0xE6, num_bytes: 2, num_cycles: "5", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0xF6, num_bytes: 2, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "INX",
        operation_summary: "Increment index register X by one",
        operation_string: "X + 1 -> X",
        category: "Inc",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "Increment X adds 1 to the current value of the X register. This is an 8-bit increment which does not affect the\n\
        carry operation, therefore, if the value of X before the increment was FF, the resulting value is 00.\n\n\
        \
        INX does not affect the carry or overflow flags; it sets the N flag if the result of the increment has a one in bit\n\
        7, otherwise resets N; sets the Z flag if the result of the increment is 0, otherwise it resets the Z flag.\n\n\
        \
        INX does not affect any other register other than the X register.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xE8, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "INY",
        operation_summary: "Increment index register Y by one",
        operation_string: "Y + 1 -> Y",
        category: "Inc",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "Increment Y increments or adds one to the current value in the Y register, storing the result in the Y register.\n\
        As in the case of INX the primary application is to step thru a set of values using the Y register.\n\n\
        \
        The INY does not affect the carry or overflow flags, sets the N flag if the result of the increment has a one in\n\
        bit 7, otherwise resets N, sets Z if as a result of the increment the Y register is zero otherwise resets the Z\n\
        flag.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xC8, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BRK",
        operation_summary: "Software breakpoint command",
        operation_string: "PC + 2|v ST, SP--, [FFFE] -> PCL, [FFFF] -> PCH",
        category: "Ctrl",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::INITIALIZED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "The break command causes the microprocessor to go through an interrupt sequence under program\n\
        control. This means that the program counter of the second byte after the BRK. is automatically stored on\n\
        the stack along with the processor status at the beginning of the break instruction. The microprocessor then\n\
        transfers control to the interrupt vector.\n\n\
        \
        Other than changing the program counter, the break instruction changes no values in either the registers or\n\
        the flags.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x00, num_bytes: 1, num_cycles: "7", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "JMP",
        operation_summary: "JMP indirect",
        operation_string: "[PC + 1] -> PCL, [PC + 2] -> PCH",
        category: "Ctrl",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction establishes a new value for the program counter.\n\n\
        \
        It affects only the program counter in the microprocessor and affects no flags in the status register.",
        addr_mode_info_len: 2,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE         , bytecode: 0x4C, num_bytes: 3, num_cycles: "3", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE_INDIRECT, bytecode: 0x6C, num_bytes: 3, num_cycles: "5", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "JSR",
        operation_summary: "Jump to subroutine",
        operation_string: "PC + 2|v ST, SP--, [PC + 1] -> PCL, [PC + 2] -> PCH",
        category: "Ctrl",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction transfers control of the program counter to a subroutine location but leaves a return pointer\n\
        on the stack to allow the user to return to perform the next instruction in the main program after the\n\
        subroutine is complete. To accomplish this, JSR instruction stores the program counter address which\n\
        points to the last byte of the jump instruction onto the stack using the stack pointer. The stack byte contains\n\
        the program count high first, followed by program count low. The JSR then transfers the addresses following\n\
        the jump instruction to the program counter low and the program counter high, thereby directing the program\n\
        to begin at that new address.\n\n\
        \
        The JSR instruction affects no flags, causes the stack pointer to be decremented by 2 and substitutes new\n\
        values into the program counter low and the program counter high.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE, bytecode: 0x20, num_bytes: 3, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "RTI",
        operation_summary: "Return from interrupt",
        operation_string: "SR|v ST, SP--, PC + 2|v ST, SP--,",
        category: "Ctrl",
        affected_flags: [
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::AFFECTED,
            Flag::AFFECTED,
        ],
        help_message: "This instruction transfers from the stack into the microprocessor the processor status and the program\n\
        counter location for the instruction which was interrupted. By virtue of the interrupt having stored this data \n\
        before executing the instruction and the fact that the RTI reinitializes the microprocessor to the same state\n\
        as when it was interrupted, the combination of interrupt plus RTI allows truly reentrant coding.\n\n\
        \
        The RTI instruction reinitializes all flags to the position to the point they were at the time the interrupt was\n\
        taken and sets the program counter back to its pre-interrupt state. It affects no other registers in the\n\
        microprocessor.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x40, num_bytes: 1, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "RTS",
        operation_summary: "Return from subroutine",
        operation_string: "SR|v ST, SP--, PC + 1 -> PC",
        category: "Ctrl",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction loads the program count low and program count high from the stack into the program\n\
        counter and increments the program counter so that it points to the instruction following the JSR. The stack\n\
        pointer is adjusted by incrementing it twice.\n\n\
        \
        The RTS instruction does not affect any flags and affects only PCL and PCH.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x60, num_bytes: 1, num_cycles: "6", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BCC",
        operation_summary: "Branch on carry clear",
        operation_string:"Branch if C == 0",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction tests the state of the carry bit and takes a conditional branch if the carry bit is reset.\n\n\
        \
        It affects no flags or registers other than the program counter and then only if the C flag is not on.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0x90, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BCS",
        operation_summary: "Branch on carry set",
        operation_string: "Branch if C == 1",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction takes the conditional branch if the carry flag is on.\n\n\
        \
        BCS does not affect any of the flags or registers except for the program counter and only then if the carry\n\
        flag is on.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0xB0, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BEQ",
        operation_summary: "Branch on result zero",
        operation_string: "Branch if Z == 1",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction could also be called \"Branch on Equal.\"\n\n\
        \
        It takes a conditional branch whenever the Z flag is on or the previous result is equal to 0.\n\n\
        \
        BEQ does not affect any of the flags or registers other than the program counter and only then when the Z\n\
        flag is set.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0xF0, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BMI",
        operation_summary: "Branch on result minus",
        operation_string: "Branch if N == 1",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction takes the conditional branch if the N bit is set.\n\n\
        \
        BMI does not affect any of the flags or any other part of the machine other than the program counter and\n\
        then only if the N bit is on.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0x30, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BNE",
        operation_summary: "Branch on result not zero",
        operation_string: "Branch if Z == 0",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction could also be called \"Branch on Not Equal.\" It tests the Z flag and takes the conditional\n\
        branch if the Z flag is not on, indicating that the previous result was not zero.\n\n\
        \
        BNE does not affect any of the flags or registers other than the program counter and only then if the Z flag is\n\
        reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0xD0, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BPL",
        operation_summary: "Branch on result plus",
        operation_string: "Branch if N == 0",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction is the complementary branch to branch on result minus. It is a conditional branch which\n\
        takes the branch when the N bit is reset (0). BPL is used to test if the previous result bit 7 was off (0) and\n\
        branch on result minus is used to determine if the previous result was minus or bit 7 was on (1).\n\n\
        \
        The instruction affects no flags or other registers other than the P counter and only affects the P counter\n\
        when the N bit is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0x10, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BVC",
        operation_summary: "Branch on overflow clear",
        operation_string: "Branch if V == 0",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction tests the status of the V flag and takes the conditional branch if the flag is not set.\n\n\
        \
        BVC does not affect any of the flags and registers other than the program counter and only when the\n\
        overflow flag is reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0x50, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "BVS",
        operation_summary: "Branch on overflow set",
        operation_string: "Branch if V == 1",
        category: "Bra",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction tests the V flag and takes the conditional branch if V is on.\n\n\
        \
        BVS does not affect any flags or registers other than the program, counter and only when the overflow flag\n\
        is set.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::RELATIVE, bytecode: 0x70, num_bytes: 2, num_cycles: "2+t+p", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "CLC",
        operation_summary: "Clear carry flag",
        operation_string: "0 -> C",
        category: "Flags",
        affected_flags: [
            Flag::RESET,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction initializes the carry flag to a 0. This operation should normally precede an ADC loop. It is\n\
        also useful when used with a R0L instruction to clear a bit in memory.\n\n\
        \
        This instruction affects no registers in the microprocessor and no flags other than the carry flag which is\n\
        reset.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x18, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "CLD",
        operation_summary: "Clear decimal mode flag",
        operation_string: "0 -> D",
        category: "Flags",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::RESET,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction sets the decimal mode flag to a 0. This all subsequent ADC and SBC instructions to operate\n\
        as simple operations.\n\n\
        \
        CLD affects no registers in the microprocessor and no flags other than the decimal mode flag which is set to\n\
        a 0.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xD8, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "CLI",
        operation_summary: "Clear interrupt disable flag",
        operation_string: "0 -> I",
        category: "Flags",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::RESET,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction initializes the interrupt disable to a 0. This allows the microprocessor to receive interrupts.\n\n\
        \
        It affects no registers in the microprocessor and no flags other than the interrupt disable which is cleared.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x58, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "CLV",
        operation_summary: "Clear overflow flag",
        operation_string: "0 -> V",
        category: "Flags",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::RESET,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction clears the overflow flag to a 0. This command is used in conjunction with the set overflow\n\
        pin which can change the state of the overflow flag with an external signal.\n\n\
        \
        CLV affects no registers in the microprocessor and no flags other than the overflow flag which is set to a 0.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xB8, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "SEC",
        operation_summary: "Set carry flag",
        operation_string: "1 -> C",
        category: "Flags",
        affected_flags: [
            Flag::INITIALIZED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction initializes the carry flag to a 1. This operation should normally precede a SBC loop. It is also\n\
        useful when used with a ROL instruction to initialize a bit in memory to a 1.\n\n\
        \
        This instruction affects no registers in the microprocessor and no flags other than the carry flag which is set.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x38, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "SED",
        operation_summary: "Set decimal mode flag",
        operation_string: "1 -> D",
        category: "Flags",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::INITIALIZED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction sets the decimal mode flag D to a 1. This makes all subsequent ADC and SBC instructions\n\
        operate as a decimal arithmetic operation.\n\n\
        \
        SED affects no registers in the microprocessor and no flags other than the decimal mode which is set to a 1.\n\
        NOTE: The value of this flag is indeterminate after a RESET",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xF8, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "SEI",
        operation_summary: "Set interrupt disable flag",
        operation_string: "1 -> I",
        category: "Flags",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::INITIALIZED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This instruction initializes the interrupt disable to a 1. It is used to mask interrupt requests during system\n\
        reset operations and during interrupt commands.\n\n\
        \
        It affects no registers in the microprocessor and no flags other than the interrupt disable which is set.",
        addr_mode_info_len: 1,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x78, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
        ]
    },
    help_message { mnemonic: "NOP",
        operation_summary: "No operation",
        operation_string: "No operation",
        category: "Nop",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "The name says it all. This command does nothing but waste time for 2 instructions. Can be used to align commands\n\
        up to execute on a certain clock cycle, also has undocumented variants that take arguments, but do nothing with them\\n
        but take more cycles to run",
        addr_mode_info_len: 8,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED            , bytecode: 0x1A, num_bytes: 1, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED            , bytecode: 0x3A, num_bytes: 1, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED            , bytecode: 0x5A, num_bytes: 1, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED            , bytecode: 0x7A, num_bytes: 1, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED            , bytecode: 0xDA, num_bytes: 1, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED            , bytecode: 0xEA, num_bytes: 1, num_cycles: "2", undocumented_instruction: false},
            addr_mode_info {addr_mode: AddressModes::IMPLIED            , bytecode: 0xFA, num_bytes: 1, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE          , bytecode: 0x80, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE          , bytecode: 0x82, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE          , bytecode: 0x89, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE          , bytecode: 0xC2, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMMEDIATE          , bytecode: 0xE2, num_bytes: 2, num_cycles: "2", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ABSOLUTE           , bytecode: 0x0C, num_bytes: 3, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x1C, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x3C, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x5C, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0x7C, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0xDC, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ABSOLUTE , bytecode: 0xFC, num_bytes: 3, num_cycles: "4+p", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x04, num_bytes: 2, num_cycles: "3", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x44, num_bytes: 2, num_cycles: "3", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::ZERO_PAGE          , bytecode: 0x64, num_bytes: 2, num_cycles: "3", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x14, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x34, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x54, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0x74, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0xD4, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::X_INDEXED_ZERO_PAGE, bytecode: 0xF4, num_bytes: 2, num_cycles: "4", undocumented_instruction: true},
        ]
    },
    help_message { mnemonic: "JAM",
        operation_summary: "Nul",
        operation_string: "CPU <- ded",
        category: "Kil",
        affected_flags: [
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
            Flag::NOT_AFFECTED,
        ],
        help_message: "This undocumented instruction stops execution. The microprocessor will not fetch further instructions, and\
        will neither handle IRQs (Interrupt Requests), nor NMIs (Non-Maskable Interrupt). It will handle a RESET though.",
        addr_mode_info_len: 2,
        addressing_modes: vec![
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x02, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x12, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x22, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x32, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x42, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x52, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x62, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x72, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0x92, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xB2, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xD2, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
            addr_mode_info {addr_mode: AddressModes::IMPLIED, bytecode: 0xF2, num_bytes: 1, num_cycles: "X", undocumented_instruction: true},
        ]
    },

]).unwrap();
}