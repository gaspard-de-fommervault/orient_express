#include "system_piston.hpp"

using namespace cgp;


void piston::initialize(){
	mesh mesh_train = mesh_load_file_obj("objets/locomotiveSansRoue.obj");
	mesh_train.position += {0,0,0.6};
	train.initialize(mesh_train, "train");

	// mise à l'echelle du train
	train.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi/2);
	train.transform.scaling = 0.8;
	train.transform.translation = {-1,0.5,-0.25};

	//creation des roues
    mesh wheel_mesh = mesh_wheel(0.5,0.1);
	wheel1.initialize(wheel_mesh,"wheel base");
    wheel2.initialize(wheel_mesh,"wheel 2");
    wheel3.initialize(wheel_mesh,"wheel 3");

	// ajout des bieles
	float r1 = 0.05;
	float r2 = 0.11;
	float h = 0.1;
	float lon = 1.0;
	float lar = 0.11;

	mesh baseRoue = disc_with_hole(0.49,0.48);
	base1.initialize(baseRoue, "base 1");

	mesh mesh_biele3 =  biele_3tous(r1, r2, h, lon, lar);
	mesh_biele3.position += {-lon-r2,0,0};
	biele3.initialize(mesh_biele3, "biele3");

	mesh mesh_biele2 =  biele_2tous(r1, r2, h, 2.2*lon, lar);
	biele2.initialize(mesh_biele2, "biele2");

	// ajout des vis

	mesh mesh_vis =  cylinder_with_texture(r1, h);
	vis1.initialize(mesh_vis, "vis 01");
	vis2.initialize(mesh_vis, "vis 02");


	// creation du 2e coté du piston
	base2.initialize(baseRoue, "base 2");

	wheel1_2.initialize(wheel_mesh,"wheel base 2");
    wheel2_2.initialize(wheel_mesh,"wheel 2 2");
    wheel3_2.initialize(wheel_mesh,"wheel 3 2");

	biele3_2.initialize(mesh_biele3, "biele3 2");

	biele2_2.initialize(mesh_biele2, "biele2 2");

	// ajout des vis

	vis1_2.initialize(mesh_vis, "vis 01 2");
	vis2_2.initialize(mesh_vis, "vis 02 2");

	// ajout des textures et couleurs
	//colors
	//train.texture = opengl_load_texture_image("assets/neige_train01.jpeg");
	train.shading.color = { 0.70f, 0.27f, 0.08f };
	train.shading.phong.ambient = 0.28;
	train.shading.phong.diffuse = 1.0;
	train.shading.phong.specular = 0.5;
	train.shading.phong.specular_exponent = 12.8;

	float alpha;
	float ambient;
	float diffuse;
	float specular;
	float c1;
	float c2;
	float c3;

	c1=0.80;
	c2=0.27;
	c3=0.08;
	ambient = 0.28;
	diffuse =1.0;
	specular = 0.5;
	alpha = 12.8;

	wheel1.shading.color = { c1,c2,c3 };
	wheel1.shading.phong.ambient = ambient;
	wheel1.shading.phong.diffuse = diffuse;
	wheel1.shading.phong.specular = specular;
	wheel1.shading.phong.specular_exponent = alpha;

	wheel3.shading.color = { c1,c2,c3 };
	wheel3.shading.phong.ambient = ambient;
	wheel3.shading.phong.diffuse = diffuse;
	wheel3.shading.phong.specular = specular;
	wheel3.shading.phong.specular_exponent = alpha;

	wheel2.shading.color = { c1,c2,c3 };
	wheel2.shading.phong.ambient = ambient;
	wheel2.shading.phong.diffuse = diffuse;
	wheel2.shading.phong.specular = specular;
	wheel2.shading.phong.specular_exponent = alpha;

	// ajout de la hierarchie
	
    hierarchy.add(train);

	//hierarchie piston 1
	hierarchy.add(base1, "train",{ 0,-0.7,0 });
	hierarchy["base 1"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi /2);
    hierarchy.add(wheel1, "base 1");
    hierarchy.add(wheel2, "base 1",{ 1.1,0,0.0 });
    hierarchy.add(wheel3, "base 1",{ -1.1,0,0.0 });
	hierarchy.add(biele3, "wheel base",{ 0.25,0,0.1 });
	hierarchy.add(biele2, "biele3",{ 0,0,0.1 });
	hierarchy["biele2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, 3.14);
	hierarchy.add(vis1, "biele3",{ 0,0,0.2 });
	hierarchy.add(vis2, "biele2",{ 2.2*lon+r2,0,0.2 });

	//hierarchie piston 2
	hierarchy.add(base2, "train");
	hierarchy["base 2"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -Pi /2);
	hierarchy["base 2"].transform.translation = {0,0.7,0};
	hierarchy.add(wheel1_2, "base 2");
    hierarchy.add(wheel2_2, "base 2",{ 1.1,0,0.0 });
    hierarchy.add(wheel3_2, "base 2",{ -1.1,0,0.0 });
	hierarchy.add(biele3_2, "wheel base 2",{ 0.25,0,0.1 });
	hierarchy.add(biele2_2, "biele3 2",{ 0,0,0.1 });
	hierarchy["biele2 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, 3.14);
	hierarchy.add(vis1_2, "biele3 2",{ 0,0,0.2 });
	hierarchy.add(vis2_2, "biele2 2",{ 2.2*lon+r2,0,0.2 });


	//hierarchy["train"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, Pi/2);
	std::cout<< "aaaaqaaaaaaqaaaaaaqaaaaaaqaa" <<std::endl;

	std::cout<< hierarchy["train"].transform.rotation <<std::endl;
	hierarchy["train"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -Pi /2);
	// hierarchy["train"].transform.scaling = 0.005f;

}


void piston::tournerRoue(float t){

	float r2 = 0.11;
	float lon = 1.0;
	float e = 0.25;

	hierarchy["wheel base"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 },-t);
	hierarchy["wheel 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -t);
    hierarchy["wheel 3"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -t);
	hierarchy["biele3"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, t);
	hierarchy["biele2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, std::asin(-std::sin(-t)*(e)/(2.2*lon+r2)));

	hierarchy["wheel base 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 },t);
	hierarchy["wheel 2 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, t);
    hierarchy["wheel 3 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, t);
	hierarchy["biele3 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -t);
	hierarchy["biele2 2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, std::asin(-std::sin(t)*(e)/(2.2*lon+r2)));


	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();
}


void piston::avancer(float vitesse){
	rotation_transform etat_train;
	etat_train = hierarchy["train"].transform.rotation;

	// hierarchy["train"].transform.translation += {}

}

void piston::translationPiston(cgp::vec3 p){
	this->hierarchy["train"].transform.translation = p;
	return;
}

void piston::update(cgp::vec3 const& p, environmentType& environment, float hauteur)
{
	// Display the interpolated position
	this->hierarchy["train"].transform.translation = p+vec3{0,0,hauteur};

}

cgp::buffer<float> piston::acceleration(float accelerationAvant, float vitesse, float vitesseMax, float dt, cgp::mesh& terrain, cgp::vec3 position, chemin chemin1, float distance, float tMin, float tMax, float degX, float degZ){
	float acceleration;
	float coef = 1.0f;
	float seuil = 0.0f;
	float hauteur = 0.55f;
	float vitesseMin = 0.01f;
	float accelerationMin = -2.0f;
	float accelerationMax = 2.0f;

	acceleration = accelerationAvant + vitesse*dt/8;
	if (acceleration<accelerationMin){
		acceleration = accelerationMin;
	}
	else if (acceleration>accelerationMax){
		acceleration = accelerationMax;
	}
	vitesse = vitesse + acceleration*dt;

	if (vitesse<vitesseMin) {
		vitesse = vitesseMin;
	}
	else if (vitesse>vitesseMax){
		vitesse = vitesseMax;
	}

	distance=distance+vitesse*dt;   //ça me semble faux ça, enfin dt toi etre tres petit
	if (distance>tMax-0.2f){
		return this->acceleration(0.1f,0.1f,vitesseMax,0.1f,terrain,interpolation(tMin+0.01f, chemin1.key_positions, chemin1.key_times, terrain), chemin1, tMin+0.01f, tMin,tMax,0,0);
	}
	vec3 p = interpolation(distance+0.1f, chemin1.key_positions, chemin1.key_times, terrain);
	this->hierarchy["train"].transform.translation = p+vec3{0,0,hauteur};
	//rotation du train en fonction de l interpolation
	vec3 pente = p - position;
	//	float degreX = std::atan(pente[1]/pente[0]);
	
	vec3 rot = rotationTrain(distance+0.1f, chemin1.key_positions2, chemin1.key_times, terrain);
	vec3 rot2 = rotationTrain(distance+0.1f, chemin1.key_positions3, chemin1.key_times, terrain);

	// Si on est pas trop proche de l'initialisation on controle les ecarts entre les degres 
	// pas trop proche = mesure par un taux
	float tauxZ = 0.05;
	float tauxX = 1.15;
	float degreZ = rot2[1]/7;
	float degreX = rot[2];
	degreZ = -std::atan(pente[2]/pente[0]);


	if(distance>tMin+0.2f){
		if(degreX*degX<0 && std::abs(degreX)>0.2){
			degreX=-degreX;
		}
		if(degreZ*degZ<0 && std::abs(degreZ)>0.2){
			degreZ=-degreZ;
		}
		/*
		if(degreX>(1+tauxX)*degX){
			degreX=degX*(1+tauxX);
		}
		if(degreX<(1-tauxX)*degX){
			degreX=degX*(1-tauxX);
		}
		*/
		if (degZ !=0){
			if(degreZ>(1+tauxZ)*degZ){
				degreZ=degZ*(1+tauxZ);
			}
			if(degreZ<(1-tauxZ)*degZ){
				degreZ=degZ*(1-tauxZ);
			}
		} 

		
	}

	//std::cout<< rot[2] <<std::endl;	
	this->hierarchy["train"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 0*degreZ)*rotation_transform::from_axis_angle({ 0,0,1 }, degreX+Pi);
			
	// quand ça descend c est positif
	acceleration = acceleration*(1+degreZ);


	cgp::buffer<float> mouvement = {acceleration, vitesse, distance,degreX,degreZ};
	std::cout<< mouvement <<std::endl;

	return mouvement;
}

