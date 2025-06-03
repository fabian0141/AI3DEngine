#include "TextureLoader.h"

GLuint TextureLoader::loadTGA(const std::string& path, bool filter) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Image could not be opened: " << path << "\n";
        return 0;
    }

    // TGA header is 18 bytes
    std::array<unsigned char, 18> header{};
    if (!file.read(reinterpret_cast<char*>(header.data()), header.size())) {
        std::cerr << "Not a correct TGA file (header): " << path << "\n";
        return 0;
    }

    // Calculate image data position and size from header
    unsigned int dataPos = header[0] + (header[6] * 256 + header[5]) * header[7];
    unsigned int width = header[12] + header[13] * 256;
    unsigned int height = header[14] + header[15] * 256;
    unsigned int bitsPerPixel = header[16];
    unsigned int imageSize = width * height * bitsPerPixel / 8;

    // Read image data
    std::vector<unsigned char> data(imageSize);

    // Seek to data position
    file.seekg(dataPos, std::ios::cur);

    if (!file.read(reinterpret_cast<char*>(data.data()), imageSize)) {
        std::cerr << "Not a correct TGA file (image data): " << path << "\n";
        return 0;
    }

    // Create OpenGL texture
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Assuming 24 bits per pixel RGB in BGR order (common TGA format)
    GLenum format = (bitsPerPixel == 24) ? GL_BGR : GL_BGRA;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());

    if (filter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    return textureID;
}

GLuint TextureLoader::loadMETA(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Image could not be opened: " << path << "\n";
        return 0;
    }

    // META file size should be 256 shorts = 512 bytes
    std::vector<short> data(256);
    if (!file.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(short))) {
        std::cerr << "Error reading META file: " << path << "\n";
        return 0;
    }

    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16I, 16, 16, 0, GL_RED_INTEGER, GL_SHORT, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureID;
}

std::vector<int> TextureLoader::loadMETAData(const std::string& path) {
    std::ifstream infile(path, std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "File cannot be opened: " << path << "\n";
        exit(-1);
    }

    std::vector<int> data(256);
    for (size_t i = 0; i < data.size(); ++i) {
        short num = 0;
        infile.read(reinterpret_cast<char*>(&num), sizeof(short));
        if (!infile) {
            std::cerr << "Error occurred while reading file: " << path << "\n";
            exit(-1);
        }
        data[i] = static_cast<int>(num);
    }

    return data;
}
