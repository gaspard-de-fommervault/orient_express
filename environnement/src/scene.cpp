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


	// Montagnes

	terrain_mesh = create_terrain_mesh();
	terrain_drawable.initialize(terrain_mesh, "terrain");
	update_terrain(terrain_mesh, terrain_drawable, parameters);
	
	// texture
	GLuint const texture_image_id = opengl_load_texture_image("assets/texture_montagne_10.jpg",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
	
	terrain_drawable.texture = texture_image_id;


	// Flocons
	
	float const sphere_radius = 0.002f;
	sphere.initialize(mesh_primitive_sphere(sphere_radius), "sphere");
	sphere.shading.color = { 1, 1, 1 };


	// positions des points de controle de la trajectoire des rails
	sphere2.initialize(mesh_primitive_sphere(0.01f), "pt_controle");
	sphere2.shading.color = { 1, 0, 0 };



	// billboards flocons
	mesh quad_mesh = mesh_primitive_quadrangle({ -0.1f, 0, 0 }, { 0.1f, 0, 0 }, { 0.1f, 0, 0.2f }, { -0.1f, 0, 0.2f });
	quad_1.initialize(quad_mesh, "Quad 1");

	quad_1.texture = opengl_load_texture_image("assets/flocon_billboard_4.png");

	quad_1.shading.phong = { 0.4f, 0.6f,0,1 };

	quad_1.transform.translation = { 0,-0.5f,0 };

	// Initial timing between the creation of two particles.
	timer.event_period = 0.1f;
}






void scene_structure::display()
{

	// skybox first => derriere les autres elements de la scene.
	draw(skybox, environment); 


	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame){
		draw(global_frame, environment);
	}




	// montagne
	draw(terrain_drawable, environment);
	if (gui.display_wireframe){
		draw_wireframe(terrain_drawable, environment);
	}
	



	// pts de controle
	pts_controle = { vec2{-0.35f, 0.85f}, vec2{-0.3f, 0.65f}, vec2{-0.3f, 0.55f}, vec2{-0.29f, 0.45f}, vec2{-0.27f, 0.35f}, vec2{-0.20f, 0.25f}, vec2{-0.17f, 0.18f}, vec2{-0.16f, 0.07f}, vec2{-0.18f, -0.03f}, vec2{-0.24f, -0.11f}, vec2{-0.39f, -0.22f}, vec2{-0.43, -0.27}, vec2{-0.44, -0.37}, vec2{-0.50, -0.47}, vec2{-0.53, -0.48}, vec2{-0.59, -0.49}, vec2{-0.64, -0.54}, vec2{-0.67, -0.65}};
	for (int i=0; i<18; i++){
		sphere2.transform.translation = vec3{pts_controle[i][0], pts_controle[i][1], evaluate_terrain_height(pts_controle[i][0], pts_controle[i][1], terrain_mesh)+0.01f};
		draw(sphere2, environment);
	}
	
	
	// Update the current time
	float dt = timer.update();



	// flocons
	bool const new_particle = timer.event;
	if (new_particle == true) {
		for (int i = 0; i<100 ; i++){
			vec3 const p0 = { rand_interval(-1, 1), rand_interval(-1, 1), 1};

			// Initial random velocity (x,y) components are uniformly distributed along a circle.
			//const float theta = rand_interval(0, 2 * Pi);
			//const float theta = Pi / 5.0f;
			//const vec3 v0 = vec3(/*0.1f*std::cos(theta)*/ 0 , /* 0.1f*std::sin(theta)*/ 0, 0);
			const vec3 v0 = vec3(0, 0, -rand_interval(0.05f, 0.1f));
			const float s0 = rand_interval(0.02f, 0.05f);
			particles.push_back({ p0, v0, s0});
		}
	}


	// Evolve position of billboards
	
	for (particle_structure& particle : particles){
		// evolution a vitesse constante = suffit au realisme (on ne voit pas les flocons "accelerer" en vrai)
		vec3& p = particle.p;
		vec3& v = particle.v;
		p = p + dt * v;
	}


	// Evolve position of particles
	/*const vec3 g = { 0.0f, 0.0f, -0.005f };
	for (particle_structure& particle : particles){

		const float m = 0.01f; // particle mass
		const float mu = 10.0f;
		vec3& p = particle.p;
		vec3& v = particle.v;

		const vec3 F = m * g; - mu * v;

		// Numerical integration
		v = v + dt * F / m;	
		p = p + dt * v;
	}*/

	//Remove particles that are too low

	for (auto it = particles.begin(); it != particles.end(); ) {
		if (it->p.z < -0.5)
			it = particles.erase(it);
		if (it != particles.end())
			++it;
	}

	// Display flocons
	/*for (particle_structure& particle : particles)
	{
		sphere.transform.translation = particle.p;
		draw(sphere, environment);
	}*/





	//draw_wireframe(quad_1, environment);



	display_semiTransparent();


}



void scene_structure::display_semiTransparent()
{
	for (particle_structure& particle : particles)
	{
		
	// Enable use of alpha component as color blending for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);


	// Re-orient the grass shape to always face the camera direction
	vec3 const front = normalize(environment.camera.front() * vec3 { 1, 1, 0 }); // front-vector of the camera without z-component
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);
	
	quad_1.transform.rotation = R;
	quad_1.transform.translation = particle.p;

	// Sort transparent shapes by depth to camera
	//   This step can be skipped, but it will be associated to visual artifacts

	// Transform matrix (the same matrix which is applied in the vertices in the shader: T = Projection x View)
	mat4 T = environment.projection.matrix() * environment.camera.matrix_view();
	// Projected vertices (center of quads) in homogeneous coordinates
	vec4 p1 = T * vec4{ 0, -0.5f, 0, 1 };
	//vec4 p2 = T * vec4{ 0, +0.5f, 0, 1 };

	// Depth to camera
	float z1 = p1.z / p1.w;
	//float z2 = p2.z / p2.w;


	// Display the quads relative to their depth
	/*if (z1 <= z2) {
		draw(quad_2, environment);
		draw(quad_1, environment);
	}
	else {
		draw(quad_1, environment);
		draw(quad_2, environment);
	}*/
	draw(quad_1, environment);

	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}
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


