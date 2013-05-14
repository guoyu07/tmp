<?php
function qsort_simple(array $number_list) {
    $len = count($number_list);
    if (1 >= $len) {
        return $number_list;
    }

    $middle = array_shift($number_list);
    $left   = array();
    $right  = array();
    foreach ($number_list as $value) {
        if ($middle > $value) {
            $left[] = $value;
        } else {
            $right[] = $value;
        }
    }

    if (count($left)) {
        $left = qsort_simple($left);
    }

    if (count($right)) {
        $right = qsort_simple($right);
    }

    return array_merge($left, array($middle), $right);
}


//test
$number_list = array(1, 6, 9, 0, 19, 29, 1, 9, 3, 2);
var_dump(qsort_simple($number_list));
