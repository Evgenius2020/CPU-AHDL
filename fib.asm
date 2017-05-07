res = 0;
prev = 1;
while (n--) {
    res += prev;
    curr = prev;
    prev = res;
    res = curr;
}