// Commands code binding
.cmd add 1 addr
.cmd sub 2 addr
.cmd load 3 addr
.cmd store 4 addr
.cmd set 5 num
.cmd jmp 6 addr
.cmd jz 7 addr

// Variables definition
.variable res

// Code
.start
set 1
store res
jmp .start
