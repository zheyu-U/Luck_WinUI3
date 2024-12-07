#include "pch.h"
#include "SettingPage.xaml.h"
#if __has_include("SettingPage.g.cpp")
#include "SettingPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Luck_WinUI3::implementation
{
    int32_t SettingPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SettingPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void SettingPage::BackButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        Frame().Navigate(winrt::xaml_typename<MainPage>());
    }
}
