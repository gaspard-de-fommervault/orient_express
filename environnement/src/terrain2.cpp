
#include "terrain2.hpp"

using namespace cgp;

mesh create_terrain_mesh()
{
	//cree un terrain plat
	int const terrain_sample = 1000;
	mesh terrain = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},terrain_sample,terrain_sample);
	return terrain;
}




/*void deformation_initiale(mesh& terrain, mesh_drawable& terrain_visual, int n_lignes, float pas, float hauteur){
	// lignes de montagnes

	// n_lignes : nombres de lignes
	// pas : entre 2 etapes successives de la marche aleatoire d'une ligne.
	// hauteur : hauteur moyenne des chaines



	// Number of samples in each direction (assuming a square grid)
	int const N = std::sqrt(terrain.position.size());

	// on commence par creer des chaines de montagne :

	for (int i = 0; i<n_lignes; i++){ // pour chaque ligne

		//On cree un point de depart, et une direction initiale
		int Ui = std::rand(N);
		int Vi = std::rand(N);

		float Theta = std::rand(360);

		//puis on itere sur chaque pas :
		
		// on tire une direction aleatoire :




	}

}*/

void update_terrain(mesh& terrain, mesh_drawable& terrain_visual, perlin_noise_parameters const& parameters)
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

float evaluate_terrain_height(float x, float y, mesh& terrain){
	int const N = std::sqrt(terrain.position.size());
	float ku = floor((x+1)*N/2);
	float kv = floor((y+1)*N/2);
	return terrain.position[ku*N+kv].z;
	
	//return noise_perlin({x, y}, parameters.octave, parameters.persistency, parameters.frequency_gain);

}

