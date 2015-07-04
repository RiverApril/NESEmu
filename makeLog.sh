#!/bin/bash

paste -d ' ' <(awk -F' ' '{print "pc:" $1}' nestest.log) <(awk -F':' 'BEGIN{OFS=":"}{print $2, $3, $4, $5}' nestest.log | awk -F' ' 'BEGIN{OFS=" "}{print "A:" $1, $2, $3, $4}') > nestest.out

