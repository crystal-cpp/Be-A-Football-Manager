#pragma once

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class MatchEngine : public Node2D {
    GDCLASS(MatchEngine, Node2D)

public:
    MatchEngine();
    ~MatchEngine();

    void _ready() override;
    void _physics_process(double delta) override;

    void start_match();
    void pause_match(bool p_pause);
    void resolve_manual_scenario(bool p_success, bool p_is_home_team = true);

    int get_home_score() const { return home_score; }
    int get_away_score() const { return away_score; }
    int get_current_minute() const { return current_minute; }
    bool is_match_paused() const { return is_paused_for_manual; }
    bool is_match_active() const { return match_active; }

    Array get_player_positions() const;
    Vector2 get_ball_position() const;

protected:
    static void _bind_methods();

private:
    void _reset_field_positions();

    float match_timer = 0.0f;
    int current_minute = 0;
    int home_score = 0;
    int away_score = 0;
    
    bool is_paused_for_manual = false;
    bool match_active = false;
    float event_cooldown = 0.0f;

    Vector2 ball_position = Vector2(525.0f, 340.0f);
    Array player_positions;
};

} // namespace godot