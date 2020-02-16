<html>
<head>
	<link rel="stylesheet" href="css/styles.css">
	<title>Reschedule reservation</title>
</head>
<body>
    <h1>Customer Profile Search Page</h1>

		<?php
      include('dbConfig.php');
      if(isset($_POST['save']))
      {
				$user_pin = $_POST['0'];
				$sql = "UPDATE registos SET end_date = '".$_POST['2']."', start_date = '".$_POST['1']."' WHERE pin = '".$_POST['0']."'";

        //$result = $conn->query($sql);
        if ($conn->query($sql) === TRUE) {
          echo "updated...";
          header("Location: http://localhost/validated.php?reg=$user_pin");
        }
        else
        {
          echo "Error: " . $sql . "<br>" . $conn->error;
        }
        $conn->close();
      }
    ?>

    <blockquote>
        <div>
            <form action="reschedule.php" method="post">
							<table id="t01">
								<tr>
									<th><label>Name</label></th>
                	<td><select name="name" id="name" class="combo">
                	<?php
										include('dbConfig.php');

                    $sql = "select * from registos";
                    $result = $conn->query($sql);

                    if ($result->num_rows > 0) {
                        while ($row = $result->fetch_assoc()) {
                            echo "<option value='" . $row["pin"] . "_" . $row["start_date"] . "_" . $row["end_date"] . "'>" . $row["name"] . "</option>";
                        }
                    } else {
                        echo "0 results";
                    }
                    $conn->close();

                    ?></select></td>
                </tr>
								<tr>
									<th><label>Pin</label></th>
									<td><input id="0" type="text" name="0" disabled></td>
								<tr>
									<th><label>Checkin</label></th>
									<td><input id="1" type="datetime" name="1"></td>
								</tr>
                <tr>
									<th><label>Checkout</label></th>
									<td><input id="2" type="datetime" name="2"></td>
								</tr>
								<tr>
                	<th></th>
									<td><button class="form_button" id = "save" type="submit" name="save">save</button><input class="form_button" type="button" onclick="location.href='http://localhost';" value="Home" /></td>

								</tr>
            </form>
        </div>
    </blockquote>
		<script type="text/javascript" src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script>
		<script type="text/javascript" src="js/jQuery.js"></script>
</body>
</html>
