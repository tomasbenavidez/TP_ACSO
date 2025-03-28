.text

MOV X0, #5         // Cargar X0 con 5
MOV X1, #10        // Cargar X1 con 10
CMP X0, X1         // Comparar X0 con X1
BGT greater        // Si X0 > X1, saltar a greater
BLT less           // Si X0 < X1, saltar a less
BEQ equal          // Si X0 == X1, saltar a equal

greater:
    MOV X2, #1     // Si X0 > X1, asignar 1 a X2
    B end          // Saltar al final

less:
    MOV X2, #2     // Si X0 < X1, asignar 2 a X2
    B end          // Saltar al final

equal:
    MOV X2, #0     // Si X0 == X1, asignar 0 a X2

end:
    MOV X7, #1     // Fin del programa, asignar 1 a X7
    HLT 0          // Detener la ejecución