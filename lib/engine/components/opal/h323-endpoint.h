/* Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2009 Damien Sandras <dsandras@seconix.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * Ekiga is licensed under the GPL license and as a special exc_managertion,
 * you have permission to link or otherwise combine this program with the
 * programs OPAL, OpenH323 and PWLIB, and distribute the combination,
 * without applying the requirements of the GNU GPL to the OPAL, OpenH323
 * and PWLIB programs, as long as you do follow the requirements of the
 * GNU GPL for all the rest of the software thus combined.
 */


/*
 *                         h323endpoint.h  -  description
 *                         ------------------------------
 *   begin                : Wed 24 Nov 2004
 *   copyright            : (C) 2000-2006 by Damien Sandras
 *   description          : This file contains the H.323 Endpoint class.
 *
 */


#ifndef _H323_ENDPOINT_H_
#define _H323_ENDPOINT_H_

#include <opal/opal.h>

#include "ekiga-settings.h"

#include "call-core.h"
#include "presence-core.h"
#include "contact-core.h"
#include "call-manager.h"
#include "opal-account.h"

#include "opal-call-manager.h"

namespace Opal {

  namespace H323 {

    class EndPoint : public H323EndPoint,
		     public Ekiga::CallProtocolManager
    {
      PCLASSINFO(EndPoint, H323EndPoint);

  public:
      EndPoint (CallManager & ep,
		const Ekiga::ServiceCore& core);

      ~EndPoint ();

      /* CallProtocolManager */
      bool dial (const std::string & uri);
      bool transfer (const std::string & uri,
                     bool attended);
      bool message (const Ekiga::ContactPtr & contact,
                    const std::string & uri);
      bool is_supported_uri (const std::string & uri);


      const std::string & get_protocol_name () const;

      void set_dtmf_mode (unsigned mode);
      unsigned get_dtmf_mode () const;

      bool set_listen_port (unsigned port);

      const Ekiga::CallProtocolManager::InterfaceList & get_interfaces () const;

      void set_initial_bandwidth (unsigned max_tx_video_bitrate);

      /* H.323 CallProtocolManager */
      void set_forward_uri (const std::string & uri);
      const std::string & get_forward_uri () const;


      /* Enable / Disable accounts. The account given as argument
       * will be updated to reflect the current account state once
       * the operation has been successful.
       */
      void enable_account (Account & account);
      void disable_account (Account & account);

      bool UseGatekeeper (const PString & address = PString::Empty (),
                          const PString & domain = PString::Empty (),
                          const PString & iface = PString::Empty ());

      bool RemoveGatekeeper (const PString & address);

      bool IsRegisteredWithGatekeeper (const PString & address);

      void setup (const std::string key = "");

  private:
      bool OnIncomingConnection (OpalConnection &connection,
                                 unsigned options,
                                 OpalConnection::StringOptions *str_options);

      // this object is really managed by opal,
      // so the way it is handled here is correct
      CallManager & manager;

      PMutex gk_name_mutex;
      PString gk_name;

      Ekiga::CallProtocolManager::Interface listen_iface;

      std::string protocol_name;
      std::string uri_prefix;
      std::string forward_uri;

      boost::shared_ptr<Ekiga::Settings> settings;
      boost::shared_ptr<Ekiga::Settings> video_codecs_settings;
      Ekiga::CallProtocolManager::InterfaceList interfaces;

      const Ekiga::ServiceCore & core;
    };
  };
};
#endif
