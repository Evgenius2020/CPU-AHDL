//  fib(n) {
//  	n -=2;    
//  	res = 1;
//  	prev = 1;
//  	while(n > 0) {
//		    n--;
//		    buf = res;
//		    res = prev;
//		    prev = buf;
//		    res += prev;
//      }
//	    return res;
// }

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
.variable prev
.variable n
.variable buf

// Code

set 2
store buf
load n
sub buf
store n

set 1
store res

set 1
store prev

.cycle
load n
jz .exit

set 1
store buf
load n
sub buf
store n

load res
store buf

load prev
store res

load buf
store prev

add res
store res

jmp .cycle

.exit
load res
jmp .exit
