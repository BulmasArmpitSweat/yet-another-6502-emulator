#! /bin/bash

srcs=(src/cpu/stack-manip \
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

# dirs=(src/cpu \
#     src/cpu/instructions \
#     src/cli)

if [[ "${0}" == "clean" ]]; then
    printf "Cleaning directory structure...\n"
    for item in "${srcs[@]}"; do
        if find . -maxdepth 1 -name "${item}.o" | grep -q .; then
            rm "${item}.o"
        fi
    done
elif [[ "${0}" == "build" ]]; then
    index=0
    for item in "${srcs[@]}"; do
        if find . -maxdepth 1 -name "${item}.o" | grep -q .; then
            true
        else
            printf "\r[%d|%d] building %s.c..." ${index} ${#srcs[@]} "${srcs[index]}"
            gcc "${item}.c" -o "${item}$(./build/gettimeofday).o"
        fi
        (( index += 1 ))
    done

    printf "\nLinking executable..."
    gcc "${srcs[*]}.o" -o "6502_emu"
else
    exit 1
fi