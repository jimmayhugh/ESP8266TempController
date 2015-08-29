<?php
  include_once('/var/www/htdocs/esp8266/PHP_GnuPlot.php'); 
  include_once("/var/www/htdocs/esp8266/accessDatabase.php");

  error_reporting(E_ERROR);

  if(isset($_GET["ip_address"])) 
  {
    $ip_address=$_GET["ip_address"];
    $deviceName=$_GET["device_name"];
//      echo "updateStatus.php: netID = $netID, service_port = $service_port, port_address = $port_address, netName = $netName <br />";
  }
  
  if(isset($_POST["ip_address"])) 
  {
    $ip_address=$_POST["ip_address"];
    $deviceName=$_POST["device_name"];
//      echo "updateStatus.php: netID = $netID, service_port = $service_port, port_address = $port_address, netName = $netName <br />";
  }
  

  set_time_limit(0);
  $hotTest = "";
  $coldTest = "";
  $pTemp = new GNUPlot();
  $actionTime = date("U");
  $rightNow = new DateTime(date());
//  echo "rightNow = ".$rightNow->format("Y-m-d H:i")."\n";
  switch($_POST["interval"])
  {
    case "60":
      $interval = "Minutes";
      break;

    case "86400":
      $interval = "Days";
      break;

    case "2592000":
      $interval = "Months";
      break;

    case "31104000":
      $interval = "Years";
      break;

    case "3600":
    default:
      $interval = "Hours";
      break;
  }
  if((is_numeric($_POST["cntBack"])) && (is_numeric($_POST["interval"])))
  {
//    echo $_POST["cntBack"] * $_POST["interval"]."\n";
    $startTime = $actionTime - ($_POST["cntBack"] * $_POST["interval"]);
//    echo $startTime."\n";
    $query = "SELECT * FROM device WHERE ipaddress='".$ip_address."' AND time>='".$startTime."'";
  }else if($_POST["cntBetween"] === "cntBetween"){
    $startDate = new DateTime($_POST["startYear"]."/".$_POST["startMonth"]."/".$_POST["startDay"]." ".$_POST["startHour"].":".$_POST["startMinute"]);
    $endDate = new DateTime($_POST["endYear"]."/".$_POST["endMonth"]."/".$_POST["endDay"]." ".$_POST["endHour"].":".$_POST["endMinute"]);
/*
    echo "startDate = ".$startDate->format("Y-m-d H:i")."\n";
    echo "endDate = ".$endDate->format("Y-m-d H:i")."\n";
    echo "startDate = ".$startDate->format("U")."\n";
    echo "endDate = ".$endDate->format("U")."\n";
*/
    $query = "SELECT * FROM device WHERE ipaddress='".$ip_address."' AND time>='".$startDate->format("U")."' AND time<='".$endDate->format("U")."'";
//    echo $query."\n";
    $timeSpan = $endDate->format("U") - $startDate->format("U");
//    echo "timeSpan = ".$timeSpan."\n";
  }else{
    $query = "SELECT * FROM device WHERE ipaddress='".$ip_address."'";
  }
//  echo $query."\n";
  $result = mysqli_query($link, $query);
  $rowCnt = mysqli_num_rows($result);

// set line colors
  $pTemp->set("style line 1 linewidth 2 linecolor rgb \"#000\""); 
  $pTemp->set("style line 2 linecolor rgb \"red\""); 
  $pTemp->set("style line 3 linecolor rgb \"blue\""); 

// assign line values
  $tempData = new PGData('Temperature');

  $oldTime = NULL; 
  while ($obj = mysqli_fetch_object($result))
  {
    if($hotTest ==="")
    {
      $hotTest = $obj->tempf;
    }else if($obj->tempf > $hotTest){
      $hotTest = $obj->tempf;
    } 
    if($coldTest ==="")
    {
      $coldTest = $obj->tempf;
    }else if($obj->tempf < $coldTest){
      $coldTest = $obj->tempf;
    } 
    if($oldTime != NULL)
    {
      $secCnt = ($obj->time) - $oldTime;
      $tempData->addDataEntry(array($obj->tempf, $secCnt ));
    }else{
      $tempData->addDataEntry(array($obj->tempf, 0));
      $oldTime = $obj->time;
    }
  }
//  echo "Select returned ".$rowCnt." rows.\n";
  mysqli_free_result($result);
  $tempTitle = "$deviceName Temperature Graph";
  $pTemp->setTitle($tempTitle);
  if(is_numeric($_POST["interval"]))
  { 
    $tickMark = "($2/".$_POST["interval"]."):1";
  }else if($_POST["cntBetween"] === "cntBetween"){
//    echo "entering timeSpan Switch Statement\n";
    switch($timeSpan)
    {
      case (($timeSpan >= 0) && ($timeSpan <= 3600)):
        $interval = "Minutes";
        $tickMark = "($2/60):1";
        break; 

      case (($timeSpan >= 86401 ) && ($timeSpan <= 2592000)):
        $interval = "Days";
        $tickMark = "($2/86400):1";
        break; 

      case (($timeSpan >= 2592001 ) && ($timeSpan <= 31104000)):
        $interval = "Months";
        $tickMark = "($2/2592000):1";
        break; 

      case ($timeSpan >= 31104001) :
        $interval = "Years";
        $tickMark = "($2/31104000):1";
        break;

      default: 
      case (($timeSpan >= 3601) && ($timeSpan <= 86400)):
        $interval = "Hours";
        $tickMark = "($2/3600):1";
        break; 
    }

  }else{
    $interval = "Hours";
    $tickMark = "($2/3600):1";
  }
//  echo "tickMark = ".$tickMark."\n";
  $pTemp->setDimLabel(x, $interval);
  $pTemp->setDimLabel(y, "Degrees");
  $pTemp->set("terminal png size 1280,400"); 
  $pTemp->plotData( $tempData, 'lines', $tickMark,'','ls 1'); 
//  $pTemp->plotData( $thData, 'lines', $tickMark,'','ls 2'); 
//  $pTemp->plotData( $tcData, 'lines', $tickMark,'','ls 3');
/* 
  $pTemp->plotData( $tempData, 'lines', '($2/3600):1','','ls 1'); 
  $pTemp->plotData( $thData, 'lines', '($2/3600):1','','ls 2'); 
  $pTemp->plotData( $tcData, 'lines', '($2/3600):1','','ls 3');
*/ 
  $pTemp->setRange('y', $coldTest-1, $hotTest+1); 
  $pTemp->export('/var/www/images/pTemp.png');
  $pTemp->close(); 
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META HTTP-EQUIV="CACHE-CONTROL" CONTENT="NO-CACHE">
<title> Temperature Data </title>
<link rel="stylesheet" type="text/css" href="style.css"/>
<script type="text/javascript" src="//ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<!-- <script type="text/javascript" src="js/jquery.js"></script> -->
<style>
input[type='text'] { font-size: 18px; text-align: center;}
input:focus, textarea:focus{background-color: lightgrey;}
</style>
</head>
  <body>
    <?php 
      include ("header.html");
      $query = "SELECT time FROM device WHERE ipaddress='".$ip_address."'";
      $result = mysqli_query($link, $query);
      $obj = mysqli_fetch_object($result);
      $startDateTime = $obj->time;
//      echo "startDateTime = ".date("Y m d H i", $startDateTime)."<br />";
      mysqli_free_result($result);
      mysqli_close($link);
    ?> 
    <!-- Table for Main Body -->
    <table width="100%" border="0" cellspacing="0" cellpadding="1">
      <tr>
        <td valign="top" align="left" colspan="2" width="150">
        <?php 
        include ("menu.php");
        ?>
        </td>
      </tr>
      <tr>
        <td valign="top" align="center" colspan="2" width="100%">
          <h2><?php echo "$deviceName ($ip_address)"; ?> Graph</h2>
        </td>
      </tr>
      <tr>
        <td align="center">
          <form method="post" action="plotData.php">
            <?php
              $formStr =
                "<input type=\"hidden\" name=\"ip_address\" value=\"".$ip_address."\">";
                echo $formStr;   
             ?>
            Go Back: <input type="text" size="4" maxsize="4" name="cntBack">
            <select name="interval">
              <option value="60">Minutes</option>
              <option value="3600">Hours</option>
              <option value="86400">Days</option>
              <option value="2592000">Months</option>
              <option value="31104000">Years</option>
            </select>
            <input type="submit" value="GRAPH">
          </form>
        </td>
        <td align="center">
          <table>
            <tr>
              <td>
                <form method="post" action="plotData.php">
                  <?php
                    $formStr =
                      "<input type=\"hidden\" name=\"ip_address\" value=\"".$ip_address."\">";
                      echo $formStr;   
                   ?>
                  <input type="hidden" name="cntBetween" value="cntBetween">
              </td>
              <td>
                <table>
                  <tr>
                    <td>
                        Start:
                        <select name="startYear">
                        <?php echo" <option value=\"".date("Y", $startDateTime)."\">".date("Y", $startDateTime)."\n"; ?>
                          <option value="2012">2012</option>
                          <option value="2013">2013</option>
                        </select>
                        <select name="startMonth">
                        <?php echo" <option value=\"".date("m", $startDateTime)."\">".date("F", $startDateTime)."</option>\n"; ?>
                          <option value="01">January</option>
                          <option value="02">February</option>
                          <option value="03">March</option>
                          <option value="04">April</option>
                          <option value="05">May</option>
                          <option value="06">June</option>
                          <option value="07">July</option>
                          <option value="08">August</option>
                          <option value="09">September</option>
                          <option value="10">October</option>
                          <option value="11">November</option>
                          <option value="12">December</option>
                        </select>
                        <select name="startDay">
                        <?php
                            echo"<option value=\"".date("d", $startDateTime)."\">".date("d", $startDateTime)."<option\n";
                            for($thisDay=1; $thisDay < 32; $thisDay++)
                            {
                              echo "\t\t\t  <option value=\"".$thisDay."\">".$thisDay."</option>\n";
                            }
                        ?>
                        </select>
                        <select name="startHour">
                          <?php
                            echo"<option value=\"".date("H", $startDateTime)."\">".date("H", $startDateTime)."</option>\n";
                            for($thisHour=0; $thisHour < 24; $thisHour++)
                            {
                              echo "\t\t\t  <option value=\"".$thisHour."\">".$thisHour."</option>\n";
                            }
                          ?>
                        </select>
                        <select name="startMinute">
                          <?php
                            echo"<option value=\"".date("i", $startDateTime)."\">".date("i", $startDateTime)."</option>\n";
                            for($thisMinute=0; $thisMinute < 60; $thisMinute++)
                            {
                              echo "\t\t\t  <option value=\"".$thisMinute."\">".$thisMinute."</option>\n";
                            }
                          ?>
                        </select>
                      </td>
                    </tr>
                    <tr>
                    <td>
                        End:&nbsp; 
                        <select name="endYear">
                        <?php echo" <option value=\"".$rightNow->format("Y")."\">".$rightNow->format("Y")."</option>\n"; ?>
                          <option value="2012">2012</option>
                          <option value="2013">2013</option>
                        </select>
                        <select name="endMonth">
                        <?php echo" <option value=\"".$rightNow->format("m")."\">".$rightNow->format("F")."</option>\n"; ?>
                          <option value="01">January</option>
                          <option value="02">February</option>
                          <option value="03">March</option>
                          <option value="04">April</option>
                          <option value="05">May</option>
                          <option value="06">June</option>
                          <option value="07">July</option>
                          <option value="08">August</option>
                          <option value="09">September</option>
                          <option value="10">October</option>
                          <option value="11">November</option>
                          <option value="12">December</option>
                        </select>
                        <select name="endDay">
                        <?php
                            echo"<option value=\"".$rightNow->format("d")."\">".$rightNow->format("d")."</option>\n";
                            for($thisDay=1; $thisDay < 32; $thisDay++)
                            {
                              echo "\t\t\t  <option value=\"".$thisDay."\">".$thisDay."</option>\n";
                            }
                        ?>
                        </select>
                        <select name="endHour">
                          <?php
                            echo"<option value=\"".$rightNow->format("H")."\">".$rightNow->format("H")."</option>\n";
                            for($thisHour=0; $thisHour < 24; $thisHour++)
                            {
                              echo "\t\t\t  <option value=\"".$thisHour."\">".$thisHour."</option>\n";
                            }
                          ?>
                        </select>
                        <select name="endMinute">
                          <?php
                            echo"<option value=\"".$rightNow->format("i")."\">".$rightNow->format("i")."</option>\n";
                            for($thisMinute=0; $thisMinute < 60; $thisMinute++)
                            {
                              echo "\t\t\t  <option value=\"".$thisMinute."\">".$thisMinute."</option>\n";
                            }
                          ?>
                        </select>
                      </td>
                  </tr>
                 </table> 
              <td>
                  <input type="submit" value="GRAPH">
              </td>
            </tr>
          </table>
        </td>
      </tr>
      <tr>
        <td colspan="2" >
          <img src="/images/pTemp.png">
        </td>
      </tr>
      <tr>
        <td valign="top" align="left" colspan="2" width="150">
        <?php 
        include ("menu.php");
        ?>
        </td>
      </tr>
    </table>
  </body>
</html>
