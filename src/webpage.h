#include <Arduino.h>

const char webpage[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>  
    <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
        h1 {color: #0F3376; padding: 2vh;}
        p {font-size: 1.5rem;}
        .button {display: inline-block; background-color: #e7bd3b; border: none; border-radius: 4px; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
        .button2 {background-color: #4286f4;}
    </style>

    
</head>
<body>
  
    <button class="button button2" onclick="window.location.href = '/config';">Web Server Config</button>
</body>
</html>







    )rawliteral";