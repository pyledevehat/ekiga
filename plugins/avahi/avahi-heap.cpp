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
 * Ekiga is licensed under the GPL license and as a special exception,
 * you have permission to link or otherwise combine this program with the
 * programs OPAL, OpenH323 and PWLIB, and distribute the combination,
 * without applying the requirements of the GNU GPL to the OPAL, OpenH323
 * and PWLIB programs, as long as you do follow the requirements of the
 * GNU GPL for all the rest of the software thus combined.
 */


/*
 *                         avahi-heap.cpp  -  description
 *                         ------------------------------------------
 *   begin                : written in 2007 by Julien Puydt
 *   copyright            : (c) 2007 by Julien Puydt
 *   description          : implementation for the avahi heap
 *
 */

#include <cstdlib>
#include <glib/gi18n.h>

#include "avahi-heap.h"

#define DEBUG 0

static void
avahi_client_callback (AvahiClient *client,
		       AvahiClientState state,
		       void *data)
{
  ((Avahi::Heap *)data)->ClientCallback (client, state);
}

static void
avahi_browser_callback (AvahiServiceBrowser *browser,
			AvahiIfIndex interface,
			AvahiProtocol protocol,
			AvahiBrowserEvent event,
			const char *name,
			const char *type,
			const char *domain,
			AvahiLookupResultFlags flags,
			void *data)
{
  ((Avahi::Heap *)data)->BrowserCallback (browser, interface, protocol,
					  event, name, type, domain, flags);
}


static void
avahi_resolver_callback (AvahiServiceResolver *resolver,
			 AvahiIfIndex interface,
			 AvahiProtocol protocol,
			 AvahiResolverEvent event,
			 const char *name,
			 const char *type,
			 const char *domain,
			 const char *host_name,
			 const AvahiAddress *address,
			 uint16_t port,
			 AvahiStringList *txt,
			 AvahiLookupResultFlags flags,
			 void *data)
{
  ((Avahi::Heap *)data)->ResolverCallback (resolver, interface, protocol,
					   event, name, type, domain,
					   host_name, address, port,
					   txt, flags);
}


Avahi::Heap::Heap (Ekiga::ServiceCore& core)
{
  const AvahiPoll *poll_api = NULL;
  int error;

  presence_core = core.get<Ekiga::PresenceCore> ("presence-core");

  /* let's make sure those are sanely initialized */
  poll = NULL;
  client = NULL;

  avahi_set_allocator (avahi_glib_allocator ());
  poll = avahi_glib_poll_new (NULL, G_PRIORITY_DEFAULT);
  poll_api = avahi_glib_poll_get (poll);

  /* this may not be the final valid client pointer according to
   * avahi's documentation... we'll take what our callback gets
   */
  client = avahi_client_new (poll_api, (AvahiClientFlags)AVAHI_CLIENT_NO_FAIL,
			     avahi_client_callback, this,
			     &error);
#if DEBUG
  if (client == NULL)
    std::cout << __PRETTY_FUNCTION__ << " client is NULL!" << std::endl;
#endif
}

Avahi::Heap::~Heap ()
{
  if (client != NULL)
    avahi_client_free (client);

  if (poll != NULL)
    avahi_glib_poll_free (poll);
}


const std::string
Avahi::Heap::get_name () const
{
  return _("Neighbours");
}

bool
Avahi::Heap::populate_menu (Ekiga::MenuBuilder& /*builder*/)
{
  return false;
}

bool
Avahi::Heap::populate_menu_for_group (const std::string /*name*/,
				      Ekiga::MenuBuilder& /*builder*/)
{
  return false;
}

void
Avahi::Heap::ClientCallback (AvahiClient *_client,
			     AvahiClientState state)
{
  /* this is the good client pointer */
  client = _client;

  switch (state) {
  case AVAHI_CLIENT_FAILURE:
    /* bad, bad: free the client and try to get another one... but
     * won't I tax the box?
     */
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_CLIENT_FAILURE" << std::endl;
#endif
    if (client != NULL)
      avahi_client_free (client);
    client = NULL;
    break;
  case AVAHI_CLIENT_S_RUNNING:
    /* ignore what we get from the new, as it may not be the final
     * valid browser pointer... we'll take what our callback gets
     */
    avahi_service_browser_new (client,
			       AVAHI_IF_UNSPEC,
			       AVAHI_PROTO_UNSPEC,
			       "_sip._udp", NULL,
			       (AvahiLookupFlags)0,
			       avahi_browser_callback,
			       this);
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_CLIENT_S_RUNNING" << std::endl;
    if (browser == NULL)
      std::cout << "but NULL browser!" << std::endl;
#endif
    break;
  case AVAHI_CLIENT_CONNECTING:
  case AVAHI_CLIENT_S_REGISTERING:
  case AVAHI_CLIENT_S_COLLISION:
    /* do nothing */
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " OTHER" << std::endl;
#endif
    break;
  default:
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " SHOULDN'T HAPPEN" << std::endl;
#endif
    /* shouldn't happen */
    break;
  }
}

void
Avahi::Heap::BrowserCallback (AvahiServiceBrowser *browser,
			      AvahiIfIndex interface,
			      AvahiProtocol protocol,
			      AvahiBrowserEvent event,
			      const char *name,
			      const char *type,
			      const char *domain,
			      AvahiLookupResultFlags /*flags*/)
{
  AvahiServiceResolver *resolver = NULL;

  switch (event) {

  case AVAHI_BROWSER_NEW:
    /* this may not be the final valid resolver pointer...
     * we'll take what our callback gets
     */
    resolver = avahi_service_resolver_new (client, interface, protocol,
					   name, type, domain,
					   AVAHI_PROTO_UNSPEC,
					   (AvahiLookupFlags)0,
					   avahi_resolver_callback, this);
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_BROWSER_NEW" << std::endl;
#endif
    if (resolver == NULL)
      std::cout << "resolver is NULL!" << std::endl;
    break;

  case AVAHI_BROWSER_REMOVE:
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_BROWSER_REMOVE" << std::endl;
#endif
    for (iterator iter = begin ();
         iter != end ();
         ++iter)
      if ((*iter)->get_name () == name) {
        (*iter)->removed ();
        break;
      }
    break;
  case AVAHI_BROWSER_CACHE_EXHAUSTED:
    // FIXME: do I care?
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_BROWSER_CACHE_EXHAUSTED" << std::endl;
#endif
    break;
  case AVAHI_BROWSER_ALL_FOR_NOW:
    // FIXME: do I care?
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_BROWSER_ALL_FOR_NOW" << std::endl;
#endif
    break;
  case AVAHI_BROWSER_FAILURE:
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_BROWSER_FAILURE" << std::endl;
#endif
    avahi_service_browser_free (browser);
    browser = NULL;
    break;
  default:
    /* shouldn't happen */
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " SHOULDN'T HAPPEN" << std::endl;
#endif
    break;
  }
}

class resolver_callback_helper
{
public:

  resolver_callback_helper (const std::string name_): name(name_)
  {}

  bool operator() (Ekiga::PresentityPtr pres_)
  {
    boost::shared_ptr<Ekiga::URIPresentity> presentity_ = boost::dynamic_pointer_cast<Ekiga::URIPresentity> (pres_);
    bool result = true;

    if (presentity_ && presentity_->get_name () == name) {

      presentity = presentity_;
      result = false;
    }
    return result;
  }

  boost::shared_ptr<Ekiga::URIPresentity> found_presentity () const
  { return presentity; }

private:
  boost::shared_ptr<Ekiga::URIPresentity> presentity;
  const std::string name;
};

void
Avahi::Heap::ResolverCallback (AvahiServiceResolver *resolver,
			       AvahiIfIndex /*interface*/,
			       AvahiProtocol /*protocol*/,
			       AvahiResolverEvent event,
			       const char * name_,
			       const char * typ,
			       const char * /*domain*/,
			       const char* host_name,
			       const AvahiAddress */*address*/,
			       uint16_t port,
			       AvahiStringList *txt,
			       AvahiLookupResultFlags flags)
{
  std::string name;
  std::string software;
  std::string presence;
  std::string status;
  gchar *url = NULL;
  AvahiStringList *txt_tmp = NULL;

  // filter out seeing ourselves
  // FIXME: doesn't it hide other people on the same box too?
  if (flags & AVAHI_LOOKUP_RESULT_LOCAL) {

    avahi_service_resolver_free (resolver);
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " LOCAL RESULT" << std::endl;
#endif
    return;
  }

  switch (event) {

  case AVAHI_RESOLVER_FOUND: {
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_RESOLVER_FOUND" << std::endl;
#endif

    name = name_;
    for (txt_tmp = txt;  txt_tmp != NULL; txt_tmp = txt_tmp->next) {

      char *ckey = NULL;
      char *cvalue = NULL;
      size_t valsize;
      if (avahi_string_list_get_pair (txt_tmp, &ckey, &cvalue, &valsize) >= 0) {

	if (ckey != NULL && cvalue != NULL) {

	  std::string key (ckey);
	  std::string value (cvalue);
	  if (key == "presence")
	    presence = value;
	  else if (key == "status")
	    status = value;
	  else if (key == "software")
	    software = value;
	}
	if (ckey != NULL) free (ckey);
	if (cvalue != NULL) free (cvalue);
      }
    }

    resolver_callback_helper helper(name);
    visit_presentities (boost::ref (helper));
    if (helper.found_presentity ()) {

      /* known contact has been updated */
      presence_received (helper.found_presentity ()->get_uri (), presence);
      status_received (helper.found_presentity ()->get_uri (), status);
    } else {

      /* ok, this is a new contact */
      gchar** broken = NULL;
      broken = g_strsplit_set (typ, "._", 0);
      boost::shared_ptr<Ekiga::PresenceCore> pcore = presence_core.lock ();
      if (broken != NULL && broken[0] != NULL && broken[1] != NULL && pcore) {

	std::list<std::string> groups;

	groups.push_back (_("Neighbours"));
	url = g_strdup_printf ("%s:neighbour@%s:%d", broken[1], host_name, port);
	boost::shared_ptr<Ekiga::URIPresentity> presentity (new Ekiga::URIPresentity (pcore, name, url, groups));
	status_received (url, status);
	presence_received (url, presence);
	add_presentity (presentity);
	g_free (url);
      }
      g_strfreev (broken);
    }
    avahi_service_resolver_free (resolver);
    break;}
  case AVAHI_RESOLVER_FAILURE:

#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " AVAHI_RESOLVER_FAILURE" << std::endl;
#endif
    avahi_service_resolver_free (resolver);
    break;
  default:
    /* shouldn't happen */
#if DEBUG
    std::cout << __PRETTY_FUNCTION__ << " SHOULDN'T HAPPEN" << std::endl;
#endif
    break;
  }
}
