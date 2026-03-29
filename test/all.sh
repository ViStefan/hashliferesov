#!/bin/sh

tst()
{
    echo $1
    ../src/hashlife read $1
    echo

}

tst test1.cells
tst test2.cells
tst test3.cells
tst test4.cells
tst test5.cells
tst test6.cells
tst test7.cells
tst glider_r.cells
tst glider_n.cells
tst glider_rn.cells
