<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<?php
  include("/var/www/htdocs/esp8266/udpRequest.php");
  include_once("/var/www/htdocs/esp8266/accessDatabase.php");

  if(isset($_POST["device_address"]) && isset($_POST["device_port"]) && isset($_POST["submit"]))
  {
    $deviceAddress = $_POST["device_address"];
    $devicePort = $_POST["device_port"];
    
    $queryStr = "INSERT INTO Addresses SET ipAddress='".$deviceAddress."', udpPort=".$devicePort;

    echo "\$queryStr = $queryStr<br />";
    
    if(mysqli_query($link, $queryStr))
    {
    }else{
      echo "Error: " . $queryStr . "<br>" . mysqli_error($link);
    }
  }

  if(isset($_POST["ip_address"]) && isset($_POST["udp_port"]) && isset($_POST["remove"]))
  {
    $deviceAddress = $_POST["ip_address"];
    $devicePort = $_POST["udp_port"];
    
    $queryStr = "DELETE FROM `Addresses` WHERE ipAddress='".$deviceAddress."'AND udpPort=".$devicePort;

    echo "\$queryStr = $queryStr<br />";
    
    if(mysqli_query($link, $queryStr))
    {
    }else{
      echo "Error: " . $queryStr . "<br>" . mysqli_error($link);
    }
  }

    $headStr = "
      <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">
      <title> ESP8266 1-Wire Temperature / Switch Status </title>
      <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"/>
      <script type=\"text/javascript\" src=\"//ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js\"></script>
      <!-- <script type=\"text/javascript\" src=\"js/jquery.js\"></script> -->
      <script>
        $(document).ready(function() {
          $(\"#container\").load(\"updateStatus.php\");
       var refreshId = setInterval(function() {
          $(\"#container\").load(\"updateStatus.php\");
       }, $udpUpdate);
       $.ajaxSetup({ 
                      cache: false
                   });
      });
      </script>
      </script>
      <style>
        input[type='text'] { font-size: 18px; text-align: center;}
        input:focus, textarea:focus{background-color: lightgrey;}
        select[type='text'] { font-size: 18px; text-align: center;}
      </style>";
  echo $headStr;
  $dataTimer = 0;
?>
</head>
<body>
<?php
  include("header.html");
?> 
<!-- Table for Main Body -->
<table width="100%" border="0" cellspacing="0" cellpadding="2">
  <tr>
    <td  align="center" colspan="6">
      <table width="100%" border="1" cellspacing="0" cellpadding="2">
        <tr>
          <td align="center" border="2" colspan="6">
            <?php
              $titleStr = "<h2><font color = \"blue\">ESP8266 1-Wire Temperature / Switch Status</font></h2>";
              echo $titleStr;
            ?>
          </td>
        </tr>
        </table>      
    </td>
  </tr>
  <tr>
    <td  align="center" colspan="6">
      <form method="post" action="/esp8266/SensorStatus.php">
        <strong>New Device IP Address:</strong> <input type="text" name="device_address">
        <strong>New Device PORT Address:</strong> <input type="text" name="device_port">
        <input type="submit" name="submit" value="SUBMIT">
      </form>
    </td>
  </tr>  
  <tr>
    <td>
      <div id="container">
      </div>
    </td>
  </tr>
</table>
</body></html>
