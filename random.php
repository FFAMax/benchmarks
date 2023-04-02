<?php
$fp = fopen('output-php.txt', 'w');
for ($i = 0; $i < 1000000; $i++) {
    $number = 900000000 + rand(0, 99999999);
    fwrite($fp, $number . "\n");
}
fclose($fp);
?>

