#!/bin/bash

#SBATCH -N 1 --ntasks-per-node=48
#SBATCH --partition=abu-long
#SBATCH --nodelist=abu5
#SBATCH --error=../results/output/posix-%j.err --output=../results/output/posix-%j.out


# ---------------------------------------- BENCHMARK PARAMS ------------------------------------
durations="1 2 4 16"
# durations="1 2 4 8 12 16 32 64 128 256 512"
runs="1 2 3"

# ---------------------------------------- OUTPUT FILES ----------------------------------------
# output="test-results.tsv"
output="../results/posix/synced-results-${SLURM_JOBID}.tsv"

# ---------------------------------------- CLUSTER SETUP ----------------------------------------
export JULEA_CONFIG=~/.config/julea/julea-posix
echo "export JULEA_CONFIG=~/.config/julea/julea-posix" >> $output

# all benchmarks are run multiple times
for run in $runs
do
	echo -e "# run: $run" >> $output

	# --- write objects
	for duration in $durations
	do
		# --- write objects
		echo " " >> $output
		echo "# " >> $output
		echo "# --- /object/object/write ---" >> $output
		echo "# " >> $output
		echo -e "# ./scripts/benchmark.sh -p /object/object/write --duration=$duration" -s safety=storage >> $output
		echo "# " >> $output

		echo -e ./scripts/benchmark.sh -p /object/object/write --duration=$duration -s safety=storage

		./scripts/benchmark.sh -p /object/object/write --duration=$duration -s safety=storage >> $output

		# --- write batched objects
		echo " " >> $output
		echo "# " >> $output
		echo "# --- /object/object/write-batch ---" >> $output
		echo "# " >> $output
		echo -e "# ./scripts/benchmark.sh -p /object/object/write-batch --duration=$duration" -s safety=storage >> $output
		echo "# " >> $output

		echo -e ./scripts/benchmark.sh -p /object/object/write-batch --duration=$duration  -s safety=storage

		./scripts/benchmark.sh -p /object/object/write-batch --duration=$duration -s safety=storage >> $output

		# --- read objects
		echo " " >> $output
		echo "# " >> $output
		echo "# --- /object/object/read ---" >> $output
		echo "# " >> $output
		echo -e "# ./scripts/benchmark.sh -p /object/object/read --duration=$duration" -s safety=storage >> $output
		echo "# " >> $output

		echo -e ./scripts/benchmark.sh -p /object/object/read --duration=$duration  -s safety=storage

		./scripts/benchmark.sh -p /object/object/read --duration=$duration -s safety=storage >> $output

		# --- read batched objects
		echo " " >> $output
		echo "# " >> $output
		echo "# --- /object/object/read-batch ---" >> $output
		echo "# " >> $output
		echo -e "# ./scripts/benchmark.sh -p /object/object/read-batch --duration=$duration" -s safety=storage >> $output
		echo "# " >> $output

		echo -e ./scripts/benchmark.sh -p /object/object/read-batch --duration=$duration  -s safety=storage

		./scripts/benchmark.sh -p /object/object/read-batch --duration=$duration -s safety=storage >> $output
	done

	# done

	# just do be sure there are no space problems the directories are deleted between all 
	killall julea-server
	rm -r /tmp/julea-1124/
	
	# # --- batched write objects
	# for duration in $durations
	# do
	# 		echo " " >> $output
	# 		echo "# " >> $output
	# 		echo "# --- /object/object/write-batch ---" >> $output
	# 		echo "# " >> $output
	# 		echo -e "# ./scripts/benchmark.sh -p /object/object/write-batch --duration=$duration" -s safety=storage >> $output
	# 		echo "# " >> $output

	# 		echo -e ./scripts/benchmark.sh -p /object/object/write-batch --duration=$duration  -s safety=storage

	# 		./scripts/benchmark.sh -p /object/object/write-batch --duration=$duration -s safety=storage >> $output


	# done

	# killall julea-server
	# rm -r /tmp/julea-1124/

	# # --- read objects
	# for duration in $durations
	# do
	# 		echo " " >> $output
	# 		echo "# " >> $output
	# 		echo "# --- /object/object/read ---" >> $output
	# 		echo "# " >> $output
	# 		echo -e "# ./scripts/benchmark.sh -p /object/object/read --duration=$duration" -s safety=storage >> $output
	# 		echo "# " >> $output

	# 		echo -e ./scripts/benchmark.sh -p /object/object/read --duration=$duration  -s safety=storage

	# 		./scripts/benchmark.sh -p /object/object/read --duration=$duration -s safety=storage >> $output
	# done

	# killall julea-server
	# rm -r /tmp/julea-1124/

	# # --- batched read objects
	# for duration in $durations
	# do
	# 		echo " " >> $output
	# 		echo "# " >> $output
	# 		echo "# --- /object/object/read-batch ---" >> $output
	# 		echo "# " >> $output
	# 		echo -e "# ./scripts/benchmark.sh -p /object/object/read-batch --duration=$duration" -s safety=storage >> $output
	# 		echo "# " >> $output

	# 		echo -e ./scripts/benchmark.sh -p /object/object/read-batch --duration=$duration  -s safety=storage

	# 		./scripts/benchmark.sh -p /object/object/read-batch --duration=$duration -s safety=storage >> $output
	# done

	# killall julea-server
	# rm -r /tmp/julea-1124/
done
