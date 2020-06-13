#!/bin/bash

for ((i=1;i<=14;i++));
do
    curl -m 1 -X POST --data @test_body.txt http://0.0.0.0:8012/
done
