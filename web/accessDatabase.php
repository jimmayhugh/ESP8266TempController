<?php
$username="esp8266";
$password="esp8266";
$database="esp8266";

$link = mysqli_connect("localhost",$username,$password);
@mysqli_select_db($link,$database) or die( "Unable to select database");

?>
