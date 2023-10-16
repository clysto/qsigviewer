//
// Created by maoyachen on 2023/10/16.
//

#ifndef QSIGVIEWER_UTILS_H
#define QSIGVIEWER_UTILS_H

#include <QString>

class Utils {
 public:
  static QString humanReadableTime(int sampleIndex, int sampleRate);
};

#endif  // QSIGVIEWER_UTILS_H
