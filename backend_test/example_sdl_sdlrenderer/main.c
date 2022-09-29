#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

SDL_Window *window = NULL;

int main(int argc, char* argv[])
{

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("failed to init: %s", SDL_GetError());
    return -1;
  }
  
  window = SDL_CreateWindow(
      "Hello", 0, 0, 1024, 768,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
      );
  if (window == NULL) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return -1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    SDL_Log("Error creating SDL_Renderer!");
    return 0;
  }

  // setup imgui
  igCreateContext(NULL);
  
  //set docking
  ImGuiIO* ioptr = igGetIO();

  igStyleColorsDark(NULL);

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer_Init(renderer);

  bool showDemoWindow = true;
  bool showAnotherWindow = false;
  ImVec4 clearColor;
  clearColor.x = 0.45f;
  clearColor.y = 0.55f;
  clearColor.z = 0.60f;
  clearColor.w = 1.00f;

  bool quit = false;
  while (!quit)
  {
    SDL_Event e;

    // we need to call SDL_PollEvent to let window rendered, otherwise
    // no window will be shown
    while (SDL_PollEvent(&e) != 0)
    {
      ImGui_ImplSDL2_ProcessEvent(&e);
      if (e.type == SDL_QUIT)
        quit = true;
      if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window))
        quit = true;
    }
    
    // start imgui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    igNewFrame();

    if (showDemoWindow)
      igShowDemoWindow(&showDemoWindow);

    // show a simple window that we created ourselves.
    {
      static float f = 0.0f;
      static int counter = 0;

      igBegin("Hello, world!", NULL, 0);

      igText("This is some useful text");
      igCheckbox("Demo window", &showDemoWindow);
      igCheckbox("Another window", &showAnotherWindow);

      igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0);
      igColorEdit3("clear color", (float*)&clearColor, 0);

      ImVec2 buttonSize;
      buttonSize.x = 0;
      buttonSize.y = 0;
      if (igButton("Button", buttonSize))
        counter++;
      igSameLine(0.0f, -1.0f);
      igText("counter = %d", counter);

      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
      igEnd();
    }

    if (showAnotherWindow)
    {
      igBegin("imgui Another Window", &showAnotherWindow, 0);
      igText("Hello from imgui");
      ImVec2 buttonSize;
      buttonSize.x = 0; buttonSize.y = 0;
      if (igButton("Close me", buttonSize))
      {
        showAnotherWindow = false;
      }
      igEnd();
    }

    // render
    igRender();
    
    SDL_SetRenderDrawColor(renderer, (Uint8)(clearColor.x * 255), (Uint8)(clearColor.y * 255), (Uint8)(clearColor.z * 255), (Uint8)(clearColor.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(igGetDrawData());
    SDL_RenderPresent(renderer);
  }

  // clean up
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  igDestroyContext(NULL);

  SDL_DestroyRenderer(renderer);
  if (window != NULL)
  {
    SDL_DestroyWindow(window);
    window = NULL;
  }
  SDL_Quit();

  return 0;
}