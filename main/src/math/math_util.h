//
// Created by Selali Adobor on 6/26/21.
//

#ifndef EROU_MAIN_MATH_UTIL_H_
#define EROU_MAIN_MATH_UTIL_H_
inline float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif //EROU_MAIN_MATH_UTIL_H_
