#pragma once

#include <fstream>
#include <vector>
#include <strstream>

#include "../math/math.hpp"

namespace LoaderOBJ {

struct Model {
	std::vector<unsigned int> indices;
	std::vector<float> positions;
	std::vector<float> texcoords;
	std::vector<float> normals;
};

inline Model Load(const std::string& _filename) {
    std::fstream file(_filename);

    if(!file.is_open()) {
        std::cerr <<  "Unable to find file: '" << _filename << "'!" << std::endl;
        return {};
    }

    std::vector<Math::Vec3> positions;
    std::vector<Math::Vec2> texcoords;
    std::vector<Math::Vec3> normals;

	Model finalModel;

	unsigned int id = 0;

    std::string line;
    while(getline(file, line)) {
        unsigned int slashCount = 0;
        for(char& c : line) {
            if(c == '/') {
                c = ' ';
                slashCount++;
            }
        }

        unsigned int verticesInFace = slashCount / 2;

        std::strstream lineStream;
        lineStream << line;

        if(line[0] == 'v' && line[1] == ' ') {
            std::string junk;
            Math::Vec3 data;
            lineStream >> junk >> data.x >> data.y >> data.z;
            positions.push_back(data);
        }
        else if(line[0] == 'v' && line[1] == 't') {
            std::string junk;
            Math::Vec2 data;
            lineStream >> junk >> data.x >> data.y;
            texcoords.push_back(data);
        }
        else if(line[0] == 'v' && line[1] == 'n') {
            std::string junk;
            Math::Vec3 data;
            lineStream >> junk >> data.x >> data.y >> data.z;
            normals.push_back(data);
        }
        else if(line[0] == 'f' && line[1] == ' ') {
            std::string junk;

            if(verticesInFace == 3) {
                unsigned int f_IndexPosition1, f_IndexTexCoord1, f_IndexNormal1,
                            f_IndexPosition2, f_IndexTexCoord2, f_IndexNormal2,
                            f_IndexPosition3, f_IndexTexCoord3, f_IndexNormal3;

                lineStream >> junk >> f_IndexPosition1 >> f_IndexTexCoord1 >> f_IndexNormal1
                            >> f_IndexPosition2 >> f_IndexTexCoord2 >> f_IndexNormal2
                            >> f_IndexPosition3 >> f_IndexTexCoord3 >> f_IndexNormal3;

                // f_FinalData.push_back(Vertex3D(f_Positions[f_IndexPosition1 - 1], f_TexCoords[f_IndexTexCoord1 - 1], f_Normals[f_IndexNormal1 - 1]));
                // f_FinalData.push_back(Vertex3D(f_Positions[f_IndexPosition2 - 1], f_TexCoords[f_IndexTexCoord2 - 1], f_Normals[f_IndexNormal2 - 1]));
                // f_FinalData.push_back(Vertex3D(f_Positions[f_IndexPosition3 - 1], f_TexCoords[f_IndexTexCoord3 - 1], f_Normals[f_IndexNormal3 - 1]));

				unsigned int i1 = id++;
				unsigned int i2 = id++;
				unsigned int i3 = id++;

				finalModel.indices.push_back(i1);
				finalModel.indices.push_back(i2);
				finalModel.indices.push_back(i3);

				finalModel.positions.push_back(positions[f_IndexPosition1 - 1].x);
				finalModel.positions.push_back(positions[f_IndexPosition1 - 1].y);
				finalModel.positions.push_back(positions[f_IndexPosition1 - 1].z);
				
					finalModel.positions.push_back(positions[f_IndexPosition2 - 1].x);
					finalModel.positions.push_back(positions[f_IndexPosition2 - 1].y);
					finalModel.positions.push_back(positions[f_IndexPosition2 - 1].z);

					finalModel.positions.push_back(positions[f_IndexPosition3 - 1].x);
					finalModel.positions.push_back(positions[f_IndexPosition3 - 1].y);
					finalModel.positions.push_back(positions[f_IndexPosition3 - 1].z);
					
				finalModel.texcoords.push_back(texcoords[f_IndexTexCoord1 - 1].x);
				finalModel.texcoords.push_back(texcoords[f_IndexTexCoord1 - 1].y);
				
					finalModel.texcoords.push_back(texcoords[f_IndexTexCoord2 - 1].x);
					finalModel.texcoords.push_back(texcoords[f_IndexTexCoord2 - 1].y);

					finalModel.texcoords.push_back(texcoords[f_IndexTexCoord3 - 1].x);
					finalModel.texcoords.push_back(texcoords[f_IndexTexCoord3 - 1].y);
				
				finalModel.normals.push_back(normals[f_IndexNormal1 - 1].x);
				finalModel.normals.push_back(normals[f_IndexNormal1 - 1].y);
				finalModel.normals.push_back(normals[f_IndexNormal1 - 1].z);

					finalModel.normals.push_back(normals[f_IndexNormal2 - 1].x);
					finalModel.normals.push_back(normals[f_IndexNormal2 - 1].y);
					finalModel.normals.push_back(normals[f_IndexNormal2 - 1].z);
				
					finalModel.normals.push_back(normals[f_IndexNormal3 - 1].x);
					finalModel.normals.push_back(normals[f_IndexNormal3 - 1].y);
					finalModel.normals.push_back(normals[f_IndexNormal3 - 1].z);
				
            }
            else {
                std::cerr << "Unable to load models with faces containing more than 4 vertices! Problematic file: " << _filename << std::endl;
                assert(false);
            }
        }
    }

    return finalModel;
}

}