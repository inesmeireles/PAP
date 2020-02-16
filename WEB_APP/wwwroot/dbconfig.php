<?php

$servername = "localhost";
$username = "arduino";
$password = "12345678";
$dbname = "arduino";

$conn = new mysqli($servername, $username, $password, $dbname);
//
// mysqli_connect($servername, $username, $password, $dbname);

if(!$conn)
{
  die("Connection failed: " . mysqli_connect_error());
}

?>
