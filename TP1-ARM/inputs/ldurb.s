.text
    MOV X2, 0x1000           // Dirección base en X2
    LSL X2, X2, 16           // X2 = 0x10000000 (inicio de memoria en el simulador)

    MOV X3, 0x123456789ABCDEF0 // Valor de prueba
    STUR X3, [X2, #0x10]     // Almacenar 64 bits en [X2 + 0x10]

    // Cargar 1 byte desde [X2 + 0x10] en W1 (usar LDURB)
    ADD X4, X2, #0x10        // X4 = X2 + 0x10
    LDURB W1, [X4]           // Cargar 1 byte en W1 desde la dirección de X4

    MOV X0, 0xBEEF           // Indicar éxito
    HLT 0
    