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

#include "GGStaticGraphic.h"

#include <GGDrawUtil.h>
#include <XMLValidators.h>

namespace GG {

////////////////////////////////////////////////
// GG::StaticGraphic
////////////////////////////////////////////////
StaticGraphic::StaticGraphic(int x, int y, int w, int h, const Texture* texture, Uint32 style/* = 0*/, Uint32 flags/* = 0*/) :
    Control(x, y, w, h, flags),
    m_style(style)
{
    Init(SubTexture(texture, 0, 0, texture->DefaultWidth(), texture->DefaultHeight()));
}

StaticGraphic::StaticGraphic(int x, int y, int w, int h, const shared_ptr<Texture>& texture, Uint32 style/* = 0*/,
                             Uint32 flags/* = 0*/) :
    Control(x, y, w, h, flags),
    m_style(style)
{
    Init(SubTexture(texture, 0, 0, texture->DefaultWidth(), texture->DefaultHeight()));
}

StaticGraphic::StaticGraphic(int x, int y, int w, int h, const SubTexture& subtexture, Uint32 style/* = 0*/,
                             Uint32 flags/* = 0*/) :
    Control(x, y, w, h, flags),
    m_style(style)
{
    Init(subtexture);
}

StaticGraphic::StaticGraphic(const XMLElement& elem) :
    Control(elem.Child("GG::Control")), 
    m_style(0)
{
    if (elem.Tag() != "GG::StaticGraphic")
        throw std::invalid_argument("Attempted to construct a GG::StaticGraphic from an XMLElement that had a tag other than \"GG::StaticGraphic\"");

    m_graphic = SubTexture(elem.Child("m_graphic").Child("GG::SubTexture"));

    m_style = FlagsFromString<GraphicStyle>(elem.Child("m_style").Text());
    ValidateStyle();
}

bool StaticGraphic::Render()
{
    Clr color_to_use = Disabled() ? DisabledColor(Color()) : Color();
    glColor4ubv(color_to_use.v);

    Pt ul = UpperLeft(), lr = LowerRight();
    Pt window_sz(lr - ul);
    Pt graphic_sz(m_graphic.Width(), m_graphic.Height());
    Pt pt1, pt2(graphic_sz); // (unscaled) default graphic size
    if (m_style & GR_FITGRAPHIC) {
        if (m_style & GR_PROPSCALE) {
            double scale_x = window_sz.x / double(graphic_sz.x),
                             scale_y = window_sz.y / double(graphic_sz.y);
            double scale = (scale_x < scale_y) ? scale_x : scale_y;
            pt2.x = int(graphic_sz.x * scale);
            pt2.y = int(graphic_sz.y * scale);
        } else {
            pt2 = window_sz;
        }
    } else if (m_style & GR_SHRINKFIT) {
        if (m_style & GR_PROPSCALE) {
            double scale_x = (graphic_sz.x > window_sz.x) ? window_sz.x / double(graphic_sz.x) : 1.0,
                             scale_y = (graphic_sz.y > window_sz.y) ? window_sz.y / double(graphic_sz.y) : 1.0;
            double scale = (scale_x < scale_y) ? scale_x : scale_y;
            pt2.x = int(graphic_sz.x * scale);
            pt2.y = int(graphic_sz.y * scale);
        } else {
            pt2 = window_sz;
        }
    }

    int shift = 0;
    if (m_style & GR_LEFT) {
        shift = ul.x;
    } else if (m_style & GR_CENTER) {
        shift = ul.x + (window_sz.x - (pt2.x - pt1.x)) / 2;
    } else { // m_style & GR_RIGHT
        shift = lr.x - (pt2.x - pt1.x);
    }
    pt1.x += shift;
    pt2.x += shift;

    if (m_style & GR_TOP) {
        shift = ul.y;
    } else if (m_style & GR_VCENTER) {
        shift = ul.y + (window_sz.y - (pt2.y - pt1.y)) / 2;
    } else { // m_style & GR_BOTTOM
        shift = lr.y - (pt2.y - pt1.y);
    }
    pt1.y += shift;
    pt2.y += shift;

    m_graphic.OrthoBlit(pt1, pt2, false);

    return true;
}

XMLElement StaticGraphic::XMLEncode() const
{
    XMLElement retval("GG::StaticGraphic");
    retval.AppendChild(Control::XMLEncode());
    retval.AppendChild(XMLElement("m_graphic", m_graphic.XMLEncode()));
    retval.AppendChild(XMLElement("m_style", StringFromFlags<GraphicStyle>(m_style)));
    return retval;
}

XMLElementValidator StaticGraphic::XMLValidator() const
{
    XMLElementValidator retval("GG::StaticGraphic");
    retval.AppendChild(Control::XMLValidator());
    retval.AppendChild(XMLElementValidator("m_graphic", m_graphic.XMLValidator()));
    retval.AppendChild(XMLElementValidator("m_style", new ListValidator<GraphicStyle>()));
    return retval;
}

void StaticGraphic::Init(const SubTexture& subtexture)
{
    ValidateStyle();  // correct any disagreements in the style flags
    SetColor(CLR_WHITE);
    m_graphic = subtexture;
}

void StaticGraphic::ValidateStyle()
{
    int dup_ct = 0;   // duplication count
    if (m_style & GR_LEFT) ++dup_ct;
    if (m_style & GR_RIGHT) ++dup_ct;
    if (m_style & GR_CENTER) ++dup_ct;
    if (dup_ct != 1) {   // exactly one must be picked; when none or multiples are picked, use GR_CENTER by default
        m_style &= ~(GR_RIGHT | GR_LEFT);
        m_style |= GR_CENTER;
    }
    dup_ct = 0;
    if (m_style & GR_TOP) ++dup_ct;
    if (m_style & GR_BOTTOM) ++dup_ct;
    if (m_style & GR_VCENTER) ++dup_ct;
    if (dup_ct != 1) {   // exactly one must be picked; when none or multiples are picked, use GR_VCENTER by default
        m_style &= ~(GR_TOP | GR_BOTTOM);
        m_style |= GR_VCENTER;
    }
    dup_ct = 0;
    if (m_style & GR_FITGRAPHIC) ++dup_ct;
    if (m_style & GR_SHRINKFIT) ++dup_ct;
    if (dup_ct > 1) {   // mo more than one may be picked; when both are picked, use GR_SHRINKFIT by default
        m_style &= ~GR_FITGRAPHIC;
        m_style |= GR_SHRINKFIT;
    }
}

} // namespace GG

