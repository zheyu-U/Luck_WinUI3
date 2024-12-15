#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include "App.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Luck_WinUI3::implementation
{
    MainWindow::MainWindow()
    {
        using namespace winrt::Windows::UI::Xaml::Media::Animation;
        InitializeComponent();
        rootFrame().Navigate(xaml_typename<MainPage>(), nullptr);
    }
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
    void MainWindow::ToggleScreenModeButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        using namespace Microsoft::UI::Windowing;
        using namespace winrt::Microsoft::UI::Xaml::Controls;
        //将AppWindow设为静态变量，避免从OnNavigatedTo事件获取Window时遇到从Page跳转而无法获取到Window的情况
        if (App::appWindow.Presenter().Kind() == AppWindowPresenterKind::FullScreen)
        {
            App::appWindow.SetPresenter(AppWindowPresenterKind::Default);
            ToggleScreenModeButtonSymbolIcon().Symbol(Symbol::SlideShow);
            ToggleScreenModeButtonTextBlock().Text(L"进入全屏");
            //App::appWindow.Resize({ 800, 600 });
            //App::appWindow.Move({ 500, 500 });
        }
        else
        {
            App::appWindow.SetPresenter(AppWindowPresenterKind::FullScreen);
            ToggleScreenModeButtonSymbolIcon().Symbol(Symbol::NewWindow);
            ToggleScreenModeButtonTextBlock().Text(L"退出全屏");
        }
    }
}
