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
 *                         foe-list.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2014 by Julien Puydt
 *   copyright            : (c) 2014 by Julien Puydt
 *   description          : interface of a delegate
 *
 */

#include <glib/gi18n.h>

#include "ekiga-settings.h"

#include "foe-list.h"

Ekiga::FoeList::FoeList(boost::shared_ptr<FriendOrFoe> fof):
  friend_or_foe(fof)
{
}


Ekiga::FoeList::~FoeList()
{
}

bool
Ekiga::FoeList::populate_menu (ContactPtr /*contact*/,
			       const std::string uri,
			       MenuBuilder& builder)
{
  bool result = false;
  boost::shared_ptr<FriendOrFoe> fof = friend_or_foe.lock ();

  if (fof) {

    Ekiga::FriendOrFoe::Identification current = fof->decide ("" /* FIXME */,
							      uri);

    if (current == Ekiga::FriendOrFoe::Unknown) {

      builder.add_action ("blacklist", _("Add to blacklist"),
			  boost::bind(&Ekiga::FoeList::add_foe, this, uri));
      result = true;
    }
  }

  return result;
}

Ekiga::FriendOrFoe::Identification
Ekiga::FoeList::decide (const std::string /*domain*/,
			const std::string token) const
{
  boost::scoped_ptr<Ekiga::Settings> settings(new Ekiga::Settings (CONTACTS_SCHEMA));
  std::list<std::string> foes = settings->get_string_list ("foe-list");
  Ekiga::FriendOrFoe::Identification result = Ekiga::FriendOrFoe::Unknown;

  if (std::find (foes.begin (), foes.end (), token) != foes.end ())
    result = Ekiga::FriendOrFoe::Foe;

  return result;
}

void
Ekiga::FoeList::add_foe (const std::string token)
{
  boost::scoped_ptr<Ekiga::Settings> settings(new Ekiga::Settings (CONTACTS_SCHEMA));
  std::list<std::string> foes = settings->get_string_list ("foe-list");
  foes.push_back (token);
  settings->set_string_list ("foe-list", foes);
}
