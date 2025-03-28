.text
    MOV X2, 0x1000         // Dirección base en X2
    LSL X2, X2, 16         // X2 = 0x10000000 (inicio de memoria en el simulador)

    // Cargar el valor de prueba 0x123456789ABCDEF0 en X3 utilizando MOV y ORR
    MOV X3, 0xDEF0         // Cargar los 16 bits menos significativos
    ORR X3, X3, 0x9ABC0000, LSL 16  // Combinar los siguientes 16 bits (9ABC) desplazados 16 bits
    ORR X3, X3, 0x56780000, LSL 32  // Combinar los siguientes 16 bits (5678) desplazados 32 bits
    ORR X3, X3, 0x12340000, LSL 48  // Combinar los 16 bits más significativos (1234) desplazados 48 bits

    STUR X3, [X2, #0x10]   // Almacenar 64 bits en [X2 + 0x10]
    LDUR X1, [X2, #0x10]   // Cargar los 64 bits en X1

    MOV X0, 0xBEEF         // Indicar éxito
    HLT 0
    