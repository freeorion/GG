#include "serialization.h"

#include "saveload.h"

#include "GGButton.h"
#include "GGDropDownList.h"
#include "GGDynamicGraphic.h"
#include "GGEdit.h"
#include "GGListBox.h"
#include "GGLayout.h"
#include "GGMenu.h"
#include "GGMultiEdit.h"
#include "GGScroll.h"
#include "GGSlider.h"
#include "GGSpin.h"
#include "GGStaticGraphic.h"
#include "GGTextControl.h"
#include "GGFileDlg.h"
#include "GGThreeButtonDlg.h"
#include "SDLGGApp.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <fstream>
#include <iostream>


// Tutorial 3: Serialization

// This file is part of the third tutorial.  The other two files are serialization.h, saveload.h, and saveload.cpp.  It
// extends the Tutorial 2 by serializing all the controls to a file called test.xml, deleting them, and recreating them
// from the XML file before showing them on the screen.  This demonstrates how GG serialization works in detail.  For
// further reference material, see the boost serialization documentation.


////////////////////////////////////////////////////////////////////////////////
// Ignore all code until the end of ControlsTestGGApp::Initialize(); the
// enclosed code is straight from Tutorial 2.
////////////////////////////////////////////////////////////////////////////////
void QuitButtonClicked()
{
    GG::ThreeButtonDlg quit_dlg(200, 100, "Are you sure... I mean, really sure?", "Vera.ttf", 12, GG::CLR_GRAY,
                                GG::CLR_GRAY, GG::CLR_GRAY, GG::CLR_WHITE, 2);
    quit_dlg.Run();

    if (quit_dlg.Result() == 0)
        GG::App::GetApp()->Exit(0);
}

struct BrowseFilesFunctor
{
    void operator()()
    {
        GG::FileDlg file_dlg("", "", false, false, "Vera.ttf", 12, GG::CLR_GRAY,
                             GG::CLR_GRAY);
        file_dlg.Run();
    }
    void operator()(int)
    {operator()();}
};

class TextUpdater
{
public:
    void SetTextControl(GG::TextControl* text_control)
    {
        m_text_control = text_control;
    }
    void SelectText(int index)
    {
        if (index)
            m_text_control->SetText("Plan to execute: Plan 9!");
        else
            m_text_control->SetText("Plan to execute: Plan 8");
    }
private:
    GG::TextControl* m_text_control;
} g_text_updater;


CustomTextRow::CustomTextRow() :
    Row()
{}

CustomTextRow::CustomTextRow(const std::string& text) :
    Row()
{
    push_back(GG::ListBox::Row::CreateControl(text, GG::App::GetApp()->GetFont("Vera.ttf", 12), GG::CLR_WHITE));
}


class ControlsTestGGApp : public SDLGGApp
{
public:
    ControlsTestGGApp();

    virtual void Enter2DMode();
    virtual void Exit2DMode();

protected:
    virtual void Render();

private:
    virtual void GLInit();
    virtual void Initialize();
    virtual void FinalCleanup();
};

ControlsTestGGApp::ControlsTestGGApp() : 
    SDLGGApp(1024, 768, false, "Control-Test GG App")
{
}

void ControlsTestGGApp::Enter2DMode()
{
    glPushAttrib(GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_TEXTURE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, AppWidth(), AppHeight());

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0, AppWidth(), AppHeight(), 0.0, 0.0, AppWidth());

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void ControlsTestGGApp::Exit2DMode()
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}

void ControlsTestGGApp::Render()
{
    const double RPM = 4;
    const double DEGREES_PER_MS = 360.0 * RPM / 60000.0;

    glRotated(DeltaT() * DEGREES_PER_MS, 0.0, 1.0, 0.0);

    glBegin(GL_QUADS);

    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(1.0, 1.0, -1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);

    glColor3d(1.0, 0.5, 0.0);
    glVertex3d(1.0, -1.0, 1.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glVertex3d(-1.0, -1.0,-1.0);
    glVertex3d(1.0, -1.0,-1.0);

    glColor3d(1.0, 0.0, 0.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glVertex3d(1.0, -1.0, 1.0);

    glColor3d(1.0, 1.0, 0.0);
    glVertex3d(1.0, -1.0, -1.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glVertex3d(1.0, 1.0, -1.0);

    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glVertex3d(-1.0, -1.0, 1.0);

    glColor3d(1.0, 0.0, 1.0);
    glVertex3d(1.0, 1.0, -1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(1.0, -1.0, 1.0);
    glVertex3d(1.0, -1.0, -1.0);

    glEnd();

    GG::App::Render();
}

void ControlsTestGGApp::GLInit()
{
    double ratio = AppWidth() / (float)(AppHeight());

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, AppWidth(), AppHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, ratio, 1.0, 10.0);
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
}

void ControlsTestGGApp::Initialize()
{
    SDL_WM_SetCaption("Control-Test GG App", "Control-Test GG App");

    boost::shared_ptr<GG::Font> font = GetFont("Vera.ttf", 12);

    GG::Layout* layout = new GG::Layout(0, 0, AppWidth(), AppHeight(), 1, 1, 10);

    GG::MenuItem menu_contents;
    GG::MenuItem file_menu("File", 0, false, false);
    file_menu.next_level.push_back(GG::MenuItem("Browse...", 1, false, false, BrowseFilesFunctor()));
    menu_contents.next_level.push_back(file_menu);
    GG::MenuBar* menu_bar =
        new GG::MenuBar(0, 0, AppWidth(), font, menu_contents, GG::CLR_WHITE);
    layout->Add(menu_bar, 0, 0, 1, 2, GG::ALIGN_TOP);

    GG::RadioButtonGroup* radio_button_group = new GG::RadioButtonGroup(10, 10);
    GG::StateButton* state_button8 =
        new GG::StateButton(0,   0, 100, 25, "Plan 8", font, GG::TF_LEFT, GG::CLR_GRAY, GG::CLR_WHITE,
                            GG::CLR_ZERO, GG::StateButton::SBSTYLE_3D_RADIO);
    GG::StateButton* state_button9 =
        new GG::StateButton(100, 0, 100, 25, "Plan 9", font, GG::TF_LEFT, GG::CLR_GRAY, GG::CLR_WHITE,
                            GG::CLR_ZERO, GG::StateButton::SBSTYLE_3D_RADIO);
    radio_button_group->AddButton(state_button8);
    radio_button_group->AddButton(state_button9);
    layout->Add(radio_button_group, 1, 0);

    GG::TextControl* plan_text_control =
        new GG::TextControl(0, 0, 150, 25, "", font, GG::CLR_WHITE);
    layout->Add(plan_text_control, 1, 1);

    GG::ListBox::Row* row;
    GG::DropDownList* drop_down_list =
        new GG::DropDownList(0, 0, 150, 25, 150, GG::CLR_GRAY, GG::CLR_GRAY);
    drop_down_list->SetStyle(GG::LB_NOSORT);
    row = new GG::ListBox::Row();
    row->push_back(GG::ListBox::Row::CreateControl("I always", font, GG::CLR_WHITE));
    drop_down_list->Insert(row);
    row = new GG::ListBox::Row();
    row->push_back(GG::ListBox::Row::CreateControl("thought", font, GG::CLR_WHITE));
    drop_down_list->Insert(row);
    row = new GG::ListBox::Row();
    row->push_back(GG::ListBox::Row::CreateControl("\"combo box\"", font, GG::CLR_WHITE));
    drop_down_list->Insert(row);
    row = new GG::ListBox::Row();
    row->push_back(GG::ListBox::Row::CreateControl("was a lousy", font, GG::CLR_WHITE));
    drop_down_list->Insert(row);
    row = new GG::ListBox::Row();
    row->push_back(GG::ListBox::Row::CreateControl("way to describe", font, GG::CLR_WHITE));
    drop_down_list->Insert(row);
    row = new GG::ListBox::Row();
    row->push_back(GG::ListBox::Row::CreateControl("controls", font, GG::CLR_WHITE));
    drop_down_list->Insert(row);
    row = new GG::ListBox::Row();
    row->push_back(GG::ListBox::Row::CreateControl("like this", font, GG::CLR_WHITE));
    drop_down_list->Insert(row);
    drop_down_list->Select(0);
    layout->Add(drop_down_list, 2, 0);

    GG::Edit* edit = new GG::Edit(0, 0, 100, 35, "Edit me.", font, GG::CLR_GRAY, GG::CLR_WHITE, GG::CLR_SHADOW);
    layout->Add(edit, 2, 1);

    GG::ListBox* list_box = new GG::ListBox(0, 0, 300, 200, GG::CLR_GRAY);
    list_box->Insert(new CustomTextRow("Item 1"));
    list_box->Insert(new CustomTextRow("Item 2"));
    list_box->Insert(new CustomTextRow("Item 3"));
    list_box->Insert(new CustomTextRow("Item 4"));
    list_box->Insert(new CustomTextRow("Item 5"));
    list_box->Insert(new CustomTextRow("Item 6"));
    list_box->Insert(new CustomTextRow("Item 7"));
    layout->Add(list_box, 3, 0);

    GG::MultiEdit* multi_edit =
        new GG::MultiEdit(0, 0, 300, 200, "Edit me\ntoo.", font, GG::CLR_GRAY, GG::TF_LINEWRAP, GG::CLR_WHITE, GG::CLR_SHADOW);
    layout->Add(multi_edit, 3, 1);

    GG::Slider* slider =
        new GG::Slider(0, 0, 300, 14, 1, 100, GG::Slider::HORIZONTAL, GG::Slider::RAISED, GG::CLR_GRAY, 10);
    layout->Add(slider, 4, 0);

    GG::Spin<int>* spin_int =
        new GG::Spin<int>(0, 0, 50, 30, 1, 1, -5, 5, false, font, GG::CLR_GRAY, GG::CLR_WHITE);
    spin_int->SetMaxSize(75, 30);
    layout->Add(spin_int, 5, 0);

    GG::Spin<double>* spin_double =
        new GG::Spin<double>(0, 0, 50, 30, 1.0, 1.5, -0.5, 16.0, true, font, GG::CLR_GRAY, GG::CLR_WHITE);
    spin_double->SetMaxSize(75, 30);
    layout->Add(spin_double, 6, 0);

    GG::Scroll* scroll =
        new GG::Scroll(0, 0, 14, 200, GG::Scroll::VERTICAL, GG::CLR_GRAY, GG::CLR_GRAY);
    scroll->SetMaxSize(14, 1000);
    layout->Add(scroll, 4, 1, 3, 1);

    boost::shared_ptr<GG::Texture> circle_texture = GetTexture("hatchcircle.png");
    glDisable(GL_TEXTURE_2D);
    GG::DynamicGraphic* dynamic_graphic =
        new GG::DynamicGraphic(0, 0, 64, 64, true, 64, 64, 0, circle_texture);
    dynamic_graphic->SetMaxSize(64, 64);
    layout->Add(dynamic_graphic, 7, 0);
    GG::StaticGraphic* static_graphic =
        new GG::StaticGraphic(0, 0, 320, 128, circle_texture);
    layout->Add(static_graphic, 7, 1);

    GG::Button* quit_button =
        new GG::Button(0, 0, 75, 25, "Quit...", font, GG::CLR_GRAY);
    GG::Button* files_button =
        new GG::Button(0, 0, 75, 25, "Files...", font, GG::CLR_GRAY);
    layout->Add(quit_button, 8, 0);
    layout->Add(files_button, 8, 1);

////////////////////////////////////////////////////////////////////////////////
// End of old tutorial code.
////////////////////////////////////////////////////////////////////////////////

    // Since we're saving to and then immediately reloading from test.xml, we need to enclose the serialiaztion code
    // inside of code blocks, so that the boost::archive::xml_*archive objects will be destroyed when they go out of
    // scope, allowing another archive to be associated with the file.  This is because there is no way to explicitly
    // close a boost serialization archive.

    {
        std::ofstream ofs("test.xml");
        boost::archive::xml_oarchive oa(ofs);
        SaveWnd(layout, "layout", oa);

        // Somthing a little odd is happening here.  We're saving these four objects singly, but they're already being
        // saved as children of layout.  Since signals do not survive serialization, we have to wait to connect the
        // signals below.  But this means that since we are going to delete all these objects a few lines below, we must
        // save these objects so that we have pointers to them when we need to connect the signals below.  Note that we
        // are not reconnecting the File->Browse.. menu item to a BrowseFilesFunctor.  Reconnecting this is left as an
        // excercise for the reader.  Also note that since the boost.serialization library is so damn smart, only one
        // copy of each of these objects is saved in the XML file.  This applies to all serialized pointers; if you save
        // 10 pointers to a single object, only one object is actually saved, and all 10 pointers will point to the same
        // object when they are loaded.

        SaveWnd(radio_button_group, "radio_button_group", oa);
        SaveWnd(plan_text_control, "plan_text_control", oa);
        SaveWnd(files_button, "files_button", oa);
        SaveWnd(quit_button, "quit_button", oa);
    }

    // Here, we delete the layout (which automatically causes all its children to be freed as well).  After doing this,
    // if we see the windows on the screen, we know they came from the reload of the XML file.
    delete layout;
    layout = 0;
    plan_text_control = 0;

    {
        std::ifstream ifs("test.xml");
        boost::archive::xml_iarchive ia(ifs);
        LoadWnd(layout, "layout", ia);
        LoadWnd(radio_button_group, "radio_button_group", ia);
        LoadWnd(plan_text_control, "plan_text_control", ia);
        LoadWnd(files_button, "files_button", ia);
        LoadWnd(quit_button, "quit_button", ia);
    }

////////////////////////////////////////////////////////////////////////////////
// More old tutorial code.
////////////////////////////////////////////////////////////////////////////////

    g_text_updater.SetTextControl(plan_text_control);
    GG::Connect(radio_button_group->ButtonChangedSignal, &TextUpdater::SelectText, &g_text_updater);
    GG::Connect(files_button->ClickedSignal, BrowseFilesFunctor());
    GG::Connect(quit_button->ClickedSignal, &QuitButtonClicked);

    Register(layout);
}

void ControlsTestGGApp::FinalCleanup()
{
}

extern "C"
int main(int argc, char* argv[])
{
    ControlsTestGGApp app;

    try {
        app();
    } catch (const std::invalid_argument& e) {
        std::cerr << "main() caught exception(std::invalid_arg): " << e.what();
    } catch (const std::runtime_error& e) {
        std::cerr << "main() caught exception(std::runtime_error): " << e.what();
    } catch (const std::exception& e) {
        std::cerr << "main() caught exception(std::exception): " << e.what();
    }
    return 0;
}