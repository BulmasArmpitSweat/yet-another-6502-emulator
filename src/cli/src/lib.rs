#![allow(unreachable_code)]
mod include;
mod dump;
mod addr_modes;
mod registers;
mod memory;
mod help;
mod instruction;

extern "C" {
    static context: include::cpu_context;
}

struct Funcs {
    main_fn: fn(argc: i8, argv: Vec<String>, cpu: &include::cpu_context) -> Result<(), std::fmt::Error>,
    help: fn() -> (),
    version: fn() -> (),
    usage: fn() -> (),
}

#[no_mangle]
pub extern "C" fn cli(argc: i8, argv: *const *const i8) -> () {
    if argc < 2 {
        err!("Missing arguments");
        wontreturn!();
    }

    let args: Vec<String> = unsafe { std::slice::from_raw_parts(argv, argc as usize) }.iter().map(|&s| unsafe { std::ffi::CStr::from_ptr(s) }.to_str().unwrap().to_string()).collect();

    let mut fn_pointers: std::collections::HashMap<String, Funcs> = std::collections::HashMap::new();
    fn_pointers.insert(String::from("dump"),      Funcs { main_fn: dump::dump,           help: dump::Help,      version: dump::Version,      usage: dump::Usage });
    fn_pointers.insert(String::from("registers"), Funcs { main_fn: registers::registers, help: registers::Help, version: registers::Version, usage: registers::Usage });
    fn_pointers.insert(String::from("memory"),    Funcs { main_fn: memory::memory,       help: memory::Help,    version: memory::Version,    usage: memory::Usage });
    fn_pointers.insert(String::from("help"),      Funcs { main_fn: help::help,           help: help::Help,      version: help::Version,      usage: help::Usage });
    
    let command = args[1].clone();
    let command_func = fn_pointers.get(&command);
    if command_func.is_none() {
        warn!("Unknown command: '{}'", command);
        return ();
    }

    if args.len() > 2 {
        if args.contains(&"--help".to_string()) || args.contains(&"-h".to_string()) {
            (command_func.unwrap().help)();
        } else if args.contains(&"--version".to_string()) || args.contains(&"-v".to_string()) {
            (command_func.unwrap().version)();
        } else if args.contains(&"--usage".to_string()) || args.contains(&"-u".to_string()) {
            (command_func.unwrap().usage)();
        } else {
            match (command_func.unwrap().main_fn)(argc - 2, args[2..].to_vec(), unsafe { &context }) {
                Ok(()) => (),
                Err(e) => warn!("Command exited with error condition: {}", e),
            }
        }
    }
    return ();
}