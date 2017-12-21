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
#include <algorithm>
#include <optional>
#include <iostream>

#include <QDateTime>

namespace resultsviewer{

static std::vector<long long> fixSeconds(std::vector <long long> seconds, std::vector<double> values)
{
  auto len = std::min(seconds.size(), values.size());
  if (len == 0) {
    return { 0 };
  }
  return std::vector<long long>(seconds.begin(), seconds.begin() + len);
}

static std::vector<double> fixValues(std::vector <long long> seconds, std::vector<double> values)
{
  auto len = std::min(seconds.size(), values.size());
  if (len == 0) {
    return { 0 };
  }
  return std::vector<double>(values.begin(), values.begin() + len);
}

static std::vector<long long> buildSeconds(long long interval, std::vector<double> values)
{
  std::vector<long long> result(values.size());
  result[0] = interval;
  for (int i = 1; i < values.size() - 1; i++) {
    result[i] = result[i - 1] + interval;
  }
  return result;
}

/**
TimeSeries is an object that connects a series of times to a series of values.
*/
struct TimeSeries
{
  TimeSeries(const QDateTime start, long long interval, std::vector<double> vals) : startDateTime(start),
    seconds(buildSeconds(interval, values)), values(values), interval(interval)
  {}

  TimeSeries(const QDateTime start, long long interval, std::vector<double> vals, const std::string units) : startDateTime(start),
    seconds(buildSeconds(interval, values)), values(values), units(units), interval(interval)
  {}

  TimeSeries(const QDateTime start, std::vector <long long> seconds, std::vector<double> values) : startDateTime(start), seconds(fixSeconds(seconds, values)),
    values(fixValues(seconds, values))
  {}

  TimeSeries(const QDateTime start, std::vector <long long> seconds, std::vector<double> values, const std::string units) : startDateTime(start), seconds(fixSeconds(seconds, values)),
    values(fixValues(seconds, values)), units(units)
  {}

  QDateTime firstReportDateTime() const
  {
    return startDateTime.addSecs(seconds[0]);
  }

  std::vector<double> daysFromFirstReport() const
  {
    std::vector<double> result(seconds.size());
    result[0] = 0.0;
    double rval = 1.0 / static_cast<double>(86400);
    for (int i = 1; i < seconds.size(); i++) {
      result[i] = rval*static_cast<double>(seconds[i] - seconds[0]);
    }
    return result;
  }

  double minimum() const
  {
    return *std::min_element(std::begin(values), std::end(values));
  }

  double maximum() const
  {
    return *std::max_element(std::begin(values), std::end(values));
  }

  double sum() const
  {
    return std::accumulate(std::begin(values), std::end(values), 0.0);
  }

  double variance() const
  {
    if (values.size() == 1) {
      return 0.0;
    }
    auto n = values.size();
    double m = values[0];
    double s = 0;
    unsigned k;
    for (k = 1; k < n; k++) {
      double temp = values[k] - m;
      m += temp / static_cast<double>(k + 1);
      s += temp*(values[k] - m);
    }
    return s / static_cast<double>(n);
  }

  double stdev() const
  {
    if (values.size() == 1) {
      return 0.0;
    }
    auto n = values.size();
    double m = values[0];
    double s = 0;
    unsigned k;
    for (k = 1; k < n; k++) {
      double temp = values[k] - m;
      m += temp / static_cast<double>(k + 1);
      s += temp*(values[k] - m);
    }
    return std::sqrt(s / static_cast<double>(n));
  }

  double mean() const
  {
    double m = 0;
    for (auto el : values) {
      m += el;
    }
    return m / static_cast<double>(values.size());
  }

  const QDateTime startDateTime;
  const std::vector<long long> seconds;
  const std::vector<double> values;
  const std::string units;
  const std::optional<long long> interval;
};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_TIMESERIES_HPP
