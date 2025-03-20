// clang-format off
#include <glad/glad.h>
#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_messagebox.h>

// internal files
#include "camera.h"
#include "debug_grid.h"
// clang-format on

#define WINDOW_TITLE "SimpleGrid"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CAMERA_NEAR 0.01f
#define CAMERA_FAR 1000.0f
#define CAMERA_FOV 75.0f
#define APP_CALLBACK __attribute__((unused))

typedef struct {
    struct {
        DebugGrid grid;
        Camera camera;
    } scene;
    SDL_Window* window;
    SDL_GLContext glcontext;
    Uint64 last_tick;
    float delta_time;
} AppState;

bool SceneLoad(AppState* state) {
    if (!DebugGridLoad(&state->scene.grid)) {
        return false;
    }

    CameraMakePerspective(&state->scene.camera, CAMERA_FOV,
                          (float)WINDOW_WIDTH / WINDOW_HEIGHT,
                          CAMERA_NEAR, CAMERA_FAR);

    return true;
}

void SceneUpdate(AppState* state) {
    CameraUpdate(&state->scene.camera, state->delta_time);
}

void SceneDraw(AppState *state) {
    DebugGridDraw(&state->scene.grid, &state->scene.camera);
}

void SceneUnload(AppState* state) {
    DebugGridUnload(&state->scene.grid);
}

APP_CALLBACK SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        return SDL_APP_FAILURE;
    }

    AppState* state = SDL_malloc(sizeof(AppState));
    SDL_zero(*state);

    // Set hints
    // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    // SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    // SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");

    // Init window
    Uint32 window_flags =
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH,
                                          WINDOW_HEIGHT, window_flags);
    if (window == NULL) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Configure window
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
    SDL_CaptureMouse(true);
    SDL_SetWindowRelativeMouseMode(window, true);

    // Set GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create OpenGL context
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if (glcontext == NULL) {
        SDL_Log("OpenGL context could not be created: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Dynamically link GLAD procedures using SDL get proc address
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        return SDL_APP_FAILURE;
    }

    // Setup render and window
    SDL_GL_MakeCurrent(window, glcontext);
    SDL_GL_SetSwapInterval(1);
    SDL_ShowWindow(window);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    state->window = window;
    state->glcontext = glcontext;
    state->last_tick = SDL_GetPerformanceCounter();
    state->delta_time = 0.0f;
    *appstate = state;

    if (!SceneLoad(state)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                 "Failed to load, check the logs", window);
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

APP_CALLBACK SDL_AppResult SDL_AppIterate(void* appstate) {
    AppState* state = (AppState*)appstate;

    Uint64 current_tick = SDL_GetPerformanceCounter();
    state->delta_time = (float) (current_tick - state->last_tick) / (float) SDL_GetPerformanceFrequency();
    state->last_tick = current_tick;

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SceneUpdate(state);
    // TODO(cedmundo): Maybe fix framerate on draw?
    SceneDraw(state);

    if (!SDL_GL_SwapWindow(state->window)) {
        SDL_Log("Failed to swap window: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

APP_CALLBACK SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    AppState* state = (AppState*)appstate;
    switch (event->type) {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_WINDOW_RESIZED:
            glViewport(0, 0, event->window.data1, event->window.data2);
            CameraMakePerspective(&state->scene.camera, CAMERA_FOV,
                                  (float)event->window.data1 / (float)event->window.data2,
                                  CAMERA_NEAR, CAMERA_FAR);
            break;
        case SDL_EVENT_KEY_UP:
            if (event->key.key == SDLK_ESCAPE) {
                SDL_CaptureMouse(false);
                SDL_SetWindowRelativeMouseMode(state->window, false);
            }
            break;
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

APP_CALLBACK void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AppState* state = (AppState*)appstate;
    if (result != SDL_APP_SUCCESS) {
        SDL_Log("Application quit with error: %d", result);
    }

    SceneUnload(state);
    if (state->window != NULL) {
        SDL_GL_DestroyContext(state->glcontext);
        SDL_DestroyWindow(state->window);
    }

    SDL_free(state);
}
