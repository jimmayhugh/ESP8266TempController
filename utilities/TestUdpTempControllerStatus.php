<?php
/*
UdpTempController - TestUdpTempControllerStatus.php

Version 0.0.1
Last Modified 06/26/2015
By Jim Mayhugh

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

This software uses multiple libraries that are subject to additional
licenses as defined by the author of that software. It is the user's
and developer's responsibility to determine and adhere to any additional
requirements that may arise.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
 
//Reduce errors
error_reporting(~E_WARNING);

 
//Create a UDP socket
if(!($sock = socket_create(AF_INET, SOCK_DGRAM, 0)))
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);
     
    die("Couldn't create socket: [$errorcode] $errormsg \n");
}

if(!(socket_set_option($sock, SOL_SOCKET, SO_KEEPALIVE, 1)))
{
  echo "socket_set_option SO_KEEPALIVE failed\n";
}else{
  $option = socket_get_option($sock, SOL_SOCKET, SO_KEEPALIVE);
  echo "SO_KEEPALIVE = ".$option."\n";
}

if(($options = socket_get_option($sock, SOL_SOCKET, SO_RCVTIMEO, array('sec'=>10,'usec'=>0))) === FALSE)
{
  echo "socket_set_option for SO_RCVTIMEO failed\n";
}else{
  echo "SO_RCVTIMEO = 5 seconds\n";
}

$localURL = "esp8266.local";
$localIP;
$localPort; 
$count = 0;
$remote_port = '2652';
$secCnt = 0;
$outputArray = array();
$execRet;

//Do some communication, this loop can handle multiple clients

if(isset($argv[1]))
{
  if(is_ipv4($argv[1]))
  {
    $remote_ip = $argv[1];
  }else{
    $execCmd = "avahi-resolve --name $argv[1]";
    $execArray = explode("\t", exec($execCmd, $outputArray, $execRet));
    //$outputCnt = count($outputArray);
    //echo "There are $outputCnt values in \$outputCnt\n";
    //echo "\$outputArray[1] = $outputArray[1]\n";
    //echo "\$execRet = $execRet\n";
    $arrayCnt = count($execArray);
    //echo "There are $arrayCnt \$arrayCnt values\n";
    for($z = 0; $z < $arrayCnt; $z++)
    {
      if( (!(isset($execArray[$z]))) || ($execArray[$z] == NULL ) )
        die("Couldn't resolve $argv[1], exiting\n");
    }
    $remote_ip = $execArray[1];
  }
}else{
  die("ERROR - Requires IP Address or domain.local, UDP Port optional - EXITING\n");
}

if(isset($argv[2]))
{
  $remote_port = $argv[2];
}

socket_getsockname ( $sock, $localIP, $localPort );
echo "setting startUdpUpdateStatus at Remote:$remote_ip:$remote_port using Local:$localIP:$localPort\n"; 

$in = "R\n";

socket_sendto($sock, $in , 100 , 0 , $remote_ip , $remote_port);

 
while(1)
{    

    $result = socket_recvfrom($sock, $buf, 2048, MSG_WAITALL, $remote_ip, $remote_port);
    
    if( $result !== FALSE )
    {
      $now = date("m/d/Y Hi");
      echo "\$buf: $buf at $now\n";
    }
}

echo "Closing socket and exiting\n";
socket_close($sock);

function is_ipv4($str)
{
	$ret = filter_var($str, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4);

	return $ret;
}
?>
