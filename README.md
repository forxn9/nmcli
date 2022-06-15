# openharmony-wifi-demo
OpenHarmony wifi command line tool

nmcli scan                 扫描一次
nmcli list                 显示wifi列表
nmcli link                 显示连接状态属性
nmcli connectid ID         连接某已连接wifi
nmcli connect SSID KeyMgmt 连接wifi

example: nmcli connect asus NONE
         nmcli connect openharmony WPA-PSK aaaaaaaa
