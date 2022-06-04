#include "scene.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	//skybox initialization (path contains 6 images (cube), with a certain order)
	skybox.initialize("assets/skybox_4/"); 
	
	
	// Basic set-up
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 0.0f,2.0f,2.0f }, { 0,0,0 });

	piston1.initialize();
	mont.initialize();
	//flocon1.initialize();

	//timer.event_period = 0.1f;
	buffer<vec3> key_positions =
	{ {-0.15f, 0.9f,0}, {-0.12f, 0.5f,0}, {-0.3f, 0.55f,0.0f}, {-0.29f, 0.45f,0.0f}, {-0.27f, 0.35f,0.0f}, {-0.20f, 0.25f,0.0f}, {-0.17f, 0.18f,0.0f}, {-0.16f, 0.07f,0.0f}, {-0.18f, -0.03f,0.0f}, {-0.24f, -0.11f,0.0f}, {-0.39f, -0.22f,0.0f}, {-0.43, -0.27,0.0f}, {-0.44, -0.37,0.0f}, {-0.50, -0.47,0.0f}, {-0.53, -0.48,0.0f}, {-0.59, -0.49,0.0f}, {-0.64, -0.54,0.0f}, {-0.67, -0.65,0.0f} };

	buffer<float> key_times = 
	{ 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0F, 14.0f, 15.0f, 16.0f, 17.0f };

	// chemin1.initialize(key_positions,key_times);

	int N = key_times.size();
	timer.t_min = key_times[1];
	timer.t_max = key_times[N - 2];
	timer.t = timer.t_min;

	float hauteur = 0.01f;

	chemin1.createAllRail(0.01f, 0.001f, mont.terrain_mesh,key_positions,key_times, timer.t_min, timer.t_max, hauteur);

}






void scene_structure::display()
{
	// skybox first => derriere les autres elements de la scene.
	draw(skybox, environment); 
	draw(chemin1.hierarchy_rail, environment); 


	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame){
		draw(global_frame, environment);
	}


	draw(mont.terrain_drawable, environment);
	if (gui.display_wireframe){
		draw_wireframe(mont.terrain_drawable, environment);
	}

	// pts de controle
	for (int i=0; i<18; i++){
		mont.sphere2.transform.translation = vec3{mont.pts_controle[i][0], mont.pts_controle[i][1], evaluate_terrain_height(mont.pts_controle[i][0], mont.pts_controle[i][1], mont.terrain_mesh)+0.01f};
		draw(mont.sphere2, environment);
	}
	int i =2;
	// piston1.hierarchy["train"].transform.translation =vec3{mont.pts_controle[i][0], mont.pts_controle[i][1], mont.evaluate_terrain_height(mont.pts_controle[i][0], mont.pts_controle[i][1], mont.terrain_mesh)+0.01f};
	timer2.update();
	timer.update();

	float t = timer.t;

	piston1.tournerRoue(t);

	//flocon1.update();
	//flocon1.display_semiTransparent(environment);

	vec3 p = interpolation(t, chemin1.key_positions, chemin1.key_times, mont.terrain_mesh);

	// Display the interpolated position (and its trajectory)
	piston1.update(p, environment, hauteur);
	draw(piston1.hierarchy, environment);


}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

/*

	// valeurs par defaut :	
	parameters.persistency = 0.574f;
	parameters.frequency_gain = 1.515f;
	parameters.octave = 15;
	parameters.terrain_height = 0.504f;
	bool update = false;

	update |= ImGui::SliderFloat("Persistance", &parameters.persistency, 0.1f, 0.6f);
	update |= ImGui::SliderFloat("Frequency gain", &parameters.frequency_gain, 1.5f, 2.5f);
	update |= ImGui::SliderInt("Octave", &parameters.octave, 1, 15);
	update |= ImGui::SliderFloat("Height", &parameters.terrain_height, 0.1f, 1.5f);

	if (update)// if any slider has been changed - then update the terrain
		update_terrain(terrain_mesh, terrain_drawable, parameters);

*/
}


