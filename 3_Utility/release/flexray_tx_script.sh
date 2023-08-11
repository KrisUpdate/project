#/bin/bash

echo "key Ctrl + C Stop…"
echo "erayinit t" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
LENGTH=0
while :
do
    echo -ne "\r["
    sleep 0.2
    while [ $LENGTH -le 10 ]
    do
        echo "Send Flexray data"
        echo "eraysend 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
        sleep 1
        (( LENGTH++ ))
    done
    LENGTH=0
    echo -en "\r                          "
done
