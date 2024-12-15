#pragma once
#include "winrt/Windows.UI.Composition.h"
using namespace winrt::Windows::UI;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Input;
using namespace winrt::Microsoft::UI::Composition;
class RippleHelper : DependencyObject
{
private:
	static  PointerEventHandler pointerEventHandler;
	static Compositor compositor;
	static ExpressionAnimation _SizeBind;
	static CompositionEasingFunction _EaseOut;
	static ScalarKeyFrameAnimation _OpacityAnimation;
	static Vector3KeyFrameAnimation _ScaleAnimation;
	static CompositionAnimationGroup _RippleAnimationGroup;
	static CompositionPropertySet _PropSet;
	static CompositionBrush _Mask;
	static ExpressionAnimation SizeBind();
	static CompositionEasingFunction EaseOut();
	static ScalarKeyFrameAnimation OpacityAnimation();
	static Vector3KeyFrameAnimation ScaleAnimation();
	static CompositionAnimationGroup RippleAnimationGroup();
	static CompositionPropertySet PropSet();
	static CompositionBrush Mask();
	static bool GetIsFillEnable(DependencyObject);
	static void SetIsFillEnable(DependencyObject,bool);
	static TimeSpan GetRippleDuration(UIElement);
	static void SetRippleDuration(UIElement, TimeSpan);
	static double GetRippleRadius(UIElement);
	static void SetRippleRadius(UIElement, double);
	static Color GetRippleColor(UIElement);
	static void SetRippleColor(UIElement, Color);
	static enum RippleHelperState GetRippleHelperState(UIElement);
	static void SetRippleHelperState(UIElement, RippleHelperState);
	static winrt::event<EventHandler<UIElement>> RippleComplated;
	static void OnRippleComplated(UIElement);
	static void StartRippleAnimation(UIElement, float2);
	static void StartRippleAnimation(UIElement, float2, Color, bool, TimeSpan, double);
	static SpriteVisual CreateSpriteVisual(UIElement, Color);
	static void Ele_PointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e);
public:
	enum RippleHelperState;
};


