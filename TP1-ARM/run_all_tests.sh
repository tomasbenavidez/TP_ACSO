#!/bin/bash

# Ejecutar un comando y guardar la salida en variables
echo "Ejecutando comando 1..."
#primero verificar que este en la posicion correcta
cd src && make && cd ..

# Verificar que ref_sim_x86 exista
if [ ! -f "./ref_sim_x86" ]; then
    echo "Error: El simulador de referencia ref_sim_x86 no se encontró."
    exit 1
fi

# preguntar por terminal a que archivo se quiere testear
echo "Ingrese el nombre del archivo a testear"
read file

#chequea si el archivo es .x o .s
if [[ $file == *.x ]]; then
    echo "El archivo es un .x"
elif [[ $file == *.s ]]; then
    echo "El archivo es un .s"
    #como es un .s usa el ./inputs/asm2hex
    ./inputs/asm2hex inputs/$file
    #cambia el nombre del archivo a .x
    file=${file%.s}.x
    #lo guarda en la carpeta de bytecodes
    mv inputs/$file inputs/bytecodes
    echo "El archivo fue cambiado a .x"
else
    echo "El archivo no es .x ni .s"
    exit 1
fi

#ejecutar el ref_sim_x86 y guardar la salida en ref_output
./ref_sim_x86 inputs/bytecodes/$file <<EOF > ref_output
go
rdump
q
EOF


# Filtrar la salida después de rdump para ref_sim_x86
awk '/rdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' ref_output > ref_rdump_output


#ejecutar el sim_x86 y guardar la salida en output
./src/sim inputs/bytecodes/$file <<EOF > my_output
go
rdump
q
EOF

# Filtrar la salida después de rdump para sim_x86
awk '/rdump/{flag=1; next} /ARM-SIM>/{flag=0} flag' my_output > my_rdump_output

#comparar las salidas
echo "Comparando salidas..."
if diff ref_rdump_output my_rdump_output > /dev/null; then
    echo "Las salidas son iguales."
else
    echo "Las salidas son diferentes."
fi