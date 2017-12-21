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

#include "FloodPlot.hpp"
#include "Utilities.hpp"

#include <algorithm>

namespace resultsviewer{

FloodPlotColorMap::FloodPlotColorMap(const std::vector<double>& colorLevels, FloodPlotColorMap::ColorMapList colorMapList)
  : QwtLinearColorMap(),
    m_colorLevels(colorLevels),
    m_colorMapList(colorMapList)
{
  setMode(QwtLinearColorMap::FixedColors); // no interpolation - can set in constructor
  init();
}

FloodPlotColorMap::~FloodPlotColorMap()
{
}

void FloodPlotColorMap::init()
{
  /// set color stops
  /// scale between 0 and 1
  unsigned int colormapLength = m_colorLevels.size();  // start and end colors
  int r, g, b;
  unsigned int i,j;
  QColor minColor, maxColor; // colors at interval ends
  auto minel = std::min_element(m_colorLevels.begin(), m_colorLevels.end());
  auto maxel = std::max_element(m_colorLevels.begin(), m_colorLevels.end());
  if(minel == std::end(m_colorLevels) || maxel == std::end(m_colorLevels)) {
    return;
  }
  double min = *minel;
  double max = *maxel;
  if (max == min)
  {
    return;
  }
  switch (m_colorMapList)
  {
    case Gray:
      minColor = QColor(0,0,0);
      maxColor = QColor(255,255,255);
      setColorInterval(minColor, maxColor); // end points
      for(i = 0; i < colormapLength; i++) {
        double gray = 1.0 * i / (colormapLength - 1);
        r = (int)(255 * gray);
        g = (int)(255 * gray);
        b = (int)(255 * gray);
        addColorStop((m_colorLevels[i] - min) / (max - min), QColor(r, g, b));
      }
      break;
    case Jet:
      Matrix cMatrix(colormapLength,3);
      unsigned int n = (int)ceil(colormapLength / 4.0);
      int nMod = 0;
      std::vector<double> fArray(3 * n - 1);
      std::vector<double> red(fArray.size());
      std::vector<double> green(fArray.size());
      std::vector<double> blue(fArray.size());


      for(i = 0; i < colormapLength; i++) {
        cMatrix(i, 0) = 0;
        cMatrix(i, 1) = 0;
        cMatrix(i, 2) = 0;
      }


      if(colormapLength % 4 == 1) {
        nMod = 1;
      }

      for(i = 0; i < fArray.size(); i++) {
        if(i < n)
          fArray[i] = (float)(i + 1) / n;
        else if(i >= n && i < 2 * n - 1)
          fArray[i] = 1.0;
        else if(i >= 2 * n - 1)
          fArray[i] = (float)(3 * n - 1 - i) / n;
        green[i] = (int)ceil(n / 2.0) - nMod + i;
        red[i] = green[i] + n;
        blue[i] = green[i] - n;
      }

      int nb = 0;
      for(i = 0; i < blue.size(); i++) {
        if(blue[i] > 0)
          nb++;
      }

      for(i = 0; i < colormapLength; i++) {
        for(j = 0; j < red.size(); j++) {
          if(i == red[j] && red[j] < colormapLength) {
            cMatrix(i, 0) = fArray[i - red[0]];
          }
        }
        for(j = 0; j < green.size(); j++) {
          if(i == green[j] && green[j] < colormapLength)
            cMatrix(i, 1) = fArray[i - (int)green[0]];
        }
        for(j = 0; j < blue.size(); j++) {
          if(i == blue[j] && blue[j] >= 0)
            cMatrix(i, 2) = fArray[fArray.size() - 1 - nb + i];
        }
      }

      // set before adding color stops
      // default from http://www.matthiaspospiech.de/blog/2008/06/16/qwt-spectrogramm-plot-with-data-arrays/
      minColor = QColor(0,0,189);
      maxColor = QColor(132,0,0);
      int bMin=256;
      int rMin=256;
            for (i = 0; i < colormapLength; i++)
            {
        r = (int)(cMatrix(i, 0) * 255);
        g = (int)(cMatrix(i, 1) * 255);
        b = (int)(cMatrix(i, 2) * 255);
        if ((r==0) && (g==0) && (b<bMin))
        {
          bMin=b;
          minColor = QColor(r,g,b);
        }
        if ((b==0) && (g==0) && (r<rMin))
        {
          rMin=r;
          maxColor = QColor(r,g,b);
        }
            }
      setColorInterval(minColor, maxColor); // end points


            for (i = 0; i < colormapLength; i++)
            {
        r = (int)(cMatrix(i, 0) * 255);
        g = (int)(cMatrix(i, 1) * 255);
        b = (int)(cMatrix(i, 2) * 255);
        addColorStop( (m_colorLevels[i] - min) / (max - min), QColor(r, g, b));
            }
      break;
  }

}

FloodPlotData::FloodPlotData() : QwtRasterData()
{

}

FloodPlotData::~FloodPlotData()
{

}

TimeSeriesFloodPlotData::TimeSeriesFloodPlotData(TimeSeries timeSeries)
: FloodPlotData(),
  m_timeSeries(timeSeries),
  m_minValue(timeSeries.minimum()),
  m_maxValue(timeSeries.maximum()),
  m_minX(timeSeries.firstReportDateTime().date().dayOfYear()),
  m_maxX(ceil(timeSeries.daysFromFirstReport()[timeSeries.daysFromFirstReport().size()-1]+timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time()))), // end day
  m_minY(0), // start hour
  m_maxY(24), // end hour
  m_startFractionalDay(timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time())),
  m_colorMapRange(QwtInterval(m_minValue, m_maxValue))
{
  if (m_colorMapRange.minValue() == m_colorMapRange.maxValue())
  {
    m_colorMapRange = QwtInterval(m_colorMapRange.minValue(), m_colorMapRange.minValue() + 1.0);
  }

  // data range
  setInterval(Qt::XAxis, QwtInterval(m_minX, m_maxX));
  setInterval(Qt::YAxis, QwtInterval(m_minY, m_maxY));
  setInterval(Qt::ZAxis, m_colorMapRange);
  m_units = timeSeries.units;
}

TimeSeriesFloodPlotData::TimeSeriesFloodPlotData(TimeSeries timeSeries,  QwtInterval colorMapRange)
: FloodPlotData(),
  m_timeSeries(timeSeries),
  m_minValue(timeSeries.minimum()),
  m_maxValue(timeSeries.maximum()),
  m_minX(timeSeries.firstReportDateTime().date().dayOfYear()),
  m_maxX(ceil(timeSeries.daysFromFirstReport()[timeSeries.daysFromFirstReport().size()-1]+timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time()))), // end day
  m_minY(0), // start hour
  m_maxY(24), // end hour
  m_startFractionalDay(timeSeries.firstReportDateTime().date().dayOfYear()+totalDays(timeSeries.firstReportDateTime().time())),
  m_colorMapRange(colorMapRange)
{
  if (m_colorMapRange.minValue() == m_colorMapRange.maxValue())
  {
    m_colorMapRange = QwtInterval(m_colorMapRange.minValue(), m_colorMapRange.minValue() + 1.0);
  }

  // data range
  setInterval(Qt::XAxis, QwtInterval(m_minX, m_maxX));
  setInterval(Qt::YAxis, QwtInterval(m_minY, m_maxY));
  setInterval(Qt::ZAxis, m_colorMapRange);
  m_units = timeSeries.units;
}

TimeSeriesFloodPlotData* TimeSeriesFloodPlotData::copy() const
{
  TimeSeriesFloodPlotData* result = new TimeSeriesFloodPlotData(m_timeSeries, m_colorMapRange);
  return result;
}

TimeSeriesFloodPlotData::~TimeSeriesFloodPlotData()
{

}

QwtInterval TimeSeriesFloodPlotData::range() const 
{ 
  return m_colorMapRange; 
}

QRectF TimeSeriesFloodPlotData::boundingRect() const
{
  return QRectF(m_minX, m_minY, m_maxX-m_minX, m_maxY-m_minY);
}


QRectF TimeSeriesFloodPlotData::pixelHint(const QRectF& area) const
{

  double dx = 1.0; // one day
  double dy = 1.0 / 24.0; // default hourly
  std::optional<long long> intervalsecs = m_timeSeries.interval;
  if (intervalsecs){
    dy = intervalsecs.value() / 86400.0;
  }
  QRectF rect(m_minX, m_minY, dx, dy);

  return rect;
}

double TimeSeriesFloodPlotData::value(double fractionalDay, double hourOfDay) const
{
  // DLM: we are flooring the day because we want to plot day vs hour in flood plot
  double fracDays = floor(fractionalDay) + hourOfDay/24.0;
  return m_timeSeries.value(fracDays-m_startFractionalDay);
}

/// minX
double TimeSeriesFloodPlotData::minX() const { return m_minX; };

/// maxX
double TimeSeriesFloodPlotData::maxX() const { return m_maxX; };

/// minY
double TimeSeriesFloodPlotData::minY() const { return m_minY; };

/// maxY
double TimeSeriesFloodPlotData::maxY() const { return m_maxY; };

/// minValue
double TimeSeriesFloodPlotData::minValue() const { return m_minValue; };

/// maxValue
double TimeSeriesFloodPlotData::maxValue() const { return m_maxValue; };

/// minValue setter
void TimeSeriesFloodPlotData::minValue(double min) { m_minValue = min; };

/// maxValue setter
void TimeSeriesFloodPlotData::maxValue(double max) { m_maxValue = max; };

/// sumValue
double TimeSeriesFloodPlotData::sumValue() const
{
  return m_timeSeries.sum();
}

/// meanValue
double TimeSeriesFloodPlotData::meanValue() const
{
  return m_timeSeries.sum()/static_cast<double>(m_timeSeries.values.size());
}

/// stdDevValue
double TimeSeriesFloodPlotData::stdDevValue() const
{
  return m_timeSeries.stdev();
}

void TimeSeriesFloodPlotData::colorMapRange(QwtInterval colorMapRange) 
{ 
  m_colorMapRange = colorMapRange; 
}

QwtInterval TimeSeriesFloodPlotData::colorMapRange() 
{ 
  return m_colorMapRange; 
}

std::string TimeSeriesFloodPlotData::units() const 
{ 
  return m_units; 
};

MatrixFloodPlotData::MatrixFloodPlotData(const Matrix& matrix)
: FloodPlotData(),
  m_xVector(linspace(0.0, static_cast<double>(matrix.size1()-1), matrix.size1())),
  m_yVector(linspace(0.0, static_cast<double>(matrix.size2()-1), matrix.size2())),
  m_matrix(matrix),
  m_interpMethod(InterpMethod::NearestInterp)
{
  init();
}

MatrixFloodPlotData::MatrixFloodPlotData(const Matrix& matrix,  QwtInterval colorMapRange)
: FloodPlotData(),
  m_xVector(linspace(0.0, static_cast<double>(matrix.size1()-1), matrix.size1())),
  m_yVector(linspace(0.0, static_cast<double>(matrix.size2()-1), matrix.size2())),
  m_matrix(matrix),
  m_interpMethod(InterpMethod::NearestInterp)
{
  init();
  m_colorMapRange = colorMapRange;
}



MatrixFloodPlotData::MatrixFloodPlotData(const std::vector<double>& xVector,
                                         const std::vector<double>& yVector,
                                         const Matrix& matrix)
: FloodPlotData(),
  m_xVector(xVector),
  m_yVector(yVector),
  m_matrix(matrix),
  m_interpMethod(InterpMethod::NearestInterp)
{
  init();
}


MatrixFloodPlotData::MatrixFloodPlotData(const std::vector<double>& xVector,
                                         const std::vector<double>& yVector,
                                         const Matrix& matrix,
                                         const InterpMethod interp)
: FloodPlotData(),
  m_xVector(xVector),
  m_yVector(yVector),
  m_matrix(matrix),
  m_interpMethod(interp)
{
  init();
}

MatrixFloodPlotData::MatrixFloodPlotData(const std::vector<double>& xVector,
                                         const std::vector<double>& yVector,
                                         const std::vector<double>& matrix)
: FloodPlotData(),
  m_xVector(xVector.size()),
  m_yVector(yVector.size()),
  m_matrix(xVector.size(),yVector.size()),
  m_interpMethod(InterpMethod::NearestInterp)
{
  std::copy(xVector.begin(),xVector.end(),m_xVector.begin());
  std::copy(yVector.begin(),yVector.end(),m_yVector.begin());
  for (unsigned j=0;j<yVector.size();j++)
    for (unsigned i=0;i<xVector.size();i++)
      m_matrix(i,j)=matrix[j*xVector.size()+i];
  init();
}


MatrixFloodPlotData::MatrixFloodPlotData(const std::vector<double>& xVector,
                                         const std::vector<double>& yVector,
                                         const std::vector<double>& matrix,
                                         const InterpMethod interp)
: FloodPlotData(), 
  m_xVector(xVector.size()),
  m_yVector(yVector.size()),
  m_matrix(xVector.size(),yVector.size()),
  m_interpMethod(interp)
{
  std::copy(xVector.begin(),xVector.end(),m_xVector.begin());
  std::copy(yVector.begin(),yVector.end(),m_yVector.begin());
  for (unsigned j=0;j<yVector.size();j++)
    for (unsigned i=0;i<xVector.size();i++)
      m_matrix(i,j)=matrix[j*xVector.size()+i];
  init();
}




MatrixFloodPlotData::MatrixFloodPlotData(const std::vector<double>& xVector,
                                         const std::vector<double>& yVector,
                                         const Matrix& matrix,
                                         QwtInterval colorMapRange)
: FloodPlotData(),
  m_xVector(xVector),
  m_yVector(yVector),
  m_matrix(matrix),
  m_interpMethod(InterpMethod::NearestInterp)
{
  init();
  m_colorMapRange = colorMapRange;
}

MatrixFloodPlotData::~MatrixFloodPlotData()
{

}

MatrixFloodPlotData* MatrixFloodPlotData::copy() const
{
  auto result = new MatrixFloodPlotData(m_xVector, m_yVector, m_matrix, m_colorMapRange);
  result->interpMethod(m_interpMethod);
  return result;
}

QwtInterval MatrixFloodPlotData::range() const { return m_colorMapRange; }

QRectF MatrixFloodPlotData::pixelHint(const QRectF& area) const
{
  double dx = (m_maxX - m_minX) / double(m_matrix.size1() * 2.0);
  double dy = (m_maxY - m_minY) / double(m_matrix.size2() * 2.0);
  QRectF rect(m_minX, m_minY, dx, dy);

  return rect;
}

double MatrixFloodPlotData::value(double x, double y) const
{
  return interp(m_xVector, m_yVector, m_matrix, x, y, m_interpMethod, NearestExtrap);
}

/// set the interp method, defaults to Nearest
void MatrixFloodPlotData::interpMethod(InterpMethod interpMethod)
{
  m_interpMethod = interpMethod;
}

/// minX
double MatrixFloodPlotData::minX() const
{
  return m_minX;
}

/// maxX
double MatrixFloodPlotData::maxX() const
{
  return m_maxX;
}

/// minY
double MatrixFloodPlotData::minY() const
{
  return m_minY;
}

/// maxY
double MatrixFloodPlotData::maxY() const
{
  return m_maxY;
}

/// minValue
double MatrixFloodPlotData::minValue() const { return m_minValue; };

/// maxValue
double MatrixFloodPlotData::maxValue() const { return m_maxValue; };

/// minValue setter
void MatrixFloodPlotData::minValue(double min) { m_minValue = min; };

/// maxValue setter
void MatrixFloodPlotData::maxValue(double max) { m_maxValue = max; };

/// sumValue
double MatrixFloodPlotData::sumValue() const
{
  return std::accumulate(std::begin(m_matrix), std::end(m_matrix), 0.0);
}

/// meanValue
double MatrixFloodPlotData::meanValue() const
{
  return std::accumulate(std::begin(m_matrix), std::end(m_matrix), 0.0)/(double)m_matrix.size();
}

/// stdDevValue
double MatrixFloodPlotData::stdDevValue() const
{
  return 0;
}

/// range of values for which to show the colormap
void MatrixFloodPlotData::colorMapRange(QwtInterval colorMapRange) { m_colorMapRange = colorMapRange; }

/// range of values for which to show the colormap
QwtInterval MatrixFloodPlotData::colorMapRange() { return m_colorMapRange; }

/// units for plotting on axes or scaling
std::string MatrixFloodPlotData::units() const { return m_units; };

// set ranges and bounding box
void MatrixFloodPlotData::init(){

  unsigned M = m_matrix.size1();
  unsigned N = m_matrix.size2();

  if ((M <= 1) || (N <= 1) || (M != m_xVector.size()) || (N != m_yVector.size())){
    throw std::runtime_error("Incorrectly sized matrix or vector for MatrixFloodPlotData");
  }

  m_minX = *std::min_element(std::begin(m_xVector), std::end(m_xVector));
  m_maxX = *std::max_element(std::begin(m_xVector), std::end(m_xVector));
  m_minY = *std::min_element(std::begin(m_yVector), std::end(m_yVector));
  m_maxY = *std::max_element(std::begin(m_yVector), std::end(m_yVector));

  m_minValue = *std::min_element(std::begin(m_matrix), std::end(m_matrix));
  m_maxValue = *std::max_element(std::begin(m_matrix), std::end(m_matrix));

  // default behavior is to have entire data range considered for colormapping
  // override by setting colorMapRange
  m_colorMapRange = QwtInterval(m_minValue, m_maxValue);

  if (m_colorMapRange.minValue() == m_colorMapRange.maxValue())
  {
    m_colorMapRange = QwtInterval(m_colorMapRange.minValue(), m_colorMapRange.minValue() + 1.0);
  }

  // set the bounding box
  setInterval(Qt::XAxis, QwtInterval(m_minX, m_maxX));
  setInterval(Qt::YAxis, QwtInterval(m_minY, m_maxY));
  setInterval(Qt::ZAxis, m_colorMapRange);
}


} // openstudio
