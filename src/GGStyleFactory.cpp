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

#include "GGStyleFactory.h"

#include "GGButton.h"
#include "GGDropDownList.h"
#include "GGDynamicGraphic.h"
#include "GGEdit.h"
#include "GGListBox.h"
#include "GGMenu.h"
#include "GGMultiEdit.h"
#include "GGScroll.h"
#include "GGSlider.h"
#include "GGSpin.h"
#include "GGStaticGraphic.h"
#include "GGTextControl.h"

#include "GGColorDlg.h"
#include "GGFileDlg.h"
#include "GGThreeButtonDlg.h"

using namespace GG;

StyleFactory::StyleFactory()
{}

StyleFactory::~StyleFactory()
{}

Button* StyleFactory::NewButton(int x, int y, int w, int h, const std::string& str, const boost::shared_ptr<Font>& font,
                                Clr color, Clr text_color/* = CLR_BLACK*/, Uint32 flags/* = CLICKABLE*/) const
{
    return new Button(x, y, w, h, str, font, color, text_color, flags);
}

StateButton* StyleFactory::NewStateButton(int x, int y, int w, int h, const std::string& str, const boost::shared_ptr<Font>& font,
                                          Uint32 text_fmt, Clr color, Clr text_color/* = CLR_BLACK*/, Clr interior/* = CLR_ZERO*/,
                                          StateButtonStyle style/* = SBSTYLE_3D_XBOX*/, Uint32 flags/* = CLICKABLE*/) const
{
    return new StateButton(x, y, w, h, str, font, text_fmt, color, text_color, interior, style, flags);
}

RadioButtonGroup* StyleFactory::NewRadioButtonGroup(int x, int y, int w, int h, Orientation orientation) const
{
    return new RadioButtonGroup(x, y, w, h, orientation);
}

DropDownList* StyleFactory::NewDropDownList(int x, int y, int w, int row_ht, int drop_ht, Clr color,
                                            Uint32 flags/* = CLICKABLE*/) const
{
    return new DropDownList(x, y, w, row_ht, drop_ht, color, flags);
}

DynamicGraphic* StyleFactory::NewDynamicGraphic(int x, int y, int w, int h, bool loop, int frame_width, int frame_height,
                                                int margin, const std::vector<boost::shared_ptr<Texture> >& textures,
                                                Uint32 style/* = 0*/, int frames/* = -1*/, Uint32 flags/* = 0*/) const
{
    return new DynamicGraphic(x, y, w, h, loop, frame_width, frame_height, margin, textures, style, frames, flags);
}

Edit* StyleFactory::NewEdit(int x, int y, int w, const std::string& str, const boost::shared_ptr<Font>& font,
                            Clr color, Clr text_color/* = CLR_BLACK*/, Clr interior/* = CLR_ZERO*/,
                            Uint32 flags/* = CLICKABLE | DRAG_KEEPER*/) const
{
    return new Edit(x, y, w, str, font, color, text_color, interior, flags);
}

ListBox* StyleFactory::NewListBox(int x, int y, int w, int h, Clr color, Clr interior/* = CLR_ZERO*/,
                                  Uint32 flags/* = CLICKABLE | DRAG_KEEPER*/) const
{
    return new ListBox(x, y, w, h, color, interior, flags);
}

MenuBar* StyleFactory::NewMenuBar(int x, int y, int w, const boost::shared_ptr<Font>& font, Clr text_color/* = CLR_WHITE*/,
                                  Clr color/* = CLR_BLACK*/, Clr interior/* = CLR_SHADOW*/) const
{
    return new MenuBar(x, y, w, font, text_color, color, interior);
}

MultiEdit* StyleFactory::NewMultiEdit(int x, int y, int w, int h, const std::string& str, const boost::shared_ptr<Font>& font,
                                      Clr color, Uint32 style/* = TF_LINEWRAP*/, Clr text_color/* = CLR_BLACK*/,
                                      Clr interior/* = CLR_ZERO*/, Uint32 flags/* = CLICKABLE | DRAG_KEEPER*/) const
{
    return new MultiEdit(x, y, w, h, str, font, color, style, text_color, interior, flags);
}

Scroll* StyleFactory::NewScroll(int x, int y, int w, int h, Orientation orientation, Clr color, Clr interior,
                                Uint32 flags/* = CLICKABLE*/) const
{
    return new Scroll(x, y, w, h, orientation, color, interior, flags);
}

Slider* StyleFactory::NewSlider(int x, int y, int w, int h, int min, int max, Orientation orientation,
                                SliderLineStyle style, Clr color, int tab_width, int line_width/* = 5*/,
                                Uint32 flags/* = CLICKABLE*/) const
{
    return new Slider(x, y, w, h, min, max, orientation, style, color, tab_width, line_width, flags);
}

Spin<int>* StyleFactory::NewIntSpin(int x, int y, int w, int value, int step, int min, int max, bool edits,
                                    const boost::shared_ptr<Font>& font, Clr color, Clr text_color/* = CLR_BLACK*/,
                                    Clr interior/* = CLR_ZERO*/, Uint32 flags/* = CLICKABLE | DRAG_KEEPER*/) const
{
    return new Spin<int>(x, y, w, value, step, min, max, edits, font, color, text_color, interior, flags);
}

Spin<double>* StyleFactory::NewDoubleSpin(int x, int y, int w, double value, double step, double min, double max, bool edits,
                                          const boost::shared_ptr<Font>& font, Clr color, Clr text_color/* = CLR_BLACK*/,
                                          Clr interior/* = CLR_ZERO*/, Uint32 flags/* = CLICKABLE | DRAG_KEEPER*/) const
{
    return new Spin<double>(x, y, w, value, step, min, max, edits, font, color, text_color, interior, flags);
}

StaticGraphic* StyleFactory::NewStaticGraphic(int x, int y, int w, int h, const boost::shared_ptr<Texture>& texture,
                                              Uint32 style/* = 0*/, Uint32 flags/* = 0*/) const
{
    return new StaticGraphic(x, y, w, h, texture, style, flags);
}

TextControl* StyleFactory::NewTextControl(int x, int y, int w, int h, const std::string& str,
                                          const boost::shared_ptr<Font>& font, Clr color/* = CLR_BLACK*/,
                                          Uint32 text_fmt/* = 0*/, Uint32 flags/* = 0*/) const
{
    return new TextControl(x, y, w, h, str, font, color, text_fmt, flags);
}

TextControl* StyleFactory::NewTextControl(int x, int y, const std::string& str, const boost::shared_ptr<Font>& font,
                                          Clr color/* = CLR_BLACK*/, Uint32 text_fmt/* = 0*/, Uint32 flags/* = 0*/) const
{
    return new TextControl(x, y, str, font, color, text_fmt, flags);
}

ColorDlg* StyleFactory::NewColorDlg(int x, int y, const boost::shared_ptr<Font>& font,
                                    Clr dialog_color, Clr border_color, Clr text_color/* = CLR_BLACK*/) const
{
    return new ColorDlg(x, y, font, dialog_color, border_color, text_color);
}


ColorDlg* StyleFactory::NewColorDlg(int x, int y, Clr original_color, const boost::shared_ptr<Font>& font,
                                    Clr dialog_color, Clr border_color, Clr text_color/* = CLR_BLACK*/) const
{
    return new ColorDlg(x, y, original_color, font, dialog_color, border_color, text_color);
}


FileDlg* StyleFactory::NewFileDlg(const std::string& directory, const std::string& filename, bool save, bool multi,
                                  const boost::shared_ptr<Font>& font, Clr color, Clr border_color,
                                  Clr text_color/* = CLR_BLACK*/) const
{
    return new FileDlg(directory, filename, save, multi, font, color, border_color, text_color);
}


ThreeButtonDlg* StyleFactory::NewThreeButtonDlg(int x, int y, int w, int h, const std::string& msg,
                                                const boost::shared_ptr<Font>& font, Clr color, Clr border_color,
                                                Clr button_color, Clr text_color, int buttons,
                                                const std::string& zero/* = ""*/, const std::string& one/* = ""*/,
                                                const std::string& two/* = ""*/) const
{
    return new ThreeButtonDlg(x, y, w, h, msg, font, color, border_color, button_color, text_color,
                              buttons, zero, one, two);
}

ThreeButtonDlg* StyleFactory::NewThreeButtonDlg(int w, int h, const std::string& msg, const boost::shared_ptr<Font>& font,
                                                Clr color, Clr border_color, Clr button_color, Clr text_color,
                                                int buttons, const std::string& zero/* = ""*/,
                                                const std::string& one/* = ""*/, const std::string& two/* = ""*/) const
{
    return new ThreeButtonDlg(w, h, msg, font, color, border_color, button_color, text_color,
                              buttons, zero, one, two);
}

void StyleFactory::DeleteWnd(Wnd* wnd) const
{
    delete wnd;
}
