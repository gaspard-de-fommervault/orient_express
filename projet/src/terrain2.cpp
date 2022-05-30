
#include "terrain2.hpp"

using namespace cgp;

mesh montagne::create_terrain_mesh()
{
	//cree un terrain plat
	int const terrain_sample = 1000;
	mesh terrain = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},terrain_sample,terrain_sample);
	return terrain;
}


void montagne::update_terrain(mesh& terrain, mesh_drawable& terrain_visual, perlin_noise_parameters const& parameters)
{
	// Number of samples in each direction (assuming a square grid)
	int const N = std::sqrt(terrain.position.size());

	// Recompute the new vertices
	for (int ku = 0; ku < N; ++ku) {
		for (int kv = 0; kv < N; ++kv) {
			
			// Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

			int const idx = ku*N+kv;

			// Compute the Perlin noise
			float const noise = noise_perlin({u, v}, parameters.octave, parameters.persistency, parameters.frequency_gain);

			// use the noise as height value
			terrain.position[idx].z = parameters.terrain_height*noise - parameters.terrain_height/2;

			// use also the noise as color value
			terrain.color[idx] = vec3(1,1,1);
		}
	}

	// Update the normal of the mesh structure
	terrain.compute_normal();
	
	// Update step: Allows to update a mesh_drawable without creating a new one
	terrain_visual.update_position(terrain.position);
	terrain_visual.update_normal(terrain.normal);
	terrain_visual.update_color(terrain.color);	
}

float montagne::evaluate_terrain_height(float x, float y, mesh& terrain){
	int const N = std::sqrt(terrain.position.size());
	float ku = floor((x+1)*N/2);
	float kv = floor((y+1)*N/2);
	return terrain.position[ku*N+kv].z;
	
	//return noise_perlin({x, y}, parameters.octave, parameters.persistency, parameters.frequency_gain);

}

void montagne::initialize(){

	// Montagnes
	terrain_mesh = create_terrain_mesh();
	terrain_drawable.initialize(terrain_mesh, "terrain");
	update_terrain(terrain_mesh, terrain_drawable, parameters);
	
	// texture
	GLuint const texture_image_id = opengl_load_texture_image("assets/texture_montagne_10.jpg",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
	
	terrain_drawable.texture = texture_image_id;

	// positions des points de controle de la trajectoire des rails
	sphere2.initialize(mesh_primitive_sphere(0.01f), "pt_controle");
	sphere2.shading.color = { 1, 0, 0 };
	pts_controle = { vec2{-0.35f, 0.85f}, vec2{-0.3f, 0.65f}, vec2{-0.3f, 0.55f}, vec2{-0.29f, 0.45f}, vec2{-0.27f, 0.35f}, vec2{-0.20f, 0.25f}, vec2{-0.17f, 0.18f}, vec2{-0.16f, 0.07f}, vec2{-0.18f, -0.03f}, vec2{-0.24f, -0.11f}, vec2{-0.39f, -0.22f}, vec2{-0.43, -0.27}, vec2{-0.44, -0.37}, vec2{-0.50, -0.47}, vec2{-0.53, -0.48}, vec2{-0.59, -0.49}, vec2{-0.64, -0.54}, vec2{-0.67, -0.65}};


}
