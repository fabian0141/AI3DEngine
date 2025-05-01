#include "TextureLoader.h"
#include <GLFW/glfw3.h>
#include <fstream>

GLuint TextureLoader::loadTGA(string path, bool filter) {

    unsigned char header[18];
    unsigned int dataPos = 0;     
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char * data;

    FILE * file = fopen(path.c_str(),"rb");
    if (!file) {
        printf("Image could not be opened\n"); 
        return 0;
    }

    if ( fread(header, 1, 18, file)!=18 ){ // If not 54 bytes read : problem
        printf("Not a correct TGA file\n");
        return false;
    }

    dataPos += header[0];
    dataPos += (header[6] * 256 + header[5]) * header[7];
    width = header[13] * 256 + header[12];
    height = header[15] * 256 + header[14];
    imageSize = width * height * header[16] / 8;

    data = new unsigned char[imageSize];
    fseek(file, dataPos, SEEK_CUR);
    int dataLen = fread(data, 1, imageSize, file);
    if (dataLen !=imageSize ){ // If not 54 bytes read : problem
        printf("Not a correct TGA file\n");
        return false;
    }
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Read the file, call glTexImage2D with the right parameters
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // Nice trilinear filtering.
    if (filter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }


    // Return the ID of the texture we just created
    return textureID;
}

GLuint TextureLoader::loadMETA(string path) {

    FILE * file = fopen(path.c_str(),"rb");
    if (!file) {
        printf("Image could not be opened\n"); 
        return 0;
    }

    short *data = new short[256];
    int dataLen = fread(data, 1, 512, file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_R16I, 16, 16, 0, GL_RED_INTEGER, GL_SHORT, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}

int* TextureLoader::loadMETAData(string path) {
    std::ifstream infile;
    infile.open(path, ios::binary | ios::in);
    if (!infile.is_open())
    {
        printf("File cannot be open!\n");;
    }

    int *data = new int[256];

    for (int i = 0; i < 256; i++)
    {
        short num;
        infile.read((char*)&num, sizeof(short));
        data[i] = num;
        //printf("%d \n", data[i]);
    }
    
    infile.close();
    if(!infile.good()) {
        printf("Error occurred at reading time!\n");
   }
    return data;
}
