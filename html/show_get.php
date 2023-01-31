


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
        <h1>CGI WITH GET 🔥🔥🔥</h1>
		<?php
            if ($_GET && !empty($_GET['name']) && !empty($_GET['login']))
            {
                echo "<h2>Hey " . $_GET['name'] . " (" . $_GET['login']. ") you just send a GET request to our CGI</h2>";
            }
            else
            {
                echo "<h2>Every request will be send to php-cgi</h2>";
            }
        ?>

    </header>
    <nav>
            <a href="startcgi.html" class="previous">Previous</a>
            <a href="post_cgi.html" class="next">Next</a>
	</nav>
</body>
</html>