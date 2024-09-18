#! /bin/bash

srcs=(src/cli/command_arguments \
    src/cli/instruction_help \
    src/cpu/stack_manip \
    src/cpu/instructions/arith \
    src/cpu/instructions/bra \
    src/cpu/instructions/ctrl \
    src/cpu/instructions/flags \
    src/cpu/instructions/inc \
    src/cpu/instructions/kil \
    src/cpu/instructions/load \
    src/cpu/instructions/logic \
    src/cpu/instructions/nop \
    src/cpu/instructions/shift \
    src/cpu/instructions/stack \
    src/cpu/instructions/trans \
    src/cpu/flags \
    src/cpu/init \
    src/cpu/mem \
    src/cpu/rotate \
    src/cpu/split \
    src/cpu/thread \
    src/cli/menu)

if [[ "${1}" == "clean" ]]; then
    printf "CLEAN .\n"
    find src/ -name "*.o" -type f -delete
    rm build/gettimeofday
    read -r -p "Clear build cache? (y/n) " choice
    case "$choice" in 
      y|Y ) find bin/ -type f -executable -exec rm -rf {} +;;
      n|N ) exit 0;;
      * ) printf "Not a valid input, kiddo.\n"; exit 1;;
    esac
elif [[ "${1}" == "build" ]]; then
    cd build || exit 1
    if ! find . -maxdepth 1 -wholename "gettimeofday" | grep -q .; then
        gcc gettimeofday.c -o gettimeofday
    fi
    cd .. || exit 1
    for item in "${srcs[@]}"; do
        obj_file="${item}.o"
        if ! [[ -f "${obj_file}" ]]; then
            printf "CC %s\n" "${obj_file}"
            gcc -c "${item}.c" -o "${obj_file}"
        fi
    done
    printf "CC timu6502asm\n"
    cd ext_lib/timu6502asm || exit 1
    cargo build --release
    cp target/release/timu6502asm ../../bin/timu6502asm
    printf "COPY /ext_lib/timu6502asm/target/release/timu6502asm -> ./bin/timu6502asm\n"
    cargo clean
    cd ../../ || exit 1
    time=$(./build/gettimeofday)
    printf "LD %s\n" "bin/6502_emu${time}"
    gcc src/main.c \
    src/cli/command_arguments.o \
    src/cli/instruction_help.o \
    src/cpu/stack_manip.o \
    src/cpu/instructions/arith.o \
    src/cpu/instructions/bra.o \
    src/cpu/instructions/ctrl.o \
    src/cpu/instructions/flags.o \
    src/cpu/instructions/inc.o \
    src/cpu/instructions/kil.o \
    src/cpu/instructions/load.o \
    src/cpu/instructions/logic.o \
    src/cpu/instructions/nop.o \
    src/cpu/instructions/shift.o \
    src/cpu/instructions/stack.o \
    src/cpu/instructions/trans.o \
    src/cpu/flags.o \
    src/cpu/init.o \
    src/cpu/mem.o \
    src/cpu/rotate.o \
    src/cpu/split.o \
    src/cpu/thread.o \
    src/cli/menu.o -o "bin/6502_emu${time}"
elif [[ "${1}" == "menuconfig" ]]; then
    ./build/menuconfig.sh
else
    exit 1
fi
exit 0