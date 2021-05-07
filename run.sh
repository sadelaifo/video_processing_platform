#! /bin/bash

i=1
end=100
inc=5
mask=0x1

while [ $i -lt $end ]; do
        rm -rf mydb/
#       echo "value of write frequency is $i"
#       taskset $mask ./main 1000000 $i | grep fraction
        taskset $mask ./main 10000000 $i | grep fraction
        ((i=i+$inc))
done
