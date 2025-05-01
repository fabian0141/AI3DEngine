#include "3DEngine/Window.h"
#include "3DEngine/Camera.h"
#include "3DEngine/MaterialGroups.h"
#include "3DEngine/Loader/ObjLoader.h"
#include "3DEngine/Loader/ShaderLoader.h"
#include "3DEngine/Models/Track/TrackGeneration.h"
#include <cstring>

#include "3DEngine/GUI/Button/Button.h"
#include "3DEngine/GUI/Text/Text.h"

/*void loadTrackScene() {
    int width = 1024;
    int height = 768;

    Camera camera(width, height);
    Window win(width, height, &camera);

    GLuint programID = ShaderLoader::LoadShaders( "./Data/Shader/Model.vert", "./Data/Shader/Model.frag" );
    GLuint trackID = ShaderLoader::LoadShaders( "./Data/Shader/Track.vert", "./Data/Shader/Track.frag" );

    MaterialGroups matGroups(&camera, programID);


    //ObjLoader::loadOBJ("./Data/Models/mclaren.obj", &matGroups);

    std::vector<Model*> models;
    models.push_back(new TrackGeneration(&camera, trackID));
    //models.push_back(new ObjModel(&camera, "./Data/Models/mclaren.obj"));
    
    win.run(models, &matGroups);

    for (int i = 0; i < models.size(); i++)
    {
        delete models[i];
    }
}*/

void loadTrackScene() {
    int width = 1400;
    int height = 1000;

    Window win(width, height);


    GUI gui;
    gui.addChildElement(new TrackGeneration(&gui));
    
    win.run2D(gui);
}

void loadMenu() {
    int width = 800;
    int height = 500;
    Window win(width, height);
    GUI gui;
    gui.addChildElement(new Text(vec2(0, 0), 16, "Hello World!", &gui));

    Button *but = new Button(vec2(100, 200), &gui);
    but->addChildElement(new Text(vec2(0, 0), 16, "Test Button", &gui));
    but->onClick([]() {
        printf("Start Track Scene!\n");
        loadTrackScene();
    });
    gui.addChildElement(but);

    win.run2D(gui);

}

void generateFont() {
    int width = 16;
    int height = 16;
    Window win(width, height);
    GLuint programID = ShaderLoader::LoadShaders( "./Data/Shader/FontMeta.vert", "./Data/Shader/FontMeta.frag" );
    win.generate(width, height, programID, &GUI::generateFontMeta);
}

int main(int argc, char *argv[]){
    if (argc == 1) {
        loadMenu();
    } else if (std::memcmp(argv[1], "Font", 4) == 0) {
        generateFont();
    } else if (std::memcmp(argv[1], "TrackGeneration", 15) == 0) {
        loadTrackScene();
    }

}