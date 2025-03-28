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

    # Ejecutar el simulador de referencia y guardar la salida
    ./ref_sim_x86 "$input_folder/bytecodes/$file_x" <<EOF > ref_output
go
rdump
q
EOF

    # Filtrar la salida después de rdump para ref_sim_x86
    awk '/rdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' ref_output > ref_rdump_output

    # Ejecutar el simulador y guardar la salida
    ./src/sim "$input_folder/bytecodes/$file_x" <<EOF > my_output
go
rdump
q
EOF

    # Filtrar la salida después de rdump para sim_x86
    awk '/rdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' my_output > my_rdump_output

    # Comparar las salidas
    echo "Comparando salidas para $file_name..."
    if ! diff ref_rdump_output my_rdump_output > /dev/null; then
        # Si las salidas no coinciden, agregar el archivo a la lista de no coincidentes
        non_matching_files+=("$file_name")
    fi
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