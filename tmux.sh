#!/bin/bash
set -Eeuxo pipefail

# api server
tmux split-window -h -l 87
tmux send 'cd api' 'C-j'
tmux send '. bin/activate.sh' 'C-j'
tmux send 'cd build' 'C-j'
tmux send 'C-l' 'make -j5 && gdb -ex r ./chess' 'C-j'

# api terminal
tmux split-window
tmux send 'cd api' 'C-j'
tmux send '. bin/activate.sh' 'C-j'
tmux send 'C-l' 'http --json --session=./session.json http://0.0.0.0:8080/api/AuthCtrl/login username=spencer'

# front server
tmux split-window -t 1 -l 20%
tmux send 'cd front' 'C-j'
tmux send 'C-l' 'npm run start' 'C-j'

# nginx server
tmux split-window -h
tmux send 'cd nginx' 'C-j'
tmux send 'C-l' './nginx.sh start' 'C-j'

# editor
tmux select-pane -t 1
tmux send 'C-l'
