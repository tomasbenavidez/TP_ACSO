.text
    MOV X2, 0x1000         // Dirección base en X2
    LSL X2, X2, 16         // X2 = 0x10000000 (inicio de memoria en el simulador)

    // Cargar el valor de prueba 0x123456789ABCDEF0 en X3 utilizando MOV y ORR
    MOV X3, 0xDEF0         // Cargar los 16 bits menos significativos
    ORR X3, X3, 0x9ABC0000, LSL #16  // Mover 0x9ABC0000 y combinar
    ORR X3, X3, 0x56780000, LSL #32  // Mover 0x56780000 y combinar
    ORR X3, X3, 0x12340000, LSL #48  // Mover 0x12340000 y combinar

    STUR X3, [X2, #0x10]   // Almacenar 64 bits en [X2 + 0x10]
    LDUR X1, [X2, #0x10]   // Cargar los 64 bits en X1

    MOV X0, 0xBEEF         // Indicar éxito
    HLT 0
    