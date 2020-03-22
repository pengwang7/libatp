#!/bin/bash

for ((i=1;i<=10000;i++));
do
    curl -m 1 -X POST --data @test_body.txt http://0.0.0.0:7788/
done
