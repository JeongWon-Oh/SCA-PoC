/*
 * This file is part of the CacheSC library (https://github.com/Miro-H/CacheSC),
 * which implements Prime+Probe attacks on virtually and physically indexed
 * caches.
 *
 * Copyright (C) 2020  Miro Haller
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact: miro.haller@alumni.ethz.ch
 *
 * Short description of this file:
 * This file contains all device specific parameters. They should be adapted
 * to the execution environment for best performance. Especially the cache level
 * sizes are crucial for the attack to work.
 */

#ifndef HEADER_DEVICE_CONF_H
#define HEADER_DEVICE_CONF_H

// General settings
#define PAGE_SIZE 4096
#define PROCESSOR_FREQ 3500000000

// Cache related settings
#define CACHELINE_SIZE 64
#define CACHE_GROUP_SIZE (PAGE_SIZE / CACHELINE_SIZE)

// Addressing:
// - virtual:   0
// - physical:  1
#define L1_ADDRESSING 0
#define L1_SETS 32
#define L1_ASSOCIATIVITY 8
#define L1_ACCESS_TIME 1

#define L2_ADDRESSING 0
#define L2_SETS 512
#define L2_ASSOCIATIVITY 16
#define L2_ACCESS_TIME 14

#define L3_ADDRESSING 0
#define L3_SETS 1024
#define L3_ASSOCIATIVITY 16
#define L3_ACCESS_TIME 44

#endif // HEADER_DEVICE_CONF_H
