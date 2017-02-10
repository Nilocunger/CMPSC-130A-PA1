rm outs.txt

for i in {1..1000}
do 
  ./bin/simulation 50 1 100 >> outs.txt
done

a=$(cat outs.txt | grep "Attacker wins" | wc -l)
b=$(cat outs.txt | grep "Sysadmin wins" | wc -l)
echo Attacker: $a Sysadmin: $b
