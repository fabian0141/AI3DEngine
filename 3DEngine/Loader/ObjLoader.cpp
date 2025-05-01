#include "ObjLoader.h"
#include <cstdio>
#include <cstring>
#include <string>
#include "../MaterialGroups.h"
#include "../Models/ObjModel/ObjModel.h"

struct MaterialIndices {
    string material;
    vector< unsigned int >* indices;
};

bool ObjLoader::loadOBJ(string path, MaterialGroups* matGroups) {


    loadMTL(path.substr(0, path.length() - 3), matGroups);

    FILE * file = fopen(path.c_str(), "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }

    vector< MaterialIndices > matIndices;
    vector< vec3 >* vertices = new vector< vec3 >();
    vector< vec2 >* uvs  = new vector< vec2 >();
    vector< vec3 >* normals  = new vector< vec3 >();

    vector< vec3 > temp_vertices;
    vector< vec2 > temp_uvs;
    vector< vec3 > temp_normals;

    while( 1 ){
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if ( strcmp( lineHeader, "usemtl" ) == 0 ){
            char material[128];
            fscanf(file, "%s\n", material );
            string matName = material;
            matIndices.push_back( MaterialIndices {matName.substr(0, matName.find(".")), new vector< unsigned int >()});

            vertices->resize(temp_vertices.size());
            uvs->resize(temp_vertices.size());
            normals->resize(temp_vertices.size());
            vector< unsigned int >* indices  = new vector< unsigned int >();


        } else if ( strcmp( lineHeader, "v" ) == 0 ){
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);

        } else if ( strcmp( lineHeader, "vt" ) == 0 ){
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);    
        } else if ( strcmp( lineHeader, "vn" ) == 0 ){
            vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        } else if ( strcmp( lineHeader, "f" ) == 0 ){
            string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            matIndices.back().indices->push_back(vertexIndex[0]-1);
            matIndices.back().indices->push_back(vertexIndex[1]-1);
            matIndices.back().indices->push_back(vertexIndex[2]-1);

            int pos1 = vertexIndex[0]-1;
            int pos2 = vertexIndex[1]-1;
            int pos3 = vertexIndex[2]-1;

            (*vertices)[pos1] = temp_vertices[vertexIndex[0]-1];
            (*vertices)[pos2] = temp_vertices[vertexIndex[1]-1];
            (*vertices)[pos3] = temp_vertices[vertexIndex[2]-1];

            (*uvs)[pos1] = temp_uvs[uvIndex[0]-1];
            (*uvs)[pos2] = temp_uvs[uvIndex[1]-1];
            (*uvs)[pos3] = temp_uvs[uvIndex[2]-1];

            (*normals)[pos1] = temp_normals[normalIndex[0]-1];
            (*normals)[pos2] = temp_normals[normalIndex[1]-1];
            (*normals)[pos3] = temp_normals[normalIndex[2]-1];
        }
    }        

    ObjModel* model = new ObjModel(vertices, matIndices[0].indices, uvs, normals);
    matGroups->addModel(matIndices[0].material, model);

    for (int i = 1; i < matIndices.size(); i++)
    {
        matGroups->addModel(matIndices[i].material, model->getSubModel(matIndices[i].indices));
    }
    
}

bool ObjLoader::loadMTL(string path, MaterialGroups* matGroups) {
    path.append("mtl");

    FILE * file = fopen(path.c_str(), "r");
    if( file == NULL ) {
        printf("Impossible to open the file !\n");
        return false;
    }

    Material* mat;
    char name[128];


    while( 1 ){
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;


        if ( strcmp( lineHeader, "newmtl" ) == 0 ){
            mat = new Material {};
            fscanf(file, "%s\n", name );

        } else if ( strcmp( lineHeader, "Ns" ) == 0 ) {
            fscanf(file, "%f\n", &mat->specularExponent );

        } else if ( strcmp( lineHeader, "Ka" ) == 0 ) {
            vec3 ambient;
            fscanf(file, "%f %f %f\n", &ambient.x, &ambient.y, &ambient.z );
            mat->ambientColor = ambient;

        } else if ( strcmp( lineHeader, "Kd" ) == 0 ) {
            vec3 diffusion;
            fscanf(file, "%f %f %f\n", &diffusion.x, &diffusion.y, &diffusion.z );
            mat->diffuseColor = diffusion;

        } else if ( strcmp( lineHeader, "Ks" ) == 0 ) {
            vec3 specular;
            fscanf(file, "%f %f %f\n", &specular.x, &specular.y, &specular.z );
            mat->specularColor = specular;

        } else if ( strcmp( lineHeader, "Ke" ) == 0 ) {
            vec3 emmisive;
            fscanf(file, "%f %f %f\n", &emmisive.x, &emmisive.y, &emmisive.z );
            mat->emmissive = emmisive;

        } else if ( strcmp( lineHeader, "Ni" ) == 0 ) {
            fscanf(file, "%f\n", &mat->refraction );

        } else if ( strcmp( lineHeader, "d" ) == 0 ) {
            fscanf(file, "%f\n", &mat->dissolve );

        } else if ( strcmp( lineHeader, "illum" ) == 0 ) {
            fscanf(file, "%d\n", &mat->illumination );
            string matName = name;
            matGroups->addMaterial(matName.substr(0, matName.find(".")), mat);
        }
    }
}
