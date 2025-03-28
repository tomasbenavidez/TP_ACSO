.text
    adds x0, x0, 2        // Sumar 2 a x0
    adds x1, x1, 3        // Sumar 3 a x1

    MOV x3, 5             // Cargar 5 en x3
    mul x0, x0, x3        // Multiplicar x0 por el valor en x3 (5)

    MOV x3, 3             // Cargar 3 en x3
    mul x1, x1, x3        // Multiplicar x1 por el valor en x3 (3)

    mul x2, x0, x1        // Multiplicar x0 y x1, almacenar el resultado en x2
    HLT 0
    