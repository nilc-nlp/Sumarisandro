<?php

	include('tmp.php');

	if($_POST){
		$tmp_file = write_tmp_file();
		#$tmp_file = write_tmp_file($_POST['sentence']);

		#$file_handle = fopen($tmp_file."1", "r");
		#$corrected_sentence = '';
		#while (!feof($file_handle)) {
 		#  $line = fgets($file_handle);
 		#  $corrected_sentence = $corrected_sentence.$line;
            	#  }
		#fclose($file_handle);
	
		#$tmp_file = write_tmp_file();
		#$eu = 'oi';
		$str = "";
		for($i=0; $i < count($_POST['files']); $i++){
			$str = $str."/tmp/".$_POST['files'][$i]." ";
		}
		#echo $str;
		exec("python test.py"." ".$str. " > ".$tmp_file);
		#exec('php call_palavras_tree_nathan.php '.$tmp_file.' > '.$tmp_file.'1');
	 	#$eu = exec('python insert_subject.py '.$tmp_file.'1');	
		#$bla = exec('python get_terminals.py '.$tmp_file.'1');
		#$tmp_file = write_tmp_file($bla);
		#exec('perl callPalavras_tree_nathan.pl "'.$tmp_file.' '.$tmp_file.'2');
		#exec('perl callPalavras_file_flat.pl '.$tmp_file.' '.$tmp_file.'2');
		$file_handle = fopen($tmp_file, "r");
		$output = '';
		while (!feof($file_handle)) {
 		  $line = fgets($file_handle);
 		  $output = $output.$line;
            	  }
		fclose($file_handle);
		#$result = file_get_contents($tmp_file);
		#$file_handle = fopen($tmp_file, "r");
		#$corrected_sentence = '';
		#while (!feof($file_handle)) {
 		#  $line = fgets($file_handle);
 		#  $corrected_sentence = $corrected_sentence.$line;
            	#  }
		#fclose($file_handle);
	
		$response['corrected_sentence'] = $output;
		#$response['output_palavras'] = "bla";

		#exec("rm ".$tmp_file.' '.$tmp_file.'0'.' '.$tmp_file.'01'.' '.$tmp_file.'011'.' '.$tmp_file.'2');
		#exec("rm ".$tmp_file.' '.$tmp_file.'0'.' '.$tmp_file.'1'.' '.$tmp_file.'01'.' '.$tmp_file.'011'.' '.$tmp_file.'2');
		echo(json_encode($response));

	}	
	else echo('Not posting!');
	exit();
?>
