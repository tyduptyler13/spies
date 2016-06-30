
echo "N,User Time,Wall Time" | tee times.csv

for i in $(seq $2 $3)
do
	for repeat in $(seq 1 $4)
	do
		t="$(time -f '%U,%e' $1 $i 2>&1 1>/dev/null )"
		echo "$i,$t" | tee -a times.csv
	done
done


