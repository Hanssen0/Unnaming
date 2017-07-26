/*Please indicate the source
Software license:GPL v3
Author:Handsome_hell
E-mail:handsome0hell@gmail.com
*/
#ifndef NEW_ERROR_H_
#define NEW_ERROR_H_
#include "StdHead.h"

/*Save the error type
*/
class Error {
 public:
   /*Return the error type
   */
  const std::string &error_type() const {return error_type_;}

  /*Add a string after error type
  */
  Error& operator+(const std::string &extern_string) {
    error_type_ += extern_string;
    return *this;
  }

  Error(const std::string &type) {error_type_ = type;}

 private:
  std::string error_type_;/*A string that describe type of error*/
  Error & operator=(Error&) = delete;
};

#endif // NEW_ERROR_H_
