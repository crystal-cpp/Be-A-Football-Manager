#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "mainplayer.h"


struct Coach {
    std::string name;
    int coachingSkill;
    double weeklySalary;
    std::string specialty; 
};

class YouthAcademy {
public:
    YouthAcademy() = default;

    void HireCoach(const Coach& coach) {
        coaches.push_back(coach);
    }

    void AddYouthPlayer(std::shared_ptr<Player> player) {
        youthRoster.push_back(player);
    }

    void ProcessSeasonDevelopment() {
        int avgCoachSkill = GetAverageCoachSkill();
        for (auto& player : youthRoster) {
            player->AdvanceAge(); // 
            
            int statBoost = std::max(1, avgCoachSkill / 20);
            player->ApplyTrainingXP(statBoost * 100);
        }
    }

    std::vector<std::shared_ptr<Player>> ExtractGraduatedPlayers() {
        std::vector<std::shared_ptr<Player>> graduated;
        auto it = youthRoster.begin();
        while (it != youthRoster.end()) {
            if ((*it)->IsAvailableForTransfer()) {
                graduated.push_back(*it);
                it = youthRoster.erase(it);
            } else {
                ++it;
            }
        }
        return graduated;
    }

    int GetAverageCoachSkill() const {
        if (coaches.empty()) return 10;
        int total = 0;
        for (const auto& c : coaches) total += c.coachingSkill;
        return total / coaches.size();
    }

    double GetTotalWeeklySalaries() const {
        double total = 0;
        for (const auto& c : coaches) total += c.weeklySalary;
        return total;
    }

private:
    std::vector<Coach> coaches;
    std::vector<std::shared_ptr<Player>> youthRoster;
};