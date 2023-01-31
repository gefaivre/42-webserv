<?php
$fileexits = 0;
if (($_FILES['my_file']['name']!="")){
// Where the file is going to be stored
    $target_dir = "./new_files/";
    $file = $_FILES['my_file']['name'];
    $path = pathinfo($file);
    $filename = $path['filename'];
    $ext = $path['extension'];
    $temp_name = $_FILES['my_file']['tmp_name'];
    $path_filename_ext = $target_dir.$filename.".".$ext;
 
// Check if file already exists
if (file_exists($path_filename_ext)) {
$fileexits = 1;
 }else{
 move_uploaded_file($temp_name,$path_filename_ext);
//  echo "Congratulations! File Uploaded Successfully.";
 }
}
?>


<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="assets/style.css">
    <title>WebServ</title>
</head>
<body class="startcgi">
    <header>
        <h1>CGI WITH POST 🔥🔥🔥</h1>
		<?php
            if ($_POST && !empty($_POST['name']))
            {
                echo "<h2>Hey " . $_POST['name'] . " you just send a POST request to our CGI</h2>";
                if ($path_filename_ext)
                    echo "<img alt=\"".$file."\" src=\"".$path_filename_ext."\">";
            }
            else
            {
                echo "<h2>Every request will be send to php-cgi</h2>";
            }
        ?>
    </header>
    <nav>
            <a href="post_cgi.html" class="previous">Previous</a>
            <a href="endcgi.html" class="next">Next</a>
	</nav>
    <?php
        if ($fileexits){
            ?>
            <script>
                alert("File already exists");
            </script>
            <?php
        }
    ?>
</body>
</html>