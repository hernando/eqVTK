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

#ifndef EQVTK_INITDATA_H
#define EQVTK_INITDATA_H

#include <eq/client/types.h>
#include <co/object.h>

namespace eqVTK
{

class InitData : public co::Object
{
public:
    /* Public member variables */

    eq::UUID frameDataID;

    /* Public constructors and destructor */

    InitData();
    ~InitData();

protected:
    /* Protected member functions */
    virtual void getInstanceData(co::DataOStream &os);
    virtual void applyInstanceData(co::DataIStream &is);
};

}
#endif

