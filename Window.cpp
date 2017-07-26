/*Please indicate the source
Software license:GPL v3
Author:Handsome_hell
E-mail:handsome0hell@gmail.com
*/
#include "Window.h"

void Picture::Create(const std::string &path) {
  if (renderer_ != nullptr) {
    clear();
    if (path.empty()) return;
    SDL_Surface *render_surface;
    render_surface = IMG_Load(path.c_str());
    if (render_surface == nullptr)
      throw Error("Unable to create surface.") + SDL_GetError();
    picture_ = SDL_CreateTextureFromSurface(renderer_, render_surface);
    if (picture_ == nullptr)
      throw Error("Unable to create texture.") + SDL_GetError();
    picture_height_ = render_surface->h, picture_width_ = render_surface->w;
    SDL_FreeSurface(render_surface);
  }
  else {
    Error("Please set a renderer to picture");
  }
}

void Picture::Create(const std::string &text, int rendering_mode_of_text, const SDL_Color &text_color,
    const Font &font, const SDL_Color &background_color_of_text) {
  if (renderer_ != nullptr) {
    clear();
    if (text.empty()) return;
    SDL_Surface * render_surface = nullptr;
    switch(rendering_mode_of_text) {
      case 0: {
        render_surface = TTF_RenderUTF8_Blended(font.font_, text.c_str(), text_color);
        break;
      }
      case 1: {
        render_surface = TTF_RenderUTF8_Shaded(font.font_, text.c_str(), text_color, background_color_of_text);
        break;
      }
      case 2: {
        render_surface = TTF_RenderUTF8_Solid(font.font_, text.c_str(), text_color);
        break;
      }
      default: {
        throw Error("Not allowed text mode.");
      }
    }
    if (render_surface == nullptr)
      throw Error("Unable to create surface.") + SDL_GetError();
    picture_ = SDL_CreateTextureFromSurface(renderer_, render_surface);
    if (picture_ == nullptr)
      throw Error("Unable to create texture.") + SDL_GetError();
    picture_height_ = render_surface->h, picture_width_ = render_surface->w;
    SDL_FreeSurface(render_surface);
  }
  else {
    Error("Please set a renderer to picture");
  }
}

void Picture::Create(const SDL_Color &rect_color, int width, int height) {
  clear();
  if (rect_color.a == 0xff)
    picture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);
  else
    picture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
  if (picture_ == nullptr)
    throw Error("Unable to create texture.") + SDL_GetError();
  picture_width_ = width, picture_height_ = height;
  SDL_SetRenderTarget(renderer_, picture_);
  SDL_Color original_color;
  SDL_GetRenderDrawColor(renderer_, &original_color.r, &original_color.g, &original_color.b, &original_color.a);
  SDL_SetRenderDrawColor(renderer_, rect_color.r, rect_color.g, rect_color.b, rect_color.a);
  SDL_Rect rect_to_draw = {0, 0, width, height};
  if (SDL_RenderFillRect(renderer_, &rect_to_draw) != 0)
    throw Error("Fill rect error.") + SDL_GetError();
  SDL_SetRenderDrawColor(renderer_, original_color.r, original_color.g, original_color.b, original_color.a);
  SDL_SetRenderTarget(renderer_, nullptr);
}

void Object::AddToDetectZone(const SDL_Rect &rect) {
  if (rect.w < 0 || rect.h < 0)
    throw Error("Unavailable detect zone.");
  if (detect_zone_.empty()) {
    detect_zone_.push_back(rect);
  }
  else {
    std::list<SDL_Rect>::iterator searcher = detect_zone_.begin();
    bool same_in_list = false;
    while (searcher->x < rect.x) {
      ++searcher;
      if (searcher == detect_zone_.end()) break;
    }
    if (searcher != detect_zone_.end()) {
      if (searcher->x == rect.x) {
        while (searcher->y < rect.y) {
          ++searcher;
          if (searcher == detect_zone_.end()) break;
          if (searcher->x != rect.x) break;
        }
        if (searcher != detect_zone_.end()) {
          while (searcher->y == rect.y && searcher->x == rect.x) {
            if (searcher->w == rect.w && searcher->h == rect.h) {
              same_in_list = true;
              break;
            }
            ++searcher;
          }
        }
      }
    }
    if (!same_in_list)
      detect_zone_.insert(searcher, rect);
  }
}

bool Object::IsInclude(int x, int y) {
  if (!detect_zone_.empty()) {
    std::list<SDL_Rect>::iterator searcher = detect_zone_.begin();
    while (searcher->x <= x) {
      if (searcher->y <= y) {
        int detect_width = searcher->w == 0 ? (styles_picture_.empty() ? 0 : styles_picture_[style_].picture_width()) : searcher -> w,            detect_height = searcher->h == 0 ? (styles_picture_.empty() ? 0 : styles_picture_[style_].picture_height()) : searcher -> h;
        if ((searcher->x + detect_width >= x) && (searcher->y + detect_height >= y)) return true;
      }else {
        int orginal_x = searcher->x;
        while (searcher->x == orginal_x)
          if (++searcher == detect_zone_.end()) break;
        --searcher;
      }
      ++searcher;
      if (searcher == detect_zone_.end()) break;
    }
  }
  return false;
}
