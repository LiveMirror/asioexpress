//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ClientServer/ServerMessage.hpp"

///
/// Example of an internal component that can be driven by incomming requests.
///
class EchoManager
{
public:
  void AsyncEcho(
    AsioExpress::MessagePort::ServerMessage serverMessage, 
    int sampleArgument,
    AsioExpress::CompletionHandler completionHandler);
};

typedef boost::shared_ptr<EchoManager> EchoManagerPointer;
