# openharmony-wifi-demo
OpenHarmony wifi command line tool

nmcli scan                &emsp; &emsp;  扫描一次 <br>
nmcli list                 &emsp; &emsp;&emsp;  显示wifi列表 <br>
nmcli link                 &emsp; &emsp;&emsp;  显示连接状态属性 <br>
nmcli connectid ID         &emsp;&emsp;  连接某已连接wifi <br>
nmcli connect SSID KeyMgmt &emsp;   连接wifi <br>

example: nmcli connect asus NONE
         nmcli connect openharmony WPA-PSK aaaaaaaa
