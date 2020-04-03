#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:30:00
#PBS -l pmem=15gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0

# Set limits
ulimit -Ht 1203 -St 1200
ulimit -Hv 14680064 -Sm 12582912

/usr/bin/time --format="%U,%M,%x," --output=result_INSTANCE_SETUP.csv SCRIPT ARG >> solver_INSTANCE_SETUP.txt

# SOLUTION
if cat solver_INSTANCE_SETUP.txt | grep "UNSAT";
then
    printf "1," >> result_INSTANCE_SETUP.csv;
elif cat solver_INSTANCE_SETUP.txt | grep -E "OPTIMUM|SAT";
then 
    # Roundingsat
    if echo SETUP | grep 2 || echo SETUP | grep 3 || echo SETUP | grep 4 || echo SETUP | grep 5;
    then 
        if cat solver_INSTANCE_SETUP.txt | sed -n -e 's/^.*o //p';
        then 
            cat solver_INSTANCE_SETUP.txt | sed -n -e 's/^.*o //p' >> result_INSTANCE_SETUP.csv;
            printf "," >> result_INSTANCE_SETUP.csv;
        else 
            printf "2," >> result_INSTANCE_SETUP.csv;
        fi

    # Clasp
    else 
        if cat solver_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+';
        then 
            cat solver_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+' >> result_INSTANCE_SETUP.csv;
            printf "," >> result_INSTANCE_SETUP.csv;
        else 
            printf "2," >> result_INSTANCE_SETUP.csv;
        fi
    fi

elif cat solver_INSTANCE_SETUP.txt | grep "UNKNOWN";
then
    printf "4," >> result_INSTANCE_SETUP.csv
else 
    printf "5," >> result_INSTANCE_SETUP.csv
fi

# remove newlines
perl -p -i -e 's/\R//g;' result_INSTANCE_SETUP.csv;

# setup
printf "SETUP," >> result_INSTANCE_SETUP.csv;

# problem family
printf "FAMILY," >> result_INSTANCE_SETUP.csv;

# instance
printf "INSTANCE\n" >> result_INSTANCE_SETUP.csv;

# remove temp files
rm time_INSTANCE_SETUP.csv
rm solver_INSTANCE_SETUP.txt