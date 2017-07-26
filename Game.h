/*Please indicate the source
Software license:GPL v3
Author:Handsome_hell
E-mail:handsome0hell@gmail.com
*/
#ifndef NEW_GAME_H_
#define NEW_GAME_H_
#include "StdHead.h"
#include "Window.h"

/*A list of picture*/
class DialogBox
{
 public:
  DialogBox() {
    pictures_height_ = 0;
    Resize(5);
  }

  /*Max number of picture that dialog box have, 0 means infinity
  */
  void Resize(int32_t size) {
    if (size < 0)
      throw Error("Unavailable pictures size.");
    pictures_size_ = size;
    if (pictures_size_ < pictures_.size() && pictures_size_ != 0) {
      std::list<Picture>::iterator eraser = pictures_.begin();
      pictures_height_ = 0;
      while (size--)
        pictures_height_ += (*(eraser++)).picture_height();
      pictures_.erase( eraser, pictures_.end());
    }
  }

  /*Add a picture to the list, true to add it to the button of the list
  */
  void add_picture(Picture&, bool = true);

  /*Return height of all pictures in the spacing
  */
  int pictures_height(int spacing) {return pictures_height_ + ( pictures_.size() - 1 ) * spacing;}

  /*Copy all pictures to the screen,
  x of pictures, y of the first picture, space between each picture, start height of list, end height of list
  */
  void CopyTo(int, int, int, int, int);

 private:
  Uint16 pictures_size_;/*Max number of pictures*/
  int pictures_height_;/*Height of all pictures*/
  std::list<Picture> pictures_;/*Pictures in the list*/
  DialogBox & operator=(DialogBox &) = delete;
};
#endif // NEW_GAME_H_
