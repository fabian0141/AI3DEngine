#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"
#include "Models/Model.h"
#include <vector>
#include "MaterialGroups.h"
using namespace std;

Window::Window(int width, int height, Camera* camera) {
    this->camera = camera;
    glewExperimental = true; // Needed for core profile
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(-1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( width, height, "Tutorial 01", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(-1);
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

Window::Window(int width, int height) {
    glewExperimental = true; // Needed for core profile
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( width, height, "Tutorial 01", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(-1);
    }
}

void Window::run(vector<Model*> models, MaterialGroups* matGroups) {
    printf("Start drawing\n");

    vec3 position = vec3( 0, 0, 5 );
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float initialFoV = 45.0f;

    float speed = 3.0f;
    float mouseSpeed = 0.02f;
    float lastTime = glfwGetTime();
    int nbFrames = 0;
    double lastFPSCall = glfwGetTime();

    do{
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;
        nbFrames++;
        if ( currentTime - lastFPSCall >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastFPSCall += 1.0;
        }


        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xpos, ypos;
        double centerX = width/2.0;
        double centerY = height/2.0;

        glfwGetCursorPos(window, &xpos, &ypos);
        glfwSetCursorPos(window, centerX, centerY);

        horizontalAngle += mouseSpeed * deltaTime * float( centerX - xpos );
        verticalAngle   += mouseSpeed * deltaTime * float( centerY - ypos );

        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );

        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
        );

        glm::vec3 up = glm::cross( right, direction );

        // Move forward
        if (glfwGetKey(window,  GLFW_KEY_W ) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(window,  GLFW_KEY_S ) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(window,  GLFW_KEY_D ) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(window,  GLFW_KEY_A ) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }

        camera->update(width, height, position, direction, up);

        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glViewport(0, 0, width, height);

        // Draw nothing, see you in tutorial 2 !



        for (int i = 0; i < models.size(); i++)
        {
            models[i]->draw();
        }

        matGroups->draw();


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );
}

void Window::run2D(GUI &gui) {
	glfwSetWindowUserPointer(window, &gui);


    gui.preDraw();
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    vec2 position = vec2( 0, 0);
    float speed = 500.0f;
    float lastTime = glfwGetTime();

    do{
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        if (glfwGetKey(window,  GLFW_KEY_W ) == GLFW_PRESS){
            position.y += deltaTime * speed;
        }
        if (glfwGetKey(window,  GLFW_KEY_S ) == GLFW_PRESS){
            position.y -= deltaTime * speed;
        }
        if (glfwGetKey(window,  GLFW_KEY_D ) == GLFW_PRESS){
            position.x -= deltaTime * speed;
        }
        if (glfwGetKey(window,  GLFW_KEY_A ) == GLFW_PRESS){
            position.x += deltaTime * speed;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, width, height);

        gui.updateEnvironment(width, height, position, vec2(xpos, ypos));
        gui.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );
}

void Window::generate(int width, int height, unsigned int programID, void (*func)(unsigned int)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);

    func(programID);

    glfwSwapBuffers(window);
    glfwPollEvents();

}


void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        gui->click(vec2(xpos, ypos), action);
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
    gui->contentZoom *= yoffset > 0 ? 1.02 : 0.98;
    //printf("%f \n", yoffset);
}
