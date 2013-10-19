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

#include "FrameData.h"

#include <co/dataOStream.h>
#include <co/dataIStream.h>

namespace eqVTK
{

FrameData::FrameData()
    : _rotation( eq::Matrix4f::ZERO )
    , _modelRotation( eq::Matrix4f::ZERO )
    , _position( eq::Vector3f::ZERO )
{
    reset();
}

void FrameData::serialize(co::DataOStream &os, const uint64_t dirtyBits)
{
    co::Serializable::serialize(os, dirtyBits);
    if (dirtyBits & DIRTY_CAMERA)
        os << _position << _rotation << _modelRotation;
}

void FrameData::deserialize(co::DataIStream &is, const uint64_t dirtyBits)
{
    co::Serializable::deserialize(is, dirtyBits);
    if (dirtyBits & DIRTY_CAMERA)
        is >> _position >> _rotation >> _modelRotation;
}

void FrameData::spinCamera(const float x, const float y)
{
    if (x == 0.0 && y == 0.0)
        return;

    _rotation.pre_rotate_x(x);
    _rotation.pre_rotate_y(y);
    setDirty(DIRTY_CAMERA);
}

void FrameData::spinModel(const float x, const float y, const float z)
{
    if (x == 0.0 && y == 0.0 && z == 0.0)
        return;

    _modelRotation.pre_rotate_x(x);
    _modelRotation.pre_rotate_y(y);
    _modelRotation.pre_rotate_z(z);
    setDirty(DIRTY_CAMERA );
}

void FrameData::moveCamera(const float x, const float y, const float z)
{
    _position.x() += x;
    _position.y() += y;
    _position.z() += z;
    setDirty(DIRTY_CAMERA );
}

void FrameData::setCameraPosition(const eq::Vector3f &position)
{
    _position = position;
    setDirty(DIRTY_CAMERA );
}

void FrameData::setRotation(const eq::Vector3f &rotation)
{
    _rotation = eq::Matrix4f::IDENTITY;
    _rotation.rotate_x(rotation.x());
    _rotation.rotate_y(rotation.y());
    _rotation.rotate_z(rotation.z());
    setDirty(DIRTY_CAMERA);
}

void FrameData::setModelRotation(const eq::Vector3f &rotation)
{
    _modelRotation = eq::Matrix4f::IDENTITY;
    _modelRotation.rotate_x(rotation.x());
    _modelRotation.rotate_y(rotation.y());
    _modelRotation.rotate_z(rotation.z());
    setDirty(DIRTY_CAMERA);
}

void FrameData::reset()
{
    eq::Matrix4f model = eq::Matrix4f::IDENTITY;
    model.rotate_x(static_cast<float>(-M_PI_2));
    model.rotate_y(static_cast<float>(-M_PI_2));

    if (_position == eq::Vector3f(0.f, 0.f, -2.f) &&
        _rotation == eq::Matrix4f::IDENTITY && _modelRotation == model)
    {
        _position.z() = 0.f;
    }
    else
    {
        _position = eq::Vector3f::ZERO;
        _position.z() = -2.0;
        _rotation = eq::Matrix4f::IDENTITY;
        _modelRotation = model;
    }
    setDirty(DIRTY_CAMERA);
}

}
