<?php
# @Author: nathanhartmann
# @Date:   2015-06-10 21:35:28
# @Last Modified by:   nathanhartmann
# @Last Modified time: 2015-06-10 21:35:28

//----------------//

$ADDRESS = '10.11.14.126';        //para acesso de dentro do laboratório
//$ADDRESS = '143.107.183.175:12680'; //para acesso de fora do laboratório

//----------------//

if($argc != 2){
	echo "Format expected:\t";
	echo "php thisfile file_to_be_processed.txt\n";
	exit(0);
}
$data_to_process = file_get_contents($argv[1]);
$url = "http://".$ADDRESS."/services/service_palavras_flat.php";
$data = array('sentence' => $data_to_process);

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

