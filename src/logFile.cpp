#pragma once
#include "logFile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>


bool sendToLogFile(short level, const std::string& text, const std::string& context){
    std::ofstream logFile("assets/logs/logFile.txt", std::ios::app);
    if(!logFile.is_open()){
        throw logException("Log file not found");
    }
    std::time_t result = std::time(nullptr);
    std::stringstream cas(std::asctime(std::localtime(&result)));
    std::stringstream vysledny_string;
    std::string pouzitelne;
    std::string trash;
    cas >> trash;
    cas >> pouzitelne;
    vysledny_string << pouzitelne;
    cas >> pouzitelne;
    vysledny_string << pouzitelne << " ";
    cas >> pouzitelne;
    vysledny_string << pouzitelne.substr(0,5) << " ";
    cas >> pouzitelne;
    vysledny_string << pouzitelne.substr(2,2);
    //    std::cout << "Level: " << level << " Context: " << context << " Text: " << text << std::endl;
    logFile <<"Time: " << vysledny_string.str() <<  " Level: " << level << " Context: " << context << " Text: " << text << std::endl;
    return true;
}