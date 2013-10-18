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

#ifndef EQVTK_CONFIG_H
#define EQVTK_CONFIG_H

#include "FrameData.h"
#include "InitData.h"

#include <eq/eq.h>

/**
 * The Config handles frame data and events
 */
class Config : public eq::Config
{
public:
    /* Public constructors/destructor */

    Config(eq::ServerPtr parent);

    /* Public member fuctions */

    /** @sa eq::Config::init. */
    virtual bool init();
    /** @sa eq::Config::exit. */
    virtual bool exit();

    /** @sa eq::Config::startFrame. */
    virtual uint32_t startFrame();

    const InitData& getInitData() const
    {
        return _initData;
    }

    /** Map per-config data to the local node process

        Invoked from Node::configInit
     */
    bool loadInitData(const eq::UUID& initDataID);

    /** @sa eq::Config::handleEvent */
    virtual bool handleEvent( const eq::ConfigEvent* event );
    virtual bool handleEvent( eq::EventICommand command );

    /** @return true if an event required a redraw. */
    bool needRedraw();

protected:
    /* Public member fuctions */

    virtual ~Config();

private:
    /* Private member variables */

    int _spinX, _spinY;
    int _advance;

    InitData _initData;
    FrameData _frameData;

    bool _redraw;

    /* Private member functions */

    void _deregisterDistributedObjects();
};

#endif
