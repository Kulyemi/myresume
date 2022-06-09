#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
 
ESP8266WebServer esp8266_server(80);// 建立ESP8266WebServer对象，对象名称为esp8266_server,括号中的数字是网路服务器响应http请求的端口号,网络服务器标准http端口号为80，因此这里使用80为端口号

String open_page = "<!DOCTYPE html><html lang=\"ch\"><head>    <meta charset=\"UTF-8\">    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">    <title>light the light</title>    <style>        .box1 {            height: 1000px;            position: relative;            background-color: #ffffffff;            border: 1px solid rgba(136, 174, 208, 0.467);        }        .box2 {            height: 200px;            width: 200px;            top: 50%;            left: 50%;            margin: -150px 0 0 -150px;            position: absolute;            line-height: 100px;            justify-content: center;            align-items: Center;        }        .a {            text-decoration: none;        }        .button1 {            width: 250px;            height: 250px;            font-size: 20px;            border-radius: 50%;            color: #023407f3;            border: 9px solid rgba(75, 75, 75, 0.881);            background: rgba(219, 219, 219, 0.921);        }        .space {            width: 100px;        }    </style></head><body>    <div class=\" box1\">        <div class=\"box2\">            <a href='./LED?light_button=0' class='a'>                <button type='button1' class='button1'>关灯</button>            </a>        </div>    </div></body></html>";

String close_page = "<!DOCTYPE html><html lang=\"ch\"><head>    <meta charset=\"UTF-8\">    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">    <title>light the light</title>    <style>        .box1 {            height: 1000px;            position: relative;            background-color: #ffffffff;            border: 1px solid rgba(93, 112, 129, 0.467);        }        .box2 {            height: 200px;            width: 200px;            top: 50%;            left: 50%;            margin: -150px 0 0 -150px;            position: absolute;            line-height: 100px;            justify-content: center;            align-items: Center;        }        .a {            text-decoration: none;        }        .button1 {            width: 250px;            height: 250px;            font-size: 20px;            border-radius: 50%;            color: #fffffff3;            border: 9px solid rgba(0, 0, 0, 0.881);            background: rgba(17, 17, 17, 0.921);        }        .space {            width: 100px;        }    </style></head><body>    <div class=\" box1\">        <div class=\"box2\">            <a href='./LED?light_button=1' class='a'>                <button type='button1' class='button1'>开灯</button>            </a>        </div>    </div></body></html>";


void setup(void){
  Serial.begin(9600);          // 启动串口通讯

  pinMode(D5, OUTPUT); // D1
  digitalWrite(D5, HIGH);
  //通过addAp函数存储  WiFi名称   WiFi密码
  wifiMulti.addAP("D_lulu", "12345678");  
   
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print('.');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
 
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
  
//--------"启动网络服务功能"程序部分开始-------- 
  esp8266_server.begin();                   
  esp8266_server.on("/", handleRoot);  
  esp8266_server.on("/LED", HTTP_GET, handleLED);  
  esp8266_server.onNotFound(handleNotFound);   

//--------"启动网络服务功能"程序部分结束--------
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
}

void loop(void){
  esp8266_server.handleClient();     // 处理http服务器访问
}
                                                                           
void handleRoot() {   //处理网站根目录“/”的访问请求 
  esp8266_server.send(200, "text/html", open_page);   // NodeMCU将调用此函数。
}

void handleLED() {
    if (esp8266_server.arg("light_button") == "1"){
      digitalWrite(D5, HIGH); // D5
      esp8266_server.send(200, "text/html", open_page);
    } // 切换前端
    else if (esp8266_server.arg("light_button") == "0"){
      digitalWrite(D5, LOW); // D5
      esp8266_server.send(200, "text/html", close_page);
    } 
}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
