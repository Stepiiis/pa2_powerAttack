
#include "Definitions.h"
#include "logFile.h"

bool CDefinitions::loadDefinitions(){
    std::ifstream fileAttacker("data/attackers/attackerDefinitions.txt");
    if(!fileAttacker.is_open()) {
        sendToLogFile(10, "Could not open file attackerDefinitions.txt", "LoadDefinitions");
        return false;
    }
    std::string line;
    while(getline(fileAttacker, line)) {
        if(line[0] == '#') {
            continue;
        }
        //TODO: figure out how to automate this part
    }

}

bool CDefinitions::loadEntity(std::string & line, defBaseEntity & ent){
    int index{};
    int length = line.length();
    while (index != length-1) {
        if(line[index] == '=')
            break;
        index++;
    }
    ent.symbol = line[++index];
    while (index < length-1) {
        if(line[index] == '=')
            break;
        index++;
    }
    int numindex{};
    while(index<length-1 && line[numindex] != ';'){
        numindex++;
    }
    ent.maxHP = std::stoi(line.substr(index, numindex - index));
    // TODO: add other stats
}
const towerDefs & CDefinitions::getDefender()const{
    return _towers;
}
const attackerDefs & CDefinitions::getAttacker()const{
    return _attackers;
}