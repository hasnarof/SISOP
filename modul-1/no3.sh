#!/bin/bash
 
awk '/Iris-setosa/ {
        split($0, idx, ",");
        if (strtonum(idx[1]) > 5.4) {
                printf "sepal length : %.1f petal area : %.2f\n", strtonum(idx[1]), 
                        strtonum(idx[3]) * strtonum(idx[4])
        }
 
}' iris.data
