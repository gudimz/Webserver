<?php
// echo "HTTP/1.1 200 OK\r\n";
// echo "Content-type: text/html\r\n\r\n";
// echo "Transfer-Encoding: chunked\r\n\r\n";
// echo "Location: http://example.com\r\n\r\n";
// echo "\r\n\r\n";
// phpinfo();
// if (headers_sent())
//     echo "header sent";
// else
//     echo "header not sent";

// header("\r\n\r\n");
//Print all environment variable names with values
$array;
if ($_SERVER['REQUEST_METHOD'] == 'GET')
{
    parse_str($_SERVER['QUERY_STRING'], $array);
    echo "<h3>The list of environment variables with values are :</h3>";
    foreach ($_SERVER as $key=>$value)
    {
        if (is_array($value))
            foreach ($value as $_key=>$_value)
                echo "$key [$_key] => $_value <br />";
        else
            echo "$key => $value <br />";
        
    }
}
else
{
    while( $line = fgets(STDIN) ) {
        parse_str($line, $array);
    }   
echo "<!DOCTYPE html>
<html>
<head>
    <title>Button</title>
</head>
<body>
    <div style = \"text-align : center ; \">
    <h1>Push the button</h1>
        <form method='GET' action=\"/cgi-bin/game.py\" method=\"get\">
            <br><br>
            <p><input type=\"submit\" value=\" Start game \"></p>
        </form>
    </div>
</body>
</html>";
}

?>