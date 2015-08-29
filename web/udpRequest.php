<?php
$setUpperSwitch     = "A";
$setLowerSwitch     = "B";
//$                 = "C";
$setDebug           = "D";
//$                 = "E";
//$                 = "F";
//$                 = "G";
//$                 = "H";
//$                 = "I";
//$                 = "J";
//$                 = "K";
//$                 = "L";
$setMode            = "M";
$setDomain          = "N";
//$                 = "O";
//$                 = "P";
//$                 = "Q";
$startStatusUpdate  = "R";
$updateChipName     = "S";
//$                 = "T";
//$                 = "U";
//$                 = "V";
//$                 = "W";
//$                 = "X";
//$                 = "Y";
$getStatus          = "Z";


$on  = "1";
$off = "0";
$tooHot =  "H";
$tooCold = "C";
$socBufSize = 2048;
$udpUpdate = 2000;

$dummyAddr="0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00";

$execCommand = "sudo arp-scan --localnet | grep ESP8266";
$execOutput = array();
$execResult;

error_reporting(E_ALL);


function udpRequest($ip_Address, $udp_Port, $in)
{

$myPID = getmypid();

error_reporting(~E_WARNING);

$fileName = "/var/log/esp8266/udp_".$ip_Address."_".$udp_Port.".log";   
$socBufSize = 2048;

  if(!($socket = socket_create(AF_INET, SOCK_DGRAM, 0)))
  {
      $errorcode = socket_last_error();
      $errormsg = socket_strerror($errorcode);
       
      die("Couldn't create socket: [$errorcode] $errormsg \n");
  }


  if(!socket_set_option($socket,SOL_SOCKET,SO_RCVTIMEO,array("sec"=>2,"usec"=>0)))
  {
      die("Couldn't set Socket Receive Timeout\n");
  }

  if( ! $fr = fopen($fileName, "a+") )
  {
	  die("Could not open file:".$fileName."\n");
  }else{
    fwrite($fr, "\n");
    fwrite($fr, date("D M j G:i:s Y"));
  } 

  $maxTries = 5;
  
  do{
    $result = "";
    fwrite($fr, "\n\$ip_address = $ip_Address, \$udp_Port = $udp_Port, pid = $myPID\n");
    socket_sendto($socket, $in , strlen($in) , 0 , $ip_Address , $udp_Port);
    fwrite($fr, "command sent: ".$in);

    if(socket_recvfrom($socket, $result, $socBufSize, MSG_WAITALL, $ip_Address, $udp_Port))
    	fwrite($fr, socket_strerror(socket_last_error($socket)));
      
    $resultStr = "";
      
    if($result === "=")
    {
      $in = "=";
      fwrite($fr, "large packet started: ".$in."\n");
      socket_sendto($socket, $in , strlen($in) , 0 , $ip_Address , $udp_Port);
      socket_recvfrom($socket, $result, $socBufSize, MSG_WAITALL, $ip_Address, $udp_Port);
      fwrite($fr, "first packet received: ".$result."\n");

      
      do{
        $resultStr .= $result;
        $result = "";
        fwrite($fr, "requesting next segment: ".$in."\n");
        socket_sendto($socket, $in , strlen($in), 0 , $ip_Address , $udp_Port);
        socket_recvfrom($socket, $result, $socBufSize, MSG_WAITALL, $ip_Address, $udp_Port);
        fwrite($fr, "next packet received: ".$result."\n");
      }while($result !== '+');
      
      if($result === '+')
      {
        fwrite($fr, "final segment received: ".$result."\n");
        $result = "";
      }
      
      $in = "";
    }else{
      $resultStr .= $result;
      $result = "";
    }
    $trimmedResultStr = trim($resultStr);
    $maxTries--;
  }while($trimmedResultStr === "ERROR" && $maxTries > 0);
  
  if($maxTries === 0)
  {
    fwrite($fr, "maxTries exceeded on PID# $myPID, I'm killing myself\n");
    fclose($fr);
    socket_close($socket);
    die();
  }

	if($resultStr === "")
	{
  	fwrite($fr, "NO Packet Received:\n");
	}else{
  	fwrite($fr, "packet received:\n".$resultStr);
  }
  fclose($fr);

  socket_close($socket);
  return $resultStr;
}
?>

