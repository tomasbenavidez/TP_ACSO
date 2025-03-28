.text
    MOV X2, 0x1000       // Dirección base en X2
    LSL X2, X2, 16       // X2 = 0x10000000 (inicio de memoria en el simulador)
    MOV X3, 0xABCD       // Valor de prueba (16 bits)

    STURH W3, [X2, #0x10] // Almacenar 16 bits en [X2 + 0x10]
    LDURH W1, [X2, #0x10] // Cargar 16 bits en W1 con padding

    MOV X0, 0xBEEF       // Indicar éxito
    HLT 0
    