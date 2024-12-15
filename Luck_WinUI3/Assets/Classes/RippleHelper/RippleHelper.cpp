#include "pch.h"
#include "RippleHelper.h"
#include "winrt/Windows.UI.Composition.h"
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>

using namespace winrt::Microsoft::UI::Xaml::Hosting;
using namespace winrt::Microsoft::UI::Xaml::Controls;
//using namespace winrt::Microsoft::UI::Xaml::Input;
using namespace winrt::Microsoft::UI::Composition;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Numerics;

using _PointerEventHandler = void(*)(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Xaml::RoutedEventArgs& e);
RippleHelper::pointerEventHandler = _PointerEventHandler(Ele_PointerReleased);
Compositor RippleHelper::compositor = Window::Current().Compositor();
ExpressionAnimation RippleHelper::_SizeBind{ nullptr };
CompositionEasingFunction RippleHelper::_EaseOut{ nullptr };
ScalarKeyFrameAnimation RippleHelper::_OpacityAnimation{ nullptr };
Vector3KeyFrameAnimation RippleHelper::_ScaleAnimation{ nullptr };
CompositionAnimationGroup RippleHelper::_RippleAnimationGroup{ nullptr };
CompositionPropertySet RippleHelper::_PropSet{ nullptr };
CompositionBrush RippleHelper::_Mask{ nullptr };
static DependencyProperty IsFillEnableProperty = DependencyProperty::RegisterAttached(L"IsFillEnable", winrt::xaml_typename<bool>(), winrt::xaml_typename<RippleHelper>(), PropertyMetadata(winrt::box_value(true)));
DependencyProperty RippleDurationProperty = DependencyProperty::RegisterAttached(L"RippleDuration", winrt::xaml_typename<TimeSpan>(), winrt::xaml_typename<RippleHelper>(), PropertyMetadata(winrt::box_value(std::chrono::milliseconds(330))));
DependencyProperty RippleRadiusProperty = DependencyProperty::RegisterAttached(L"RippleRadius", winrt::xaml_typename<double>(), winrt::xaml_typename<RippleHelper>(), PropertyMetadata(winrt::box_value(100.0)));
DependencyProperty RippleColorProperty = DependencyProperty::RegisterAttached(L"RippleColor", winrt::xaml_typename<Color>(), winrt::xaml_typename<RippleHelper>(), PropertyMetadata(winrt::box_value(Colors::White)));
DependencyProperty RippleHelperStateProperty = DependencyProperty::RegisterAttached(L"RippleHelperState",winrt::xaml_typename<RippleHelperState>(),winrt::xaml_typename<RippleHelper>(),PropertyMetadata(winrt::box_value(RippleHelperState::None), [](DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args)
		{
			if (args.NewValue() && args.NewValue() != args.OldValue())
			{
				RippleHelperState value = winrt::unbox_value<RippleHelperState>(args.NewValue());
				RippleHelperState oldvalue = winrt::unbox_value<RippleHelperState>(args.OldValue());
				if (sender.try_as<UIElement>())
				{
					UIElement ele = sender.as<UIElement>();
					switch (value)
					{
					case RippleHelperState::Pressed:
					{
						ele.RemoveHandler(UIElement::PointerReleasedEvent(), pointerEventHandler);
						ele.AddHandler(UIElement::PointerPressedEvent(), pointerEventHandler, true);
					}
					break;
					case RippleHelperState::Released:
					{
						ele.RemoveHandler(UIElement::PointerPressedEvent(), pointerEventHandler);
						ele.AddHandler(UIElement::PointerReleasedEvent(), pointerEventHandler, true);
					}
					break;
					case RippleHelperState::None:
					{
						ele.RemoveHandler(UIElement::PointerPressedEvent(), pointerEventHandler);
						ele.RemoveHandler(UIElement::PointerReleasedEvent(), pointerEventHandler);
						ElementCompositionPreview::SetElementChildVisual(ele, nullptr);
					}
					break;
					}
				}
			}
		}
	)
);
ExpressionAnimation RippleHelper::SizeBind()
{
	if (_SizeBind == nullptr)
	{
		_SizeBind = compositor.CreateExpressionAnimation(L"hostVisual.Size");
	}
	return  _SizeBind;
}
CompositionEasingFunction RippleHelper::EaseOut()
{
	if (_EaseOut == nullptr)
	{
		_EaseOut = compositor.CreateCubicBezierEasingFunction(float2(0.0f , 0.0f), float2(0.9f, 1.0f));
	}
	return _EaseOut;
}
ScalarKeyFrameAnimation RippleHelper::OpacityAnimation()
{
	if (_OpacityAnimation == nullptr)
	{
		_OpacityAnimation = compositor.CreateScalarKeyFrameAnimation();
		_OpacityAnimation.InsertKeyFrame(0.0f, 1.0f, RippleHelper::EaseOut());
		_OpacityAnimation.InsertKeyFrame(1.0f, 0.0f, EaseOut());
		_OpacityAnimation.Duration() = std::chrono::milliseconds(350);
		_OpacityAnimation.Target(L"Opacity");
	}
	return _OpacityAnimation;
}

Vector3KeyFrameAnimation RippleHelper::ScaleAnimation()
{
	if (_ScaleAnimation == nullptr)
	{
		_ScaleAnimation = compositor.CreateVector3KeyFrameAnimation();
		_ScaleAnimation.InsertKeyFrame(0.0f, float3(0.0f, 0.0f, 1.0f), EaseOut());
		_ScaleAnimation.InsertExpressionKeyFrame(0.8f, L"Vector3(propSet.ScaleValue,propSet.ScaleValue,1f)", EaseOut());
		_ScaleAnimation.InsertExpressionKeyFrame(1.0f, L"Vector3(propSet.ScaleValue,propSet.ScaleValue,1f)", EaseOut());
		_ScaleAnimation.SetReferenceParameter(L"propSet", PropSet());
		_ScaleAnimation.Duration() = std::chrono::milliseconds(320);
		_ScaleAnimation.Target(L"Scale");
	}
	return _ScaleAnimation;
}

CompositionAnimationGroup RippleHelper::RippleAnimationGroup()
{
	if (_RippleAnimationGroup == nullptr)
	{
		_RippleAnimationGroup = compositor.CreateAnimationGroup();
		_RippleAnimationGroup.Add(OpacityAnimation());
		_RippleAnimationGroup.Add(ScaleAnimation());
	}
	return _RippleAnimationGroup;
}

CompositionPropertySet RippleHelper::PropSet()
{
	if (_PropSet == nullptr)
	{
		_PropSet = compositor.CreatePropertySet();
		PropSet().InsertScalar(L"ScaleValue", 2.0f);
	}
	return _PropSet;
}

CompositionBrush RippleHelper::Mask()
{
	using namespace winrt::Windows::UI::Xaml::Media;
	if (_Mask == nullptr)
	{
		auto surface = LoadedImageSurface::StartLoadFromUri(Uri(L"ms-appx:///Assets/Photos/RippleMask.png"), Size(100.0, 100.0));
		_Mask = compositor.CreateSurfaceBrush(surface);
	}
	return _Mask;
}

bool RippleHelper::GetIsFillEnable(DependencyObject obj)
{
	return (bool)obj.GetValue(IsFillEnableProperty);
}

void RippleHelper::SetIsFillEnable(DependencyObject obj, bool value)
{
	obj.SetValue(IsFillEnableProperty, winrt::box_value(value));
}

 TimeSpan RippleHelper::GetRippleDuration(UIElement obj)
 {
	 auto value = obj.GetValue(RippleDurationProperty);
	 return winrt::unbox_value<TimeSpan>(value);
 }

 void RippleHelper::SetRippleDuration(UIElement obj, TimeSpan value)
 {
	 obj.SetValue(RippleDurationProperty, winrt::box_value(value));
 }

 double RippleHelper::GetRippleRadius(UIElement obj)
 {
	 auto value = obj.GetValue(RippleRadiusProperty);
	 return winrt::unbox_value<double>(value);
 }

 void RippleHelper::SetRippleRadius(UIElement obj, double value)
 {
	 obj.SetValue(RippleRadiusProperty, winrt::box_value(value));
 }

 Color RippleHelper::GetRippleColor(UIElement obj)
 {
	 auto value = obj.GetValue(RippleColorProperty);
	 return winrt::unbox_value<Color>(value);
 }

 void RippleHelper::SetRippleColor(UIElement obj, Color value)
 {
	 obj.SetValue(RippleColorProperty, winrt::box_value(value));
 }

enum RippleHelperState RippleHelper::GetRippleHelperState(UIElement obj)
 {
    auto value = obj.GetValue(RippleHelperStateProperty);
	return winrt::unbox_value<RippleHelperState>(value);
 }

void RippleHelper::SetRippleHelperState(UIElement obj,RippleHelperState value)
{
	obj.SetValue(RippleHelperStateProperty, winrt::box_value(value));
}

void RippleHelper::OnRippleComplated(UIElement ele)
{
	for (auto const& handler : RippleComplated)
	{
		handler(ele, nullptr);
	}
}

void RippleHelper::StartRippleAnimation(UIElement ele, float2 position)
{
	StartRippleAnimation(ele, position, GetRippleColor(ele), GetIsFillEnable(ele), GetRippleDuration(ele), GetRippleRadius(ele));
}

void RippleHelper::StartRippleAnimation(UIElement ele, float2 position, Color color, bool isFillEnable, TimeSpan duration, double radius)
{
	Visual hostVisual = ElementCompositionPreview::GetElementVisual(ele);
	Compositor compositor = Window::Current().Compositor();
	ContainerVisual cVisual = ElementCompositionPreview::GetElementChildVisual(ele).try_as<ContainerVisual>();
	if (!cVisual)
	{
		cVisual = compositor.CreateContainerVisual();
		RippleHelper::SizeBind().ClearParameter(L"hostVisual");
		RippleHelper::SizeBind().SetReferenceParameter(L"hostVisual", hostVisual);
		cVisual.StartAnimation(L"Size", RippleHelper::SizeBind());
		Control control = ele.try_as<Control>();
		if (control)
		{
			CompositionRoundedRectangleGeometry geometry = compositor.CreateRoundedRectangleGeometry();
			geometry.Size(float2{ (float)ele.RenderSize().Width, (float)ele.RenderSize().Height });
			geometry.CornerRadius(float2{ (float)control.CornerRadius().TopLeft, (float)control.CornerRadius().TopRight });
			cVisual.Clip(compositor.CreateGeometricClip(geometry));
		}
		ElementCompositionPreview::SetElementChildVisual(ele, cVisual);
	}
	SpriteVisual sVisual = RippleHelper::CreateSpriteVisual(ele, color);
	cVisual.Children().InsertAtTop(sVisual);
	sVisual.Offset(float3{ position.x, position.y, 0.0f});

	if (isFillEnable)
	{
		double nWidth = std::max(std::max(position.x, ele.RenderSize().Width - position.x), std::max(position.y, ele.RenderSize().Height - position.y));
		double r = std::sqrt(nWidth * nWidth * 2);
		float finalScale = (float)r / 45.0f;
		RippleHelper::PropSet().InsertScalar(L"ScaleValue", finalScale);
		RippleHelper::ScaleAnimation().Duration(std::chrono::milliseconds(400));
		RippleHelper::OpacityAnimation().Duration(std::chrono::milliseconds(430));
	}
	else
	{
		if (radius == 100.0)
		{
			RippleHelper::PropSet().InsertScalar(L"ScaleValue", 2.0f);
		}
		else
		{
			RippleHelper::PropSet().InsertScalar(L"ScaleValue", (float)RippleHelper::GetRippleRadius(ele) / 45.0f);
		}
	}

	RippleHelper::ScaleAnimation().Duration(duration);
	RippleHelper::OpacityAnimation().Duration(duration);
	CompositionCommitBatch batch = compositor.GetCommitBatch(CompositionBatchTypes::Animation);
	batch.Completed([ele](const winrt::Windows::Foundation::IInspectable& sender, const winrt::Windows::UI::Composition::CompositionBatchCompletedEventArgs& args)
		{
			RippleHelper::OnRippleComplated(ele);
			cVisual.Children().Remove(sVisual);
		});
	sVisual.StartAnimationGroup(RippleHelper::RippleAnimationGroup());
}

SpriteVisual RippleHelper::CreateSpriteVisual(UIElement ele, Color color)
{
	SpriteVisual sVisual = compositor.CreateSpriteVisual();
	CompositionMaskBrush maskBrush = compositor.CreateMaskBrush();
	sVisual.AnchorPoint(float2{ 0.5f, 0.5f });
	sVisual.Size(float2{ 100.0f, 100.0f });
	maskBrush.Mask(Mask());
	maskBrush.Source(compositor.CreateColorBrush(color));
	sVisual.Brush(maskBrush);
	return sVisual;
}

void RippleHelper::Ele_PointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e)
{
	if (sender.try_as<UIElement>())
	{
		UIElement ele = sender.as<UIElement>();
		auto point = e.GetCurrentPoint(ele).Position();
		float2 position{ point.X, point.Y };
		RippleHelper::StartRippleAnimation(ele, position);
	}
}

enum RippleHelperState
{
	Pressed, Released, None
};