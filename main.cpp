#include "3DEngine/Window.h"
#include "3DEngine/Camera.h"
#include "3DEngine/MaterialGroups.h"
#include "3DEngine/Loader/ObjLoader.h"
#include "3DEngine/Loader/ShaderLoader.h"
#include "3DEngine/Models/Track/TrackGeneration.h"

#include "3DEngine/GUI/Layout/Pane.h"
#include "3DEngine/GUI/Button/Button.h"
#include "3DEngine/GUI/Text/Text.h"

#include <vector>
#include <memory>
#include <string_view>

void loadTrackScene() {
    constexpr int width = 1400;
    constexpr int height = 1000;

    Window win(width, height);
    
    unique_ptr<GUIElement> pane = make_unique<Pane>();
    GUI &gui = GUI::Get();

    pane->addChildElement(std::make_unique<TrackGeneration>());
    
    gui.setChild(std::move(pane));
    win.run2D();
}

void loadMenu() {
    constexpr int width = 800;
    constexpr int height = 500;

    Window win(width, height);
    unique_ptr<GUIElement> pane = make_unique<Pane>();
    GUI &gui = GUI::Get();

    pane->addChildElement(std::move(make_unique<Text>(vec2{0, 0}, 16, "Choose Scene!")));

    auto but = std::make_unique<Button>(vec2{100, 200});
    but->addChildElement(std::make_unique<Text>(vec2{0, 0}, 16, "Track Generation"));
    but->onClick([] {
        std::puts("Start Track Scene!");
        loadTrackScene();
    });

    pane->addChildElement(std::move(but));

    gui.setChild(std::move(pane));
    win.run2D();
}

void generateFont() {
    constexpr int width = 16;
    constexpr int height = 16;

    Window win(width, height);
    GLuint programID = ShaderLoader::LoadShaders(string("./Data/Shader/FontMeta.vert"), string("./Data/Shader/FontMeta.frag"));
    win.generate(width, height, programID, &GUI::generateFontMeta);
}

int main(int argc, char* argv[]) {
    using namespace std::literals;

    if (argc == 1) {
        loadMenu();
    } else {
        const std::string_view arg = argv[1];
        if (arg.starts_with("Font"sv)) {
            generateFont();
        } else if (arg.starts_with("TrackGeneration"sv)) {
            loadTrackScene();
        }
    }

    return 0;
}
