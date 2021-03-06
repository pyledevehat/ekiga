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
 *                         heap.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2007 by Julien Puydt
 *   copyright            : (c) 2007 by Julien Puydt
 *   description          : declaration of the interface of a heap
 *                          of presentities (equivalent of an addressbooks for
 *                          contacts)
 *
 */

#ifndef __HEAP_H__
#define __HEAP_H__

#include "presentity.h"

namespace Ekiga
{

/**
 * @addtogroup presence
 * @{
 */

  class Heap:
    public virtual LiveObject,
    public Actor
  {

  public:

    /** The destructor.
     */
    virtual ~Heap () { }

    /** Returns the name of the Heap
     * @return The Heap's name
     */
    virtual const std::string get_name () const = 0;

    /** Get the list of Presentities by visiting them with a callback.
     * @param The callback user to know about presentities (the return value
     * means "go on" and allows stopping the visit)
     */
    virtual void visit_presentities (boost::function1<bool, PresentityPtr >) const = 0;

    /**
     * Signals on that object
     */

    /** This signal is emitted  when a Presentity has been added to the Heap.
     */
    boost::signals2::signal<void(PresentityPtr)> presentity_added;

    /** This signal is emitted when a Presentity has been updated in the Heap.
     */
    boost::signals2::signal<void(PresentityPtr)> presentity_updated;

    /** This signal is emitted when a Presentity has been removed from the Heap.
     */
    boost::signals2::signal<void(PresentityPtr)> presentity_removed;
  };

  typedef boost::shared_ptr<Heap> HeapPtr;

/**
 * @}
 */
};

#endif
