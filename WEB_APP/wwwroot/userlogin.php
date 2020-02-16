<html>
<head>
	<link rel="stylesheet" href="css/styles.css">
	<title>Users login</title>
</head>
<body>
    <h1>User login list page</h1>
    <blockquote>
        <div>
            <form action="reschedule.php" method="post">
							<table id="t01" align="center" border="1">

									<tr>
                    <th>User access ID</th>
                    <th>Date</th>
                    <th>Device</th>
                  </tr>
                	<?php
										include('dbConfig.php');

                    $sql = "select * from user_login order by date desc";
                    $result = $conn->query($sql);

                    if ($result->num_rows > 0) {
                        while ($row = $result->fetch_assoc()) {
                            echo "<tr><td width='100px'>". $row["card"] ."</td>";
                            echo "<td width='100px'>". $row["date"] ."</td>";
                            echo "<td width='100px'>". $row["device"] ."</td></tr>";
                        }
                    } else {
                        echo "0 results";
                    }
                    $conn->close();

                    ?>
                </tr>
								<tr>
                	<th></th>
									<td><input class="form_button" type="button" onclick="location.href='http://localhost';" value="Home" /></td>
                  <td></td>

								</tr>
            </form>
        </div>
    </blockquote>
		<script type="text/javascript" src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script>
		<script type="text/javascript" src="js/jQuery.js"></script>
</body>
</html>
