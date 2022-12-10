<h1>DER BIMMLER</h2>
<?php
function move_number_closer(&$headn, &$tailn) {
    $distance = $headn - $tailn;
    if (abs($distance) > 1) {
        $tailn += ($distance <=> 0);
    }
}

/* ---------- */
/* ---H---1H- */
/* -1----2--- */
/* -2-------- */
function move_tail(&$head, &$tail, $tailnr, $i) {
    print(strval($i)."-".strval($tailnr)."Head: ");
    print_pos($head);
    print(": ");
    print_pos($tail);

    $dx = $head[0] - $tail[0];
    $dy = $head[1] - $tail[1];
    if (abs($dx) > 2 || abs($dy) > 2) {
        exit(1);
    }
    if (abs($dx) == 2)  {  // move both
        if (abs($dy) >= 1) {
            $tail[1] += ($dy <=> 0);
        }
        $tail[0] += ($dx <=> 0);
    } 
    else if (abs($dy) == 2) {
        if (abs($dx) >= 1) {
            $tail[0] += ($dx <=> 0);
        }
        $tail[1] += ($dy <=> 0);
    }
    /* move_number_closer($head[0], $tail[0]); */
    /* move_number_closer($head[1], $tail[1]); */
    print("->");
    print_pos($tail);
    print("<br>");
}

function print_pos($pos) {
    print("[".strval($pos[0]).",".strval($pos[1])."]");
}

$display_lines = array();
function display($head, $tail) {
    ob_clean();
    $req_size_x = max($head[0], $tail[0]);
    $req_size_y = max($head[1], $tail[1]);
    for ($y = 0; $y < $req_size_y; $y++) {
        $display_lines[$y] = str_repeat('.', $req_size_x);
    }
    $display_lines[$head[1]][$head[0]] = "H";
    $display_lines[$tail[1]][$tail[0]] = "T";
    for ($y = 0; $y < count($display_lines); $y++) {
        print($display_lines[$y]."<br>");
    }
}

$filename = "ropes.txt";
/* $filename = "test2.txt"; */
$ropefile = fopen($filename, "r");
$input = fread($ropefile, filesize($filename));
$head_movements = explode("\n", $input);
fclose($ropefile);

$visited_positions = array("0_0" => 0);
$current_positions = array_fill(0, 10, [0, 0]);

for ($i = 0; $i < count($head_movements); $i++) {
    /* print($head_movements[$i]); */
    $direction = $head_movements[$i][0];
    $amount = (int) substr($head_movements[$i], 2);

    for ($a = 0; $a < $amount; $a++) {
        // move head
        switch ($direction) {
            case "L":
                $current_positions[0][0] -= 1;
                break;
            case "R":
                $current_positions[0][0] += 1;
                break;
            case "U":
                $current_positions[0][1] += 1;
                break;
            case "D":
                $current_positions[0][1] -= 1;
                break;
        }
        // move tails
        for ($t = 1; $t < count($current_positions); $t++) {
            move_tail($current_positions[$t-1], $current_positions[$t], $t, $i);
        }
        $last_tail = count($current_positions) - 1;
        $visited_positions[strval($current_positions[$last_tail][0])."_".strval($current_positions[$last_tail][1])] += 1;
        /* display($current_positions[0], $current_positions[$last_tail]); */
    }
}
print_r($visited_positions);
print("Total number of positions=".array_sum($visited_positions));
print("Number of unique visited positions=".count($visited_positions));
