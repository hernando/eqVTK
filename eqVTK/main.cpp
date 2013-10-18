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
#include "Channel.h"
#include "Config.h"
#include "Node.h"
#include "Pipe.h"
#include "Window.h"

class NodeFactory : public eq::NodeFactory
{
public:
    virtual eq::Config *createConfig(eq::ServerPtr parent)
    {
        return new Config(parent);
    }
    virtual eq::Node *createNode(eq::Config *parent)
    {
        return new Node(parent);
    }
    virtual eq::Pipe *createPipe(eq::Node *parent)
    {
        return new Pipe(parent);
    }
    virtual eq::Window *createWindow(eq::Pipe *parent)
    {
        return new Window(parent);
    }
    virtual eq::Channel *createChannel(eq::Window *parent)
    {
        return new Channel(parent);
    }
};

int main(int argc, char *argv[])
{
    NodeFactory factory;

    if (!eq::init(argc, argv, &factory))
    {
        LBERROR << "Equalizer init failed" << std::endl;
        return EXIT_FAILURE;
    }

    InitData initData;

    /* Initializing local client node */
    lunchbox::RefPtr<eqVTK> client(new eqVTK());
    if (!client->initLocal(argc, argv))
    {
        LBERROR << "Can't init client" << std::endl;
        eq::exit();
        return EXIT_FAILURE;
    }

    client->run();

    client->exitLocal();
    client = 0;

    eq::exit();

}
