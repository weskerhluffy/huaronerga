
caca=$1
for ((x=1;x<=caca;++x))
do
#       echo "generando mierda $x"
        fila=$(genera_mierda_pares.py|xargs)
#       echo "fila caca $fila"
        echo "$fila"
done