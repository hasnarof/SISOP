#!/bin/bash

script=`du -h ~/Downloads > ~/log/"$(date).log"`
echo $script