#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Luck_WinUI3::implementation
{
    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        window = make<MainWindow>();
        window.ExtendsContentIntoTitleBar(true);//将内容延伸到标题栏区域

        // 导航到 MainPage
        try
        {
            winrt::Microsoft::UI::Xaml::Controls::Frame rootFrame = Microsoft::UI::Xaml::Controls::Frame();
            window.Content(rootFrame);
            rootFrame.Navigate(xaml_typename<MainPage>(), window);
        }
        catch (winrt::hresult_error const& ex)
        {
            winrt::hresult hr = ex.code();
            winrt::hstring message = ex.message();

            __debugbreak();
            MessageBox(NULL, L"严重错误,抽奖程序无法启动。\n", LUCK_ERROE_TITLE, MB_OK);
            throw ex;
        }
       
        // 加载奖项数据
        Windows::Storage::ApplicationDataContainer localSettings = Windows::Storage::ApplicationData::Current().LocalSettings();
        if (localSettings.Values().HasKey(L"FirstPrizeCount")
            && localSettings.Values().HasKey(L"SecondPrizeCount")
            && localSettings.Values().HasKey(L"ThirdPrizeCount")
            && localSettings.Values().HasKey(L"FirstPrizeProbability")
            && localSettings.Values().HasKey(L"SecondPrizeProbability")
            && localSettings.Values().HasKey(L"ThirdPrizeProbability"))
        {
            int firstPrizeCount = unbox_value<int>(localSettings.Values().Lookup(L"FirstPrizeCount"));
            int secondPrizeCount = unbox_value<int>(localSettings.Values().Lookup(L"SecondPrizeCount"));
            int thirdPrizeCount = unbox_value<int>(localSettings.Values().Lookup(L"ThirdPrizeCount"));
            int firstPrizeProbability = unbox_value<int>(localSettings.Values().Lookup(L"FirstPrizeProbability"));
            int secondPrizeProbability = unbox_value<int>(localSettings.Values().Lookup(L"SecondPrizeProbability"));
            int thirdPrizeProbability = unbox_value<int>(localSettings.Values().Lookup(L"ThirdPrizeProbability"));
            LuckSet::Setup(firstPrizeCount, secondPrizeCount, thirdPrizeCount);
            LuckSet::SetPrizeProbability(firstPrizeProbability, secondPrizeProbability, thirdPrizeProbability);
        }
        else
        {
            LuckSet::Setup(10, 90, 400);
            LuckSet::SetPrizeProbability(10, 30, 60);
            localSettings.Values().Insert(L"FirstPrizeCount", box_value(10));
            localSettings.Values().Insert(L"SecondPrizeCount", box_value(90));
            localSettings.Values().Insert(L"ThirdPrizeCount", box_value(400));
            localSettings.Values().Insert(L"FirstPrizeProbability", box_value(10));
            localSettings.Values().Insert(L"SecondPrizeProbability", box_value(30));
            localSettings.Values().Insert(L"ThirdPrizeProbability", box_value(60));
        }


        window.Activate();
    }
}
