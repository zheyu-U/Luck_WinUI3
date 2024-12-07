#pragma once

#include "SettingPage.g.h"

namespace winrt::Luck_WinUI3::implementation
{
    struct SettingPage : SettingPageT<SettingPage>
    {
        SettingPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void BackButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::Luck_WinUI3::factory_implementation
{
    struct SettingPage : SettingPageT<SettingPage, implementation::SettingPage>
    {
    };
}
