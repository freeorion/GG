// -*- C++ -*-
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

/** \file GGSlider.h
    Contains the Slider class, which provides a slider control that allows the user to select a value from a range 
    if integers. */

#ifndef _GGSlider_h_
#define _GGSlider_h_

#ifndef _GGControl_h_
#include "GGControl.h"
#endif

namespace GG {

class Button;

/** a slider control.  This control allows the user to drag a tab to a desired setting; it is somewhat like a Scroll.
    Sliders can be either vertical or horizontal, but cannot switch between the two.  Unlike vertical Scrolls, whose
    values increase downward, vertical Sliders increase upward by default.  Note that it is acceptible to define a range
    that increases from min to max, or one that decreases from min to max; both are legal. */
class GG_API Slider : public Control
{
public:
    using Wnd::SizeMove;

    /// the orientation of the slider must be one of these two values
    enum Orientation {
        VERTICAL,
        HORIZONTAL
    };
    /// the rendering styles of the line the tab slides over
    enum LineStyleType {
        FLAT,
        RAISED,
        GROOVED
    };

    /** \name Signal Types */ //@{
    typedef boost::signal<void (int, int, int)> SlidSignalType;           ///< emitted whenever the slider is moved; the tab position and the upper and lower bounds of the slider's range are indicated, respectively
    typedef boost::signal<void (int, int, int)> SlidAndStoppedSignalType; ///< emitted when the slider's tab is stopped after being dragged, the slider is adjusted using the keyboard, or the slider is moved programmatically; the tab position and the upper and lower bounds of the slider's range are indicated, respectively
    //@}

    /** \name Slot Types */ //@{
    typedef SlidSignalType::slot_type SlidSlotType;           ///< type of functor(s) invoked on a SlidSignalType
    typedef SlidSignalType::slot_type SlidAndStoppedSlotType; ///< type of functor(s) invoked on a SlidAndStoppedSignalType
    //@}

    /** \name Structors */ //@{
    Slider(int x, int y, int w, int h, int min, int max, Orientation orientation, LineStyleType style, Clr color, int tab_width, int line_width = 5, Uint32 flags = CLICKABLE); ///< ctor
    Slider(int x, int y, int w, int h, int min, int max, Orientation orientation, LineStyleType style, Clr color, Button* tab, int line_width = 5, Uint32 flags = CLICKABLE); ///< ctor
    //@}

    /** \name Accessors */ //@{
    int            Posn() const;           ///< returns the current tab position
    std::pair<int, int>
                   SliderRange() const;    ///< returns the defined possible range of control
    Orientation    GetOrientation() const; ///< returns the orientation of the slider (VERTICAL or HORIZONTAL)
    int            TabWidth() const;       ///< returns the width of the slider's tab, in pixels
    int            LineWidth() const;      ///< returns the width of the line along which the tab slides, in pixels
    LineStyleType  LineStyle() const;      ///< returns the style of line used to render the control

    mutable SlidSignalType           SlidSignal;           ///< returns the slid signal object for this Slider
    mutable SlidAndStoppedSignalType SlidAndStoppedSignal; ///< returns the slid-and-stopped signal object for this Slider
    //@}

    /** \name Mutators */ //@{
    virtual void   Render();
    virtual void   LButtonDown(const Pt& pt, Uint32 keys);
    virtual void   LDrag(const Pt& pt, const Pt& move, Uint32 keys);
    virtual void   LButtonUp(const Pt& pt, Uint32 keys);
    virtual void   LClick(const Pt& pt, Uint32 keys);
    virtual void   MouseHere(const Pt& pt, Uint32 keys);
    virtual void   Keypress(Key key, Uint32 key_mods);

    virtual void   SizeMove(int x1, int y1, int x2, int y2); ///< sizes the control, then resizes the tab as needed
    virtual void   Disable(bool b = true);

    void           SizeSlider(int min, int max); ///< sets the logical range of the control; \a min must not equal \a max
    void           SetMax(int max);              ///< sets the maximum value of the control
    void           SetMin(int min);              ///< sets the minimum value of the control

    void           SlideTo(int p); ///< slides the control to a certain spot

    void           SetLineStyle(LineStyleType style); ///< returns the style of line used to render the control

    virtual void   DefineAttributes(WndEditor* editor);
    //@}

protected:
    /** \name Structors */ //@{
    Slider(); ///< default ctor
    //@}

    /** \name Accessors */ //@{
    int TabDragOffset() const; ///< returns the offset from the cursor to the left edge of the tab; -1 when the tab is not being dragged

    const boost::shared_ptr<Button>& Tab() const; ///< returns a pointer to the Button used as this control's sliding tab
    //@}

private:
    void  MoveTabToPosn();
    void  UpdatePosn();

    int                       m_posn;
    int                       m_range_min;
    int                       m_range_max;

    Orientation               m_orientation;

    int                       m_line_width;
    int                       m_tab_width;
    LineStyleType             m_line_style;

    int                       m_tab_drag_offset;
    boost::shared_ptr<Button> m_tab;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

// define EnumMap and stream operators for Slider::Orientation
GG_ENUM_MAP_BEGIN(Slider::Orientation)
    GG_ENUM_MAP_INSERT(Slider::VERTICAL)
    GG_ENUM_MAP_INSERT(Slider::HORIZONTAL)
GG_ENUM_MAP_END

GG_ENUM_STREAM_IN(Slider::Orientation)
GG_ENUM_STREAM_OUT(Slider::Orientation)

// define EnumMap and stream operators for Slider::LineStyleType
GG_ENUM_MAP_BEGIN(Slider::LineStyleType)
    GG_ENUM_MAP_INSERT(Slider::FLAT)
    GG_ENUM_MAP_INSERT(Slider::RAISED)
    GG_ENUM_MAP_INSERT(Slider::GROOVED)
GG_ENUM_MAP_END

GG_ENUM_STREAM_IN(Slider::LineStyleType)
GG_ENUM_STREAM_OUT(Slider::LineStyleType)

} // namespace GG

// template implementations
template <class Archive>
void GG::Slider::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Control)
        & BOOST_SERIALIZATION_NVP(m_posn)
        & BOOST_SERIALIZATION_NVP(m_range_min)
        & BOOST_SERIALIZATION_NVP(m_range_max)
        & BOOST_SERIALIZATION_NVP(m_orientation)
        & BOOST_SERIALIZATION_NVP(m_line_width)
        & BOOST_SERIALIZATION_NVP(m_tab_width)
        & BOOST_SERIALIZATION_NVP(m_line_style)
        & BOOST_SERIALIZATION_NVP(m_tab_drag_offset)
        & BOOST_SERIALIZATION_NVP(m_tab);
}

#endif // _GGSlider_h_
