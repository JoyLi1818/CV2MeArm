const char HTML[] = R"=====(
<!--    R""表示不转义，这个写法表示显示原始字符串，会原样输出括号里的字符串   -->
<html><head>
  <meta http-equiv=Content-Type content="text/html; charset=utf-8">
  <title>417室内环境监控</title>
  <meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0'/>
  <style type="text/css">
  .button {
    background-color: #4CAF50; /* Green */
    border: none;
    color: white;
    padding: 15px 32px;
    margin-left: 20px;
    margin-right: 20px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
  }
  </style>
  <body style="background-color: #FFCCCC ">
  <center>
  <div>
  <br><br><br>
  <h1>基于WiFi的MeArm机械臂</h1><BR><BR>
    <button id=top_arm class="button" onclick="top_arm()">Top</button>
    <button id=bottom_arm class="button" onclick="bottom_arm()">Bottom</button><BR><BR>
    <button id=left_arm class="button" onclick="left_arm()">Left</button>
    <button id=right_arm class="button" onclick="right_arm()">Right</button><BR><BR>
  <!--    <form action="/ON" method="POST"><input type="submit" value="开灯"></form>-->
  <!--    <form action="/OFF" method="POST"><input type="submit" value="关灯"></form>-->
  </div>
   <br>
  <div><h2>
    温度: <span id="temp_val">0</span>
    <span id="Tdanwei"> ℃</span><br><br>
   
     湿度: <span id="humi_val">0</span>
    <span id="Hdanwei"> %RH</span><br><br>
   
    LED 状态: <span id="state">测试中</span>
  </h2>
  </div>
  
  <script>
  <!--    开灯   -->
  function top_arm()
  {
      window.location.href="/top";
  }
  <!--    关灯   -->
  function bottom_arm()
  {
      window.location.href="/bottom";
  }
  <!--    开空调   -->
  function left_arm()
  {
      window.location.href="/left";
  }
  <!--    关空调   -->
  function right_arm()
  {
      window.location.href="/right";
  }
  </script>
  
  </center>
  </body>
</html>

)=====";