.text
    MOV X1, 0xF000000000000000  // Load a large number into X1
    LSR X2, X1, #4              // Logical shift right by 4 bits (should divide by 16)
    CBZ X2, foo                 // If result is zero, jump to foo
    ADDS X3, X0, 10             // Should not execute if X2 == 0

bar:
    HLT 0

foo:
    MOV X4, 42                  // Some operation if branch is taken
    HLT 0
    