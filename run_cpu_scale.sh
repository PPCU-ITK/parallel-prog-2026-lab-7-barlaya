#!/bin/bash

OUTFILE="euler_cpu_out.txt"
echo "CPU Scaling Results" > $OUTFILE
echo "===================" >> $OUTFILE

module load nvhpc

# Base sizes
BASE_NX=200
BASE_NY=100

# Scaling factors
SCALES=(1 4 8 16)

for s in "${SCALES[@]}"
do
    NX=$((BASE_NX * s))
    NY=$((BASE_NY * s))

    echo "" >> $OUTFILE
    echo "Running Nx=$NX Ny=$NY" >> $OUTFILE
    echo "----------------------" >> $OUTFILE

    # override constants at compile time
    nvc++ -mp -Ofast cfd_euler.cpp -o euler_cpu \
        -DNX_OVERRIDE=$NX -DNY_OVERRIDE=$NY >> $OUTFILE 2>&1
    export OMP_NUM_THREADS=4
    srun -p cpu -c 4 --mem-per-cpu=2000 --time=00:05:00 \
        ./euler_cpu >> $OUTFILE 2>&1
done

echo "Done." >> $OUTFILE
