#!/bin/bash

OUTFILE="euler_gpu_out.txt"
echo "GPU Scaling Results" > $OUTFILE
echo "===================" >> $OUTFILE

#load NVIDIA HPC compiler (needed for nvc++)
module load nvhpc

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

    #GPU with OpenMP offload
    nvc++ -mp=gpu -gpu=cc80 -Ofast cfd_euler.cpp -o euler_gpu \
        -DNX_OVERRIDE=$NX -DNY_OVERRIDE=$NY >> $OUTFILE 2>&1

    #run on GPU partition
    srun -p gpu --gres=gpu:1 -c 4 --time=00:05:00 \
        ./euler_gpu >> $OUTFILE 2>&1
done

echo "Done." >> $OUTFILE
