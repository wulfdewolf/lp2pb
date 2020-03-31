#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:40:00
#PBS -l pmem=16gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0

../../runlim -o runlim_INSTANCE.txt -t 1500 -s 14336 SCRIPT solver_INSTANCE.txt ARG

# solution
if cat solver.txt | grep -i -E "OPTIMUM|UNSAT";
then 
    printf "1," >> ../../result.csv
elif cat solver.txt | grep -i "UNKNOWN";
then
    printf "2," >> ../../result.csv
else 
    printf "0," >> ../../result.csv
fi

# outcode 
printf cat runlim.txt | sed -n -e 's/^.*result //p' >> ../../result.csv
printf ","

# time
printf cat runlim.txt | sed -n -e 's/^.*real //p' >> ../../result.csv
printf ","

# status
printf cat runlim.txt | sed -n -e 's/^.*status //p' >> ../../result.csv
printf ","

# memusage
printf cat runlim.txt | sed -n -e 's/^.*space //p' >> ../../result.csv
printf ","

# setup
printf "SETUP," >> ../../result.csv;

# problem family
printf '%s' "FAMILY," >> ../../result.csv;

# instance
printf "INSTANCE\n" >> ../../result.csv;

# remove temp files
rm runlim_INSTANCE.txt
rm solver_INSTANCE.txt