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

#ifndef RESULTSVIEWER_MATRIX_HPP
#define RESULTSVIEWER_MATRIX_HPP

#include <vector>

namespace resultsviewer{

/**
Matrix is a VERY basic two-dimensional matrix implementation.
*/
class Matrix
{
public:
  Matrix() : m_nrows(0), m_ncols(0)
  {}

  Matrix(int const nrows, int const ncols) : m_nrows(nrows > 0 ? nrows : 0), m_ncols(ncols > 0 ? ncols : 0),
    m_nij(m_nrows*m_ncols), m_values(m_nij)
  {
    for(int ij = 0; ij < m_nij; ij++) {
      m_values[ij] = 0;
    }
  }

  double& operator()(int row, int col)
  {
    return m_values[row*m_ncols + col];
  }

  const double& operator()(int row, int col) const
  {
    return m_values[row*m_ncols + col];
  }

  int size1() const
  {
    return m_nrows;
  }

  int size2() const
  {
    return m_ncols;
  }

  std::vector<double>::const_iterator begin() const
  {
    return m_values.begin();
  }

  std::vector<double>::const_iterator end() const
  {
    return m_values.end();
  }

  int size() const
  {
    return m_nij;
  }

private:
  int m_nrows;
  int m_ncols;
  int m_nij;
  std::vector<double> m_values;
};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_TIMESERIES_HPP
