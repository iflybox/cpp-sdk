// $Id: Packets_Received_Monitor.cpp 85945 2009-07-09 06:46:39Z olli $

#include "ace/Monitor_Control/Packets_Received_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    const char* Packets_Received_Monitor::default_name_ =
      "OS/Network/PacketsReceived";

    Packets_Received_Monitor::Packets_Received_Monitor (const char* name)
      : Monitor_Base (name, Monitor_Control_Types::MC_NUMBER)
#if defined (ACE_HAS_WIN32_PDH)
      , Windows_Multi_Instance_Monitor (
            ACE_TEXT ("\\Network Interface(*)\\Packets Received/sec"))
#elif defined (linux) || defined (AIX)
      , Linux_Network_Interface_Monitor (
            " %*[^:]: %*u %lu %*u %*u %*u %*u %*u %*u %*u %*u")
            /// Scan format for /proc/net/dev
#elif defined (ACE_HAS_KSTAT)
      , Solaris_Network_Interface_Monitor (ACE_TEXT ("ipackets"))
#elif defined (__FreeBSD__)
      , FreeBSD_Network_Interface_Monitor (ACE_TEXT ("ipackets"))
#elif defined (__NetBSD__) || defined (__OpenBSD__) 
      , BSD_Network_Interface_Monitor (ACE_TEXT ("ipackets"))
#endif
    {}

    void
    Packets_Received_Monitor::update (void)
    {
      this->update_i ();
      
      /// On some platforms, value_ is an ACE_UINT64.
      this->receive (static_cast<double> (this->value_));
    }

    const char*
    Packets_Received_Monitor::default_name (void)
    {
      return Packets_Received_Monitor::default_name_;
    }
    
    void
    Packets_Received_Monitor::clear_i (void)
    {
      this->clear_impl ();
      this->Monitor_Base::clear_i ();
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

