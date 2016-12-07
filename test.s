.start
ici:  read	1000  ; Load from memory
      push	1000  ; Store
      push#	0
      op	0
      jpc	fin
      push	1000
      op	10
      pop	1000
      write	1000
      jmp	ici
fin:  halt