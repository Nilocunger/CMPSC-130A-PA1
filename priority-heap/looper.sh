rm outs.txt

for i in {1..20}
do 
  ./bin/simulation 100 1 100 | grep wins >> outs.txt
  sleep 0.5
  echo $i trials complete
done

a=$(cat outs.txt | grep "Attacker wins" | wc -l)
b=$(cat outs.txt | grep "Sysadmin wins" | wc -l)
echo Attacker: $a Sysadmin: $b > results.txt
cat results.txt
