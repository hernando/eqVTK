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

#ifndef EQVTK_EQVTKRENDERWINDOW_H
#define EQVTK_EQVTKRENDERWINDOW_H

#include "Window.h"

#include <vtkOpenGLRenderWindow.h>

namespace eqVTK
{

class vtkRenderWindow : public vtkOpenGLRenderWindow
{
public:
    /* Public static member functions */

    static vtkRenderWindow *New();

    /* Public member functions */

    void setWindow(eq::Window *window);

    virtual void OpenGLInit();

    virtual void OpenGLInitState();

    virtual void OpenGLInitContext();

    virtual void SetDisplayId(void*);
    virtual void SetWindowId(void*);
    virtual void SetParentId(void*);
    virtual void* GetGenericDisplayId();
    virtual void* GetGenericWindowId();
    virtual void* GetGenericParentId();
    virtual void* GetGenericContext();
    virtual void* GetGenericDrawable();
    virtual void SetWindowInfo(char*);
    virtual void SetParentInfo(char*);
    virtual int* GetScreenSize();
    virtual void MakeCurrent();
    virtual void Start();
    virtual void Finalize();
    virtual void Frame();
    virtual void HideCursor();
    virtual void ShowCursor();
    virtual void SetFullScreen(int);
    virtual void WindowRemap();
    virtual int GetEventPending();
    virtual void SetNextWindowId(void*);
    virtual void SetNextWindowInfo(char*);
    virtual bool IsCurrent();

    virtual void CreateAWindow();
    virtual void DestroyWindow();

private:
    /* Private member variables */

    eq::Window *_window;
    int _screenSize[2];

    /* Private constructors and destructor */

    vtkRenderWindow();

    virtual ~vtkRenderWindow() {}
};

}
#endif
