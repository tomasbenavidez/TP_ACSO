.text
    CMP X1, X2
    BNE foo
    ADDS X2, X0, 10

bar:
    HLT 0

foo:
    CMP X1, X2
    BNE bar
    ADDS X3, X0, 10

    HLT 0