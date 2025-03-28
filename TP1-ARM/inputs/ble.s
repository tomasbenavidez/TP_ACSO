.text
    CMP X1, X2
    BLE foo
    ADDS X2, X0, 10

bar:
    HLT 0

foo:
    CMP X1, X2
    BLE bar
    ADDS X3, X0, 10

    HLT 0
    