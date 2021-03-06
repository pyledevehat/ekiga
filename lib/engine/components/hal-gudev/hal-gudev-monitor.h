/* Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2014 Damien Sandras <dsandras@seconix.com>
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
 *                         hal-gudev-monitor.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2014 by Julien Puydt
 *   copyright            : (c) 2014 Julien Puydt
 *   description          : declaration of the GUDev device monitor
 *
 */

#ifndef __HAL_GUDEV_MONITOR_H__
#define __HAL_GUDEV_MONITOR_H__

#include "services.h"
#include "hal-manager.h"

#include <gudev/gudev.h>

class GUDevMonitor:
  public Ekiga::Service,
  public Ekiga::HalManager
{
public:
  GUDevMonitor();

  ~GUDevMonitor();

  const std::string get_name () const
  { return "gudev"; }

  const std::string get_description () const
  { return "\tComponent monitoring Video4Linux devices using GUDev"; }

private:

  typedef struct {
    std::string framework;
    std::string name;
    int caps;
  } VideoInputDevice;
  friend void gudev_monitor_videoinput_uevent_handler (GUdevClient* client,
						       const gchar* action,
						       GUdevDevice* device,
						       GUDevMonitor* monitor);
  void videoinput_add (GUdevDevice* device);
  void videoinput_remove (GUdevDevice* device);
  std::vector<VideoInputDevice> videoinput_devices;
  GUdevClient* videoinput;
};

#endif
