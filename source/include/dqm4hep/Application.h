/// \file DQMApplication.h
/*
 *
 * DQMApplication.h header template automatically generated by a class generator
 * Creation date : mer. juin 17 2015
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


#ifndef DQM4HEP_APPLICATION_H
#define DQM4HEP_APPLICATION_H

// -- dqm4hep headers
#include "dqm4hep/Internal.h"
#include "dqm4hep/Server.h"
#include "dqm4hep/Client.h"
#include "dqm4hep/AppEvent.h"
#include "dqm4hep/AppEvents.h"
#include "dqm4hep/AppEventLoop.h"

namespace dqm4hep {

  namespace online {

    /** 
     *  @brief  Application class
     */
    class Application {
    public:  
      /**
       *  @brief  Default constructor
       */
      Application() {};
      
      /** 
       *  @brief  Destructor
       */
      virtual ~Application() {}

      /** 
       *  @brief  Parse the command line arguments.
       *          After parsing the cmd line, the application must provide
       *          a valid type and name. At this step the server is not yet created. 
       *
       *  @param  argc the argc argument from the main function
       *  @param  argc the argv argument from the main function
       */
      virtual void parseCmdLine(int argc, char **argv) = 0;
      
      /** 
       *  @brief  Custom application initialization function. At this step the 
       *          server is created but is not yet running.
       *          This is the place where the application should create:
       *            - all needed services 
       *            - all stats entries
       */
      virtual void onInit() = 0;
      
      /**
       *  @brief  Process an event from the event loop 
       *  
       *  @param  pAppEvent the event to process
       */
      virtual void onEvent(AppEvent *pAppEvent) = 0;
      
      /**
       *  @brief  Custom user function called just before starting the event loop.
       *          At this step the internal server is already running.
       *          This function may e.g send an initial event to the event loop.
       */
      virtual void onStart() = 0;
      
      /**
       *  @brief  Custom user function called just after exiting the event loop.
       */
      virtual void onStop() = 0;

      /** 
       *  @brief  Get the application type
       */
      const std::string &type() const;
      
      /** 
       *  @brief  Set the application type.
       *          Throw exception if called after init() is called
       *
       *  @param  type the application type
       */
      void setType(const std::string &type);

      /** 
       *  @brief  Get the application name
       */
      const std::string &name() const;
      
      /** 
       *  @brief  Set the application name
       *          Throw exception if called after init() is called
       *
       *  @param  name the application name
       */
      void setName(const std::string &name);
      
      /**
       *  @brief  Get the application state
       */
      const std::string &state() const;
      
      /**
       *  @brief  Change the application state
       */
      void setState(const std::string &state);
      
      /**
       *  @brief  Create a statistics entry.
       *  
       *  @param  name the stat entry name
       *  @param  description the stat entry description
       */
      void createStatsEntry(const std::string &name, const std::string &unit, const std::string &description);
      
      /**
       *  @brief  Send the stat entry with a new value
       */
      void sendStat(const std::string &name, double stats);
      
      /**
       *  @brief  Set whether to send statistics on update
       *  
       *  @param  enable
       */
      void enableStats(bool enable);
      
      /**
       *  @brief  Whether the statistics are enabled
       */
      bool statsEnabled() const;
      
      /**
       *  @brief  Initialize the application.
       *          Calls userInit()
       *  
       *  @param  argc [description]
       *  @param  argv [description]
       */
      void init(int argc, char **argv);
      
      /** 
       *  @brief  Run the application
       */
      int exec();

      /** 
       *  @brief  Exit the application with the specified return code
       */
      void exit(int returnCode);
      
      /**
       *  @brief  Whether the application has been initialized
       */
      bool initialized() const;
      
      /**
       *  @brief  Whether the application is running
       */
      bool running() const;
      
      /// Client interface
      /**
       *  @brief  Send a request using the net client interface. 
       *          See net::Client class.
       *  
       *  @param  name the command name
       *  @param  contents the command contents to send
       *  @param  blocking whether to receive aknowledgment while send the command
       */
      template <typename T>
      void sendCommand(const std::string &name, const T &contents, bool blocking = false) const;
      
      /**
       *  @brief  Send a request using the net client interface. 
       *          See net::Client class.
       *
       *  @param  name the request name
       *  @param  request the buffer to send
       *  @param  operation the operation to perform on answer reception 
       */
      template <typename Operation>
      void sendRequest(const std::string &name, const net::Buffer &request, Operation operation);
      
      /**
       *  @brief  Subscribe to service. On service update, the content is posted
       *          using the postEvent() function with a ServiceUpdateEvent event.
       *          The received service buffer is copied and can be accessed using
       *          the method ServiceUpdateEvent::buffer() in the callback method
       *          Application::onEvent(). A maximum limit of posted event from the 
       *          service can be set using the argument maxNEvents. If this limit
       *          is reached, the service update is not posted.
       *  
       *  @param  serviceName the service name to subscribe
       *  @param  priority the priority of the event in the event queue
       *  @param  maxNEvents the maximum number of updates for this service to post
       */
      void queuedSubscribe(const std::string &serviceName, int priority = 50, int maxNEvents = std::numeric_limits<int>::max());
      
      /**
       *  @brief  Subscribe to service. On service update, the content is posted
       *          using the sendEvent() function with a ServiceUpdateEvent event.
       *          The received service buffer is not copied but directly passed to
       *          the user. The buffer can be accessed using the method 
       *          ServiceUpdateEvent::buffer() in the callback method
       *          Application::onEvent().
       *          ATTN! The server thread will not be released until the event has
       *          been processed by the sendEvent method !
       *  
       *  @param  serviceName the service name to subscribe
       */
      void directSubscribe(const std::string &serviceName);
      
      /// Server interface
      /**
       *  @brief  Create a new service
       *  
       *  @param  name the service name
       */
      net::Service *createService(const std::string &name);
      
      /**
       *  @brief  Create a request handler. On request, the request content is posted
       *          using the sendEvent() function with a RequestEvent event.
       *          The received service buffer is not copied but directly passed to
       *          the user. The request buffer and the response buffer can be accessed 
       *          respectively using the method RequestEvent::request() and 
       *          RequestEvent::response() in the callback method Application::onEvent().
       *          ATTN! The server thread will not be released until the event has
       *          been processed by the sendEvent method !
       *  
       *  @param requestName the request name to handle
       */
      void createRequestHandler(const std::string &requestName);
      
      /**
       *  @brief  Create a command handler. On command reception, the content is posted
       *          using the postEvent() function with a CommandEvent event.
       *          The received service buffer is copied and can be accessed using
       *          the method CommandEvent::buffer() in the callback method
       *          Application::onEvent(). A maximum limit of posted event from the 
       *          command can be set using the argument maxNEvents. If this limit
       *          is reached, the command handling is not posted.
       *  
       *  @param  commandName the command name to handle
       *  @param  priority the priority of the event in the event queue
       *  @param  maxNEvents the maximum number of event for this command to post
       */
      void createQueuedCommand(const std::string &commandName, int priority = 50, int maxNEvents = std::numeric_limits<int>::max());
      
      /**
       *  @brief  Create a command handler. On command reception, the content is posted
       *          using the sendEvent() function with a CommandEvent event.
       *          The received command buffer is not copied but directly passed to
       *          the user. The buffer can be accessed using the method 
       *          CommandEvent::buffer() in the callback method
       *          Application::onEvent().
       *          ATTN! The server thread will not be released until the event has
       *          been processed by the sendEvent method !
       *  
       *  @param commandName the command name to subscribe
       */
      void createDirectCommand(const std::string &commandName);
      
      int serverClientId() const;
      
      void sendClientExitEvent(int clientId);
      
      template <typename Controller>
      void createTimer(const std::string &name, unsigned int nSeconds, bool singleShot,
                       Controller *controller, void (Controller::*function)());
                       
      void removeTimer(const std::string &name);
      
    private:
      /**
       *  @brief  Send application statistics
       */
      void sendAppStats();
      
      /**
       *  @brief  Create the internal stat entries (memory, CPU, etc ...)
       */
      void createInternalStats();
      
    private:
      /**
       *  @brief  NetworkHandler class
       */
      class NetworkHandler {
      public:
        /**
         *  @brief  Constructor
         *  
         *  @param eventLoop the application event loop
         *  @param name the name of service/command/request to handle
         *  @param priority the event priority, if the event is posted
         */
        NetworkHandler(AppEventLoop &eventLoop, const std::string &name, int priority = 50, int maxNEvents = std::numeric_limits<int>::max());
        
        /**
         *  @brief  Post the service content in the event loop. The received buffer
         *          from the service update is copied and pushed to the event queue
         *          using the AppEventLoop::postEvent() method.
         *          
         *  @param  buffer the received buffer from the service update
         */
        void postServiceContent(const net::Buffer &buffer);
        
        /**
         *  @brief  Send directly the service content. The received buffer
         *          from the service update is not copied but directly sent to 
         *          the user using the AppEventLoop::postEvent() method.
         *          
         *  @param  buffer the received buffer from the service update
         */
        void sendServiceContent(const net::Buffer &buffer);
        
        /**
         *  @brief  Send directly the request content. The received buffer
         *          from the request handler is not copied but directly sent to 
         *          the user using the AppEventLoop::postEvent() method.
         *          
         *  @param  request the received buffer from the request handler
         *  @param  response the buffer to send back as response
         */
        void sendRequestEvent(const net::Buffer &request, net::Buffer &response);
        
        /**
         *  @brief  Post the command content in the event loop. The received buffer
         *          from the command is copied and pushed to the event queue
         *          using the AppEventLoop::postEvent() method.
         *          
         *  @param  buffer the received buffer from the command handler
         */
        void postCommandEvent(const net::Buffer &buffer);
        
        /**
         *  @brief  Send directly the command content. The received buffer
         *          from the command handler is not copied but directly sent to 
         *          the user using the AppEventLoop::postEvent() method.
         *          
         *  @param  buffer the received buffer from the command handler
         */
        void sendCommandEvent(const net::Buffer &buffer);
        
      private:
        AppEventLoop           &m_eventLoop;
        const std::string       m_name;
        const int               m_priority;
        const int               m_maxNEvents;
      };
      
      typedef std::shared_ptr<NetworkHandler> NetworkHandlerPtr;
      typedef std::map<const std::string, NetworkHandlerPtr> NetworkHandlerPtrMap;
      
    private:
      std::string                  m_type = {""};                     ///< The application type
      std::string                  m_name = {""};                     ///< The application name
      std::string                  m_state = {"Unknown"};             ///< The application state
      bool                         m_running = {false};               ///< Whether the application is running
      bool                         m_initialized = {false};           ///< Whether the application is initialized
      bool                         m_statsEnabled = {true};           ///< Whether the application statistics are available  

      AppEventLoop                 m_eventLoop = {};                  ///< The application event loop
      std::shared_ptr<net::Server> m_server = {nullptr};              ///< The main server interface of the application
      net::Service                *m_pAppStateService = {nullptr};    ///< The service for application state, updated when the state changes
      net::Service                *m_pAppStatsService = {nullptr};    ///< The service for application statistics, updated when a statistic changes
      net::Client                  m_client;                          ///< The main client interface of the application
      
      NetworkHandlerPtrMap         m_serviceHandlerPtrMap;            ///< The map handling service updates from the client interface
      NetworkHandlerPtrMap         m_requestHandlerPtrMap;            ///< The map handling request handlers from the server interface
      NetworkHandlerPtrMap         m_commandHandlerPtrMap;            ///< The map handling command handlers from the server interface

      core::json                   m_statistics = {};                 ///< The json value handling the application statistics
    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    template <typename T>
    inline void Application::sendCommand(const std::string &name, const T &contents, bool blocking) const {
      m_client.sendCommand(name, contents, blocking);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    template <typename Operation>
    inline void Application::sendRequest(const std::string &name, const net::Buffer &request, Operation operation) {
      m_client.sendRequest(name, request, operation);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    template <typename Controller>
    inline void Application::createTimer(const std::string &name, unsigned int nSeconds, bool singleShot,
                     Controller *controller, void (Controller::*function)()) {
      m_eventLoop.createTimer(name, nSeconds, singleShot, controller, function);
    }
    
  }

} 

#endif  //  DQM4HEP_APPLICATION_H
