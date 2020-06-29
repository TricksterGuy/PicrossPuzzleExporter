#include "XlsxExporter.hpp"

#include <algorithm>
#include <sstream>
#include <xlsxwriter.h>

void ExportXlsxClassic(lxw_workbook* workbook, const Picross& picross);
void ExportXlsxGrayscale(lxw_workbook* workbook, const Picross& picross);
void ExportXlsxPainting(lxw_workbook* workbook, const Picross& picross);
void ExportXlsxLight(lxw_workbook* workbook, const Picross& picross);
void WriteType(lxw_workbook* workbook, lxw_worksheet* worksheet, const std::string& type, int r, int c, int size);
void WriteHints(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int layer, int r, int c, int size);
void WriteBoard(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c);
void WriteExtraHints(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int layer, int r, int c, int size);
int GetMaxHintsSize(const Picross& picross);

std::string GetRange(int column, int row, int width, int height)
{
    std::stringstream ss;
    if (column >= 26)
        ss << static_cast<char>(column / 26 - 1 + 'A');
    ss << static_cast<char>(column % 26 + 'A');
    ss << (row + 1) << ":";
    int end_column = column + width;
    if (end_column >= 26)
        ss << static_cast<char>(end_column / 26 - 1 + 'A');
    ss << static_cast<char>(end_column % 26 + 'A');
    ss << (row + 1 + height);
    return ss.str();
}

void ExportXlsx(const std::string& path, const Picross& picross)
{
    lxw_workbook* workbook  = workbook_new(path.c_str());

    switch(picross.GetType())
    {
        case PicrossPuzzle::TYPE_BW:
            ExportXlsxClassic(workbook, picross);
            break;
        case PicrossPuzzle::TYPE_GRAY:
            ExportXlsxGrayscale(workbook, picross);
            break;
        case PicrossPuzzle::TYPE_RGB:
            ExportXlsxLight(workbook, picross);
            break;
        case PicrossPuzzle::TYPE_RBY:
            ExportXlsxPainting(workbook, picross);
            break;
        default:
            workbook_close(workbook);
            return;
    }

    /* Save the workbook and free any allocated memory. */
    workbook_close(workbook);
}

void ExportXlsxClassic(lxw_workbook* workbook, const Picross& picross)
{
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, nullptr);

    int offset = GetMaxHintsSize(picross);

    WriteHints(workbook, worksheet, picross, 0, 0, 0, offset);
    WriteBoard(workbook, worksheet, picross, offset, offset);

    worksheet_set_column(worksheet, 0, offset + picross.GetWidth(), 2, nullptr);
}

void ExportXlsxGrayscale(lxw_workbook* workbook, const Picross& picross)
{
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, nullptr);

    int offset = std::max(2, GetMaxHintsSize(picross));

    WriteHints(workbook, worksheet, picross, 0, 0, 0, offset);
    WriteBoard(workbook, worksheet, picross, offset, offset);
    if (picross.GetBpc() > 1)
        WriteExtraHints(workbook, worksheet, picross, 0, offset, offset, offset);

    worksheet_set_column(worksheet, 0, offset + picross.GetWidth() + (1 << picross.GetBpc()), 2, nullptr);
}

void ExportXlsxLight(lxw_workbook* workbook, const Picross& picross)
{
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, nullptr);

    int offset = std::max(2, GetMaxHintsSize(picross));
    int bpc = picross.GetBpc();
    int extra = bpc > 1 ? (1 << bpc) : 0;
    int w = picross.GetWidth();
    int h = picross.GetHeight();

    WriteType(workbook,  worksheet, "Red",    0, 0, offset);
    WriteHints(workbook, worksheet, picross, 0, 0, 0, offset);
    WriteBoard(workbook, worksheet, picross   , offset, offset);

    WriteType(workbook,  worksheet, "Green",     0, offset + w + extra + 1, offset);
    WriteHints(workbook, worksheet, picross, 1,  0, offset + w + extra + 1, offset);
    WriteBoard(workbook, worksheet, picross   , offset, 2 * offset + w + extra + 1);

    WriteType(workbook,  worksheet, "Blue",      offset + h + extra + 1, 0, offset);
    WriteHints(workbook, worksheet, picross, 2, offset + h + extra + 1, 0, offset);
    WriteBoard(workbook, worksheet, picross   , 2 * offset + h + extra + 1, offset);

    if (picross.GetBpc() > 1)
    {
        WriteExtraHints(workbook, worksheet, picross, 0, offset, offset, offset);
        WriteExtraHints(workbook, worksheet, picross, 1, offset, 2 * offset + w + extra + 1, offset);
        WriteExtraHints(workbook, worksheet, picross, 2, 2 * offset + h + extra + 1, offset, offset);
    }

    WriteBoard(workbook, worksheet, picross   , 2 * offset + h + extra + 1, 2 * offset + w + extra + 1);

    worksheet_set_column(worksheet, 0, 2 * offset + 2 * (w + extra) + 1, 2, nullptr);
}

void ExportXlsxPainting(lxw_workbook* workbook, const Picross& picross)
{
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, nullptr);

    int offset = std::max(3, GetMaxHintsSize(picross));
    int w = picross.GetWidth();
    int h = picross.GetHeight();

    WriteType(workbook,  worksheet, "Black",    0, 0, offset);
    WriteHints(workbook, worksheet, picross, 0, 0, 0, offset);
    WriteBoard(workbook, worksheet, picross   , offset, offset);

    WriteType(workbook,  worksheet, "White",     0, offset + w + 1, offset);
    WriteHints(workbook, worksheet, picross, 1,  0, offset + w + 1, offset);
    WriteBoard(workbook, worksheet, picross   , offset, 2 * offset + w + 1);

    WriteType(workbook,  worksheet, "Red",      offset + h + 1, 0, offset);
    WriteHints(workbook, worksheet, picross, 2, offset + h + 1, 0, offset);
    WriteBoard(workbook, worksheet, picross   , 2 * offset + h + 1, offset);

    WriteType(workbook,  worksheet, "Blue",     offset + h + 1, offset + w + 1, offset);
    WriteHints(workbook, worksheet, picross, 3, offset + h + 1, offset + w + 1, offset);
    WriteBoard(workbook, worksheet, picross   , 2 * offset + h + 1, 2 * offset + w + 1);

    WriteType(workbook,  worksheet, "Yellow",   2 * offset + 2 * (h + 1), 0, offset);
    WriteHints(workbook, worksheet, picross, 4, 2 * offset + 2 * (h + 1), 0, offset);
    WriteBoard(workbook, worksheet, picross   , 3 * offset + 2 * (h + 1), offset);

    WriteBoard(workbook, worksheet, picross   , 3 * offset + 2 * (h + 1), 2 * offset + w + 1);

    worksheet_set_column(worksheet, 0, 2 * offset + 2 * w + 2, 2, nullptr);
}

int GetMaxHintsSize(const Picross& picross)
{
    unsigned int m = 1;
    for (int k = 0; k < picross.GetLayers(); k++)
    {
        const auto& rows = picross.GetRowHints().at(k);
        const auto& columns = picross.GetColumnHints().at(k);

        for (const auto& hints : columns)
            m = std::max(m, static_cast<unsigned int>(hints.size()));
        for (const auto& hints : rows)
            m = std::max(m, static_cast<unsigned int>(hints.size()));
    }
    return m;
}

void WriteType(lxw_workbook* workbook, lxw_worksheet* worksheet, const std::string& type, int r, int c, int size)
{
    auto* format = workbook_add_format(workbook);
    format_set_align(format, LXW_ALIGN_CENTER);
    format_set_align(format, LXW_ALIGN_VERTICAL_CENTER);
    format_set_top(format, LXW_BORDER_THIN);
    format_set_bottom(format, LXW_BORDER_THIN);
    format_set_left(format, LXW_BORDER_THIN);
    format_set_right(format, LXW_BORDER_THIN);
    worksheet_merge_range(worksheet, r, c, r + size-1, c + size-1, type.c_str(), format);
}

void WriteHints(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int layer, int r, int c, int size)
{
    const auto& rows = picross.GetRowHints().at(layer);
    const auto& columns = picross.GetColumnHints().at(layer);

    auto* center = workbook_add_format(workbook);
    format_set_align(center, LXW_ALIGN_CENTER);
    for (unsigned int i = 0; i < columns.size(); i++)
    {
        for (unsigned int j = 0; j < columns[i].size(); j++)
        {
            worksheet_write_number(worksheet, r + size - columns[i].size() + j, c + size + i, columns[i][j], center);
        }
    }

    for (unsigned int i = 0; i < rows.size(); i++)
    {
        for (unsigned int j = 0; j < rows[i].size(); j++)
        {
            worksheet_write_number(worksheet, r + size + i, c + size - rows[i].size() + j, rows[i][j], center);
        }
    }
}

void WriteBoard(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c)
{
    for (int i = 0; i < picross.GetWidth(); i++)
    {
        auto* top_border = workbook_add_format(workbook);
        auto* bottom_border = workbook_add_format(workbook);

        format_set_top(top_border, LXW_BORDER_THIN);
        format_set_align(top_border, LXW_ALIGN_CENTER);
        format_set_bottom(bottom_border, LXW_BORDER_THIN);
        format_set_align(bottom_border, LXW_ALIGN_CENTER);

        worksheet_write_blank(worksheet, r, c + i, top_border);
        worksheet_write_blank(worksheet, r + picross.GetHeight() - 1, c + i, bottom_border);
    }
    for (int i = 0; i < picross.GetHeight(); i++)
    {
        auto* left_border = workbook_add_format(workbook);
        auto* right_border = workbook_add_format(workbook);

        format_set_left(left_border, LXW_BORDER_THIN);
        format_set_align(left_border, LXW_ALIGN_CENTER);
        format_set_right(right_border, LXW_BORDER_THIN);
        format_set_align(right_border, LXW_ALIGN_CENTER);

        worksheet_write_blank(worksheet, r + i, c, left_border);
        worksheet_write_blank(worksheet, r + i, c + picross.GetWidth() - 1, right_border);
    }

    auto* lt_border = workbook_add_format(workbook);
    auto* rt_border = workbook_add_format(workbook);
    auto* lb_border = workbook_add_format(workbook);
    auto* rb_border = workbook_add_format(workbook);

    format_set_left(lt_border, LXW_BORDER_THIN);
    format_set_top(lt_border, LXW_BORDER_THIN);
    format_set_align(lt_border, LXW_ALIGN_CENTER);

    format_set_left(lb_border, LXW_BORDER_THIN);
    format_set_bottom(lb_border, LXW_BORDER_THIN);
    format_set_align(lb_border, LXW_ALIGN_CENTER);

    format_set_right(rt_border, LXW_BORDER_THIN);
    format_set_top(rt_border, LXW_BORDER_THIN);
    format_set_align(rt_border, LXW_ALIGN_CENTER);

    format_set_right(rb_border, LXW_BORDER_THIN);
    format_set_bottom(rb_border, LXW_BORDER_THIN);
    format_set_align(rb_border, LXW_ALIGN_CENTER);

    worksheet_write_blank(worksheet, r, c, lt_border);
    worksheet_write_blank(worksheet, r + picross.GetHeight() - 1, c, lb_border);
    worksheet_write_blank(worksheet, r, c + picross.GetWidth() - 1, rt_border);
    worksheet_write_blank(worksheet, r + picross.GetHeight() - 1, c + picross.GetWidth() - 1, rb_border);


    auto* center = workbook_add_format(workbook);
    format_set_align(center, LXW_ALIGN_CENTER);
    for (int i = 1; i < picross.GetHeight() - 1; i++)
    {
        for (int j = 1; j < picross.GetWidth() - 1; j++)
            worksheet_write_blank(worksheet, r + i, c + j, center);
    }
}

void WriteExtraHints(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int layer, int r, int c, int size)
{
    const auto& rows = picross.GetRowExtraHints().at(layer);
    const auto& shading_rows = picross.GetRowShadingHints().at(layer);
    const auto& columns = picross.GetColumnExtraHints().at(layer);
    const auto& shading_columns = picross.GetColumnShadingHints().at(layer);

    auto* center = workbook_add_format(workbook);
    format_set_align(center, LXW_ALIGN_CENTER);

    unsigned int shadings = (1 << picross.GetBpc()) - 1;

    for (unsigned int i = 0; i < columns.size(); i++)
    {
        for (unsigned int j = 0; j < shadings; j++)
        {
            std::stringstream ss;
            ss << "=" << columns[i][j] << "-COUNTIF(" << GetRange(c + i, r, 0, picross.GetHeight() - 1) << "," << (j + 1) << ")";
            worksheet_write_formula_num(worksheet, r + picross.GetHeight() + j, c + i, ss.str().c_str(), center, columns[i][j]);

        }
        worksheet_write_number(worksheet, r + picross.GetHeight() + shadings, c + i, shading_columns[i], center);
    }

    for (unsigned int i = 0; i < rows.size(); i++)
    {
        for (unsigned int j = 0; j < shadings; j++)
        {
            std::stringstream ss;
            ss << "=" << rows[i][j] << "-COUNTIF(" << GetRange(c, r + i, picross.GetWidth() - 1, 0) << "," << (j + 1) << ")";
            worksheet_write_formula_num(worksheet, r + i, c + picross.GetWidth() + j, ss.str().c_str(), center, rows[i][j]);
        }
        worksheet_write_number(worksheet, r + i, c + picross.GetWidth() + shadings, shading_rows[i], center);
    }
}
