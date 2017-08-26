#pragma once

#include "Common.h"

#include "Graphics/Renderer/Renderer2D.h"
#include "Graphics/UI/Panel.h"
#include "Graphics/UI/Slider.h"

#include "DebugMenuItem.h"
#include "DebugMenuSlider.h"

namespace gebase { namespace debug {

	struct IAction;
	struct PathAction;
	typedef std::vector<IAction*> ActionList;

	struct DebugMenuSettings
	{
		float horizontalPadding;
		float verticalPadding;
		float fontSize;
	};

	class GE_API DebugMenu
	{
	private:
		static DebugMenu* s_Instance;
		
		bool m_Visible;
		DebugMenuSettings m_Settings;

		ActionList m_ActionList;
		PathAction* m_Path;

		graphics::ui::Panel* m_Panel;
		graphics::ui::Slider** m_Slider;

		String m_LastEditedName;

		DebugMenu();
		void Refresh();
		PathAction* CreateOrFindPaths(std::vector<String>& paths, PathAction* action = nullptr);
	public:
		static DebugMenu* Get();

		static void Init();
		static bool isVisible();
		static void setVisible(bool visible);
		static void setPath(PathAction* path);
		static DebugMenuSettings& getSettings();

		PathAction* FindPath(const String& path);

		void OnActivate();
		void OnDeactivate();

		void OnUpdate(float delta);
		void OnRender(graphics::Renderer2D& renderer);

		void EditValues(const String& name, float* value, uint count, const graphics::ui::Slider::ValueChangedCallback* callback);
		void EditValues(const String& name, int32* value, uint count, const graphics::ui::Slider::ValueChangedCallback* callback);

		bool OnMousePressed(events::MousePressedEvent& e);
		bool OnMouseReleased(events::MouseReleasedEvent& e);

		static void Add(const String& path);
		static void Add(const String& path, IAction* action);
		static void Add(const String& path, const std::function<void()>& function);
		static void Add(const String& path, bool* value);
		static void Add(const String& path, float* value);
		static void Add(const String& path, float* value, float min, float max);
		static void Add(const String& path, math::Vector2f* value, float min = 0.0f, float max = 100.0f);
		static void Add(const String& path, math::Vector3f* value, float min = 0.0f, float max = 100.0f);
		static void Add(const String& path, math::Vector4f* value, float min = 0.0f, float max = 100.0f);

		static void Remove(const String& path);
	};

} }