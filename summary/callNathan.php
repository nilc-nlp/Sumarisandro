<?php
	include('tmp.php');

	if($_POST){

		$url = "http://10.11.14.126/services/service_rsumm.php";
		$data = array('document1' => $_POST['document1'], 'document2' => $_POST['document2']);
	
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
		# Create a temporal file with the summary given by RSumm		
		$temp = tmpfile();
		fwrite($temp, $ret);
		fseek($temp, 0);
		$metaDatas = stream_get_meta_data($temp);
		$tmpFilename = $metaDatas['uri'];
	
		$tmp_file = write_tmp_file();

		$max_size = $_POST['max_size'];

		exec("python test.py"." ".$tmpFilename." ".$max_size." > ".$tmp_file);

		$file_handle = fopen($tmp_file, "r");
		$output = '';
		while (!feof($file_handle)) {
 			$line = fgets($file_handle);
 			$output = $output.$line;
            	}
		fclose($file_handle);
		$file_handle = fopen($tmp_file, "r");
		$corrected_sentence = '';
	
		$response['corrected_sentence'] = $output;
		$response['output_palavras'] = "blassksakjskaj";

		echo(json_encode($response));
	}	
	else echo('Not posting!');
	exit();
?>
