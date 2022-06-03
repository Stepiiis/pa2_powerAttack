
#include "Definitions.h"
#include "logFile.h"

bool CDefinitions::loadDefinitions(){
    std::string path ="attackers/attackerDefinition.txt";
    if(loadEntity(path,_attackerDefinitions)) {
        path = "towers/towerDefinition.txt";
        if (loadEntity(path, _towerDefinitions))
            return true;
    }else
        return false;
    return true;
}

bool CDefinitions::loadEntity(std::string & path, defEntity & ent){
    ent.clear();
    std::ifstream fileDef("assets/definitions/"+path); // attackers/attackerDefinition.txt
    if(!fileDef.is_open()) {
        sendToLogFile(10, "Could not open file " + path , "LoadDefinitions");
        return false;
    }
    std::string line;
    std::string entName;
    while(getline(fileDef, line)) {
        if(line.empty()) continue;
        if(line[0] == '#') {
            entName= line.substr(1);
            ent.insert(std::pair<std::string,std::map<std::string, int> >(entName, std::map<std::string, int>()));
        }
        else{
            std::stringstream ss(line);
            std::string key;
            std::string value;
            std::string test;
            int final;
            ss >> key >> value >> test;
            if(!test.empty()){
                sendToLogFile(10, "Error in file " + path + " at line " + line, "LoadDefinitions");
                throw(syntaxErr( "Error in file " + path + " at line " + line));
            }
            try{final = std::stoi(value);}
            catch(std::invalid_argument){
                final = *(value.substr(0,1).c_str());
            }

            ent[entName].insert(std::pair<std::string, int>(key, final));
        }
    }
    sendToLogFile(0, "Loaded file " + path, "LoadDefinitions");
//    std::cout << "Loaded " << ent.size() << " attacker definitions" << std::endl;
//    std::cout << "For each attacker, there are " << ent.begin()->second.size() << " attributes" << std::endl;
//    for(const auto & type: ent){
//        std::cout << "Attacker type: " << type.first << std::endl;
//        for(const auto & attr: type.second){
//            std::cout << "Attribute: " << attr.first << " value: " << attr.second << std::endl;
//        }
//    }
    return true;
}


const defEntity & CDefinitions::getTower(){
    return _towerDefinitions;
}
const defEntity & CDefinitions::getAttacker(){
    return _attackerDefinitions;
}

// converts from map to structure which can be used inside
//bool CDefinitions::convertDefinitions() {
//    return false;
//}
