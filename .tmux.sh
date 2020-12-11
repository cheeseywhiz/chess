#!/bin/bash
set -Eeuox pipefail

# terminal
tmux split-window -h -l 87
tmux send 'git status' 'C-j'

# debugger
tmux split-window
tmux send 'sudo gdb -p '

# editor
tmux select-pane -t 1
tmux send vim 'C-j'
tmux send '\c'
tmux send ':e chess.cpp' 'C-j'
tmux send ':vs chess.hpp' 'C-j'
tmux send ':sp draw.hpp' 'C-j'
tmux send 'C-h'
tmux send ':sp draw.cpp' 'C-j'
tmux send ':e main.cpp' 'C-j'
