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

#include "vtkRenderWindow.h"

namespace eqVTK
{

vtkRenderWindow *vtkRenderWindow::New(eq::Window *window)
{
    vtkRenderWindow *w = new vtkRenderWindow(window);
    return w;
}

vtkRenderWindow::vtkRenderWindow(eq::Window *window)
    : _window(window)
{
    const eq::PixelViewport &pvp = _window->getPixelViewport();
    Size[0] = pvp.w;
    Size[1] = pvp.h;
}

void vtkRenderWindow::OpenGLInit()
{
}

void vtkRenderWindow::OpenGLInitState()
{
}

void vtkRenderWindow::OpenGLInitContext()
{
}

void vtkRenderWindow::SetDisplayId(void*)
{
}

void vtkRenderWindow::SetWindowId(void*)
{
}

void vtkRenderWindow::SetParentId(void*)
{
}

void* vtkRenderWindow::GetGenericDisplayId()
{
    return 0;
}

void* vtkRenderWindow::GetGenericWindowId()
{
    return 0;
}

void* vtkRenderWindow::GetGenericParentId()
{
    return 0;
}

void* vtkRenderWindow::GetGenericContext()
{
    return 0;
}

void* vtkRenderWindow::GetGenericDrawable()
{
    return 0;
}

void vtkRenderWindow::SetWindowInfo(char*)
{
}

void vtkRenderWindow::SetParentInfo(char*)
{
}

int* vtkRenderWindow::GetScreenSize()
{
    const eq::PixelViewport &pvp = _window->getPixelViewport();
    _screenSize[0] = pvp.w;
    _screenSize[1] = pvp.h;
    return _screenSize;
}

void vtkRenderWindow::MakeCurrent()
{
}

void vtkRenderWindow::Start()
{
}

void vtkRenderWindow::Finalize()
{
}

void vtkRenderWindow::Frame()
{
}

void vtkRenderWindow::HideCursor()
{
}

void vtkRenderWindow::ShowCursor()
{
}

void vtkRenderWindow::SetFullScreen(int)
{
}

void vtkRenderWindow::WindowRemap()
{
}

int vtkRenderWindow::GetEventPending()
{
    return 0;
}

void vtkRenderWindow::SetNextWindowId(void*)
{
}

void vtkRenderWindow::SetNextWindowInfo(char*)
{
}

bool vtkRenderWindow::IsCurrent()
{
    return true;
}

void vtkRenderWindow::CreateAWindow()
{
}

void vtkRenderWindow::DestroyWindow()
{
}

}
