#!/bin/bash

PROGRAM=../philosophers
TEST_FILE="TEST/tests.txt"
TIME_LIMIT=4

#--------EJECUCION-----------

mkdir -p TEST

if [ ! -f $PROGRAM ]; then
    echo "Error:el programa $PROGRAM no existe"
    exit 1
fi

if [ ! -f $TEST_FILE ]; then
    echo " " > $TEST_FILE
    echo "No hay casos de prueba, ejecutando sin argumentos"
fi

counter=1

while IFS= read -r line; do
    echo "ejecutando $PROGRAM $line"
    output_file="TEST/${counter}"
    timeout $TIME_LIMIT $PROGRAM $line >> "${output_file}.log" 2> "${output_file}.err"
    exit_code=$?
    if [ exit_code -eq 124 ]; then
        echo "El programa excedió el tiempo límite de $TIME_LIMIT segundos" >> "${output_file}.err"
    fi
    echo "exit code = $?" >> "${output_file}.err"

    check_consistency "$line" "${output_file}.log"
    counter=(($counter + 1))
done < "$TEST_FILE"



#-----VERIFICACION-----
