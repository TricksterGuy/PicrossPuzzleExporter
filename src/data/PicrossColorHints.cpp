#include "PicrossColorHints.hpp"

void PicrossColorHints::DrawBoard(wxDC& dc) const
{
    wxRect rect;
    dc.GetClippingBox(rect);
    wxSize size = rect.GetSize();

    auto [unused1, extra_hints_height, unused2, extra_hints_width] = CalculateHintBounds();
    // hints_width/height already removed via GetClippingBox.
    int w = std::min((size.GetWidth() - extra_hints_width) / width,
                     (size.GetHeight() - extra_hints_height) / height);

    dc.SetPen(*wxTRANSPARENT_PEN);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = data.Get(x, y);
            if (!index) continue;
            dc.SetBrush(wxBrush(palette[index-1]));
            dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
        }
    }
}
