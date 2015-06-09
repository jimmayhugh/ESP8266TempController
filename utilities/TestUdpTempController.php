<?php
/*
UdpTempController - TestUdpTempController.php

Version 0.0.1
Last Modified 06/09/2015
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
$count = 0;
$remote_port = '2652';
$secCnt = 0;

//Do some communication, this loop can handle multiple clients

$execCmd = "avahi-resolve --name $argv[1]";
$execArray = explode("\t", exec($execCmd));
$arrayCnt = count($execArray);
//echo "There are $arrayCnt values\n";
$remote_ip = $execArray[1];
echo "Using address $remote_ip\n"; 


 
while(1)
{    

    //Take some input to send
    echo 'Enter a message to send : ';
    $in = fgets(STDIN);
    $out = $in."\n";


    socket_sendto($sock, $in , 100 , 0 , $remote_ip , $remote_port);
    socket_recvfrom($sock, $buf, 2048, MSG_WAITALL, $remote_ip, $remote_port);
    
    $resultStr = "";
    if($buf === "=")
    {
      $in = "=";
      socket_sendto($sock, $in , 100 , 0 , $remote_ip , $remote_port);
      socket_recvfrom($sock, $buf, 2048, MSG_WAITALL, $remote_ip, $remote_port);

      while($buf !== '+')
      {
        $resultStr .= $buf;
        socket_sendto($sock, $in , 100 , 0 , $remote_ip , $remote_port);
        socket_recvfrom($sock, $buf, 2048, MSG_WAITALL, $remote_ip, $remote_port);
      }
    }else{
      $resultStr .= $buf;
    }
    echo "\$resultStr: \n$resultStr\n";
}
 
socket_close($sock);
?>
