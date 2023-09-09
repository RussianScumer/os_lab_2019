#!/bin/bash

printenv PATH

set | grep PATH

dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"

