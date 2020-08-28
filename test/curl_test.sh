#!/bin/bash

for ((i=1;i<=10000;i++));
do
    curl -m 1 -X POST --data @test_body.txt http://127.0.0.1:7788/
done
