#!/bin/bash
awk 'BEGIN{sum=0} {if (NF != 0) {sum+=$1} else {print sum; sum=0}}' input.txt | sort -n | tail -n 3 | awk 'BEGIN{sum=0} {sum+=$1} END{print sum}'
