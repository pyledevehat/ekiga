/*
 * Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2009 Damien Sandras <dsandras@seconix.com>

 * This program is free software; you can  redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Ekiga is licensed under the GPL license and as a special exception, you
 * have permission to link or otherwise combine this program with the
 * programs OPAL, OpenH323 and PWLIB, and distribute the combination, without
 * applying the requirements of the GNU GPL to the OPAL, OpenH323 and PWLIB
 * programs, as long as you do follow the requirements of the GNU GPL for all
 * the rest of the software thus combined.
 */


/*
 *                         call-core.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2007 by Damien Sandras 
 *   copyright            : (c) 2007 by Damien Sandras
 *   description          : declaration of the interface of a call core.
 *                          A call core manages CallManagers.
 *
 */

#ifndef __CALL_CORE_H__
#define __CALL_CORE_H__

#include "form-request.h"
#include "chain-of-responsibility.h"
#include "services.h"
#include "scoped-connections.h"
#include "reflister.h"

#include "friend-or-foe/friend-or-foe.h"
#include "call.h"
#include "call-manager.h"
#include "call-protocol-manager.h"
#include "contact-core.h"

#include <boost/smart_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include <set>
#include <map>

namespace Ekiga
{

/**
 * @defgroup calls Calls and protocols
 * @{
 */

  class CallManager;

  class CallCore:
    public Service,
    protected RefLister<CallManager>
    {

  public:
      typedef RefLister<CallManager>::iterator iterator;
      typedef RefLister<CallManager>::const_iterator const_iterator;

      /** The constructor
       */
      CallCore (boost::shared_ptr<Ekiga::FriendOrFoe> iff);


      /*** Service Implementation ***/

      /** Returns the name of the service.
       * @return The service name.
       */
      const std::string get_name () const
        { return "call-core"; }


      /** Returns the description of the service.
       * @return The service description.
       */
      const std::string get_description () const
        { return "\tCall Core managing Call Manager objects"; }


      /** Adds a call handled by the CallCore serice.
       * @param call is the call to be added.
       */
      void add_call (boost::shared_ptr<Call> call);

      /** Remove a call handled by the CallCore serice.
       * @param call is the call to be removed.
       */
      void remove_call (boost::shared_ptr<Call> call);

      /** Adds a CallManager to the CallCore service.
       * @param The manager to be added.
       */
      void add_manager (boost::shared_ptr<CallManager> manager);

      /** Removes the CallManager from the CallCore service.
       * @param The manager to be removed.
       */
      void remove_manager (boost::shared_ptr<CallManager> manager);

      /** Return iterator to beginning
       * @return iterator to beginning
       */
      iterator begin ();
      const_iterator begin () const;

      /** Return iterator to end
       * @return iterator to end
       */
      iterator end ();
      const_iterator end () const;

      /** This signal is emitted when a Ekiga::CallManager has been
       * added to the CallCore Service.
       */
      boost::signals2::signal<void(boost::shared_ptr<CallManager>)> manager_added;

      /** This signal is emitted when a Ekiga::CallManager has been
       * removed to the CallCore Service.
       */
      boost::signals2::signal<void(boost::shared_ptr<CallManager>)> manager_removed;


      /*** Call Management ***/

      /** Create a call based on the remote uri given as parameter
       * @param an uri to call
       * @return true if a Ekiga::Call could be created
       */
      bool dial (const std::string & uri);

      /** Hang up all active calls (if any).
       */
      void hang_up ();

      /* Return true if URI can be handled by the CallCore,
       * false otherwise.
       * @param the URI to test
       * @return true of the URI can be handled, false otherwise
       */
      bool is_supported_uri (const std::string & uri);


      /*** Codecs Management ***/
      void set_codecs (Ekiga::CodecList & codecs);
      Ekiga::CodecList get_codecs () const;


      /*** Call Related Signals ***/

      /** See call.h for the API
       */
      boost::signals2::signal<void(boost::shared_ptr<Call>)> ringing_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>)> setup_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>)> missed_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>, std::string)> cleared_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>)> created_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>)> established_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>)> held_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>)> retrieved_call;
      boost::signals2::signal<void(boost::shared_ptr<Call>, std::string, Call::StreamType, bool)> stream_opened;
      boost::signals2::signal<void(boost::shared_ptr<Call>, std::string, Call::StreamType, bool)> stream_closed;
      boost::signals2::signal<void(boost::shared_ptr<Call>, std::string, Call::StreamType)> stream_paused;
      boost::signals2::signal<void(boost::shared_ptr<Call>, std::string, Call::StreamType)> stream_resumed;

      /** This chain allows the CallCore to report errors to the user
       */
      ChainOfResponsibility<std::string> errors;

  private:

      void on_call_removed (boost::shared_ptr<Call> call);

      boost::shared_ptr<Ekiga::FriendOrFoe> iff;
      std::map<std::string, boost::shared_ptr<Ekiga::scoped_connections> > call_connections;
    };

/**
 * @}
 */

};


#endif
