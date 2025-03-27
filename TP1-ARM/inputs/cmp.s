.text

MOV R0, #5      @ Load 5 into R0
MOV R1, #10     @ Load 10 into R1
CMP R0, R1      @ Compare R0 and R1
BGT greater     @ Branch to 'greater' if R0 > R1
BLT less        @ Branch to 'less' if R0 < R1
BEQ equal       @ Branch to 'equal' if R0 == R1

greater:
    MOV R2, #1      @ Set R2 to 1 (R0 > R1)
    B end

less:
    MOV R2, #2      @ Set R2 to 2 (R0 < R1)
    B end

equal:
    MOV R2, #0      @ Set R2 to 0 (R0 == R1)
    HLT 0

end:
    MOV R7, #1      @ Exit syscall
    HLT 0