/******************************************************************************************************
 * Picross
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * excluding commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    An acknowledgement in your documentation and link to the original version is required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************************************/

#include "QRCodePanel.hpp"

QRCodePanel::QRCodePanel(QRcode* code, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :
    wxScrolledWindow(parent, id, pos, size, style), qr(code)
{
    FormBitmap();
}

QRCodePanel::~QRCodePanel()
{
    QRcode_free(qr);
}

void QRCodePanel::FormBitmap()
{
    wxImage image(qr->width, qr->width);
    for (int y = 0; y < qr->width; y++)
    {
        for (int x = 0; x < qr->width; x++)
        {
            if (qr->data[x + y * qr->width] & 1)
                image.SetRGB(x, y, 0, 0, 0);
            else
                image.SetRGB(x, y, 255, 255, 255);
        }
    }
    bitmap = wxBitmap(image);
    SetSize(qr->width, qr->width);
}


void QRCodePanel::OnDraw(wxDC& dc)
{
    dc.DrawBitmap(bitmap, 0, 0);
}
