/*Please indicate the source
Software license:GPL v3
Author:Handsome_hell
E-mail:handsome0hell@gmail.com
*/
#ifndef NEW_WINDOW_H_
#define NEW_WINDOW_H_
#include "StdHead.h"
#include "Error.h"

/*Create a window to display something
*/
class Window {
 public:
  Window() {
    target_window_ = nullptr;
    target_renderer_ = nullptr;
  }

  ~Window() {clear();}

  /*Create a window, and create a renderer for it
  */
  void NewWindow(const std::string &title, int x, int y, int w, int h, Uint32 window_flag, Uint32 renderer_flag) {
    clear();
    if ((target_window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, window_flag)) == nullptr) {
      throw Error("Unable to create window.") + SDL_GetError();
    }else {
      if ((target_renderer_ = SDL_CreateRenderer(target_window_, -1, renderer_flag)) == nullptr)
        throw Error("Unable to create renderer.") + SDL_GetError();
    }
  }

  /*Clear the window to copy pictures to it
  */
  Window& ReadyToRender() {
    SDL_RenderClear(target_renderer_);
    return *this;
  }

  /*Display everything copy to the window
  */
  Window& Display() {
    SDL_RenderPresent(target_renderer_);
    return *this;
  }

  /*Delete he window and the renderer to ready for next window
  */
  void clear() {
    if (target_window_ != nullptr)
      SDL_DestroyWindow(target_window_);
    if (target_renderer_ != nullptr)
      SDL_DestroyRenderer(target_renderer_);
    target_window_ = nullptr;
    target_renderer_ = nullptr;
  }

 private:
  SDL_Window *target_window_;/*window, nullptr when there is no window*/
  friend class Picture;/*To let picture get the renderer of window*/
  SDL_Renderer *target_renderer_;/*renderer of window, nullptr when there is no window*/
  Window& operator=(Window&) = delete;
};

class Font
{
 public:
  Font() {font_ = nullptr;}

  ~Font() {clear();}

  /*Open a font with the given size
  */
  void open(std::string path, int size) {
    if (size <= 0)
      throw Error("Unavailable font size.");
    clear();
    if ((font_ = TTF_OpenFont(path.c_str(), size)) == nullptr)
      throw Error("Unable to open font.") + SDL_GetError();
  }

  /*Close the font to ready for next font
  */
  void clear() {
    if (font_ != nullptr)
      TTF_CloseFont(font_);
    font_ = nullptr;
  }

 private:
  friend class Picture;/*To create a picture by text with the font*/
  TTF_Font *font_;/*Font, nullptr when there is no font*/
  Font& operator=(Font&) = delete;
};

/*A picture that can display on window
*/
class Picture
{
 public:
  Picture() {
    picture_ = nullptr;
    renderer_ = nullptr;
    is_visible_ = true;
    source_picture_ = nullptr, derivative_picture_ = nullptr;
    picture_height_ = picture_width_ = 0;
    part_of_picture_ = {0, 0, 0, 0} ;
  }

  ~Picture() {clear();}

  /*Copy picture, when one of these pictures call clear(), picture will not be destroyed to make another picture can use it
  */
  Picture& operator=(Picture &source) {
    source_picture_ = &source;
    source.derivative_picture_ = this;
    picture_ = source.picture_, renderer_ = source.renderer_;
    part_of_picture_ = source.part_of_picture_;
    is_visible_ = source.is_visible_;
    picture_width_ = source.picture_width_, picture_height_ = source.picture_height_;
    return *this;
  }

  /*Create picture from a PNG file
  */
  void Create(const std::string&);

  void set_color_mod(int r, int g, int b) {SDL_SetTextureColorMod(picture_, r, g, b);}

  /*Create picture from text
  text, rending mode of text, text color, font, background color of text
  Mode:0:Blended, 1:Shaded, 2:Solid*/
  void Create(const std::string&, int, const SDL_Color&, const Font&, const SDL_Color& = {0, 0, 0, 255});

  /*Create a rectangular picture with given color and size
  rect_color, width, height*/
  void Create(const SDL_Color&, int, int);

  /*Set the window that the picture copy to
  */
  void set_renderer(const Window& target_window) {renderer_ = target_window.target_renderer_;}

  /*Set the part of picture when copy
  */
  void set_part_of_picture(const SDL_Rect &part) {part_of_picture_ = part;}

  /*Copy picture to the given zone on the window
  */
  void CopyTo(const SDL_Rect &target) {
    if (!is_visible_ || picture_ == nullptr)
      return;
    SDL_Rect source = part_of_picture_, derivative = target;
    if (source.w <= 0)
      source.w = picture_width_ - source.x;
    if (source.h <= 0)
      source.h = picture_height_ - source.y;
    if (derivative.w <= 0)
      derivative.w = source.w;
    if (derivative.h <= 0)
      derivative.h = source.h;
    if (SDL_RenderCopy(renderer_, picture_, &source, &derivative) != 0)
      throw Error("Unable to copy texture.") + SDL_GetError();
  }

  void set_is_visible(bool is_visible) {is_visible_ = is_visible;}

  /*Clear thing to ready for next picture
  */
  void clear() {
    if (source_picture_ == nullptr && derivative_picture_ == nullptr) {
      if (picture_ != nullptr)
        SDL_DestroyTexture(picture_);
    }else {
      if (source_picture_ != nullptr)
        source_picture_->derivative_picture_ = (derivative_picture_ == source_picture_ ? nullptr : derivative_picture_);
      if (derivative_picture_ != nullptr)
        derivative_picture_->source_picture_ = (source_picture_ == derivative_picture_ ? nullptr : source_picture_);
    }
    source_picture_ = nullptr, derivative_picture_ = nullptr;
    picture_ = nullptr;
    picture_height_ = picture_width_ = 0;
    part_of_picture_ = {0, 0, 0, 0} ;

  }

  int picture_width() {return picture_width_;}

  int picture_height() {return picture_height_;}

 private:
  int picture_width_;
  int picture_height_;
  Picture *source_picture_, *derivative_picture_;/*nullptr if only one picture use the texture*/
  SDL_Texture *picture_;/*Picture*/
  SDL_Renderer *renderer_;/*Renderer of target window*/
  SDL_Rect part_of_picture_;
  bool is_visible_;/*Copy picture or not*/
};

/*Some pictures that have detect zone
*/
class Object {
 public:
  Object() {style_ = -1;}

  /*set style of Object
  */
  void set_style(int style) {
    if (style < 0 || static_cast<unsigned int>(style) >= styles_picture_.size())
      throw Error("Unavailable object style!");
    style_ = style;
  }

  /*Return the picture of the style to edit it
  */
  Picture& styles_picture() {
    if (style_ == -1)
      throw Error("Object have no style!");
    return styles_picture_[style_];
  }

  /*Set the number of style that Object have
  */
  void resize_styles(int size) {
    try {
      styles_picture_.resize(size);
    }catch(...) {
      throw Error("Unable to resize object styles.");
    }
    style_ = (size == 0 ? -1 : 0);
  }

  /*Add a zone to the detect zone of object*/
  void AddToDetectZone(const SDL_Rect &);

  void ClearDetectZone() {detect_zone_.clear();}

  /*Check if a point is in the detect zone of object
  */
  bool IsInclude(int, int);

 private:
  std::list<SDL_Rect> detect_zone_;/*Detect zone*/
  std::vector<Picture> styles_picture_;/*Pictures of styles*/
  int style_;/*Now object's style*/
  Object & operator=(Object &) = delete;
};

/*To measure the time
*/
class Timer
{
 public:
  Timer() {start();}

  Timer & operator=(Timer &timer) {
    start_time_ = timer.start_time_;
    return *this;
  }

  /*Start timer
  */
  void start() {start_time_ = SDL_GetTicks();}

  /*Return how long the time between start and call this function
  */
  Uint32 time() {return SDL_GetTicks() - start_time_;}

 private:
  Uint32 start_time_;/*Save the time when timer start*/
};
#endif // NEW_WINDOW_H_
