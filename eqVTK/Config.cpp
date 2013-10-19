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

#include "Config.h"

namespace eqVTK
{

Config::Config(eq::ServerPtr parent)
    : eq::Config(parent)
    , _spinX(0)
    , _spinY(0)
    , _advance(0)
    , _redraw(true)
{
}

Config::~Config()
{
}

bool Config::init()
{
    /* Distributed objects */

    registerObject(&_frameData);
    _frameData.setAutoObsolete(getLatency());

    _initData.frameDataID = _frameData.getID();
    registerObject(&_initData);

    /* Init config */

    if (!eq::Config::init(_initData.getID()))
    {
        _deregisterDistributedObjects();
        return false;
    }

    return true;
}

bool Config::exit()
{
    const bool ret = eq::Config::exit();
    _deregisterDistributedObjects();

    return ret;
}

void Config::_deregisterDistributedObjects()
{
    deregisterObject(&_initData);
    deregisterObject(&_frameData);

    _initData.frameDataID = eq::UUID();
}

bool Config::loadInitData(const eq::UUID& id)
{
    LBASSERT(!_initData.isAttached());
#if CO_VERSION_LT(1,1,1)
    if (!mapObject(&_initData, id))
        return false;
    /* This data is static, we can unmap immediately */
    unmapObject(&_initData);
#else
    if (!syncObject(&_initData, getApplicationNode(), id))
            return false;
#endif
    return true;
}

uint32_t Config::startFrame()
{
    const eq::uint128_t& version = _frameData.commit();

    _redraw = false;
    return eq::Config::startFrame(version);
}


bool Config::needRedraw()
{
    return (_spinX != 0 || _spinY != 0 || _redraw);
}

bool Config::handleEvent(const eq::ConfigEvent* event)
{
    switch (event->data.type)
    {
        case eq::Event::CHANNEL_POINTER_BUTTON_RELEASE:
        {
            const eq::PointerEvent& releaseEvent =
                event->data.pointerButtonRelease;
            if (releaseEvent.buttons == eq::PTR_BUTTON_NONE)
            {
                if (releaseEvent.button == eq::PTR_BUTTON1)
                {
                    _spinX = releaseEvent.dy;
                    _spinY = releaseEvent.dx;
                    _redraw = true;
                    return true;
                }
                if (releaseEvent.button == eq::PTR_BUTTON2)
                {
                    _advance = -releaseEvent.dy;
                    _redraw = true;
                    return true;
                }
            }
            break;
        }
        case eq::Event::CHANNEL_POINTER_MOTION:
        {
            switch (event->data.pointerMotion.buttons)
            {
              case eq::PTR_BUTTON1:
                  _spinX = 0;
                  _spinY = 0;
                  _frameData.spinModel(
                      -0.005 * event->data.pointerMotion.dy,
                      -0.005 * event->data.pointerMotion.dx, 0);
                  _redraw = true;
                  return true;

              case eq::PTR_BUTTON2:
                  _frameData.moveCamera(0.0005 * event->data.pointerMotion.dx,
                                        -0.0005 * event->data.pointerMotion.dy,
                                        0);
                  _redraw = true;
                  return true;

              case eq::PTR_BUTTON3:
                  _advance = event->data.pointerMotion.dy;
                  _frameData.moveCamera(0, 0, 0.005 * _advance);
                  _redraw = true;
                  return true;
            }
            break;
        }

        case eq::Event::CHANNEL_POINTER_WHEEL:
        {
            _frameData.moveCamera(0.05 * event->data.pointerWheel.yAxis,
                                  0,
                                  0.0f * event->data.pointerWheel.xAxis);
            _redraw = true;
            return true;
        }
        case eq::Event::WINDOW_EXPOSE:
        case eq::Event::WINDOW_RESIZE:
        case eq::Event::WINDOW_CLOSE:
        case eq::Event::VIEW_RESIZE:
            _redraw = true;
            break;

        default:
            break;
    }

    _redraw |= eq::Config::handleEvent(event);
    return _redraw;
}

bool Config::handleEvent(eq::EventICommand command)
{
    _redraw |= eq::Config::handleEvent(command);
    return _redraw;
}

}
