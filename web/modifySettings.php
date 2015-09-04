<?php
  include_once("udpRequest.php");
  include_once("accessDatabase.php");

  error_reporting(E_ERROR);

  if(isset($_POST["ip_address"])) 
    $ip_Address=trim($_POST["ip_address"]);
  if(isset($_POST["udp_port"])) 
    $udp_Port=trim($_POST["udp_port"]);
  if(isset($_POST["device_name"]))
    $deviceName=trim($_POST["device_name"]);
  if(isset($_POST["upperTempc"]))
    $upperTempc=trim($_POST["upperTempc"]);
  if(isset($_POST["upperTempf"]))
    $upperTempf=trim($_POST["upperTempf"]);
  if(isset($_POST["upperDelay"]))
    $upperDelay=trim($_POST["upperDelay"]);
  if(isset($_POST["lowerTempc"]))
    $lowerTempc=trim($_POST["lowerTempc"]);
  if(isset($_POST["lowerTempf"]))
    $lowerTempf=trim($_POST["lowerTempf"]);
  if(isset($_POST["lowerDelay"]))
    $lowerDelay=trim($_POST["lowerDelay"]);

  //echo "unconditional: \$ip_Address = $ip_Address, \$udp_Port = $udp_Port, \$deviceName = $deviceName<br />";
  
  //echo "\$upperTempc = $upperTempc, \$upperTempf = $upperTempf, \$upperDelay = $upperDelay<br />\$lowerTempc = $lowerTempc, \$lowerTempf = $lowerTempf, \$lowerDelay = $lowerDelay<br />";

  if(isset($_POST["setUpdate"]))
  {
    $in = $setDomain." ".$deviceName."\n";
    $udpStatus = udpRequest($ip_Address, $udp_Port, $in);
    if(isset($_POST["tempu"]))
    {
      if($_POST["tempu"] === "c")
      {
        if(isset($_POST["upperTemp"]))
        {
          $upperTemp = trim($_POST["upperTemp"]);
        }else{
          $upperTemp = $upperTempc;
        }
        $in=$setAutoTemp.",U,C,".$upperTemp.",".$upperDelay."\n";
        //echo "\$in = $in<br />";
        $tempuStr = 
        "
          <input type=\"radio\" name=\"tempu\" value=\"f\">&deg;F
          <br />
          <input type=\"radio\" name=\"tempu\" value=\"c\" checked>&deg;C
        ";
      }else{
        if(isset($_POST["upperTemp"]))
        {
          $upperTemp = trim($_POST["upperTemp"]);
        }else{
          $upperTemp = $upperTempf;
        }
        $in=$setAutoTemp.",U,F,".$upperTemp.",".$upperDelay."\n";
        //echo "\$in = $in<br />";
        $tempuStr =
        "
          <input type=\"radio\" name=\"tempu\" value=\"f\" checked>&deg;F
          <br />
          <input type=\"radio\" name=\"tempu\" value=\"c\">&deg;C
        ";
      }
    }
    $udpStatus = udpRequest($ip_Address, $udp_Port, $in);
    //echo "$udpStatus<br />";

    if(isset($_POST["templ"]))
    {
      if($_POST["templ"] === "c")
      {
        if(isset($_POST["lowerTemp"]))
        {
          $lowerTemp = trim($_POST["lowerTemp"]);
        }else{
          $lowerTemp = $lowerTempc;
        }
        $in=$setAutoTemp.",L,C,".$lowerTemp.",".$lowerDelay."\n";
        //echo "\$in = $in<br />";
        $templStr =
        "
          <input type=\"radio\" name=\"templ\" value=\"f\">&deg;F
          <br />
          <input type=\"radio\" name=\"templ\" value=\"c\" checked>&deg;C
        ";
      }else{
        if(isset($_POST["lowerTemp"]))
        {
          $lowerTemp = trim($_POST["lowerTemp"]);
        }else{
          $lowerTemp = $lowerTempf;
        }
        $in=$setAutoTemp.",L,F,".$lowerTemp.",".$lowerDelay."\n";
        //echo "\$in = $in<br />";
        $templStr =
        "
          <input type=\"radio\" name=\"templ\" value=\"f\" checked>&deg;F
          <br />
          <input type=\"radio\" name=\"templ\" value=\"c\">&deg;C
        ";
      }
    }
    $udpStatus = udpRequest($ip_Address, $udp_Port, $in);
    //echo "$udpStatus<br />";

    $in = $setMode."A\n";
    $udpStatus = udpRequest($ip_Address, $udp_Port, $in);
    //echo "$udpStatus<br />";
    
    $in = $getStatus."\n";
    $udpStatus = udpRequest($ip_Address, $udp_Port, $in);
    //echo "$udpStatus<br />";

    $titleStr = "<h2><font color = \"red\">Settings Updated</font></h2>";
  }else{
    $in = $getStatus."\n";
    //echo "\$in = $in<br />";
    $udpStatus = udpRequest($ip_Address, $udp_Port, $in);
    //echo "\$udpStatus = $udpStatus<br />";
    $udpType = explode(",", $udpStatus);
    $deviceName = trim($udpType[4]);
    //echo "\$deviceName = $deviceName<br />";

    $in = $getMode."\n";
    //echo "\$in = $in <br />";
    
    $udpStatus = udpRequest($ip_Address, $udp_Port, $in);
    //echo "\$udpStatus = $udpStatus<br />";
    $udpType = explode(",", $udpStatus);
    $upperTempf = $udpType[0];
    $upperTempc = $udpType[1];
    $upperDelay = trim($udpType[2]);
    $lowerTempf = $udpType[3];
    $lowerTempc = $udpType[4];
    $lowerDelay = trim($udpType[5]);
    $upperTemp = $upperTempf;
    $lowerTemp = $lowerTempf;
    $tempuStr =
    "
      <input type=\"radio\" name=\"tempu\" value=\"f\" checked>&deg;F
      <br />
      <input type=\"radio\" name=\"tempu\" value=\"c\">&deg;C
    ";

    $templStr =
    "
      <input type=\"radio\" name=\"templ\" value=\"f\" checked>&deg;F
      <br />
      <input type=\"radio\" name=\"templ\" value=\"c\">&deg;C
    ";
    $titleStr = "<h2><font color = \"blue\">Modify Settings</font></h2>";
  }
  
  //echo "updateStatus.php: \$ip_Address = $ip_Address, \$udp_Port = $udp_Port, \$deviceName = $deviceName<br />";
  
  //echo "\$upperTempc = $upperTempc, \$upperTempf = $upperTempf, \$upperDelay = $upperDelay<br />\$lowerTempc = $lowerTempc, \$lowerTempf = $lowerTempf, \$lowerDelay = $lowerDelay<br />";
  
  $headStr =
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">
    <html>
    <head>
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">
    <META HTTP-EQUIV=\"CACHE-CONTROL\" CONTENT=\"NO-CACHE\">
    <title> Temperature Data </title>
    <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"/>
    <script type=\"text/javascript\" src=\"//ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js\"></script>
    <!-- <script type=\"text/javascript\" src=\"js/jquery.js\"></script> -->
    <style>
    input[type='text'] { font-size: 18px; text-align: center;}
    input:focus, textarea:focus{background-color: lightgrey;}
    </style>
    </head>
      <body>";

  $headerStr =
  "
    <table height=\"250\" width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"1\">
      <tr>
        <td>
          <a href=\"http://".$_SERVER['HTTP_HOST']."/esp8266/SensorStatus.php\">
            <img src=\"/images/ESP8266Net.png\" width=\"100%\" height=\"250\" border=\"0\" alt=\"logo\">
          </a>
        </td>
      </tr>
    </table>
  ";

  $bodyStr =
  "
  <!-- Table for Main Body -->
  <table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">
    <tr>
      <td  align=\"center\" colspan=\"6\">
        <table width=\"100%\" border=\"1\" cellspacing=\"0\" cellpadding=\"2\">
          <tr>
            <td align=\"center\" border=\"2\" colspan=\"6\">
              $titleStr
            </td>
          </tr>
          </table>      
      </td>
    </tr>  
    <tr>
      <td align=\"center\" border=\"2\" colspan=\"6\">
        <form method=\"post\" action=\"modifySettings.php\">
         <input type=\"hidden\" name=\"ip_address\" value=\"".$ip_Address."\">
         <input type=\"hidden\" name=\"udp_port\" value=\"".$udp_Port."\">
         <input type=\"hidden\" name=\"device_name\" value=\"".$deviceName."\">
         <input type=\"hidden\" name=\"setUpdate\" value=\"setUpdate\">
      </td>
    </tr>
  </table>
  <table align=\"center\" width=\"50%\" border=\"1\" cellspacing=\"0\" cellpadding=\"2\">
    <tr>  
      <td align=\"center\" colspan=\"3\">
        <input type=\"submit\" value=\"SAVE\">  
      </td>
    </tr>
    <tr>  
      <td align=\"center\" colspan=\"3\">
        NAME: <input type=\"text\" name=\"device_name\" value=\"$deviceName\">  
      </td>
    </tr>
    <tr>
      <td align=\"center\">
        Upper Temperature<br /><input type=\"text\" name=\"upperTemp\" value=\"$upperTemp\">  
      </td>
      <td align=\"center\">
        $tempuStr
      </td>
      <td align=\"center\">
        Upper Delay (Seconds)<br /><input type=\"text\" name=\"upperDelay\" value=\"$upperDelay\">  
      </td>
    </tr>
    <tr>
      <td align=\"center\">
        Lower Temperature<br /><input type=\"text\" name=\"lowerTemp\" value=\"$lowerTemp\">  
      </td>
      <td align=\"center\">
        $templStr
      </td>
      <td align=\"center\">
        Lower Delay (Seconds)<br /><input type=\"text\" name=\"lowerDelay\" value=\"$lowerDelay\">  
      </td>
    </tr>
    <tr>  
      <td align=\"center\" colspan=\"3\">
        <input type=\"submit\" value=\"SAVE\">  
      </td>
    </tr>
  </table>
  ";

  $footStr =
    "
      </body>
      </html>
    ";
  echo "$headStr$headerStr$bodyStr$footStr";
?>
