#!/bin/bash

# Verificar que estemos en la posición correcta
cd src && make && cd ..

# Verificar que ref_sim_x86 exista
if [ ! -f "./ref_sim_x86" ]; then
    echo "Error: El simulador de referencia ref_sim_x86 no se encontró."
    exit 1
fi

# Carpeta donde se encuentran los archivos .s
input_folder="inputs"

# Lista para almacenar los archivos que no coinciden
non_matching_files=()

# Iterar sobre todos los archivos .s en la carpeta inputs
for file in $input_folder/*.s; do
    # Extraer el nombre del archivo sin la ruta
    file_name=$(basename "$file")
    
    echo "Procesando el archivo: $file_name"

    # Convertir archivo .s a .x usando asm2hex
    ./inputs/asm2hex "$file"
    
    # Cambiar la extensión a .x
    file_x="${file_name%.s}.x"
    
    # Mover el archivo a la carpeta de bytecodes
    mv "$input_folder/$file_x" "$input_folder/bytecodes"

    # Contar el número de instrucciones en el archivo .x
    instruction_count=$(wc -l < "$input_folder/bytecodes/$file_x")
    echo "El archivo $file_x tiene $instruction_count instrucciones."

    # Ejecutar el simulador de referencia paso a paso y guardar la salida
    echo "Ejecutando paso a paso en ref_sim_x86 para $file_name..."
    for ((i=1; i<=instruction_count; i++)); do
        ./ref_sim_x86 "$input_folder/bytecodes/$file_x" <<EOF > ref_output
r1
rdump
memdump
EOF
    done

    # Filtrar la salida después de rdump y memdump para ref_sim_x86
    awk '/rdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' ref_output > ref_rdump_output
    awk '/memdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' ref_output > ref_memdump_output

    # Añadir espacio en blanco para la legibilidad entre pruebas
    echo ""

    # Ejecutar el simulador paso a paso y guardar la salida
    echo "Ejecutando paso a paso en sim_x86 para $file_name..."
    for ((i=1; i<=instruction_count; i++)); do
        ./src/sim "$input_folder/bytecodes/$file_x" <<EOF > my_output
r1
rdump
memdump
EOF
    done

    # Filtrar la salida después de rdump y memdump para sim_x86
    awk '/rdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' my_output > my_rdump_output
    awk '/memdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' my_output > my_memdump_output

    # Comparar las salidas de rdump
    echo "Comparando rdump para $file_name..."
    if ! diff ref_rdump_output my_rdump_output > /dev/null; then
        non_matching_files+=("$file_name (rdump)")
    fi

    # Comparar las salidas de memdump
    echo "Comparando memdump para $file_name..."
    if ! diff ref_memdump_output my_memdump_output > /dev/null; then
        non_matching_files+=("$file_name (memdump)")
    fi

    # Añadir espacio en blanco entre archivos procesados
    echo ""
done

# Si hay archivos que no coinciden, mostrarlos
if [ ${#non_matching_files[@]} -gt 0 ]; then
    echo "Los siguientes archivos no coinciden:"
    for file in "${non_matching_files[@]}"; do
        echo "$file"
    done
else
    echo "Todos los archivos pasaron la prueba."
fi