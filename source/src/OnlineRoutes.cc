/// \file OnlineRoutes.cc
/*
 *
 * OnlineRoutes.cc source template automatically generated by a class generator
 * Creation date : mar. mars 22 2016
 *
 * This file is part of DQM4HEP libraries.
 * 
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "dqm4hep/OnlineRoutes.h"

namespace dqm4hep {

  namespace online {

    const std::string OnlineRoutes::RunControl::status(const std::string &runControlName) {
      return "/dqm4hep/RunControl/" + runControlName + "/Status";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    const std::string OnlineRoutes::RunControl::sor(const std::string &runControlName) {
      return "/dqm4hep/RunControl/" + runControlName + "/SOR";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    const std::string OnlineRoutes::RunControl::eor(const std::string &runControlName) {
      return "/dqm4hep/RunControl/" + runControlName + "/EOR";
    }
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    std::string OnlineRoutes::Application::serverName(const std::string &type, const std::string &name) {
      return "/dqm4hep/app/" + type + "/" + name;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    std::string OnlineRoutes::Application::state(const std::string &type, const std::string &name) {
      return serverName(type, name) + "/state";
    }
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    std::string OnlineRoutes::EventCollector::applicationType() {
      return "evtcol";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    std::string OnlineRoutes::EventCollector::registerSource(const std::string &collector) {
      return OnlineRoutes::Application::serverName(applicationType(), collector) + "/register";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    std::string OnlineRoutes::EventCollector::unregisterSource(const std::string &collector) {
      return OnlineRoutes::Application::serverName(applicationType(), collector) + "/unregister";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    std::string OnlineRoutes::EventCollector::collectEvent(const std::string &collector) {
      return OnlineRoutes::Application::serverName(applicationType(), collector) + "/collect";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    std::string OnlineRoutes::EventCollector::eventUpdate(const std::string &collector, const std::string &source) {
      return OnlineRoutes::Application::serverName(applicationType(), collector) + "/updates/" + source;
    }
    
    //-------------------------------------------------------------------------------------------------

    std::string OnlineRoutes::EventCollector::eventRequest(const std::string &collector) {
      return OnlineRoutes::Application::serverName(applicationType(), collector) + "/lastevt";
    }
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    const std::string OnlineRoutes::OnlineManager::serverName() {
      return "/dqm4hep/onlineMgr";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    const std::string OnlineRoutes::OnlineManager::logs() {
      return "/dqm4hep/onlineMgr/logs";
    }
    
    //-------------------------------------------------------------------------------------------------

    const std::string OnlineRoutes::OnlineManager::appStats() {
      return "/dqm4hep/onlineMgr/appStats";
    }

    //-------------------------------------------------------------------------------------------------    

    const std::string OnlineRoutes::OnlineManager::collectLog() {
      return "/dqm4hep/onlineMgr/collectLog";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    const std::string OnlineRoutes::OnlineManager::collectAppStat() {
      return "/dqm4hep/onlineMgr/collectAppStat";
    }
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    const std::string OnlineRoutes::ModuleApplication::applicationType() {
      return "module";
    }
    
    //-------------------------------------------------------------------------------------------------
    
    const std::string OnlineRoutes::ModuleApplication::subscribe(const std::string &moduleName) {
      return OnlineRoutes::Application::serverName(applicationType(), moduleName) + "/subscribe";
    }
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
  }

} 

