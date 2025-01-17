#!/bin/bash
set --debug

CONFIG_FILE="../src/config.h"

HEIGHT="15"
WIDTH="50"
LIST_HEIGHT="5"

# tempfile=$(mktemp)

dialog --checklist "6502 Emulator build options:" "${HEIGHT}" "${WIDTH}" "${LIST_HEIGHT}" \
    1 "Enable pre-June 1976 ROR bug" off \
    2 "Enable undocumented 6502 instructions" off 2>tempfile

choices=$(cat tempfile)
printf "${choices}"
rm tempfile

printf "#ifndef FD1B647D_B794_4E19_A02E_B74CE37E0138\n\
#define FD1B647D_B794_4E19_A02E_B74CE37E0138\n\n" > "${CONFIG_FILE}"
IFS=' '
read -r -a choices <<< "$choices"

for((i=0;i<${#choices[@]};i++)); do
    case "${choices[i]}" in
        1)
            printf "#define INCLUDE_ROR_BUG\n" >> "${CONFIG_FILE}"
        ;;
        2)
            printf "#define INCLUDE_ILLEGAL_OPCODES\n" >> "${CONFIG_FILE}"
        ;;
        *)
            exit 1
        ;;
    esac
done

printf "#endif /* FD1B647D_B794_4E19_A02E_B74CE37E0138 */" >> "${CONFIG_FILE}"