.text

adds X0, X10, #1        // Sumar 1 a X10 y guardar el resultado en X0
mov X3, target          // Cargar la dirección de 'target' en X3
br X3                   // Hacer salto a la dirección en X3

adds X1, X11, X0        // Esta instrucción nunca se ejecutará, está después del salto

target:
    mov X2, #42         // Al llegar a la etiqueta 'target', cargar 42 en X2
    HLT 0               // Terminar el programa