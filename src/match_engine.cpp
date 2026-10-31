#include "match_engine.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void MatchEngine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("start_match"), &MatchEngine::start_match);
    ClassDB::bind_method(D_METHOD("resolve_manual_scenario", "success"), &MatchEngine::resolve_manual_scenario);
    ClassDB::bind_method(D_METHOD("get_player_positions"), &MatchEngine::get_player_positions);
    ClassDB::bind_method(D_METHOD("get_ball_position"), &MatchEngine::get_ball_position);

    ADD_SIGNAL(MethodInfo("interactive_event_triggered", PropertyInfo(Variant::INT, "scenario_type")));
    ADD_SIGNAL(MethodInfo("goal_scored", PropertyInfo(Variant::INT, "home_score"), PropertyInfo(Variant::INT, "away_score")));
}

MatchEngine::MatchEngine() {}
MatchEngine::~MatchEngine() {}

void MatchEngine::_ready() {
    for (int i = 0; i < 22; i++) {
        player_positions.append(Vector2(100.0f + (i * 30), 200.0f));
    }
}

void MatchEngine::_physics_process(double delta) {
    if (is_paused_for_manual) return;

    match_timer += (float)delta;
    current_minute = (int)(match_timer / 1.0f); // 1 sec = 1 minute

    if (current_minute >= 90) {
        set_physics_process(false);
        return;
    }

    if (rand() % 2000 < 2) {
        is_paused_for_manual = true;
        emit_signal("interactive_event_triggered", 1);
    }
}

void MatchEngine::start_match() {
    match_timer = 0.0f;
    home_score = 0;
    away_score = 0;
    is_paused_for_manual = false;
    set_physics_process(true);
}

void MatchEngine::resolve_manual_scenario(bool p_success) {
    if (p_success) {
        home_score++;
        emit_signal("goal_scored", home_score, away_score);
    }
    is_paused_for_manual = false;
}

Array MatchEngine::get_player_positions() const { return player_positions; }
Vector2 MatchEngine::get_ball_position() const { return ball_position; }