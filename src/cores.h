#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "mainplayer.h"

class PlayerDataBase {
public:
    bool LoadFromCSV(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Cannot open file (database): " << filepath << std::endl;
            return false;
        }

        std::string line;
        bool isHeader = true;

        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;

            if (isHeader) {
                isHeader = false;
                continue;
            }

            std::stringstream ss(line);
            std::string name, posStr;
            std::string pacStr, shoStr, pasStr, driStr, defStr, phyStr, ageStr, potStr, debutStr;

            std::getline(ss, name, ',');
            std::getline(ss, posStr, ',');
            std::getline(ss, pacStr, ',');
            std::getline(ss, shoStr, ',');
            std::getline(ss, pasStr, ',');
            std::getline(ss, driStr, ',');
            std::getline(ss, defStr, ',');
            std::getline(ss, phyStr, ',');
            
            if (!std::getline(ss, ageStr, ',')) ageStr = "8";
            if (!std::getline(ss, potStr, ',')) potStr = "88";
            if (!std::getline(ss, debutStr, ',')) debutStr = "14"; // Default DebutAge = 14

            Position pos = StringToPosition(posStr);
            PlayerAttributes attrs{
                std::stoi(pacStr), std::stoi(shoStr), std::stoi(pasStr),
                std::stoi(driStr), std::stoi(defStr), std::stoi(phyStr)
            };

            auto player = std::make_shared<Player>(
                name, pos, attrs, 
                std::stoi(ageStr), 
                std::stoi(potStr), 
                std::stoi(debutStr)
            );

            players[name] = player;
            allPlayers.push_back(player);
        }

        file.close();
        return true;
    }

    std::vector<std::shared_ptr<Player>> GetAcademyPlayers() const {
        std::vector<std::shared_ptr<Player>> academyList;
        for (const auto& player : allPlayers) {
            if (!player->IsAvailableForTransfer()) {
                academyList.push_back(player);
            }
        }
        return academyList;
    }

    std::vector<std::shared_ptr<Player>> GetMarketAvailablePlayers() const {
        std::vector<std::shared_ptr<Player>> marketList;
        for (const auto& player : allPlayers) {
            if (player->IsAvailableForTransfer()) {
                marketList.push_back(player);
            }
        }
        return marketList;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Player>> allPlayers;

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
        return Position::ST;
    }
};