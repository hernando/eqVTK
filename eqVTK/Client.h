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

#ifndef EQVTK_EQVTK_H
#define EQVTK_EQVTK_H

#include <eq/eq.h>

#include "Pipeline.h"

/**
   An example application that does VTK and Equalizer integration.
*/

namespace eqVTK
{

class InitData;
class Config;

class Client : public eq::Client
{
public:
    /* Public constructors and destructor */
    Client();

    virtual ~Client() {}

    /* Public member functions */
    int run();

    /**
       Invoked from Channel::initConfig to create a per channel pipeline
    */
    virtual PipelinePtr createPipeline(const InitData &initData) const = 0;

    virtual void initConfig(Config &config) = 0;
};

}
#endif

