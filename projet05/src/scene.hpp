#pragma once

#include "cgp/cgp.hpp"
#include "system_piston.hpp"
#include "terrain2.hpp"
#include "flocon.hpp"
#include "interpolation.hpp"
#include "igloo.hpp"




// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame      = false;
	bool display_wireframe  = false;
};

struct keyboard_state_parameters{
	bool left  = false; 
	bool right = false; 
	bool up    = false; 
	bool down  = false;
};

struct user_interaction_parameters {
    cgp::vec2 mouse_prev;
	cgp::timer_fps fps_record;
	cgp::mesh_drawable global_frame;
	gui_parameters gui;
    keyboard_state_parameters keyboard_state;
	bool cursor_on_gui;
};



// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	perlin_noise_parameters parameters;
	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                       // Standard GUI element storage
	
	//Train
	piston piston1;
	cgp::timer_basic timer2;
	cgp::mesh_drawable test;     
	cgp::mesh_drawable train;         
    
	chemin chemin1;
	cgp::mesh rail_mesh;
	cgp::mesh_drawable rail;          // The standard global frame
	cgp::timer_interval timer;

	cgp::buffer<float> mouvement;
	float vitesseMax;

	// skybox
	cgp::skybox_drawable skybox;

	//Montagne
	montagne mont;
	
	cgp::mesh_drawable trouPoisson;          // The standard global frame


	//igloo
	std::list<igloo> vecIgloo;
	// Flocon
	flocon flocon1;

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

	// Display function for semi-transparent shapes
	//void display_semiTransparent();

};




