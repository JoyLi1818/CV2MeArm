/**********************************************************************
项目名称/Project          : MeArm机械臂控制程序
程序名称/Program name     : 
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : 
日期/Date（YYYYMMDD）     : 
程序目的/Purpose          : http控制MeArm机械臂
-----------------------------------------------------------------------
如需要获得具体电路连接细节，请查阅太极创客制作的
《零基础入门学用Arduino教程 - MeArm篇》页面。
 nodemcu引脚对应数字：      * 16=D0;  *  4=D2; *  0=D3; *  2=D4; * 14=D5; * 12=D6; * 13=D7; * 15=D8; *  3=D9; *  1=D10;
***********************************************************************/
#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
#include "webhtml.h"
#include <Servo.h>

IPAddress local_IP(192, 168, 31, 123);//想要设定的静态ip
IPAddress gateway(192, 168, 31, 1);//当前网关
IPAddress subnet(255, 255, 255, 0);//当前子网掩码
 
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

Servo base, RArm, LArm, claw ;    //创建4个servo对象

// 建立4个int型变量存储当前电机角度值
// 初始角度值为设备启动后初始状态所需要的电机角度数值
int basePos = 90;
int LArmPos = 90;
int RArmPos = 90;
int clawPos = 90;

// 设置舵机转速
int speed =10;

//存储电机极限值(const指定该数值为常量,常量数值在程序运行中不能改变)
const int baseMin = 0;
const int baseMax = 180;
const int LArmMin = 45;
const int LArmMax = 180;
const int RArmMin = 35;
const int RArmMax = 120;
const int clawMin = 25;
const int clawMax = 100;
 
void setup(void){
  base.attach(4);     // base 伺服舵机连接引脚11 舵机代号'b'
  delay(200);          // 稳定性等待
  LArm.attach(0);     // LArm 伺服舵机连接引脚10 舵机代号'r'
  delay(200);          // 稳定性等待
  RArm.attach(2);      // RArm 伺服舵机连接引脚9  舵机代号'f'
  delay(200);          // 稳定性等待
  claw.attach(14);      // claw 伺服舵机连接引脚6  舵机代号'c'
  delay(200);          // 稳定性等待

  base.write(basePos); 
  delay(10);
  RArm.write(RArmPos); 
  delay(10);
  LArm.write(LArmPos); 
  delay(10);
  claw.write(clawPos);  
  delay(10);   

  Serial.begin(9600);   // 启动串口通讯
  
  wifiMulti.addAP("417", "chengda417417"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  WiFi.config(local_IP, gateway, subnet);//设置静态IP
  WiFi.mode(WIFI_STA);//设置为接收WiFi模式

  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP
 
  esp8266_server.begin();                           // 启动网站服务
  esp8266_server.on("/", HTTP_GET, handleRoot);     // 设置服务器根目录即'/'的函数'handleRoot'
  // esp8266_server.on("/top", HTTP_GET, top_arm);  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.on("/bottom", HTTP_GET, bottom_arm);  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.on("/left", HTTP_GET, left_arm);  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.on("/right", HTTP_GET, right_arm);  // 设置处理LED控制请求的函数'handleLED'

  esp8266_server.onNotFound(handleNotFound);        // 设置处理404情况的函数'handleNotFound'
 
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
}
 
void loop(void){
  esp8266_server.handleClient();                     // 检查http服务器访问
}
 
/*设置服务器根目录即'/'的函数'handleRoot'
  该函数的作用是每当有客户端访问NodeMCU服务器根目录时，
  NodeMCU都会向访问设备发送 HTTP 状态 200 (Ok) 这是send函数的第一个参数。
  同时NodeMCU还会向浏览器发送HTML代码，以下示例中send函数中第三个参数，
  也就是双引号中的内容就是NodeMCU发送的HTML代码。该代码可在网页中产生LED控制按钮。 
  当用户按下按钮时，浏览器将会向NodeMCU的/LED页面发送HTTP请求，请求方式为POST。
  NodeMCU接收到此请求后将会执行handleLED函数内容*/
void handleRoot() {       
  esp8266_server.send(200, "text/html", String(HTML));
}
 
// bottom
void bottom_arm() {                         

  for (int i=90; i>=1; i--){
    base.write(i);
    delay (speed);
  }

  for (int i=0; i<=179; i++){
    base.write(i);
    delay (speed);
  }

  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}

// left
void left_arm() {                         

  for (int i=90; i>=1; i--){
    LArm.write(i);
    delay (speed);
  }

  for (int i=0; i<=179; i++){
    LArm.write(i);
    delay (speed);
  }

  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}

// right
void right_arm() {                         

  for (int i=90; i>=1; i--){
    RArm.write(i);
    delay (speed);
  }

  for (int i=0; i<=179; i++){
    RArm.write(i);
    delay (speed);
  }

  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}

 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){
  esp8266_server.send(404, "text/plain", "你干嘛？哎哟！"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}