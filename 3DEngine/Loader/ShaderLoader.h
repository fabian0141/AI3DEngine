
typedef unsigned int GLuint;

class ShaderLoader {
    public:
        static GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path, const char * geometry_file_path = nullptr);
};