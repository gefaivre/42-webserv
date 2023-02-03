<?php

if ($_SERVER['REQUEST_METHOD'] === 'DELETE') { 
   		echo "You send a DELETE REQUEST\n\n";
	$status=unlink('bedelet.php');    
	if($status){  
		echo "File deleted successfully\n";    
	}else{
		echo "Sorry!\n";    
	}  
}
else
	echo "SAlut !\n";    
