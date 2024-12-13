#include "pch.h"
#include "MainPage.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif
#include "App.xaml.h"

using namespace winrt;
using namespace Microsoft::UI;
using namespace Xaml;
using namespace Windowing;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Luck_WinUI3::implementation
{
	int32_t MainPage::MyProperty()
	{
		throw hresult_not_implemented();
	}

	void MainPage::MyProperty(int32_t /* value */)
	{
		throw hresult_not_implemented();
	}

	void MainPage::myButton_Click(IInspectable const&, RoutedEventArgs const&)
	{
		//myButton().Content(box_value(L"Clicked"));
	}
	void MainPage::Button_Click(IInspectable const& sender, RoutedEventArgs const& args)
	{
		using namespace Microsoft::UI::Xaml::Controls;
		LuckSet::SelectedNumber = 1;
		TextBlock name, dialogContent;
		name.Text(L"结果");

		ContentDialog dialog;
		dialog.XamlRoot(this->XamlRoot());
		dialog.Title(name);
		dialog.DefaultButton(ContentDialogButton::Primary);
		switch (LuckSet::JudgePrize())
		{
		case 0: 
			dialogContent.Text(L"请选择你的幸运数字");
			break;
		case 1: 
			dialogContent.Text(L"恭喜你抽到一等奖"); 
			break;
		case 2: 
			dialogContent.Text(L"恭喜你抽到二等奖"); 
			break;
		case 3:
			dialogContent.Text(L"恭喜你抽到三等奖");
			break;
		case -1:
			dialogContent.Text(L"所有奖项均已抽完"); 
			break;
		default:
			break;
		}
		dialog.Content(dialogContent);
		dialog.PrimaryButtonText(L"好的");
		dialog.SecondaryButtonText(L"不好");
		dialog.ShowAsync();
	}
	void MainPage::ExitButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{
		Application::Current().Exit();
	}
	void MainPage::ToggleScreenModeButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{
		using namespace Microsoft::UI::Windowing;
		//将AppWindow设为静态变量，避免从OnNavigatedTo事件获取Window时遇到从Page跳转而无法获取到Window的情况
		if (App::appWindow.Presenter().Kind() == AppWindowPresenterKind::FullScreen)
		{
			App::appWindow.SetPresenter(AppWindowPresenterKind::Default);
			//App::appWindow.Resize({ 800, 600 });
			//App::appWindow.Move({ 500, 500 });
		}

		else App::appWindow.SetPresenter(AppWindowPresenterKind::FullScreen);
	}
	void MainPage::SettingsButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{
		try
		{
			Frame().Navigate(winrt::xaml_typename<SettingPage>());
		}
		catch (winrt::hresult_error const& ex)
		{
			winrt::hresult hr = ex.code();
			winrt::hstring message = ex.message();
			__debugbreak();
			MessageBox(NULL, L"严重错误,设置页面无法加载。\n", LUCK_ERROE_TITLE, MB_OK);
			throw ex;
		}
	}
}
