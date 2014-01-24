//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpress/MessagePort/Tcp/ErrorCodes.hpp"
#include "AsioExpress/MessagePort/Tcp/private/TcpSetSocketOptions.hpp"

#ifdef _MSC_VER

#include "AsioExpressError/Windows/WinErrorUtility.hpp"

void AsioExpress::MessagePort::Tcp::SetSocketOptions(SocketPointer const & socket)
{
  SOCKET s = socket->native();
  int keepalive = 1;
  int returnCode = setsockopt(
    s, 
    SOL_SOCKET, 
    SO_KEEPALIVE, 
    (char*)&keepalive, 
    sizeof(keepalive));
  if (returnCode == SOCKET_ERROR)
  {
    std::string message ("setsockopt failed; ");
    message += AsioExpress::GetWindowsErrorString(WSAGetLastError());
    AsioExpress::Error error(
      AsioExpress::MessagePort::Tcp::ErrorCode::SocketInitializationFailed,
      message);
    throw AsioExpress::CommonException(error);
  }

  DWORD bytes = 0;  
  tcp_keepalive settings= {0}, returned = {0};  
  settings.onoff = 1;  
  settings.keepalivetime = 10*1000;
  settings.keepaliveinterval = 1000;
  returnCode = WSAIoctl(
        s, 
        SIO_KEEPALIVE_VALS, 
        &settings, 
        sizeof(settings), 
        &returned, 
        sizeof(returned), 
        &bytes, 
        NULL, 
        NULL);
  if (returnCode == SOCKET_ERROR)
  {  
    std::string message ("WSAIoctl failed; ");
    message += AsioExpress::GetWindowsErrorString(WSAGetLastError());
    AsioExpress::Error error(
      AsioExpress::MessagePort::Tcp::ErrorCode::SocketInitializationFailed,
      message);
    throw AsioExpress::CommonException(error);
  }
}

#else // _MSC_VER

static void set_socket_option(
        boost::asio::ip::tcp::socket::native_handle_type s,
        int type, 
        int option, 
        int value)
{
    if(setsockopt(s, type, option, &value, sizeof value) < 0) 
    {
         std::ostringstream message;
         message << "setsockopt failed; errno=" << errno << "; " << strerror(errno);
         AsioExpress::Error error(
             AsioExpress::MessagePort::Tcp::ErrorCode::SocketInitializationFailed,
             message.str());        
         throw AsioExpress::CommonException(error);
    }    
}

void AsioExpress::MessagePort::Tcp::SetSocketOptions(SocketPointer const & socket)
{
    //TODO: add keep alive code
    boost::asio::ip::tcp::socket::native_handle_type s = socket->native();
    
    // Active keep alive
    set_socket_option(s, SOL_SOCKET, SO_KEEPALIVE, 1);
    
    // 10 sec before starting probes
    set_socket_option(s, SOL_TCP, TCP_KEEPIDLE, 10);
    
    // 5 probes max
    set_socket_option(s, SOL_TCP, TCP_KEEPCNT, 5);
    
    // 10 seconds between each probe
    set_socket_option(s, SOL_TCP, TCP_KEEPINTVL, 10);
}

#endif // _MSC_VER
