.text
    MOV X1, 5       // Set X1 to a nonzero value
    CBNZ X1, foo    // Should branch to foo
    ADDS X2, X0, 10 // Should not execute if branch works

bar:
    HLT 0

foo:
    MOV X3, 42      // Some operation in the branch
    CBNZ X1, bar    // Another CBNZ check
    ADDS X4, X0, 10 // Should not execute if branch works

    HLT 0