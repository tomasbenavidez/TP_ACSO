.text
    MOV X1, 0       // Set X1 to 0
    CBZ X1, foo     // Should branch to foo
    ADDS X2, X0, 10 // Should not execute if branch works

bar:
    HLT 0

foo:
    MOV X3, 42      // Some operation in the branch
    CBZ X1, bar     // Another CBZ check
    ADDS X4, X0, 10 // Should not execute if branch works

    HLT 0