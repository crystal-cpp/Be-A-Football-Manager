extends Node3D

signal highlight_finished(success: bool, is_home_team: bool)

@onready var camera_3d: Camera3D = $Camera3D
@onready var ball_3d: Node3D = $Ball3D
@onready var players_container: Node3D = $Players3D

var current_is_home_team: bool = true
var current_scenario: int = 1

func setup_highlight(scenario_type: int, is_home_team: bool, ball_pos: Vector3) -> void:
	current_scenario = scenario_type
	current_is_home_team = is_home_team

	# Activate 3D Viewport Camera
	if camera_3d:
		camera_3d.make_current()

	# Set 3D Ball Location from C++ Signal
	if ball_3d:
		ball_3d.global_position = ball_pos

	# Orient Camera behind the ball looking toward the goal
	_setup_camera_and_spawns(ball_pos, is_home_team)

	# Temporary auto-resolution placeholder (Replace with manual 3D touch/joystick controls)
	get_tree().create_timer(3.0).timeout.connect(_simulate_highlight_result)

func _setup_camera_and_spawns(ball_pos: Vector3, is_home: bool) -> void:
	if not camera_3d:
		return

	# Position camera behind ball relative to attacking direction
	var attack_dir: float = 1.0 if is_home else -1.0
	var camera_offset: Vector3 = Vector3(0.0, 4.5, -8.0 * attack_dir)
	
	camera_3d.global_position = ball_pos + camera_offset
	camera_3d.look_at(ball_pos + Vector3(0.0, 0.5, 10.0 * attack_dir), Vector3.UP)

	# Clear previous 3D scenario nodes
	if players_container:
		for child in players_container.get_children():
			child.queue_free()

	# TODO: Spawn attacker, defender, and goalkeeper 3D capsule meshes in players_container

func _simulate_highlight_result() -> void:
	# Temporary resolution: 70% chance of success for testing loop
	var success: bool = randf() < 0.7
	emit_signal("highlight_finished", success, current_is_home_team)