<!-- <?php echo "this is cgi ". $_POST["name"] . "!!!";  ?> -->

<?php var_dump($_POST); ?>
<?php var_dump($_GET); ?>

<?php

/*
QUERY_STRING=name=value
REQUEST_METHOD=GET
SCRIPT_FILENAME=/mnt/nfs/homes/mgoncalv/test.php
REDIRECT_STATUS=200
/usr/bin/php-cgi -f /mnt/nfs/homes/mgoncalv/test.php
*/