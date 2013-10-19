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

#include "eqVTK.h"

#include "Config.h"

namespace eqVTK
{

eqVTK::eqVTK()
{
}

int eqVTK::run()
{
    /* Conecting to the server */
    eq::ServerPtr server = new eq::Server;
    if (!connectServer(server))
    {
        LBERROR << "Can't open server" << std::endl;
        return EXIT_FAILURE;
    }

    /* Choosing config and intialiazing */
    eq::fabric::ConfigParams configParams;
    Config* config = static_cast<Config*>(server->chooseConfig(configParams));

    if (!config)
    {
        LBERROR << "No matching config on server" << std::endl;
        disconnectServer(server);
        return EXIT_FAILURE;
    }

    if (!config->init())
    {
        server->releaseConfig(config);
        disconnectServer(server);
        return EXIT_FAILURE;
    }

    while (config->isRunning())
    {
        config->startFrame();
        config->finishFrame();

        while (!config->needRedraw())
        {
            if (hasCommands())
            {
                processCommand();
                config->handleEvents();
            }
            else
            {
                /* no pending commands, block on user event. */
                const eq::EventICommand& event = config->getNextEvent();
                if (!config->handleEvent(event))
                    LBVERB << "Unhandled " << event << std::endl;
            }
        }
        config->handleEvents(); // process all pending events
    }
    config->finishAllFrames();

    config->exit();

    server->releaseConfig(config);
    if( !disconnectServer(server))
        LBERROR << "Client::disconnectServer failed" << std::endl;

    return EXIT_SUCCESS;
}

}
