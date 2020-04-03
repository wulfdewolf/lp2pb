#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:30:00
#PBS -l pmem=14gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0

# Set limits
ulimit -Ht 1203 -St 1200
ulimit -Hv 14336 -Sm 12288

/usr/bin/time --format="%U,%M," --output=time_INSTANCE_SETUP.csv SCRIPT ARG >> solver_INSTANCE_SETUP.txt

# outcode 
ret = $?
printf "$ret," >> result_INSTANCE_SETUP.csv

# memory and time 
cat time_INSTANCE_SETUP.csv >> result_INSTANCE_SETUP.csv;

# setup
printf "SETUP," >> result_INSTANCE_SETUP.csv;

# problem family
printf "FAMILY," >> result_INSTANCE_SETUP.csv;

# instance
printf "INSTANCE\n" >> result_INSTANCE_SETUP.csv;

# SOLUTION
if cat solver_INSTANCE_SETUP.txt | grep -E "UNSAT";
then
    printf "1," >> result_INSTANCE_SETUP.csv
elif cat solver_INSTANCE_SETUP.txt | grep -E "OPTIMUM|SAT";
then 
    # Roundingsat
    if [SETUP = 2] || [SETUP = 3] || [SETUP = 4] || [SETUP = 5];
    then 
        if cat solver_INSTANCE_SETUP.txt | sed -n -e 's/^.*o //p';
        then 
            cat solver_INSTANCE_SETUP.txt | sed -n -e 's/^.*o //p' >> result_INSTANCE_SETUP.csv;
            printf "," >> result_INSTANCE_SETUP.csv;
        else 
            printf "2," >> result_INSTANCE_SETUP.csv;

    # Clasp
    else 
        if cat solver_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+';
        then 
            cat solver_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+' >> result_INSTANCE_SETUP.csv;
            printf "," >> result_INSTANCE_SETUP.csv;
        else 
            printf "2," >> result_INSTANCE_SETUP.csv;

elif cat solver_INSTANCE_SETUP.txt | grep "UNKNOWN";
then
    printf "4," >> result_INSTANCE_SETUP.csv
else 
    printf "5," >> result_INSTANCE_SETUP.csv
fi

# remove temp files
rm time_INSTANCE_SETUP.txt
rm solver_INSTANCE_SETUP.txt