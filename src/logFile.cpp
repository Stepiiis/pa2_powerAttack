#pragma once
#include "logFile.h"
#include <iostream>
#include <fstream>
#include <string>




bool sendToLogFile(short level, const std::string& text, const std::string context){
    std::ofstream logFile("data/logs/logFile.txt", std::ios::app);
    if(!logFile.is_open()){
        throw logException("Log file not found");
    }
    std::cout << "Level: " << level << " Context: " << context << " Text: " << text << std::endl;
    logFile << "Level: " << level << " Context: " << context << " Text: " << text << std::endl;
    return true;
}