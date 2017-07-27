/*Please indicate the source
Software license:GPL v3
Author:Handsome_hell
E-mail:handsome0hell@gmail.com
*/
#include "Error.h"
#include "Game.h"
#include "Window.h"
#include "StdHead.h"


std::string IntegerToString(int number)
{
  std::string result;
  bool is_bigger_than_zero = true;
  if (number < 0)
    is_bigger_than_zero = false, number = -number;
  else if (number == 0)
    result = '0';
  while (number != 0) {
    result = static_cast<char>(number % 10 + '0') + result;
    number /= 10;
  }
  if(!is_bigger_than_zero)
    result = '-' + result;
  return result;
}

/*Initialize SDL lib
Call it before use SDL lib
*/
void Init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    throw Error("Initializing SDL2 error:") + SDL_GetError();
  if (TTF_Init() == -1)
    throw Error("Initializing SDL2_ttf error:") + SDL_GetError();
  if (IMG_Init(IMG_INIT_PNG) == -1)
    throw Error("Initializing SDL2_image error:") + SDL_GetError();
  SDL_StartTextInput();
}

/*Close SDL lib
Call it after used SDL lib
*/
void Exit() {
  SDL_StopTextInput();
  SDL_Quit();
  TTF_Quit();
  IMG_Quit();
}

int main(int argc, char** argv) {
  std::string path_of_itself = argv[0];
  path_of_itself = path_of_itself.substr(0, path_of_itself.rfind('/') + 1);
  try {
    Init();
    Window main_window;
    main_window.NewWindow("Unnaming", 50, 50, 700, 300, SDL_WINDOW_SHOWN,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_TEXTUREACCESS_TARGET);
    Font main_font;
    main_font.open(path_of_itself + "Data/Font/WQYzenhei.ttf", 16);
    const int block_width = 1, block_height = 1;
    Picture creature_picture, game_world_surface, nothing_picture;
    creature_picture.set_renderer(main_window);
    creature_picture.Create({0xff, 0xff, 0xff}, block_width, block_height);
    nothing_picture.set_renderer(main_window);
    nothing_picture.Create({0, 0, 0}, block_width, block_height);
    const int game_world_width = 700, game_world_height = 200;
    game_world_surface.set_renderer(main_window);
    game_world_surface.Create({0, 0, 0}, game_world_width * block_width, game_world_height * block_height);
    bool game_world_data[2][game_world_width][game_world_height];
    bool game_world_data_right_now = false;
    for (int loopx = 0; loopx < game_world_width; ++loopx) {
      for (int loopy = 0; loopy < game_world_height; ++loopy) {
        game_world_data[0][loopx][loopy] = rand() % 2;
      }
    }
    DialogBox main_dialogbox;
    bool is_running = true;
    SDL_Event main_event;
    int click_counter = 0, fps_counter = 0;
    Picture counter, fps;
    counter.set_renderer(main_window);
    fps.set_renderer(main_window);
    Timer fps_controller, fps_monitor;
    while (is_running) {
      game_world_surface.HijackWindow();
      for (int loopx = 0; loopx < game_world_width; ++loopx) {
        for (int loopy = 0; loopy < game_world_height; ++loopy) {
          short count = 0;
          if (loopx != 0) {
            if (game_world_data[game_world_data_right_now][loopx - 1][loopy] == true) ++count;
            if (loopy != 0 && game_world_data[game_world_data_right_now][loopx - 1][loopy - 1] == true) ++count;
            if (loopy != game_world_width - 1 && game_world_data[game_world_data_right_now][loopx - 1][loopy + 1] == true) ++count;
          }
          if (loopy != 0 && game_world_data[game_world_data_right_now][loopx][loopy - 1] == true) ++count;
          if (loopy != game_world_width - 1 && game_world_data[game_world_data_right_now][loopx][loopy + 1] == true) ++count;
          if (loopx != game_world_width - 1) {
            if (game_world_data[game_world_data_right_now][loopx + 1][loopy] == true) ++count;
            if (loopy != 0 && game_world_data[game_world_data_right_now][loopx + 1][loopy - 1] == true) ++count;
            if (loopy != game_world_width - 1 && game_world_data[game_world_data_right_now][loopx + 1][loopy + 1] == true) ++count;
          }
          if (count < 2 || count > 3) game_world_data[!game_world_data_right_now][loopx][loopy] = false;
          if (count == 2) game_world_data[!game_world_data_right_now][loopx][loopy] = game_world_data[game_world_data_right_now][loopx][loopy];
          if (count == 3) game_world_data[!game_world_data_right_now][loopx][loopy] = true;
          if (game_world_data[!game_world_data_right_now][loopx][loopy] != game_world_data[game_world_data_right_now][loopx][loopy]) {
            if (game_world_data[!game_world_data_right_now][loopx][loopy] == false) nothing_picture.CopyTo({loopx * block_width, loopy * block_height, 0, 0});
            else creature_picture.CopyTo({loopx * block_width, loopy * block_height, 0, 0});
          }
        }
      }
      game_world_surface.ReleaseWindow();
      game_world_data_right_now = !game_world_data_right_now;
      while (SDL_PollEvent(&main_event) != 0) {
        switch(main_event.type) {
          case SDL_QUIT: {
              is_running = false;
              break;
          }
          case SDL_MOUSEBUTTONDOWN: {
            if (main_event.button.button == SDL_BUTTON_LEFT) {
              int center_x = main_event.button.x / block_width, center_y = main_event.button.y / block_height;
              game_world_data[game_world_data_right_now][center_x][center_y] = true;
              creature_picture.CopyTo({center_x * block_width, center_y * block_height, 0, 0});
              if (center_x != 0) {
                game_world_data[game_world_data_right_now][center_x - 1][center_y] = true;
                if (center_y != 0) game_world_data[game_world_data_right_now][center_x - 1][center_y - 1] = true;
                if (center_y != game_world_height - 1) game_world_data[game_world_data_right_now][center_x - 1][center_y + 1] = true;
              }
              if (center_y != 0) game_world_data[game_world_data_right_now][center_x][center_y - 1] = true;
              if (center_y != game_world_height - 1) game_world_data[game_world_data_right_now][center_x][center_y + 1] = true;
              if (center_x != game_world_width - 1) {
                game_world_data[game_world_data_right_now][center_x + 1][center_y] = true;
                if (center_y != 0) game_world_data[game_world_data_right_now][center_x + 1][center_y - 1] = true;
                if (center_y != game_world_height - 1) game_world_data[game_world_data_right_now][center_x + 1][center_y + 1] = true;
              }
            }
            break;
          }
          case SDL_MOUSEBUTTONUP: {
            break;
          }
        }
      }
      main_window.ReadyToRender();
      //game_world_surface.CopyTo({0, 0, 0, 0});
      fps.CopyTo({0, game_world_height * block_height, 0, 0});
      main_window.Display();
      if (fps_controller.time() < 16) SDL_Delay(16 - fps_controller.time());
      fps_controller.start();
      fps_counter++;
      if (fps_monitor.time() >= 1000) {
        fps.Create("每秒帧数:" + IntegerToString(fps_counter),0, {0xff, 0xff, 0xff, 0xff}, main_font);
        fps_counter = 0;
        fps_monitor.start();
      }
    }
  }
  catch(Error &Mes) {
    std::ofstream ErrorStream;
    ErrorStream.open("./Error.log");
    ErrorStream << Mes.error_type() << std::endl;
    ErrorStream.close();
    return 1;
  }
  Exit();
  return 0;
}
