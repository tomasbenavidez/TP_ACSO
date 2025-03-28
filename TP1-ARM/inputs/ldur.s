.text
    MOV X2, 0x1000       // Dirección base en X2
    LSL X2, X2, 16       // X2 = 0x10000000 (inicio de memoria en el simulador)
    MOV X3, 0x123456789ABCDEF0  // Valor de prueba

    STUR X3, [X2, #0x10] // Almacenar 64 bits en [X2 + 0x10]
    LDUR X1, [X2, #0x10] // Cargar los 64 bits en X1

    MOV X0, 0xBEEF       // Indicar éxito
    HLT 0