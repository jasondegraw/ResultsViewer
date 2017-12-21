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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TimeSeries.hpp"

TEST_CASE("Basic TimeSeries Tests", "[timeseries]")
{
  QDateTime start(QDate(2017, 1, 1));
  std::vector<long long> seconds{ {3600, 7200, 10800, 14400, 18000} };
  std::vector<double> values{ {10, 20, 30, 40, 50} };
  resultsviewer::TimeSeries ts(start, seconds, values);
  REQUIRE(ts.values.size() == 5);
  REQUIRE(ts.seconds.size() == 5);
  REQUIRE(!ts.interval.has_value());
  auto first = ts.firstReportDateTime();
  auto date = first.date();
  REQUIRE(date.day() == 1);
  REQUIRE(date.month() == 1);
  REQUIRE(date.year() == 2017);
  auto time = first.time();
  REQUIRE(time.hour() == 1);
  REQUIRE(time.minute() == 0);
  REQUIRE(ts.minimum() == 10);
  REQUIRE(ts.maximum() == 50);
  REQUIRE(ts.sum() == 150);
  REQUIRE(ts.variance() == 200.0);
  std::vector<double> days = ts.daysFromFirstReport();
  REQUIRE(days.size() == 5);
  REQUIRE(days[0] == 0.0);
  REQUIRE(days[1] * 24.0 == 1);
  REQUIRE(days[2] * 24.0 == 2);
  REQUIRE(days[3] * 24.0 == 3);
  REQUIRE(days[4] * 24.0 == 4);
}

