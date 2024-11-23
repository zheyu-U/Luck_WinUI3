#pragma once

#include "SettingWindow.g.h"

namespace winrt::Luck_WinUI3::implementation
{
    struct SettingWindow : SettingWindowT<SettingWindow>
    {
        SettingWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void myButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::Luck_WinUI3::factory_implementation
{
    struct SettingWindow : SettingWindowT<SettingWindow, implementation::SettingWindow>
    {
    };
}
