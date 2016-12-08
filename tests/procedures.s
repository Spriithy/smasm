.echo        ; register echo

.start
do: push 10
    pop  11
    call echo
    call mov ; ERROR: forward declaration needed
    ret

.mov
    ret
    