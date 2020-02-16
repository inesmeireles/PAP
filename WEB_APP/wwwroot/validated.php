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
    <?php
    if (isset($_GET['reg'])) {
        include('dbConfig.php');
        include('qrlib/qrlib.php');

        QRcode::png($_GET['reg'], "qrlib/".$_GET['reg'].".png", 'Q', 4, 2);
        //echo '<img src="data:image/jpeg;base64,qrlib/".$_GET['reg'].".png height="100" width="100"/>';

        $style = array(
            'border' => true,
            'padding' => 4,
            'fgcolor' => array(0,0,0),
            'bgcolor' => false, //array(255,255,255)
        );
        // $pdf->write2DBarcode('PHP QR Code :)', 'QR,L', '', '', 30, 30, $style, 'N');
        // $tab = $qr->encode('PHP QR Code :)');
        // QRspec::debug($tab, true);

        $sql = "SELECT name, pin, DATE(start_date) as start_date, DATE(end_date) as end_date FROM registos where pin = ".$_GET['reg']."";
        $result = $conn->query($sql);
        if ($result->num_rows > 0) {
            while ($row = $result->fetch_assoc()) {
              ?>
              <table id="t01">
                <tr>
                  <td></td>
                  <td><img src="qrlib/<?php echo $_GET['reg'] ?>.png" height="200" width="200"/></td>
                </tr>
                <tr>
                </tr>
                <tr>
                  <th><label>Key: </label></th>
                  <td><?php echo $row['pin']; ?></td>
                </tr>
                <tr>
                  <th><label>Name: </label></th>
                  <td><?php echo $row['name']; ?></td>
                </tr>
                <tr>
                  <th><label>Check in date: </label></th>
                  <td><?php echo $row['start_date']; ?></td>
                </tr>
                <tr>
                  <th><label>Check out date: </label></th>
                  <td><?php echo $row['end_date']; ?></td>
                </tr>
                <tr>
                  <th></th>
                  <td><input type="button" onclick="location.href='http://localhost';" value="Home" /></td>
                </tr>
              </table>
            <?php
            }
        } else {
            echo "0 results";
        }
        $conn->close();

    } else {
        // Fallback behaviour goes here
    }
    ?>
  </body>
</html>
