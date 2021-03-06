/// \file Application.cc
/*
 *
 * Application.cc source template automatically generated by a class generator
 * Creation date : ven. sept. 5 2014
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

// -- dqm4hep headers
#include "dqm4hep/Application.h"
#include "dqm4hep/RemoteLogger.h"
#include "dqm4hep/Logging.h"

// -- system headers
#include <sys/time.h>

namespace dqm4hep {

  namespace online {
    
    Application::~Application() {
      removeTimer(m_appStatTimer);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    const std::string &Application::type() const {
      return m_type;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::setType(const std::string &appType) {
      if(initialized()) {
        dqm_error( "Application::setType(): Couldn't set app type, app is already initialized !" );
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      dqm_debug( "Application set type to '{0}'", appType );
      m_type = appType;
    }
    
    //-------------------------------------------------------------------------------------------------

    const std::string &Application::name() const {
      return m_name;
    }
    
    //-------------------------------------------------------------------------------------------------

    void Application::setName(const std::string &appName) {
      if(initialized()) {
        dqm_error( "Application::setName(): Couldn't set app name, app is already initialized !" );
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      dqm_debug( "Application set name to '{0}'", appName );
      m_name = appName;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    const std::string &Application::state() const {
      return m_state;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::setState(const std::string &appState) {
      m_state = appState;
      dqm_debug( "Changing app state to '{0}'", appState );
      
      if(m_pAppStateService && m_server && m_server->isRunning())
        m_pAppStateService->send(m_state);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    core::Logger::Level Application::logLevel() const {
      return m_logLevel;
    }
    
    //-------------------------------------------------------------------------------------------------

    void Application::setLogLevel(core::Logger::Level lvl) {
      m_logLevel = lvl;
      if(nullptr != m_logger) {
        m_logger->set_level(m_logLevel);
      }
    }
    
    //-------------------------------------------------------------------------------------------------

    void Application::createStatsEntry(const std::string &entryName, const std::string &unit, const std::string &description) {
      if(initialized()) {
        dqm_error( "Application::createStatsEntry(): Couldn't create stat entry, app is already initialized !" );
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      
      if(not m_statistics[entryName].is_null())
        throw core::StatusCodeException(core::STATUS_CODE_ALREADY_PRESENT);
      
      // add stat entry
      m_statistics[entryName] = {
        {"description", description},
        {"unit", unit}
      };
      
      dqm_debug( "Creating stat entry '{0}' (unit {1}): {2}", entryName, unit, description );
    }

    //-------------------------------------------------------------------------------------------------    

    void Application::enableStats(bool enable) {
      if(initialized()) {
        dqm_error( "Application::enableStats(): Couldn't enable/disable stats, app is already initialized !" );
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      m_statsEnabled = enable;
    }
    
    //-------------------------------------------------------------------------------------------------

    bool Application::statsEnabled() const {
      return m_statsEnabled;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::setNoServer(bool nosrv) {
      if(initialized()) {
        dqm_error( "Application::enableStats(): Couldn't enable/disable stats, app is already initialized !" );
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      m_noServer = nosrv;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    bool Application::noServer() const {
      return m_noServer;
    }
    
    //-------------------------------------------------------------------------------------------------

    void Application::init(int argc, char **argv) {
      if(initialized()) {
        dqm_error( "Application::init(): app already initialized !" );
        throw core::StatusCodeException(core::STATUS_CODE_ALREADY_INITIALIZED);        
      }
      try {
        this->parseCmdLine(argc, argv);        
      }
      catch(...) {
        dqm_error( "Application::init(): failed to parse cmd line !" );
        throw core::StatusCodeException(core::STATUS_CODE_FAILURE);
      }
      
      if(noServer()) {
        // configure logger
        m_logger = core::Logger::createLogger(this->type() + ":" + this->name(), {
          core::Logger::coloredConsole()
        });        
      }
      else {
        // configure logger
        m_logger = core::Logger::createLogger(this->type() + ":" + this->name(), {
          core::Logger::coloredConsole(),
          RemoteLogger::make_shared()
        });
      }
      core::Logger::setMainLogger(m_logger->name());
      m_logger->set_level(m_logLevel);
        
      if(not noServer()) {
        // configure server
        dqm_debug( "Creating server ..." );
        m_server = std::make_shared<net::Server>(OnlineRoutes::Application::serverName(this->type(), this->name()));
        m_pAppStateService = m_server->createService(OnlineRoutes::Application::state(this->type(), this->name()));

        if(m_statsEnabled) {
          dqm_debug( "Creating internal app stats ..." );
          createInternalStats();
        }
        m_server->onClientExit().connect(this, &Application::sendClientExitEvent);      
      }

      try {
        dqm_info( "User init ..." );
        this->onInit();        
      }
      catch(...) {
        dqm_error( "Application::init(): failed to initialize the app !" );
        throw core::StatusCodeException(core::STATUS_CODE_FAILURE);
      }
      
      dqm_info( "Application successfully initialized !" );
      m_initialized = true;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    int Application::exec() {
      m_running = true;
      if(not noServer()) {
        m_server->start();
        sleep(1);        
      }
      this->setState("Running");
      
      try {
        dqm_info( "Application on start ..." );
        this->onStart();
      }
      catch(...) {
        dqm_error( "Application::init(): failed to start the app !" );
        throw core::StatusCodeException(core::STATUS_CODE_FAILURE);
      }
      
      int returnCode(0);
      
      try {
        if(statsEnabled() and not noServer()) {
          // send statistics every 5 seconds
          dqm_info( "Statistics enabled. Creating timer (5 secs) ..." );
          m_appStatTimer = createTimer();
          m_appStatTimer->setInterval(5000); // 5 seconds
          m_appStatTimer->setSingleShot(false);
          m_appStatTimer->onTimeout().connect(this, &Application::sendAppStats);
          m_appStatTimer->start();
        }
        m_eventLoop.connectOnEvent(this, &Application::onEvent);
        dqm_info( "Starting event loop ..." );
        returnCode = m_eventLoop.exec();
        dqm_info( "Exiting event loop ..." );
        m_eventLoop.disconnectOnEvent(this);
        if(not noServer()){
          m_server->onClientExit().disconnect();          
        }
        this->onStop();
      }
      catch(...) {
        dqm_error( "Application::init(): failed to start the app !" );
        throw core::StatusCodeException(core::STATUS_CODE_FAILURE);
      }      
      std::stringstream appState; appState << "Exiting (" << returnCode << ")"; 
      this->setState(appState.str());
      m_running = false;

      return returnCode;
    }

    //-------------------------------------------------------------------------------------------------

    void Application::exit(int returnCode) {
      m_eventLoop.exit(returnCode);
    }
    
    //-------------------------------------------------------------------------------------------------

    bool Application::initialized() const {
      return m_initialized;
    }

    //-------------------------------------------------------------------------------------------------    

    bool Application::running() const {
      return m_running;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::queuedSubscribe(const std::string &serviceName, int priority, int maxNEvents) {
      if(noServer()){
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      auto findIter = m_serviceHandlerPtrMap.find(serviceName);
      
      if(m_serviceHandlerPtrMap.end() != findIter) {
        dqm_error( "Application::queuedSubscribe(): couldn't subscribe twice to service '{0}'", serviceName );
        throw core::StatusCodeException(core::STATUS_CODE_ALREADY_PRESENT);
      }
      
      auto handler = std::make_shared<NetworkHandler>(m_eventLoop, serviceName, priority, maxNEvents);
      m_serviceHandlerPtrMap.insert(NetworkHandlerPtrMap::value_type(serviceName, handler));
      m_client.subscribe(serviceName, handler.get(), &Application::NetworkHandler::postServiceContent);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    net::Service *Application::createService(const std::string &sname) {
      if(noServer()){
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      if(not m_server) {
        dqm_error( "Application::createService(): couldn't create service '{0}', server is not yet allocated", sname );
        throw core::StatusCodeException(core::STATUS_CODE_NOT_INITIALIZED);
      }
      return m_server->createService(sname);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::createQueuedCommand(const std::string &commandName, int priority, int maxNEvents) {
      if(noServer()){
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      if(not m_server) {
        dqm_error( "Application::createQueuedCommand(): couldn't create command handler '{0}', server is not yet allocated", commandName );
        throw core::StatusCodeException(core::STATUS_CODE_NOT_INITIALIZED);
      }
      
      auto findIter = m_commandHandlerPtrMap.find(commandName);
      
      if(m_commandHandlerPtrMap.end() != findIter) {
        dqm_error( "Application::createQueuedCommand(): couldn't create command handler '{0}' twice", commandName );
        throw core::StatusCodeException(core::STATUS_CODE_ALREADY_PRESENT);
      }
      
      auto handler = std::make_shared<NetworkHandler>(m_eventLoop, commandName, priority, maxNEvents);
      m_commandHandlerPtrMap.insert(NetworkHandlerPtrMap::value_type(commandName, handler));
      m_server->createCommandHandler(commandName, handler.get(), &Application::NetworkHandler::postCommandEvent);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    int Application::serverClientId() const {
      if(noServer()){
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      return m_server->clientId();
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::sendClientExitEvent(int id) {
      if(noServer()){
        throw core::StatusCodeException(core::STATUS_CODE_NOT_ALLOWED);
      }
      auto event = new StoreEvent<int>(AppEvent::CLIENT_EXIT, id);
      m_eventLoop.sendEvent(event);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::sendAppStats() {
      core::MemoryStats memInfo;
      core::memStats(memInfo);
      core::procStats(m_stats);

      core::dqm_long procVm = m_stats.vm/1024.;   // in MB
      core::dqm_long procRss = m_stats.rss/1024.; // in MB

      dqm_debug("Sending internal app stats ..." );
      sendStat("VmProc", procVm);
      sendStat("VmTotal", (procVm/(memInfo.vmTot*1.))*100.);
      sendStat("VmInUse", (procVm/(memInfo.vmUsed*1.))*100.);

      sendStat("RSSProc", procRss);
      sendStat("RSSTotal", (procRss/(memInfo.rssTot*1.))*100.);
      sendStat("RSSInUse", (procRss/(memInfo.rssUsed*1.))*100.);

      sendStat("CpuUser", m_stats.cpuUser);
      sendStat("CpuSys", m_stats.cpuSys);
      sendStat("CpuTotal", m_stats.cpuTot);

      auto tm_time = localtime(&m_stats.lastPollTime.tv_sec);
      char date_buf[100];
      std::strftime(date_buf, sizeof(date_buf), "%Y-%m-%d %H:%M:%S", tm_time);

      sendStat("LastUpdate", date_buf );
      dqm_debug( "Sending internal app stats ... OK" );
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::createInternalStats() {
      // Virtual memory
      createStatsEntry("VmProc", "Mo", "The current virtual memory in use by the application (unit Mo)");
      createStatsEntry("VmTotal", "%", "The current virtual memory percentage in use by the application compared to the total available on the host (unit %)");
      createStatsEntry("VmInUse", "%", "The current virtual memory percentage in use by the application compared to the total used by the running processes (unit %)");
      
      // Resident set size
      createStatsEntry("RSSProc", "Mo", "The current resident set size memory in use by the application (unit Mo)");
      createStatsEntry("RSSTotal", "%", "The current resident set size memory percentage in use by the application compared to the total available on the host (unit %)");
      createStatsEntry("RSSInUse", "%", "The current resident set size memory percentage in use by the application compared to the total used by the running processes (unit %)");

      // Cpu
      createStatsEntry("CpuUser", "%", "The current user cpu load of the application (unit %)");
      createStatsEntry("CpuSys", "%", "The current system cpu load of the application (unit %)");
      createStatsEntry("CpuTotal", "%", "The current total cpu load of the application (unit %)");

      // Last Update
      createStatsEntry("LastUpdate", "%Y-%m-%d %H:%M:%S", "The time the last statistics update occured (unit %Y-%m-%d %H:%M:%S)");

      // Network
      // createStatsEntry("CPU", "%", "The current resident set size memory in use by the application (unit Mo)");
      // createStatsEntry("CPU", "%", "The current resident set size memory percentage in use by the application compare to the total available on the host (unit %)");
      // createStatsEntry("CPU", "%", "The current resident set size memory percentage in use by the application compare to the total used by the running processes (unit %)");

      // Initialise first reading
      struct timeval timeStart;
      gettimeofday(&timeStart, NULL);
      m_stats.lastPollTime = timeStart;
      core::procStats(m_stats);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    AppTimer* Application::createTimer() {
      auto timer = new AppTimer(m_eventLoop);
      m_eventLoop.addTimer(timer);
      return timer;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::removeTimer(AppTimer *timer) {
      m_eventLoop.removeTimer(timer);
    }
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    Application::NetworkHandler::NetworkHandler(AppEventLoop &eventLoop, const std::string &name, int priority, int maxNEvents) :
      m_eventLoop(eventLoop),
      m_name(name),
      m_priority(priority),
      m_maxNEvents(maxNEvents)
    {
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::NetworkHandler::postServiceContent(const net::Buffer &buffer) {
      int count = m_eventLoop.count([this](AppEvent* ptr){
        ServiceUpdateEvent *evt = dynamic_cast<ServiceUpdateEvent*>(ptr);
        return (evt && evt->serviceName() == this->m_name);
      });
      
      if(count >= m_maxNEvents) {
        dqm_debug( "NetworkHandler::postServiceContent(): maximum of posted service updates reached ({0}) !", m_maxNEvents );
        return;
      }
      
      // copy buffer content
      std::string content;
      content.assign(buffer.begin(), buffer.size());
      
      // create new buffer model
      auto bufferModel = buffer.createModel<std::string>();
      bufferModel->move(std::move(content));
      
      // create the event to post, pass the copied buffer in ctor
      ServiceUpdateEvent *pEvent = new ServiceUpdateEvent(m_name, bufferModel);
      pEvent->setPriority(m_priority);
      
      // post the event !
      m_eventLoop.postEvent(pEvent);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::NetworkHandler::sendServiceContent(const net::Buffer &buffer) {
      m_eventLoop.processFunction([&](){
        m_sendContentSignal.emit(buffer);
      });
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::NetworkHandler::sendRequestEvent(const net::Buffer &request, net::Buffer &response) {
      m_eventLoop.processFunction([&](){
        m_sendRequestSignal.emit(request, response);
      });
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void Application::NetworkHandler::postCommandEvent(const net::Buffer &buffer) {
      int count = m_eventLoop.count([this](AppEvent* ptr){
        CommandEvent *evt = dynamic_cast<CommandEvent*>(ptr);
        return (evt && evt->commandName() == this->m_name);
      });
      
      if(count >= m_maxNEvents) {
        dqm_debug( "NetworkHandler::postCommandEvent(): maximum of posted command handling reached ({0}) !", m_maxNEvents );
        return;
      }
      
      // copy buffer content
      std::string content;
      content.assign(buffer.begin(), buffer.size());
      
      // create new buffer model
      auto bufferModel = buffer.createModel<std::string>();
      bufferModel->move(std::move(content));
      
      // create the event to post, pass the copied buffer in ctor
      CommandEvent *pEvent = new CommandEvent(m_name, bufferModel);
      pEvent->setPriority(m_priority);
      
      // post the event !
      m_eventLoop.postEvent(pEvent);
    }
    
    //-------------------------------------------------------------------------------------------------

    void Application::NetworkHandler::sendCommandEvent(const net::Buffer &buffer) {
      m_eventLoop.processFunction([&](){
        m_sendContentSignal.emit(buffer);
      });
    }
    
  }

}

