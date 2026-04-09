// Generated with ImRAD 0.10-WIP
// visit https://github.com/tpecholt/imrad

#include "gui.h"

Gui gui;


void Gui::Open()
{
    isOpen = true;
}

void Gui::Close()
{
    isOpen = false;
}

void Gui::ResetLayout()
{
    // ImGui::GetCurrentWindow()->HiddenFramesCannotSkipItems = 2;
    vb1.Reset();
    hb1.Reset();
    vb01.Reset();
    hb01.Reset();
    hb03.Reset();
}

void Gui::DrawPopups()
{
    // TODO: Draw dependent popups here
}

void Gui::Draw()
{
    /// @dpi-info 141.357,1
    /// @style d
    /// @unit px
    /// @begin TopWindow
    if (isOpen)
    {
        ImGui::SetNextWindowSize({ 512, 380 }, ImGuiCond_FirstUseEver); //{ 512, 380 }
        ImGui::SetNextWindowSizeConstraints({ 0, 0 }, { FLT_MAX, FLT_MAX });
        if (ImGui::Begin("title###Gui", &isOpen, 0))
        {
            DrawPopups();
            /// @separator

            /// @begin Child
            vb1.BeginLayout();
            hb1.BeginLayout();
            if (ImGui::BeginChild("child1", { hb1.GetSize(), vb1.GetSize() }, ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings))
            {
                /// @separator

                /// @begin Child
                vb01.BeginLayout();
                hb01.BeginLayout();
                if (ImGui::BeginChild("child1", { hb01.GetSize(), vb01.GetSize() }, ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings))
                {
                    /// @separator

                    /// @begin Text
                    ImGui::TextUnformatted("text");
                    /// @end Text

                    /// @separator
                }
                ImGui::EndChild();
                vb01.AddSize(0 * ImGui::GetStyle().ItemSpacing.y, ImRad::VBox::Stretch(1.0f));
                hb01.AddSize(0 * ImGui::GetStyle().ItemSpacing.x, ImRad::HBox::Stretch(1.0f));
                /// @end Child

                /// @begin Separator
                ImRad::SeparatorEx(ImRad::SeparatorFlags_Horizontal);
                vb01.AddSize(1 * ImGui::GetStyle().ItemSpacing.y, ImRad::VBox::ItemSize);
                /// @end Separator

                /// @begin Child
                hb03.BeginLayout();
                if (ImGui::BeginChild("child2", { hb03.GetSize(), vb01.GetSize() }, ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings))
                {
                    /// @separator

                    /// @begin Text
                    ImGui::TextUnformatted("text");
                    /// @end Text

                    /// @separator
                }
                ImGui::EndChild();
                vb01.AddSize(1 * ImGui::GetStyle().ItemSpacing.y, ImRad::VBox::Stretch(1.0f));
                hb03.AddSize(0 * ImGui::GetStyle().ItemSpacing.x, ImRad::HBox::Stretch(1.0f));
                /// @end Child

                /// @separator
            }
            ImGui::EndChild();
            vb1.AddSize(0 * ImGui::GetStyle().ItemSpacing.y, ImRad::VBox::Stretch(1.0f));
            hb1.AddSize(0 * ImGui::GetStyle().ItemSpacing.x, ImRad::HBox::Stretch(1.0f));
            /// @end Child

            /// @separator
        }
        ImGui::End();
    }
    /// @end TopWindow
}
