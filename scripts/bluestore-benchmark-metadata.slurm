#!/bin/bash

#SBATCH -N 1 --ntasks-per-node=48
#SBATCH --partition=abu-long
#SBATCH --nodelist=abu2
#SBATCH --error=../results/output/bluestore-%j.err --output=../results/output/bluestore-%j.out


# ---------------------------------------- BENCHMARK PARAMS ------------------------------------
durations="10"
# durations="1 2 4 16"
# durations="1 2 4 8 12 16 32 64 128 256 512"
runs="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20"

# ---------------------------------------- OUTPUT FILES ----------------------------------------
# output="test-results.tsv"
output_unsync="../results/bluestore/unsynced-metadata-results-${SLURM_JOBID}.tsv"
output_sync="../results/bluestore/synced-metadata-results-${SLURM_JOBID}.tsv"

# ---------------------------------------- CLUSTER SETUP ----------------------------------------
export JULEA_CONFIG=~/.config/julea/julea-bluestore
echo "export JULEA_CONFIG=~/.config/julea/julea-bluestore" >> $output_unsync
echo "export JULEA_CONFIG=~/.config/julea/julea-bluestore" >> $output_sync


# all benchmarks are run multiple times
for run in $runs
do
	echo -e "# run: $run" >> $output_unsync
	echo -e "# run: $run" >> $output_sync

	killall julea-server
	# just do be sure there are no space problems the directories are deleted between all 
	rm -r /tmp/julea-1124/
	
	mkdir /tmp/julea-1124/
	mkdir /tmp/julea-1124/bluestore

	for duration in $durations
	do
	# --- unsynced
		echo " " >> $$output_unsync
		echo "# " >> $$output_unsync
		echo "# --- /object/object ---" >> $$output_unsync
		echo "# " >> $$output_unsync
		echo -e "# ./scripts/benchmark.sh -p /object/object --duration=$duration" >> $output_unsync
		echo "# " >> $$output_unsync

		echo -e ./scripts/benchmark.sh -p /object/object --duration=$duration 

		./scripts/benchmark.sh -p /object/object --duration=$duration >> $output

	# --- synced
		echo " " >> $output_sync
		echo "# " >> $output_sync
		echo "# --- /object/object ---" >> $output_sync
		echo "# " >> $output_sync
		echo -e "# ./scripts/benchmark.sh -p /object/object --duration=$duration" -s safety=storage >> $output_sync
		echo "# " >> $output_sync

		echo -e ./scripts/benchmark.sh -p /object/object --duration=$duration -s safety=storage

		./scripts/benchmark.sh -p /object/object --duration=$duration -s safety=storage >> $output_sync
	done
done
