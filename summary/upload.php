<?php

/* Insira aqui a pasta que deseja salvar o arquivo*/

$uploaddir = '/tmp/';

for($i=0; $i<count($_FILES['file']['name']); $i++){
	$uploadfile = $uploaddir . $_FILES['file']['name'][$i];
	if (move_uploaded_file($_FILES['file']['tmp_name'][$i], $uploadfile)){
//echo "Arquivo Enviado";
}
else {
//echo "Arquivo não enviado";
}
}



?>             
