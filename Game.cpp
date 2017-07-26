/*Please indicate the source
Software license:GPL v3
Author:Handsome_hell
E-mail:handsome0hell@gmail.com
*/
#include "Game.h"

void DialogBox::CopyTo(int x, int y, int spacing, int start_height, int end_height) {
  if (start_height > end_height || start_height > pictures_height(spacing))
    throw Error("Unavailable dialog box height.");
  if (start_height == end_height || start_height == pictures_height(spacing)) return;
  std::list<Picture>::iterator reader = pictures_.begin();
  int add_y = 0;
  bool is_bigger_than_height = false;
  while(reader != pictures_.end()) {
    if (add_y + (*reader).picture_height() + spacing <= start_height) {
      /*The pictures that don't need to be copy
      */
      add_y += (*reader).picture_height() + spacing;
      ++reader;
      if(add_y == start_height) break;
    }else {
      if(add_y + (*reader).picture_height() <= start_height) {
        /*Start in an empty space
        */
        add_y = start_height;
        break;
      }else {
        if (add_y + (*reader).picture_height() <= end_height) {
          (*reader).set_part_of_picture({0, start_height - add_y, 0, 0});
          (*reader).CopyTo({x, y, 0, 0});
          add_y += (*reader).picture_height() + spacing;
          break;
        }else {
          (*reader).set_part_of_picture({0, start_height - add_y, 0, end_height - start_height});
          (*reader).CopyTo({x, y, 0, 0});
          is_bigger_than_height = true;
          break;
        }
      }
    }
  }
  if(reader != pictures_.end())
    ++reader;
  while(reader != pictures_.end() && !is_bigger_than_height) {
    if(add_y + (*reader).picture_height() <= end_height) {
      (*reader).set_part_of_picture({0, 0, 0, 0});
    }else {
      (*reader).set_part_of_picture({0, 0, 0, end_height - add_y});
      is_bigger_than_height = true;
    }
    (*(reader++)).CopyTo({x, y + add_y - start_height, 0, 0});
    add_y += (*reader).picture_height() + spacing;
  }
}

void DialogBox::add_picture(Picture &picture, bool is_button) {
  if (pictures_.size() == pictures_size_ && pictures_size_ != 0) {
    if (is_button) {
      pictures_height_ -= pictures_.front().picture_height();
      pictures_.pop_front();
    }else {
      pictures_height_ -= pictures_.back().picture_height();
      pictures_.pop_back();
    }
  }
  if (is_button) {
    pictures_.push_back(Picture());
    pictures_.back() = picture;
    pictures_height_ += pictures_.back().picture_height();
  }else {
    pictures_.push_front(Picture());
    pictures_.front() = picture;
    pictures_height_ += pictures_.front().picture_height();
  }
}
