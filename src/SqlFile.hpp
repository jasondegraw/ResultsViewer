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

#ifndef RESULTSVIEWER_SQLFILE_HPP
#define RESULTSVIEWER_SQLFILE_HPP

//#include "TimeSeries.hpp"
#include <sqlite3/sqlite3.h>
#include <string>
#include <memory>
#include <regex>
#include <sstream>
#include <iostream>

namespace resultsviewer{

// HVAC System Timestep, Zone Timestep, Hourly, Daily, Monthly, RunPerio
enum class ReportingFrequency { Detailed=1, Timestep, Hourly, Daily, Monthly, RunPeriod };

/**
SqlFile is a sqlite3 database interface class for E+ output.
*/
class SqlFile
{
public:
  explicit SqlFile(const std::string &path) : m_sqlite3(NULL), m_path(path), m_connected(false)
  {
    open(m_path);
  }

  ~SqlFile()
  {
    close();
  }

  std::string versionString() const
  {
    std::string result;
    if (m_sqlite3) {
      sqlite3_stmt* sqlStmtPtr;
      sqlite3_prepare_v2(m_sqlite3, "SELECT EnergyPlusVersion FROM Simulations", -1, &sqlStmtPtr, 0);
      int code = sqlite3_step(sqlStmtPtr);
      if (code == SQLITE_ROW) {
        // in 8.1 this is 'EnergyPlus-Windows-32 8.1.0.008, YMD=2014.11.08 22:49'
        // in 8.2 this is 'EnergyPlus, Version 8.2.0-8397c2e30b, YMD=2015.01.09 08:37'
        // radiance script is writing 'EnergyPlus, VERSION 8.2, (OpenStudio) YMD=2015.1.9 08:35:36'
        // in 8.8 this is 'EnergyPlus, Version 8.8.0-7c3bbe4830, YMD=2017.11.23 11:10'
        std::regex version_regex("\\d\\.\\d[\\.\\d]*");
        std::string version_line = columnText(sqlite3_column_text(sqlStmtPtr, 0));
        std::smatch version_match;

        if (std::regex_search(version_line, version_match, version_regex)) {
          result = version_match[0].str();
        }
      }
      sqlite3_finalize(sqlStmtPtr);
    }
    return result;
  }

private:

  std::string columnText(const unsigned char* column) const
  {
    return std::string(reinterpret_cast<const char*>(column));
  }

  bool versionCheck()
  {
    std::string energyPlusVersion = versionString();
    if (energyPlusVersion.empty()) {
      return false;
    }

    /*
    VersionString version(energyPlusVersion);
    if (version >= VersionString(7, 0) && version <= VersionString(energyPlusVersionMajor(), energyPlusVersionMinor())) {
      m_supportedVersion = true;
    }
    else {
      m_supportedVersion = false;
      LOG(Warn, "Using unsupported EnergyPlus version " << version.str());
    }
    */

    return true;
  }

  bool open(const std::string &path)
  {

    int result = sqlite3_open_v2(path.c_str(), &m_sqlite3, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE, NULL);

    if (result == 0) {
      if (!versionCheck()) {
        sqlite3_close(m_sqlite3);
        //throw openstudio::Exception("OpenStudio is not compatible with this file.");
      }
      // Set a 1 second timeout
      result = sqlite3_busy_timeout(m_sqlite3, 1000);

      // set locking mode to exclusive
      //code = sqlite3_exec(m_db, "PRAGMA locking_mode=EXCLUSIVE", NULL, NULL, NULL);

      // retrieve DataDictionaryTable
      //retrieveDataDictionary();
      m_connected = true;
    }
    else {
      //throw openstudio::Exception("File not successfully opened.");
    }
    return m_connected;
  }

  bool SqlFile::close()
  {
    if (m_sqlite3)
    {
      sqlite3_close(m_sqlite3);
      m_sqlite3 = NULL;
    }
    return true;
  }

  void retrieveDataDictionary()
  {
    std::string table, name, keyValue, units, rf;

    if (m_sqlite3)
    {
      int dictionaryIndex, code;

      std::stringstream s;
      sqlite3_stmt* sqlStmtPtr;
      std::map<int, std::string> envPeriods;
      std::map<int, std::string>::iterator envPeriodsItr;

      s << "SELECT EnvironmentPeriodIndex, EnvironmentName FROM EnvironmentPeriods";
      sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, nullptr);
      code = sqlite3_step(sqlStmtPtr);
      while (code == SQLITE_ROW)
      {
        std::string queryEnvPeriod = boost::to_upper_copy(columnText(sqlite3_column_text(sqlStmtPtr, 1)));
        envPeriods.insert(std::pair<int, std::string>(sqlite3_column_int(sqlStmtPtr, 0), queryEnvPeriod));
        code = sqlite3_step(sqlStmtPtr);
      }
      sqlite3_finalize(sqlStmtPtr);

      s.str("");
      s << "SELECT ReportMeterDataDictionaryIndex, VariableName, KeyValue, ReportingFrequency, VariableUnits";
      s << " FROM ReportMeterDataDictionary";
      code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, nullptr);

      table = "ReportMeterData";

      code = sqlite3_step(sqlStmtPtr);
      while (code == SQLITE_ROW)
      {
        dictionaryIndex = sqlite3_column_int(sqlStmtPtr, 0);
        name = columnText(sqlite3_column_text(sqlStmtPtr, 1));
        keyValue = columnText(sqlite3_column_text(sqlStmtPtr, 2));
        rf = columnText(sqlite3_column_text(sqlStmtPtr, 3));
        units = columnText(sqlite3_column_text(sqlStmtPtr, 4));

        for (envPeriodsItr = envPeriods.begin();
          envPeriodsItr != envPeriods.end();
          ++envPeriodsItr)
        {
          std::string queryEnvPeriod = boost::to_upper_copy(envPeriodsItr->second);
          m_dataDictionary.insert(DataDictionaryItem(dictionaryIndex, envPeriodsItr->first, name, keyValue, queryEnvPeriod, rf, units, table));
          LOG(Trace, "Creating data dictionary item " << dictionaryIndex << ", " << (*envPeriodsItr).first
            << ", " << name << ", " << keyValue << ", " << queryEnvPeriod << ", " << rf << ", "
            << units << ", " << table << ".");
        }

        code = sqlite3_step(sqlStmtPtr);
      }
      sqlite3_finalize(sqlStmtPtr);

      s.str("");
      s << "SELECT ReportVariableDatadictionaryIndex, VariableName, KeyValue, ReportingFrequency, VariableUnits";
      s << " FROM ReportVariableDatadictionary";
      code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, nullptr);

      table = "ReportVariableData";

      code = sqlite3_step(sqlStmtPtr);
      while (code == SQLITE_ROW)
      {
        dictionaryIndex = sqlite3_column_int(sqlStmtPtr, 0);
        name = columnText(sqlite3_column_text(sqlStmtPtr, 1));
        keyValue = columnText(sqlite3_column_text(sqlStmtPtr, 2));
        rf = columnText(sqlite3_column_text(sqlStmtPtr, 3));
        units = columnText(sqlite3_column_text(sqlStmtPtr, 4));

        for (envPeriodsItr = envPeriods.begin();
          envPeriodsItr != envPeriods.end();
          ++envPeriodsItr)
        {
          std::string queryEnvPeriod = boost::to_upper_copy(envPeriodsItr->second);
          m_dataDictionary.insert(DataDictionaryItem(dictionaryIndex, envPeriodsItr->first, name, keyValue, queryEnvPeriod, rf, units, table));
        }

        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }
      sqlite3_finalize(sqlStmtPtr);
    }
    LOG(Debug, "Dictionary Built");
  }

  sqlite3* m_sqlite3;
  std::string m_path;
  bool m_connected;

};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_SQLFILE_HPP
