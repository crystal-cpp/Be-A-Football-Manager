#include "match_engine.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void MatchEngine::_bind_methods() {
    // Methods
    ClassDB::bind_method(D_METHOD("start_match"), &MatchEngine::start_match);
    ClassDB::bind_method(D_METHOD("pause_match", "pause"), &MatchEngine::pause_match);
    ClassDB::bind_method(D_METHOD("resolve_manual_scenario", "success", "is_home_team"), &MatchEngine::resolve_manual_scenario, DEFVAL(true));
    
    // Getters
    ClassDB::bind_method(D_METHOD("get_home_score"), &MatchEngine::get_home_score);
    ClassDB::bind_method(D_METHOD("get_away_score"), &MatchEngine::get_away_score);
    ClassDB::bind_method(D_METHOD("get_current_minute"), &MatchEngine::get_current_minute);
    ClassDB::bind_method(D_METHOD("is_match_paused"), &MatchEngine::is_match_paused);
    ClassDB::bind_method(D_METHOD("is_match_active"), &MatchEngine::is_match_active);
    ClassDB::bind_method(D_METHOD("get_player_positions"), &MatchEngine::get_player_positions);
    ClassDB::bind_method(D_METHOD("get_ball_position"), &MatchEngine::get_ball_position);

    // Signals
    ADD_SIGNAL(MethodInfo("interactive_event_triggered", PropertyInfo(Variant::INT, "scenario_type"), PropertyInfo(Variant::BOOL, "is_home_team")));
    ADD_SIGNAL(MethodInfo("goal_scored", PropertyInfo(Variant::INT, "home_score"), PropertyInfo(Variant::INT, "away_score"), PropertyInfo(Variant::BOOL, "is_home_team")));
    ADD_SIGNAL(MethodInfo("minute_changed", PropertyInfo(Variant::INT, "minute")));
    ADD_SIGNAL(MethodInfo("match_ended", PropertyInfo(Variant::INT, "final_home_score"), PropertyInfo(Variant::INT, "final_away_score")));
}

MatchEngine::MatchEngine() {}
MatchEngine::~MatchEngine() {}

void MatchEngine::_ready() {
    set_physics_process(false);
    _reset_field_positions();
}

void MatchEngine::_physics_process(double delta) {
    if (!match_active || is_paused_for_manual) {
        return;
    }

    match_timer += static_cast<float>(delta);
    int new_minute = static_cast<int>(match_timer / 1.0f); // 1s real time = 1 match minute

    if (new_minute != current_minute) {
        current_minute = new_minute;
        emit_signal("minute_changed", current_minute);
    }

    if (current_minute >= 90) {
        match_active = false;
        set_physics_process(false);
        emit_signal("match_ended", home_score, away_score);
        return;
    }

    float dt = static_cast<float>(delta);

    // 1. Simulate active 2D player drift
    for (int i = 0; i < player_positions.size(); i++) {
        Vector2 pos = player_positions[i];
        float jitterX = (UtilityFunctions::randf() - 0.5f) * 20.0f * dt;
        float jitterY = (UtilityFunctions::randf() - 0.5f) * 20.0f * dt;
        player_positions[i] = pos + Vector2(jitterX, jitterY);
    }

    // 2. Simulate ball movement
    ball_position += Vector2((UtilityFunctions::randf() - 0.5f) * 15.0f * dt, (UtilityFunctions::randf() - 0.5f) * 10.0f * dt);

    // 3. Scenario trigger logic with cooldown
    if (event_cooldown > 0.0f) {
        event_cooldown -= dt;
    } else {
        if (UtilityFunctions::randf() < 0.0015f) {
            is_paused_for_manual = true;
            event_cooldown = 10.0f;
            
            bool is_home_event = UtilityFunctions::randf() > 0.5f;
            int scenario_type = static_cast<int>(UtilityFunctions::randi_range(1, 3));
            
            emit_signal("interactive_event_triggered", scenario_type, is_home_event);
        }
    }
}

void MatchEngine::start_match() {
    match_timer = 0.0f;
    current_minute = 0;
    home_score = 0;
    away_score = 0;
    is_paused_for_manual = false;
    match_active = true;
    event_cooldown = 5.0f;

    _reset_field_positions();
    set_physics_process(true);
}

void MatchEngine::pause_match(bool p_pause) {
    is_paused_for_manual = p_pause;
}

void MatchEngine::resolve_manual_scenario(bool p_success, bool p_is_home_team) {
    if (p_success) {
        if (p_is_home_team) {
            home_score++;
        } else {
            away_score++;
        }
        emit_signal("goal_scored", home_score, away_score, p_is_home_team);
    }
    
    is_paused_for_manual = false;
}

void MatchEngine::_reset_field_positions() {
    player_positions.clear();
    
    // 11 Home Players (Left)
    for (int i = 0; i < 11; i++) {
        player_positions.append(Vector2(200.0f + (i * 25.0f), 300.0f));
    }
    // 11 Away Players (Right)
    for (int i = 0; i < 11; i++) {
        player_positions.append(Vector2(600.0f + (i * 25.0f), 300.0f));
    }
    
    ball_position = Vector2(525.0f, 340.0f);
}

Array MatchEngine::get_player_positions() const { return player_positions; }
Vector2 MatchEngine::get_ball_position() const { return ball_position; }