<?php
include('dbConfig.php');

$sql = "INSERT INTO customer_login (pin,new,date_in) VALUES ('".$_GET['pin']."',true,now())";

//$result = $conn->query($sql);
if ($conn->query($sql) === TRUE) {
  echo "inserted...";
}
else
{
  echo "Error: " . $sql . "<br>" . $conn->error;
}
$conn->close();

?>
