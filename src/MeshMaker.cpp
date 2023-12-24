#include "MeshMaker.h"



float* MeshMaker::fromObj(std::string nomfic, float x, float y, float z ,float size, unsigned int &nPoint){
	nPoint=0;
	std::vector<triangle> listeTriangle;

	std::ifstream f(nomfic);
	if (!f.is_open()){
		std::cout << "Erreur\n";
		return nullptr;
	}


	// Local cache of verts
	std::vector<vec3d> verts;

	while (!f.eof()){
		char line[128];
		f.getline(line, 128);

		std::stringstream s;
		s << line;

		char junk;

		if (line[0] == 'v'){
			vec3d v;
			s >> junk >> v.x >> v.y >> v.z;

			v.x = x + (size * v.x);
			v.y = y + (size * v.y);
			v.z = z + (size * v.z);
			verts.push_back(v);
		}

		if (line[0] == 'f'){
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			listeTriangle.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			nPoint+=3;
		}
	}

	float* tabTriangle =(float*)malloc(sizeof(float) * listeTriangle.size() * 9);

	//Affecte chaque coordonnees des points des triangles de la liste dans leurs case du tableau
	for(unsigned int i=0;i<(unsigned int)listeTriangle.size();i++){
		triangle temp = listeTriangle.at(i);

		tabTriangle[i*9]=temp.p[0].x;
		tabTriangle[i*9+1]=temp.p[0].y;
		tabTriangle[i*9+2]=temp.p[0].z;

		tabTriangle[i*9+3]=temp.p[1].x;
		tabTriangle[i*9+4]=temp.p[1].y;
		tabTriangle[i*9+5]=temp.p[1].z;

		tabTriangle[i*9+6]=temp.p[2].x;
		tabTriangle[i*9+7]=temp.p[2].y;
		tabTriangle[i*9+8]=temp.p[2].z;
	}

    return tabTriangle;
}