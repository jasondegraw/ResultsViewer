/***********************************************************************************************************************
 *  Copyright (c) 2017, Jason W. DeGraw. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef RESULTSVIEWER_UTILITIES_HPP
#define RESULTSVIEWER_UTILITIES_HPP

#include <vector>

namespace resultsviewer{

enum class InterpMethod { LinearInterp, NearestInterp, HoldLastInterp, HoldNextInterp };

template <typename T> std::vector<T> intervalspace(const T start, const int count, const T interval)
{
  std::vector<T> result{ {start} };
  for (int i = 1; i < count; i++) {
    result.push_back(result[i - 1] + interval);
  }
  return result;
}

template <typename T> std::vector<T> linspace(const T start, const T end, const int count=100)
{
  int n = count - 1;
  T interval = (end - start) / static_cast<T>(n);
  std::vector<T> result{ { start } };
  T last = start;
  for (int i = 1; i < n; i++) {
    last += interval;
    result.push_back(last);
  }
  result.push_back(end);
  return result;
}

template <typename T> double totalDays(const T &time)
{
  static double daysPerSecond = 1.0 / 86400.0;
  return daysPerSecond*(3600 * time.hour() + 60 * time.minute() + time.second());
}

template <typename T> T fromFracDays(const double days)
{
    static double msPerDay = 8.64e7;
    return T::fromMSecsSinceStartOfDay(static_cast<int>(std::round(msPerDay*days)));
}

}; // resultsviewer namespace

#endif // RESULTSVIEWER_TIMESERIES_HPP
