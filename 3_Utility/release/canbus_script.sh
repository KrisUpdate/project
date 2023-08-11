#/bin/bash

echo "key Ctrl + C Stop…"
echo "caninit 1000000" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
echo "caninit 1000000" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
echo "canprintrx enable" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
echo "canprintrx enable" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
LENGTH=0
while :
do
    echo -ne "\r["
    sleep 0.2
    while [ $LENGTH -le 10 ]
    do
        echo "Send CAN_A"
        echo "cansend a 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
        echo "cansend a 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
        echo "Send CAN_B"
        echo "cansend b 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
        echo "cansend b 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
        echo "Send CAN_C"
        echo "cansend c 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
        echo "cansend c 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
        echo "Send CAN_D"
        echo "cansend d 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
        echo "cansend d 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
        echo "Send CAN_E"
        echo "cansend e 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
        echo "cansend e 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
        echo "Send CAN_F"
        echo "cansend f 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002
        echo "cansend f 0 0x55555555 0x55555555" | timeout 3 nc -u -q 2 -p 53000 10.42.0.147 53002
        sleep 1
        (( LENGTH++ ))
    done
    LENGTH=0
    echo -en "\r                          "
done
