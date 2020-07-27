/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Class for performances analysis
//

#pragma once

#include <iostream>
#include <chrono>

class ElapsedTime
{
  private:
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point end_;

  public:
  ElapsedTime()
  {
      start_ = std::chrono::steady_clock::now();
  }
  ~ElapsedTime()
  {
      end_ = std::chrono::steady_clock::now();
      std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count()<<std::endl;
  }
};