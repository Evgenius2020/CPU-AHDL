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
.variable res 6

// Code
load res
