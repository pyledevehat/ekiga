/*
 * Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2014 Damien Sandras <dsandras@seconix.com>

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
 *                         sip-heap.h -  description
 *                         ------------------------------------------
 *   begin                : written in 2014 by Julien Puydt
 *   copyright            : (c) 2014 by Julien Puydt
 *   description          : declaration of an Ekiga::Heap implementation for a SIP::Conversation
 *
 */

#ifndef __SIP_HEAP_H__
#define __SIP_HEAP_H__

#include "heap-impl.h"

namespace SIP
{
  class Heap: public Ekiga::HeapImpl<Ekiga::Presentity>
  {
  public:
    // let's put everything in public so SIP::Conversation can control us fully!
    using Ekiga::HeapImpl<Ekiga::Presentity>::add_presentity;
    using Ekiga::HeapImpl<Ekiga::Presentity>::remove_presentity;
    using Ekiga::HeapImpl<Ekiga::Presentity>::add_connection;

    // this is the heap's name
    const std::string get_name () const;
    // try to find a nice name to display from the uri of a presentity
    const std::string get_name (const std::string uri) const;
  };
};

#endif
