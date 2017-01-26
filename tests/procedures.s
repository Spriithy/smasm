jmp main

main: read 0
      write 0
      ; call main ; infinite loop
exit: halt

; the previous code is equivalent to the C code
; int main() {
;   int i;
;   scanf("%d", &i);
;   printf("%d\n", i);
;   main();
; }
