#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:21:00
#PBS -l pmem=15gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0

/usr/bin/time --format="%U,%M,%x,SETUP,FAMILY,INSTANCE" --output=result_INSTANCE_SETUP.csv SCRIPT ARG >> "$TMPDIR"/"$TMPDIR"/solver_INSTANCE_SETUP.txt

# SOLUTION
if cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep "UNSAT";
then
    printf "1," >> result_INSTANCE_SETUP.csv;
elif cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep "OPTIMUM";
then 
    # Roundingsat
    if echo SETUP | grep 2 || echo SETUP | grep 3 || echo SETUP | grep 4 || echo SETUP | grep 5;
    then 
        printf "2/" >> result_INSTANCE_SETUP.csv;
        cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | sed -n -e 's/^.*o //p' >> result_INSTANCE_SETUP.csv;
        printf "," >> result_INSTANCE_SETUP.csv;

    # Clasp
    else 
        printf "2/" >> result_INSTANCE_SETUP.csv;
        cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+' >> result_INSTANCE_SETUP.csv;
        printf "," >> result_INSTANCE_SETUP.csv;
    fi
elif cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep "SAT";
then 
    # check if it's a suboptimal solution or just a decision

    # Roundingsat
    if echo SETUP | grep 2 || echo SETUP | grep 3 || echo SETUP | grep 4 || echo SETUP | grep 5;
    then 
        if cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep 'bounds'    ;
        then 
            printf "3/" >> result_INSTANCE_SETUP.csv;
            cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | sed -n -e 's/^.*c bounds //p' | tail -1 | tr -d " "  >> result_INSTANCE_SETUP.csv;
            printf "," >> result_INSTANCE_SETUP.csv;
        else 
            printf "3," >> result_INSTANCE_SETUP.csv;
        fi

    # Clasp
    else 
        if cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+';
        then 
            printf "3/" >> result_INSTANCE_SETUP.csv;
            cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+' >> result_INSTANCE_SETUP.csv;
            printf "," >> result_INSTANCE_SETUP.csv;
        else
            printf "3," >> result_INSTANCE_SETUP.csv;
        fi
    fi 
elif cat "$TMPDIR"/solver_INSTANCE_SETUP.txt | grep "UNKNOWN";
then
    printf "4," >> result_INSTANCE_SETUP.csv
else 
    printf "5," >> result_INSTANCE_SETUP.csv
fi

# remove newlines
perl -p -i -e 's/\R//g;' result_INSTANCE_SETUP.csv;

# remove temp files
lslsrm "$TMPDIR"/solver_INSTANCE_SETUP.txt