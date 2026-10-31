#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <memory>
#include "mainplayer.h"

class PlayerDataBase {
public:
    bool LoadFromCSV(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open player database: " << filepath << std::endl;
            return false;
        }

        std::string line;
        bool isHeader = true;

        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (line.empty()) continue;

            if (isHeader) {
                isHeader = false;
                continue; 
            }

            std::stringstream ss(line);
            std::string name, posStr;
            std::string pacStr, shoStr, pasStr, driStr, defStr, phyStr, ageStr, potStr;

            std::getline(ss, name, ',');
            std::getline(ss, posStr, ',');
            std::getline(ss, pacStr, ',');
            std::getline(ss, shoStr, ',');
            std::getline(ss, pasStr, ',');
            std::getline(ss, driStr, ',');
            std::getline(ss, defStr, ',');
            std::getline(ss, phyStr, ',');
            
            // Handle optional age & potential fields if missing in older CSV files
            if (!std::getline(ss, ageStr, ',')) ageStr = "21";
            if (!std::getline(ss, potStr, ',')) potStr = "85";

            // Convert parsed string values to integers safely
            int pace = std::stoi(pacStr);
            int shooting = std::stoi(shoStr);
            int passing = std::stoi(pasStr);
            int dribbling = std::stoi(driStr);
            int defending = std::stoi(defStr);
            int physical = std::stoi(phyStr);
            int age = std::stoi(ageStr);
            int potential = std::stoi(potStr);

            Position pos = StringToPosition(posStr);
            PlayerAttributes attrs{ pace, shooting, passing, dribbling, defending, physical };
            
            auto player = std::make_shared<Player>(name, pos, attrs, age, potential);
            players[name] = player;
        }

        file.close();
        return true;
    }

    std::shared_ptr<Player> GetPlayer(const std::string& name) {
        if (players.find(name) != players.end()) {
            return players[name];
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Player>> players;

    Position StringToPosition(const std::string& posStr) {
        if (posStr == "GK") return Position::GK;
        if (posStr == "CB") return Position::CB;
        if (posStr == "LB") return Position::LB;
        if (posStr == "RB") return Position::RB;
        if (posStr == "CM") return Position::CM;
        if (posStr == "CDM") return Position::CDM;
        if (posStr == "CAM") return Position::CAM;
        if (posStr == "LW") return Position::LW;
        if (posStr == "RW") return Position::RW;
        return Position::ST; // Default fallback
    }
};