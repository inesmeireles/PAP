<!Doctype html>
<html>
  <head>
    <link rel="stylesheet" href="css/styles.css">
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta name="description" content="$1">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Booking</title>
  </head>
  <body>
    <h1>NEW BOOKING</h1>
    <?php
      include('dbConfig.php');
      if(isset($_POST['save']))
      {
        $user_pin = rand(10001,99999);

        $sql = "INSERT INTO registos (name,pin,start_date,end_date) VALUES ('".$_POST["name"]."','".$user_pin."','".$_POST["start_date"]." 14:00','".$_POST["end_date"]." 12:00')";

        //$result = $conn->query($sql);
        if ($conn->query($sql) === TRUE) {
          echo "inserted...";
          header("Location: http://localhost/validated.php?reg=$user_pin");
        }
        else
        {
          echo "Error: " . $sql . "<br>" . $conn->error;
        }
        $conn->close();
      }
    ?>

    <form action="booking.php" method="post">
      <table id="t01">
        <tr>
          <th><label>Name</label></th>
          <td><input type="text" name="name"></td>
        </tr>
        <tr>
          <th><label>Check in date</label></th>
          <td><input type="date" name="start_date" value="<?php echo date('Y-m-d'); ?>"></td>
        </tr>
        <tr>
          <th><label>Check out date</label></th>
          <td><input type="date" name="end_date" value="<?php echo date('Y-m-d'); ?>"></td>
        </tr>
        <tr>
          <td></td>
          <td><button class="form_button" type="submit" name="save">save</button><input class="form_button" type="button" onclick="location.href='http://localhost';" value="Home" /></td>
        </tr>
      </table>
    </form>
  </body>
</html>
