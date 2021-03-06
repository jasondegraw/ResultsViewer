/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "LinePlot.hpp"
#include <cfloat>
#include <qwt/qwt_painter.h>

#include <algorithm>

namespace resultsviewer{

TimeSeriesLinePlotData::TimeSeriesLinePlotData(TimeSeries timeSeries)
: m_timeSeries(timeSeries),
  m_minX(timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time())),
  m_maxX(timeSeries.daysFromFirstReport()[timeSeries.daysFromFirstReport().size()-1]+timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time())), // end day
  m_minY(timeSeries.minimum()),
  m_maxY(timeSeries.maximum()),
  m_size(timeSeries.values.size())
{
  m_boundingRect = QRectF(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
  m_minValue = m_minY;
  m_maxValue = m_maxY;
  m_units = QString::fromStdString(timeSeries.units);
  m_fracDaysOffset = 0.0;
  m_x = m_timeSeries.daysFromFirstReport();
  m_y = m_timeSeries.values;
}

TimeSeriesLinePlotData::TimeSeriesLinePlotData(TimeSeries timeSeries, double fracDaysOffset)
: m_timeSeries(timeSeries),
  m_minX(timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time())),
  m_maxX(timeSeries.daysFromFirstReport()[timeSeries.daysFromFirstReport().size()-1]+timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time())), // end day
  m_minY(timeSeries.minimum()),
  m_maxY(timeSeries.maximum()),
  m_size(timeSeries.values.size())
{
  m_boundingRect = QRectF(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
  m_minValue = m_minY;
  m_maxValue = m_maxY;
  m_units = QString::fromStdString(timeSeries.units);
  m_fracDaysOffset = fracDaysOffset; // note updating in xValue does not affect scaled axis
  m_x = m_timeSeries.daysFromFirstReport();
  m_y = m_timeSeries.values;
}

TimeSeriesLinePlotData::~TimeSeriesLinePlotData()
{

}

TimeSeriesLinePlotData* TimeSeriesLinePlotData::copy() const
{
  return (new TimeSeriesLinePlotData(m_timeSeries, m_fracDaysOffset));
}

double TimeSeriesLinePlotData::x(size_t pos) const
{
  return m_x[pos] + m_fracDaysOffset + m_minX; // hourly
}

double TimeSeriesLinePlotData::y(size_t pos) const
{
  return m_y[pos];
}

/// units for plotting on axes or scaling
void TimeSeriesLinePlotData::units(const std::string &unit) { m_units = QString::fromStdString(unit); }

/// units for plotting on axes or scaling
QString TimeSeriesLinePlotData::units() const { return m_units; }

/// minX
double TimeSeriesLinePlotData::minX() const { return m_minX; }

/// maxX
double TimeSeriesLinePlotData::maxX() const { return m_maxX; }

/// minY
double TimeSeriesLinePlotData::minY() const { return m_minY; }

/// maxY
double TimeSeriesLinePlotData::maxY() const { return m_maxY; }

/// minValue
double TimeSeriesLinePlotData::minValue() const { return m_minValue; }

/// maxValue
double TimeSeriesLinePlotData::maxValue() const { return m_maxValue; }

/// sumValue
double TimeSeriesLinePlotData::sumValue() const
{
  return m_timeSeries.sum();
}

/// meanValue
double TimeSeriesLinePlotData::meanValue() const
{
  return m_timeSeries.mean();
}

/// stdDevValue
double TimeSeriesLinePlotData::stdDevValue() const
{
  return m_timeSeries.stdev();
}

/// reimplement bounding rect for speed
QRectF TimeSeriesLinePlotData::boundingRect() const 
{ 
  return m_boundingRect; 
}

/// reimplement sample
QPointF TimeSeriesLinePlotData::sample(size_t i) const
{ 
  return QPointF(x(i), y(i)); 
}

/// reimplement abstract function size
size_t TimeSeriesLinePlotData::size(void) const
{ 
  return m_size; 
}

VectorLinePlotData::VectorLinePlotData(const std::vector<double>& xVector,
                                       const std::vector<double>& yVector)
: m_xVector(xVector),
  m_yVector(yVector),
  m_interpMethod(InterpMethod::NearestInterp)
{
  init();
}

VectorLinePlotData* VectorLinePlotData::copy() const
{
  auto result = new VectorLinePlotData(m_xVector, m_yVector);
  result->interpMethod(m_interpMethod);
  return result;
}

/// set the interp method, defaults to Nearest
void VectorLinePlotData::interpMethod(InterpMethod interpMethod)
{
  m_interpMethod = interpMethod;
}

/// minX
double VectorLinePlotData::minX() const
{
  return m_minX;
}

/// maxX
double VectorLinePlotData::maxX() const
{
  return m_maxX;
}

/// minY
double VectorLinePlotData::minY() const
{
  return m_minY;
}

/// maxY
double VectorLinePlotData::maxY() const
{
  return m_maxY;
}

/// minValue
double VectorLinePlotData::minValue() const
{
  // Dangerous?
  return *std::min_element(std::begin(m_yVector), std::end(m_yVector));
}

/// maxValue
double VectorLinePlotData::maxValue() const
{
  // Dangerous?
  return *std::max_element(std::begin(m_yVector), std::end(m_yVector));
}

/// sumValue
double VectorLinePlotData::sumValue() const
{
  return std::accumulate(std::begin(m_yVector), std::end(m_yVector), 0.0);
}

/// meanValue
double VectorLinePlotData::meanValue() const
{
  return std::accumulate(std::begin(m_yVector), std::end(m_yVector), 0.0)/(double)m_yVector.size();
}

/// stdDevValue
double VectorLinePlotData::stdDevValue() const
{
  return 0;
}

QRectF VectorLinePlotData::boundingRect() const
{ 
  return m_boundingRect; 
}

QPointF VectorLinePlotData::sample(size_t i) const
{ 
  return QPointF(x(i), y(i)); 
}

size_t VectorLinePlotData::size(void) const
{ 
  return m_size; 
}

// set ranges and bounding box
void VectorLinePlotData::init(){

  unsigned M = m_xVector.size();
  unsigned N = m_yVector.size();

  if ((M <= 1) || (N <= 1) ||  (M != N)){
    throw std::runtime_error("Incorrectly sized matrix or vector for VectorLinePlotData");
  }

  m_size = N;
  m_minX = *std::min_element(std::begin(m_xVector), std::end(m_xVector));
  m_maxX = *std::max_element(std::begin(m_xVector), std::end(m_xVector));
  m_minY = *std::min_element(std::begin(m_yVector), std::end(m_yVector));
  m_maxY = *std::max_element(std::begin(m_yVector), std::end(m_yVector));

  // set the bounding box
  m_boundingRect = QRectF(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
}

 double VectorLinePlotData::x(size_t pos) const
{
  return m_xVector[pos];
}

double VectorLinePlotData::y(size_t pos) const
{
  return m_yVector[pos];
}

/// units for plotting on axes or scaling
void VectorLinePlotData::units(const std::string &unit) { m_units = QString::fromStdString(unit); }

/// units for plotting on axes or scaling
QString VectorLinePlotData::units() const { return m_units; };

} // resultsviewer
