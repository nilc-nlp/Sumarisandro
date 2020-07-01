<?php
$data_to_process = file_get_contents($argv[1]);
$url = "http://10.11.14.126/services/service_rsumm.php";
$data = array('sentence' => $data_to_process);

// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'POST',
        'content' => http_build_query($data),
    ),
);
$context  = stream_context_create($options);
$ret = file_get_contents($url, false, $context);

echo $ret;
?>

