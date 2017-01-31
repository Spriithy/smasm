call main
halt

main: push# 0
      call gets
      push# 0
      call puts
      call ln
      push# 0
      call strlen
      pop 1000
      write 1000
      call ln
      ret

gets: pop 3000
_L1:  pop 3001  ; l'adresse courrante de la chaine de caractères
      getc
      dup
      push# 10
      op 0
      jpc _L2   ; char == \n
      push 3001
      ipop
      push 3001
      push# 1
      op 11
      jmp _L1
_L2:  push# 0
      push 3001
      iPop
      pop 3001
      push 3000
      ret

puts: pop 3000  ; return address
_L3:  pop 3001  ; string address
      push 3001
      iPush
      dup
      jpc _L4   ; print it if it is not the null byte
      pop 3001
      push 3000 ; otherwise return to the caller
      ret
_L4:  putc
      push 3001
      push# 1
      op 11
      jmp _L3

ln:   push# 10
      putc
      ret

strlen: pop 3000  ; return address
      pop 3001  ; string address
      push# 0
      pop 3002
_L5:  push 3001
      ipush
      jpc _L6
      push 3002
      push 3000
      ret
_L6:  push 3001
      push# 1
      op 11
      pop 3001
      push 3002
      push# 1
      op 11
      pop 3002
      jmp _L5
