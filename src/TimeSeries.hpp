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

#ifndef RESULTSVIEWER_TIMESERIES_HPP
#define RESULTSVIEWER_TIMESERIES_HPP

#include <string>
#include <vector>

#include <QDateTime>

namespace resultsviewer{

static std::vector<long long> fixSeconds(std::vector <long long> seconds, std::vector<double> values)
{
  auto len = std::min(seconds.size(), values.size());
  if(len == 0) {
    return { 0 };
  }
  return std::vector<long long>(seconds.begin(), seconds.begin() + len);
}

static std::vector<double> fixValues(std::vector <long long> seconds, std::vector<double> values)
{
  auto len = std::min(seconds.size(), values.size());
  if(len == 0) {
    return { 0 };
  }
  return std::vector<double>(values.begin(), values.begin() + len);
}

/**
TimeSeries is an object that connects a series of times to a series of values.
*/
struct TimeSeries
{
  TimeSeries(QDateTime start, std::vector <long long> seconds, std::vector<double> values) : startDateTime(start), seconds(fixSeconds(seconds, values)), 
    values(fixValues(seconds, values))
  {}

  TimeSeries(QDateTime start, std::vector <long long> seconds, std::vector<double> values, QString const units) : startDateTime(start), seconds(fixSeconds(seconds, values)),
    values(fixValues(seconds, values))
  {}

  QDateTime firstReport() const
  {
    return startDateTime.addSecs(seconds[0]);
  }

  const QDateTime startDateTime;
  const std::vector<long long> seconds;
  const std::vector<double> values;
  const QString units;
};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_TIMESERIES_HPP
