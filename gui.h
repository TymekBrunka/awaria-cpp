// Generated with ImRAD 0.10-WIP
// visit https://github.com/tpecholt/imrad

#pragma once
#include <imrad.h>

class Gui
{
public:
    /// @begin interface
    void Open();
    void Close();
    void Draw();

    /// @end interface

private:
    /// @begin impl
    void DrawPopups();
    void ResetLayout();

    bool isOpen = true;
    ImRad::VBox vb1;
    ImRad::HBox hb1;
    ImRad::VBox vb01;
    ImRad::HBox hb01;
    ImRad::HBox hb03;
    /// @end impl
};

extern Gui gui;
