#!/bin/bash

CONFIG_FILE="../src/config.h"

dialog --checklist "6502 Emulator build options:" 15 40 5 \
    1 "Enable pre-June 1976 ROR bug" off \
    2 "Enable undocumented 6502 instructions" off 2>tempfile

choices=$(<tempfile)
rm tempfile

rm "${CONFIG_FILE}"

printf "#ifndef FD1B647D_B794_4E19_A02E_B74CE37E0138\n\
#define FD1B647D_B794_4E19_A02E_B74CE37E0138\n\n" > "${CONFIG_FILE}"

if [[ ${#choices[@]} == "0" ]]; then
    printf "#endif /* FD1B647D_B794_4E19_A02E_B74CE37E0138 */" >> "${CONFIG_FILE}"
    exit 0
else
    for((i=0;i<${#choices[@]};i++)); do
        case "${choices[i]}" in
            1)
                printf "#define INCLUDE_ROR_BUG\n" > "${CONFIG_FILE}"
            ;;
            2)
                printf "#define INCLUDE_ILLEGAL_OPCODES\n" > "${CONFIG_FILE}"
            ;;
            *)
                exit 1
            ;;
        esac
    done
fi