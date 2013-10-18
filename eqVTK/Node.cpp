/*
 * eqVTK
 * Copyright (C) 2013 Juan Hernando jhernando@fi.upm.es
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "Node.h"

#include "Config.h"

bool Node::configInit(const eq::uint128_t& initID)
{
    /* Running asynchronously */
    if (getIAttribute(IATTR_THREAD_MODEL) == eq::UNDEFINED)
        setIAttribute(IATTR_THREAD_MODEL, eq::ASYNC);

    if (!eq::Node::configInit(initID))
        return false;

    Config *config = static_cast<Config*>(getConfig());
    if (!isApplicationNode() &&
        !config->loadInitData(eq::UUID(initID)))
    {
        return false;
    }
    return true;
}

