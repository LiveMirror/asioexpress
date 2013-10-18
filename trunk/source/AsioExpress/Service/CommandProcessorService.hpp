//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp> 
#include <boost/function.hpp>
#include <boost/thread.hpp> 
#include <boost/bind.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/system/error_code.hpp> 
#include <boost/system/system_error.hpp> 
#include <boost/ptr_container/ptr_vector.hpp>

#include "AsioExpress/Service/private/ReferenceCallbackProcessor.hpp"
#include "AsioExpress/Service/private/ReferenceProcessor.hpp"
#include "AsioExpress/Service/private/PointerCallbackProcessor.hpp"
#include "AsioExpress/Service/private/PointerProcessor.hpp"

namespace AsioExpress {

class CommandProcessorService : public boost::asio::io_service::service 
{ 
public: 
    typedef void* implementation_type;

    explicit CommandProcessorService(
            boost::asio::io_service &ioService, 
            int numberOfThreads=1) : 
        boost::asio::io_service::service(ioService), 
        asyncExecuteWork(new boost::asio::io_service::work(asyncExecuteIoService)),
        numberOfThreads(numberOfThreads),
        asyncExecuteThread(numberOfThreads)
    { 
        for(int i = 0; i < numberOfThreads; ++i)
        {
            asyncExecuteThread.push_back(
                new boost::thread(
                    boost::bind(&boost::asio::io_service::run, 
                    &asyncExecuteIoService)));
        }
    }

    ~CommandProcessorService() 
    { 
      Shutdown();
    }

    void Shutdown()
    {
        // The async execute threads will finish when asyncExecuteWork is 
        // reset.
        asyncExecuteWork.reset(); 

        // Event processing is stopped to discard queued operations.
        // no this is bad, qued operations should end naturally.
        //asyncExecuteIoService.stop(); 

        // The async execute threads are joined to make sure the service is 
        // destroyed after the threads are finished.
        ThreadList::iterator   i = asyncExecuteThread.begin();
        ThreadList::iterator end = asyncExecuteThread.end();
        for(; i != end; ++i)
        {
          i->join();
        }
        asyncExecuteThread.clear();
    }

    template <typename Handler> 
    void AsyncExecute(ServiceCommand * command, Handler handler) 
    { 
      ServiceCommandPointer commandPointer(command);
      this->asyncExecuteIoService.post(
        AsioExpress::Service::PointerCallbackProcessor<Handler>(
          this->get_io_service(), 
          commandPointer, 
          handler)); 
    } 

    template <typename Handler> 
    void AsyncExecute(ServiceCommandPointer commandPointer, Handler handler) 
    { 
      this->asyncExecuteIoService.post(
        AsioExpress::Service::PointerCallbackProcessor<Handler>(
          this->get_io_service(), 
          commandPointer, 
          handler)); 
    } 

    void AsyncExecute(ServiceCommand * command) 
    { 
      ServiceCommandPointer commandPointer(command);
      this->asyncExecuteIoService.post(
        AsioExpress::Service::PointerProcessor(
          this->get_io_service(), 
          commandPointer)); 
    } 

    void AsyncExecute(ServiceCommandPointer commandPointer) 
    { 
      this->asyncExecuteIoService.post(
        AsioExpress::Service::PointerProcessor(
          this->get_io_service(), 
          commandPointer)); 
    } 

    static boost::asio::io_service::id id; 

private: 
    void shutdown_service() 
    { 
    } 

    typedef boost::ptr_vector<boost::thread>          ThreadList;

    boost::asio::io_service                           asyncExecuteIoService; 
    boost::scoped_ptr<boost::asio::io_service::work>  asyncExecuteWork;
    int const                                         numberOfThreads;
    ThreadList                                        asyncExecuteThread;
}; 

} // namespace AsioExpress
