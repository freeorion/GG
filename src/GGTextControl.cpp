/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@hotmail.com */

/* $Header$ */

#include "GGTextControl.h"

#include <GGApp.h>
#include <GGDrawUtil.h>
#include <GGWndEditor.h>

using namespace GG;

////////////////////////////////////////////////
// GG::TextControl
////////////////////////////////////////////////
TextControl::TextControl() :
    Control(),
    m_dirty_load(false)
{
}

TextControl::TextControl(int x, int y, int w, int h, const std::string& str, const boost::shared_ptr<Font>& font, Clr color/* = CLR_BLACK*/,
                         Uint32 text_fmt/* = 0*/, Uint32 flags/* = 0*/) :
    Control(x, y, w, h, flags),
    m_format(text_fmt),
    m_text_color(color),
    m_clip_text(false),
    m_set_min_size(false),
    m_font(font),
    m_fit_to_text(false),
    m_dirty_load(false)
{
    ValidateFormat();
    SetText(str);
}

TextControl::TextControl(int x, int y, int w, int h, const std::string& str, const std::string& font_filename, int pts, Clr color/* = CLR_BLACK*/,
                         Uint32 text_fmt/* = 0*/, Uint32 flags/* = 0*/) :
    Control(x, y, w, h, flags),
    m_format(text_fmt),
    m_text_color(color),
    m_clip_text(false),
    m_set_min_size(false),
    m_font(App::GetApp()->GetFont(font_filename, pts)),
    m_fit_to_text(false),
    m_dirty_load(false)
{
    ValidateFormat();
    SetText(str);
}

TextControl::TextControl(int x, int y, const std::string& str, const boost::shared_ptr<Font>& font, Clr color/* = CLR_BLACK*/, Uint32 text_fmt/* = 0*/,
                         Uint32 flags/* = 0*/) :
    Control(x, y, 0, 0, flags),
    m_format(text_fmt),
    m_text_color(color),
    m_clip_text(false),
    m_set_min_size(false),
    m_font(font),
    m_fit_to_text(true),
    m_dirty_load(false)
{
    ValidateFormat();
    SetText(str);
}

TextControl::TextControl(int x, int y, const std::string& str, const std::string& font_filename, int pts, Clr color/* = CLR_BLACK*/, Uint32 text_fmt/* = 0*/,
                         Uint32 flags/* = 0*/) :
    Control(x, y, 0, 0, flags),
    m_format(text_fmt),
    m_text_color(color),
    m_clip_text(false),
    m_set_min_size(false),
    m_font(App::GetApp()->GetFont(font_filename, pts)),
    m_fit_to_text(true),
    m_dirty_load(false)
{
    ValidateFormat();
    SetText(str);
}

Uint32 TextControl::TextFormat() const
{
    return m_format;
}

Clr TextControl::TextColor() const
{
    return m_text_color;
}

bool TextControl::ClipText() const
{
    return m_clip_text;
}

bool TextControl::SetMinSize() const
{
    return m_set_min_size;
}

TextControl::operator const std::string&() const
{
    return Control::m_text;
}

bool TextControl::Empty() const
{
    return Control::m_text.empty();
}

int TextControl::Length() const
{
    return Control::m_text.length();
}

Pt TextControl::TextUpperLeft() const
{
    return UpperLeft() + m_text_ul;
}

Pt TextControl::TextLowerRight() const
{
    return UpperLeft() + m_text_lr;
}

void TextControl::Render()
{
    if (m_dirty_load)
        SetText(WindowText());
    Clr clr_to_use = Disabled() ? DisabledColor(TextColor()) : TextColor();
    glColor4ubv(clr_to_use.v);
    if (m_font) {
        if (m_clip_text)
            BeginClipping();
        m_font->RenderText(UpperLeft(), LowerRight(), m_text, m_format, &m_line_data);
        if (m_clip_text)
            EndClipping();
    }
}

void TextControl::SetText(const std::string& str)
{
    Control::m_text = str;
    if (m_font) {
        Pt text_sz = m_font->DetermineLines(WindowText(), m_format, ClientSize().x, m_line_data);
        m_text_ul = Pt();
        m_text_lr = text_sz;
        AdjustMinimumSize();
        if (m_fit_to_text) {
            Resize(text_sz);
        } else {
            RecomputeTextBounds();
        }
    }
    m_dirty_load = false;
}

void TextControl::SetText(const char* str)
{
    SetText(std::string(str));
}

void TextControl::SizeMove(int x1, int y1, int x2, int y2)
{
    Wnd::SizeMove(x1, y1, x2, y2);
    RecomputeTextBounds();
}

void TextControl::SetTextFormat(Uint32 format)
{
    m_format = format;
    ValidateFormat();
    SetText(WindowText());
}

void TextControl::SetTextColor(Clr color)
{
    m_text_color = color;
}

void TextControl::SetColor(Clr c)
{
    Control::SetColor(c);
    m_text_color = c;
}

void TextControl::ClipText(bool b)
{
    m_clip_text = b;
}

void TextControl::SetMinSize(bool b)
{
    m_set_min_size = b;
    AdjustMinimumSize();
}

void TextControl::operator+=(const std::string& str)
{
    SetText(Control::m_text + str);
}

void TextControl::operator+=(const char* str)
{
    SetText(Control::m_text + str);
}

void TextControl::operator+=(char ch)
{
    SetText(Control::m_text + ch);
}

void TextControl::Clear()
{
    SetText("");
}

void TextControl::Insert(int pos, char ch)
{
    Control::m_text.insert(pos, 1, ch);
    SetText(Control::m_text);
}

void TextControl::Erase(int pos, int num/* = 1*/)
{
    Control::m_text.erase(pos, num);
    SetText(Control::m_text);
}

void TextControl::DefineAttributes(WndEditor* editor)
{
    if (!editor)
        return;
    Control::DefineAttributes(editor);
    editor->Label("TextControl");
    editor->BeginFlags(m_format);
    editor->FlagGroup("V. Alignment", TF_VCENTER, TF_BOTTOM);
    editor->FlagGroup("H. Alignment", TF_CENTER, TF_RIGHT);
    editor->Flag("Word-break", TF_WORDBREAK);
    editor->Flag("Line-wrap", TF_LINEWRAP);
    editor->Flag("Ignore Tags", TF_IGNORETAGS);
    editor->EndFlags();
    editor->Attribute("Text Color", m_text_color);
    editor->Attribute("Font", m_font);
    editor->Attribute("Fit Size to Text", m_fit_to_text);
}

const std::vector<Font::LineData>& TextControl::GetLineData() const
{
    return m_line_data;
}

const boost::shared_ptr<Font>& TextControl::GetFont() const
{
    return m_font;
}

bool TextControl::FitToText() const
{
    return m_fit_to_text;
}

bool TextControl::DirtyLoad() const
{
    return m_dirty_load;
}

void TextControl::ValidateFormat()
{
    int dup_ct = 0;   // duplication count
    if (m_format & TF_LEFT) ++dup_ct;
    if (m_format & TF_RIGHT) ++dup_ct;
    if (m_format & TF_CENTER) ++dup_ct;
    if (dup_ct != 1) {   // exactly one must be picked; when none or multiples are picked, use TF_CENTER by default
        m_format &= ~(TF_RIGHT | TF_LEFT);
        m_format |= TF_CENTER;
    }
    dup_ct = 0;
    if (m_format & TF_TOP) ++dup_ct;
    if (m_format & TF_BOTTOM) ++dup_ct;
    if (m_format & TF_VCENTER) ++dup_ct;
    if (dup_ct != 1) {   // exactly one must be picked; when none or multiples are picked, use TF_VCENTER by default
        m_format &= ~(TF_TOP | TF_BOTTOM);
        m_format |= TF_VCENTER;
    }
    if ((m_format & TF_WORDBREAK) && (m_format & TF_LINEWRAP))   // only one of these can be picked; TF_WORDBREAK overrides TF_LINEWRAP
        m_format &= ~TF_LINEWRAP;
}

void TextControl::AdjustMinimumSize()
{
    if (m_set_min_size) {
        Pt text_sz = TextLowerRight() - TextUpperLeft();
        Pt curent_min = MinSize();
        SetMinSize(std::max(text_sz.x, curent_min.x),
                   std::max(text_sz.y, curent_min.y));
    }
}

void TextControl::RecomputeTextBounds()
{
    Pt text_sz = TextLowerRight() - TextUpperLeft();
    m_text_ul.y = 0; // default value for TF_TOP
    if (m_format & TF_BOTTOM)
        m_text_ul.y = Size().y - text_sz.y;
    else if (m_format & TF_VCENTER)
        m_text_ul.y = static_cast<int>((Size().y - text_sz.y) / 2.0);
    m_text_ul.x = 0; // default for TF_LEFT
    if (m_format & TF_RIGHT)
        m_text_ul.x = Size().x - text_sz.x;
    else if (m_format & TF_CENTER)
        m_text_ul.x = static_cast<int>((Size().x - text_sz.x) / 2.0);
    m_text_lr = m_text_ul + text_sz;
}
