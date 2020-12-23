#!/bin/bash
export PS1="(api) ${PS1}"
export PATH="${PATH}:$(realpath deps/bin)"
export PATH="${PATH}:$(realpath bin)"

function deactivate() {
    function get_path() {
        cat <<EOF | python -
import os
print(':'.join(
    dir
    for dir in os.environ['PATH'].split(':')
    if 'chess' not in dir
))
EOF
    }

    export PS1="${PS1##(api) }"
    export PATH="$(get_path)"
    unset deactivate
}
