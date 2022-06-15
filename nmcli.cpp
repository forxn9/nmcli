/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022 UOS, Inc.
 */

#include "wifi_device.h"
#include "wifi_scan.h"
#include "iostream"
#include <unistd.h>
#include <iomanip>

using namespace OHOS::Wifi;
using namespace std;

std::unique_ptr<WifiDevice> wifiDevicePtr = OHOS::Wifi::WifiDevice::GetInstance(WIFI_DEVICE_ABILITY_ID);
std::unique_ptr<WifiScan> wifiScanPtr = OHOS::Wifi::WifiScan::GetInstance(WIFI_SCAN_ABILITY_ID);

void ConvertEncryptionModeDemo(WifiSecurity securityType, std::string& keyMgmt)
{
    switch (securityType) {
        case WifiSecurity::OPEN:
        case WifiSecurity::WEP:
            keyMgmt = KEY_MGMT_NONE;
            break;

        case WifiSecurity::PSK:
            keyMgmt = KEY_MGMT_WPA_PSK;
            break;

        case WifiSecurity::SAE:
            keyMgmt = KEY_MGMT_SAE;
            break;

        default:
            keyMgmt = KEY_MGMT_NONE;
            break;
    }
}

void Scan(){
    ErrCode ret = wifiScanPtr->Scan();
    if (ret){
        cout << "scan wifi failed!" << endl;
    }else{
        cout << "scan wifi success!" << endl;
    }
    
}

//按照rssi降序排列
bool compare(const WifiScanInfo &s1, const WifiScanInfo &s2)
{
    return s1.rssi > s2.rssi;
}

void DumpWifiList(std::vector<WifiScanInfo> vec){
    if(vec.empty()){
        cout << "Scan info list is null" << endl;
        return;
    }
    std::sort(vec.begin(),vec.end(),compare);
    for (auto iter = vec.begin(); iter != vec.end(); iter++) {
        std::string keyMgmt;
        ConvertEncryptionModeDemo((*iter).securityType,keyMgmt);
        std::cout << "BSSID:" << (*iter).bssid << " RSSI:" << (*iter).rssi << " [" << setw(7) <<setiosflags(ios::left)<<setfill(' ')
            << keyMgmt << "] SSID:" << (*iter).ssid << endl;
    }
}

void GetScanInfos(){
    std::vector<WifiScanInfo> vecScanInfos;
    ErrCode ret = wifiScanPtr->GetScanInfoList(vecScanInfos);
    if (!ret){
        DumpWifiList(vecScanInfos);
    }else{
        cout << "GetScanInfos Failed!" << endl;
    }
    
}

void GetLinkedInfo(){
    WifiLinkedInfo linkedInfo;
    ErrCode ret = wifiDevicePtr->GetLinkedInfo(linkedInfo);
    if(ret){
        cout << "GetLinkedInfo Failed!" << endl;
        return;
    }
    std::cout << "SSID:" << linkedInfo.ssid << " ConnState:" << linkedInfo.connState
        << " SupplicantState:"<< int(linkedInfo.supplicantState) << " DetailedState: " << int(linkedInfo.detailedState) << endl;
}

void ConnectToNetwork(long int networkId){
    ErrCode ret = wifiDevicePtr->ConnectToNetwork(networkId);
    if(!ret){
        cout << "ConnectToNetwork success!" << endl;
        sleep(2);
        GetLinkedInfo();
    }else{
        cout << "ConnectToNetwork failed!" << endl;
    }
}

void ConnectToDevice (char* Ssid, char* KeyMgmt, char* PreSharedkey){
    WifiDeviceConfig config;
    std::string ssid(Ssid);
    std::string keyMgmt(KeyMgmt);
    config.ssid = ssid;
    config.keyMgmt = keyMgmt;
    if (PreSharedkey != nullptr)
    {
        std::string preSharedKey(PreSharedkey);
        config.preSharedKey = preSharedKey;
    }
    
    ErrCode ret = wifiDevicePtr->ConnectToDevice(config);
    if (ret != WIFI_OPT_SUCCESS) {
        cout << "ConnectToDevice failed!" << endl;
    }else{
        cout << "ConnectToDevice success!" << endl;
    }
}

void PrintHelp(){
    cout << "OpenHarmony wifi command line tool" << endl << endl;
    cout << "nmcli scan                 扫描一次" << endl;
    cout << "nmcli list                 显示wifi列表 " << endl;
    cout << "nmcli link                 显示连接状态属性" << endl;
    cout << "nmcli connectid ID         连接某已连接wifi" << endl;
    cout << "nmcli connect SSID KeyMgmt 连接wifi" << endl << endl;
    cout << "example: nmcli connect asus NONE" << endl;
    cout << "         nmcli connect openharmony WPA-PSK aaaaaaaa" << endl;
}

int main(int argc, char *argv[]){

    if(strcmp("scan",argv[1]) == 0){
        Scan();
    }else if(strcmp("list",argv[1]) == 0){
        GetScanInfos();
    }else if(strcmp("link", argv[1]) == 0){
        GetLinkedInfo();
    }else if (strcmp("connectid", argv[1]) == 0){
        long int arg = strtol(argv[2], NULL, 10);
        ConnectToNetwork(arg);
    }else if(strcmp("connect", argv[1]) == 0){
        ConnectToDevice(argv[2],argv[3],argv[4]);
    }else{
        PrintHelp();
    }
    
    return 0;
}
