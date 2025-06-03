cmake_minimum_required(VERSION 3.22)
project(3DAIEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_subdirectory(AIEngine)

set(all_src
        3DEngine/Camera.h
        3DEngine/Camera.cpp
        3DEngine/Window.h
        3DEngine/Window.cpp
        3DEngine/MaterialGroups.h
        3DEngine/MaterialGroups.cpp

        3DEngine/GUI/GUI.h
        3DEngine/GUI/GUI.cpp
        3DEngine/GUI/GUIElement.h
        3DEngine/GUI/Button/Button.h
        3DEngine/GUI/Button/Button.cpp        
        3DEngine/GUI/Layout/Pane.h
        3DEngine/GUI/Layout/Pane.cpp   
        3DEngine/GUI/Text/Text.h
        3DEngine/GUI/Text/Text.cpp

        3DEngine/Helper/Intersection.h
        3DEngine/Helper/Intersection.cpp

        3DEngine/Loader/ShaderLoader.h
        3DEngine/Loader/ShaderLoader.cpp
        3DEngine/Loader/TextureLoader.h
        3DEngine/Loader/TextureLoader.cpp
        3DEngine/Loader/ObjLoader.h
        3DEngine/Loader/ObjLoader.cpp

        3DEngine/Models/Model.h
        3DEngine/Models/Cube/Cube.h
        3DEngine/Models/Cube/Cube.cpp
        3DEngine/Models/Triangle/Triangle.h
        3DEngine/Models/Triangle/Triangle.cpp
        3DEngine/Models/Track/Spline.h
        3DEngine/Models/Track/Spline.cpp
        3DEngine/Models/Track/Track.h
        3DEngine/Models/Track/Track.cpp
        3DEngine/Models/Track/TrackGeneration.h
        3DEngine/Models/Track/TrackGeneration.cpp
        3DEngine/Models/ObjModel/ObjModel.h
        3DEngine/Models/ObjModel/ObjModel.cpp


    )

add_executable(Engine main.cpp)
target_sources(Engine PRIVATE ${all_src})

target_link_libraries(Engine -lGL -lglfw -lGLEW)
