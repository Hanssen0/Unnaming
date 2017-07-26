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
    throw Error("initializing SDL error:") + SDL_GetError();
  if (TTF_Init() == -1)
    throw Error("initializing SDL_ttf error:") + SDL_GetError();
  if (IMG_Init(IMG_INIT_PNG) == -1)
    throw Error("initializing SDL_image error:") + SDL_GetError();
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
    main_window.NewWindow("你好", 50, 50, 700, 300, SDL_WINDOW_SHOWN,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_TEXTUREACCESS_TARGET);
    Object plus_one_second_button;
    plus_one_second_button.resize_styles(2);
    plus_one_second_button.set_style(0);
    Font main_font;
    main_font.open(path_of_itself + "Data/Font/WQYzenhei.ttf", 32);
    plus_one_second_button.styles_picture().set_renderer(main_window);
    plus_one_second_button.styles_picture().Create("+1s", 0, {0xff, 0xff, 0xff, 0xff}, main_font);
    int rect_width, rect_height;
    rect_width = plus_one_second_button.styles_picture().picture_width();
    rect_height = plus_one_second_button.styles_picture().picture_height();
    plus_one_second_button.set_style(1);
    plus_one_second_button.styles_picture().set_renderer(main_window);
    plus_one_second_button.styles_picture().Create({100, 100, 100, 0xff}, rect_width, rect_height);
    plus_one_second_button.styles_picture().set_is_visible(false);
    plus_one_second_button.AddToDetectZone({0, 0, 0, 0});
    DialogBox main_dialogbox;
    bool is_running = true;
    SDL_Event main_event;
    int click_counter = 0, fps_counter = 0;
    Picture counter, fps;
    counter.set_renderer(main_window);
    fps.set_renderer(main_window);
    Timer fps_controller, fps_monitor;
    while (is_running) {
      while (SDL_PollEvent(&main_event) != 0) {
        switch(main_event.type) {
          case SDL_QUIT: {
              is_running = false;
              break;
          }
          case SDL_MOUSEBUTTONDOWN: {
            if (main_event.button.button == SDL_BUTTON_LEFT) {
              if (plus_one_second_button.IsInclude(main_event.button.x, main_event.button.y)) {
                plus_one_second_button.set_style(1);
                plus_one_second_button.styles_picture().set_is_visible(true);
                ++click_counter;
                counter.Create(
                    "长者的生命已经延续了" + IntegerToString(click_counter) + "s,感谢你的无私续命!",
                    0, {0xff, 0xff, 0xff, 0xff}, main_font);
                main_dialogbox.add_picture(counter, false);
              }
            }
            break;
          }
          case SDL_MOUSEBUTTONUP: {
              plus_one_second_button.set_style(1);
              plus_one_second_button.styles_picture().set_is_visible(false);
            break;
          }
        }
      }
      main_window.ReadyToRender();
      plus_one_second_button.set_style(1);
      plus_one_second_button.styles_picture().CopyTo({0, 0, 0, 0});
      plus_one_second_button.set_style(0);
      plus_one_second_button.styles_picture().CopyTo({0, 0, 0, 0});
      main_dialogbox.CopyTo(0, plus_one_second_button.styles_picture().picture_height(), 0, 0, main_dialogbox.pictures_height(0));
      fps.CopyTo({0, plus_one_second_button.styles_picture().picture_height() + main_dialogbox.pictures_height(0), 0, 0});
      main_window.Display();
      if (fps_controller.time() < 16) SDL_Delay(16-fps_controller.time());
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
