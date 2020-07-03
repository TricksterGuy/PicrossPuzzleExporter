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
void WriteSolutionBoard(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c, std::vector<std::pair<int, int>> boards);
void WriteExtraHints(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int layer, int r, int c, int size);

void WriteClassicFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c);
void WriteGrayscaleFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c);
void WriteLightFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c);
void WritePaintingFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c);
int GetMaxHintsSize(const Picross& picross);

std::string GetColumnName(int column)
{
    std::stringstream ss;
    column += 1;
    while (column > 0)
    {
        column--;
        ss << static_cast<char>('A' + column % 26);
        column /= 26;
    }
    std::string s = ss.str();

    std::reverse(s.begin(), s.end());
    return s;
}

std::string GetCell(int column, int row)
{
    std::stringstream ss;
    ss << GetColumnName(column);
    ss << (row+1);
    return ss.str();
}

std::string GetRange(int column, int row, int width, int height)
{
    std::stringstream ss;
    ss << GetCell(column, row) << ":" << GetCell(column + width, row + height);
    return ss.str();
}

std::string GetLightFormula(const std::vector<std::pair<int, int>>& boards, int r, int c, int bpc)
{
    std::stringstream ss;
    ss << "=";
    for (unsigned int i = 0; i < boards.size(); i++)
    {
        auto [br, bc] = boards[i];
        ss << GetCell(bc + c, br + r);
        if (i != 0)
            ss << " * " << (1 << (bpc * i));
        if (i != boards.size() - 1)
            ss << " + ";
    }
    return ss.str();
}

std::string GetPaintingFormula(const std::vector<std::pair<int, int>>& boards, int r, int c, int bpc)
{
    std::stringstream ss;
    ss << "=";
    for (unsigned int i = 0; i < boards.size(); i++)
    {
        auto [br, bc] = boards[i];
        ss << GetCell(bc + c, br + r);
        if (i != 0)
            ss << " * " << (1 << i);
        if (i != boards.size() - 1)
            ss << " + ";
    }
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
    WriteClassicFormatting(workbook, worksheet, picross, offset, offset);
    WriteBoard(workbook, worksheet, picross, offset, offset);

    worksheet_set_column(worksheet, 0, offset + picross.GetWidth(), 2, nullptr);
}

void ExportXlsxGrayscale(lxw_workbook* workbook, const Picross& picross)
{
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, nullptr);

    int offset = std::max(2, GetMaxHintsSize(picross));

    WriteHints(workbook, worksheet, picross, 0, 0, 0, offset);
    WriteBoard(workbook, worksheet, picross, offset, offset);
    WriteGrayscaleFormatting(workbook, worksheet, picross, offset, offset);
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

    std::vector<std::pair<int, int>> boards = {
        {offset, offset},
        {offset, 2 * offset + w + extra + 1},
        {2 * offset + h + extra + 1, offset}
    };
    WriteSolutionBoard(  workbook, worksheet, picross   , 2 * offset + h + extra + 1, 2 * offset + w + extra + 1, boards);
    WriteLightFormatting(workbook, worksheet, picross   , 2 * offset + h + extra + 1, 2 * offset + w + extra + 1);

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


    std::vector<std::pair<int, int>> boards = {
        {offset, offset},
        {offset, 2 * offset + w + 1},
        {2 * offset + h + 1, offset},
        {2 * offset + h + 1, 2 * offset + w + 1},
        {3 * offset + 2 * (h + 1), offset}
    };
    WriteSolutionBoard(workbook, worksheet, picross   , 3 * offset + 2 * (h + 1), 2 * offset + w + 1, boards);
    WritePaintingFormatting(workbook, worksheet, picross   , 3 * offset + 2 * (h + 1), 2 * offset + w + 1);

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

void WriteSolutionBoard(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c, std::vector<std::pair<int, int>> boards)
{
    int bpc = picross.GetBpc();
    std::function<std::string(const std::vector<std::pair<int, int>>& boards, int r, int c, int bpc)> formula;
    if (picross.GetType() == PicrossPuzzle::TYPE_RGB)
        formula = GetLightFormula;
    else if (picross.GetType() == PicrossPuzzle::TYPE_RBY)
        formula = GetPaintingFormula;

    for (int i = 0; i < picross.GetWidth(); i++)
    {
        auto* top_border = workbook_add_format(workbook);
        auto* bottom_border = workbook_add_format(workbook);

        format_set_top(top_border, LXW_BORDER_THIN);
        format_set_align(top_border, LXW_ALIGN_CENTER);
        format_set_bottom(bottom_border, LXW_BORDER_THIN);
        format_set_align(bottom_border, LXW_ALIGN_CENTER);

        worksheet_write_formula_num(worksheet, r, c + i, formula(boards, 0, i, bpc).c_str(), top_border, 0);
        worksheet_write_formula_num(worksheet, r + picross.GetHeight() - 1, c + i, formula(boards, picross.GetHeight() - 1, i, bpc).c_str(), bottom_border, 0);
    }
    for (int i = 0; i < picross.GetHeight(); i++)
    {
        auto* left_border = workbook_add_format(workbook);
        auto* right_border = workbook_add_format(workbook);

        format_set_left(left_border, LXW_BORDER_THIN);
        format_set_align(left_border, LXW_ALIGN_CENTER);
        format_set_right(right_border, LXW_BORDER_THIN);
        format_set_align(right_border, LXW_ALIGN_CENTER);

        worksheet_write_formula_num(worksheet, r + i, c, formula(boards, i, 0, bpc).c_str(), left_border, 0);
        worksheet_write_formula_num(worksheet, r + i, c + picross.GetWidth() - 1, formula(boards, i, picross.GetWidth() - 1, bpc).c_str(), right_border, 0);
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

    worksheet_write_formula_num(worksheet, r, c, formula(boards, 0, 0, bpc).c_str(), lt_border, 0);
    worksheet_write_formula_num(worksheet, r + picross.GetHeight() - 1, c, formula(boards, picross.GetHeight() - 1, 0, bpc).c_str(), lb_border, 0);
    worksheet_write_formula_num(worksheet, r, c + picross.GetWidth() - 1, formula(boards, 0, picross.GetWidth() - 1, bpc).c_str(), rt_border, 0);
    worksheet_write_formula_num(worksheet, r + picross.GetHeight() - 1, c + picross.GetWidth() - 1, formula(boards, picross.GetHeight() - 1, picross.GetWidth() - 1, bpc).c_str(), rb_border, 0);


    auto* center = workbook_add_format(workbook);
    format_set_align(center, LXW_ALIGN_CENTER);
    for (int i = 1; i < picross.GetHeight() - 1; i++)
    {
        for (int j = 1; j < picross.GetWidth() - 1; j++)
            worksheet_write_formula_num(worksheet, r + i, c + j, formula(boards, i, j, bpc).c_str(), center, 0);
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

void WriteClassicFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c)
{
#ifdef ENABLE_EXPERIMENTAL_LIBXLSXWRITER
    auto* black = workbook_add_format(workbook);
    format_set_pattern(black, LXW_PATTERN_SOLID);
    format_set_bg_color(black, LXW_COLOR_BLACK);
    format_set_fg_color(black, LXW_COLOR_BLACK);
    auto* cf = lxw_conditional_format_new();
    lxw_conditional_format_cell(cf, LXW_CRITERIA_EQUAL, 1, black);
    worksheet_conditional_format(worksheet, r, c, r + picross.GetHeight() - 1, c + picross.GetWidth() - 1, cf);
#endif
}

void WriteGrayscaleFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c)
{
#ifdef ENABLE_EXPERIMENTAL_LIBXLSXWRITER
    int shadings = 1 << picross.GetBpc();
    for (int i = 1; i < shadings; i++)
    {
        unsigned int cval = 255 * (shadings - i) / shadings;
        unsigned int color = cval << 16 | cval << 8 | cval;

        auto* format = workbook_add_format(workbook);
        format_set_pattern(format, LXW_PATTERN_SOLID);
        format_set_bg_color(format, color);
        format_set_fg_color(format, color);
        auto* cf = lxw_conditional_format_new();
        lxw_conditional_format_cell(cf, LXW_CRITERIA_EQUAL, i, format);
        worksheet_conditional_format(worksheet, r, c, r + picross.GetHeight() - 1, c + picross.GetWidth() - 1, cf);
    }
#endif
}

void WriteLightFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int row, int column)
{
#ifdef ENABLE_EXPERIMENTAL_LIBXLSXWRITER
    int bpc = picross.GetBpc();
    int shadings = 1 << picross.GetBpc();
    int colorbits = 256 / (1 << bpc);

    for (int r = 0; r < shadings; r++)
    {
        for (int b = 0; b < shadings; b++)
        {
            for (int g = 0; g < shadings; g++)
            {
                unsigned int rval = r * colorbits;
                unsigned int gval = g * colorbits;
                unsigned int bval = b * colorbits;
                unsigned int color = rval << 16 | gval << 8 | bval;

                auto* format = workbook_add_format(workbook);
                format_set_pattern(format, LXW_PATTERN_SOLID);
                format_set_bg_color(format, color);
                format_set_fg_color(format, color);
                auto* cf = lxw_conditional_format_new();
                lxw_conditional_format_cell(cf, LXW_CRITERIA_EQUAL, r | g << bpc | b << (bpc*2), format);
                worksheet_conditional_format(worksheet, row, column, row + picross.GetHeight() - 1, column + picross.GetWidth() - 1, cf);
            }
        }
    }
#endif
}

void WritePaintingFormatting(lxw_workbook* workbook, lxw_worksheet* worksheet, const Picross& picross, int r, int c)
{
#ifdef ENABLE_EXPERIMENTAL_LIBXLSXWRITER
    constexpr uint8_t BLACK = 1;
    constexpr uint8_t WHITE = 2;
    constexpr uint8_t RED = 4;
    constexpr uint8_t BLUE = 8;
    constexpr uint8_t YELLOW = 16;
    constexpr uint8_t PURPLE = 12;
    constexpr uint8_t ORANGE = 20;
    constexpr uint8_t GREEN = 24;
    constexpr uint8_t GRAY = 28;

    struct {
        uint32_t r;
        uint32_t g;
        uint32_t b;
    } color = {0, 0, 0};

    for (int i = 1; i < 32; i++)
    {
        if ((i & GRAY) == GRAY)
            color = {128, 128, 128};
        else if ((i & PURPLE) == PURPLE)
            color = {128, 0, 128};
        else if ((i & ORANGE) == ORANGE)
            color = {255, 128, 0};
        else if ((i & GREEN) == GREEN)
            color = {0, 128, 0};
        else if ((i & RED) == RED)
            color = {255, 0, 0};
        else if ((i & BLUE) == BLUE)
            color = {0, 0, 255};
        else if ((i & YELLOW) == YELLOW)
            color = {255, 255, 0};
        else if (i == WHITE)
            color = {255, 255, 255};
        else if (i == BLACK)
            color = {0, 0, 0};

        if ((i & WHITE) == WHITE)
            color = {color.r / 2 + 128, color.g / 2 + 128, color.b / 2 + 128};
        if ((i & BLACK) == BLACK)
            color = {color.r / 2, color.g / 2, color.b / 2};

        uint32_t cval = color.r << 16 | color.g << 8 | color.b;
        auto* format = workbook_add_format(workbook);
        format_set_pattern(format, LXW_PATTERN_SOLID);
        format_set_bg_color(format, cval);
        format_set_fg_color(format, cval);
        auto* cf = lxw_conditional_format_new();
        lxw_conditional_format_cell(cf, LXW_CRITERIA_EQUAL, i, format);
        worksheet_conditional_format(worksheet, r, c, r + picross.GetHeight() - 1, c + picross.GetWidth() - 1, cf);
    }
#endif
}
