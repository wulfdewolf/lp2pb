#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:40:00
#PBS -l pmem=16gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0

if CALL | grep -i -E "OPTIMUM FOUND|UNSAT"
then 
    printf "1," >> result.csv
else 
    printf "0," >> result.csv
fi

# setup
printf "SETUP," >> ../../result.csv;

# problem family
printf '%s' "FAMILY," >> ../../result.csv;

# instance
printf "INSTANCE\n" >> ../../result.csv;

# outcode
# memusage
# time