#!/bin/bash

# 1. sepal length in cm
# 2. sepal width in cm
# 3. petal length in cm
# 4. petal width in cm
# 5. class:
# -- Iris Setosa
# -- Iris Versicolour
# -- Iris Virginica

awk '/Iris-setosa/ {
        split($0, idx, ",");
        if (strtonum(idx[1]) > 5.4) {
                # printf "sepal length : %.1f petal area : %.2f\n", strtonum(idx[1]), 
                #         strtonum(idx[3]) * strtonum(idx[4]);
                printf "Luas petal pada baris %d adalah %.2f\n", NR, strtonum(idx[3]) * strtonum(idx[4]);
        }
 
}' iris.data
