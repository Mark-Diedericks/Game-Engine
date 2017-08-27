#include "ge.h"
#include "DebugMenu.h"
#include "DebugMenuAction.h"

#include "Application/Window.h"
#include "Graphics/UI/Button.h"
#include "System/Memory.h"

namespace gebase { namespace debug {

	using namespace math;
	using namespace graphics;
	using namespace ui;

	DebugMenu* DebugMenu::s_Instance = nullptr;

	DebugMenu* DebugMenu::Get()
	{
		return s_Instance;
	}

	DebugMenu::DebugMenu() : m_Visible(false), m_Slider(nullptr), m_Path(nullptr)
	{
		s_Instance = this;

		m_Settings.horizontalPadding = 0.4f;
		m_Settings.verticalPadding = 0.6f;
		m_Settings.fontSize = 20.0f;

		Add("Debug Menu/Horizontal Padding", &m_Settings.horizontalPadding, 0.0f, 4.0f);
		Add("Debug Menu/Vertical Padding", &m_Settings.verticalPadding, 0.0f, 4.0f);
		Add("Debug Menu/Font Size", &m_Settings.fontSize, 8.0f, 64.0f);

		m_Slider = genew Slider*[4];
		m_Panel = genew Panel();
	}

	void DebugMenu::Init()
	{
		if (s_Instance != nullptr)
			return;

		genew DebugMenu();
	}

	void DebugMenu::EditValues(const String& name, float* values, uint count, const Slider::ValueChangedCallback* callbacks)
	{
		for (uint i = 0; i < 4; i++)
		{
			m_Slider[i]->setActive(false);

			if (i < count)
			{
				m_Slider[i]->setCallback(callbacks[i]);
				m_Slider[i]->setActive(m_LastEditedName != name);
				m_Slider[i]->setValue(values [i]);
			}
		}

		m_LastEditedName = m_LastEditedName != name ? name : "";
	}

	void DebugMenu::EditValues(const String& name, int32* values, uint count, const Slider::ValueChangedCallback* callbacks)
	{
		for (uint i = 0; i < 4; i++)
		{
			m_Slider[i]->setActive(false);

			if (i < count)
			{
				m_Slider[i]->setCallback(callbacks[i]);
				m_Slider[i]->setActive(m_LastEditedName != name);
				m_Slider[i]->setValue((float)values[i]);
			}
		}

		m_LastEditedName = m_LastEditedName != name ? name : "";
	}

	PathAction* DebugMenu::FindPath(const String& name)
	{
		for (IAction* a : m_ActionList)
		{
			if (a->type == IAction::ActionType::PATH)
			{
				PathAction* action = (PathAction*)a;
				if (action->name == name)
					return action;
				else
					action->FindPath(name);
			}
		}
		return nullptr;
	}

	bool DebugMenu::isVisible()
	{
		return s_Instance->m_Visible;
	}

	void DebugMenu::setVisible(bool visible)
	{
		s_Instance->m_Visible = visible;
		visible ? s_Instance->OnActivate() : s_Instance->OnDeactivate();
	}
	 
	void DebugMenu::setPath(PathAction* path)
	{
		s_Instance->m_Path = path;
		s_Instance->Refresh();
	}

	DebugMenuSettings& DebugMenu::getSettings()
	{
		return s_Instance->m_Settings;
	}

	bool DebugMenu::OnMousePressed(events::MousePressedEvent& e)
	{
		return false;
	}

	bool DebugMenu::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		return false;
	}

	void DebugMenu::OnActivate()
	{
		float maxWidth = 0.0f;
		float height = m_Settings.verticalPadding;
		float yOffset = height;

		ActionList* actionList = m_Path ? &m_Path->actionList : &m_ActionList;
		if (m_Path)
		{
			DebugMenuItem* item = genew DebugMenuItem(genew BackAction(m_Path->parent), Rectangle(0.0f, 18.0f - yOffset, 0.0f, height));
			m_Panel->Add(item);
			yOffset += height * 2.0f;
			maxWidth = item->getFont().getWidth(item->getAction()->ToString()) * 0.5f;
		}

		for (IAction* action : *actionList)
		{
			float y = 18.0f - yOffset;
			DebugMenuItem* item = genew DebugMenuItem(action, Rectangle(0.0f, y, 0.0f, height));
			m_Panel->Add(item);
			yOffset += height * 2.0f;

			float strWidth = item->getFont().getWidth(item->getAction()->ToString()) * 0.5f;
			if (strWidth > maxWidth)
				maxWidth = strWidth;
		}

		maxWidth += m_Settings.horizontalPadding;

		for (Widget* widget : m_Panel->getWidgets())
		{
			DebugMenuItem* item = (DebugMenuItem*)widget;
			Rectangle& bounds = item->getBounds();
			bounds.x = maxWidth;
			bounds.width = maxWidth;
		}

		const float sliderWidth = 0.75f;
		float sliderX = maxWidth * 2.0f + sliderWidth;
		
		for (uint i = 0; i < 4; i++)
		{
			m_Slider[i] = genew Slider({ sliderX + i * sliderWidth * 2.0f, 9.0f, sliderWidth, 9.0f }, true);
			m_Panel->Add(m_Slider[i])->setActive(false);
		}
	}

	void DebugMenu::OnDeactivate()
	{
		m_Panel->Clear();
	}

	void DebugMenu::Refresh()
	{
		if (!m_Panel || !isVisible())
			return;

		OnDeactivate();
		OnActivate();
	}

	void DebugMenu::OnUpdate(float delta)
	{

	}

	void DebugMenu::OnRender(Renderer2D& renderer)
	{

	}

	PathAction* DebugMenu::CreateOrFindPaths(std::vector<String>& paths, PathAction* action)
	{
		if (paths.empty())
			return action;

		String name = paths.front();
		paths.erase(paths.begin());

		ActionList* actionList = action ? &action->actionList : &m_ActionList;
		for (IAction* a : *actionList)
			if (a->type == IAction::ActionType::PATH && a->name == name)
				return CreateOrFindPaths(paths, (PathAction*)a);

		PathAction* pathAction = genew PathAction(name, action);
		actionList->push_back(pathAction);
		return CreateOrFindPaths(paths, pathAction);
	}

	void DebugMenu::Add(const String& path, IAction* action)
	{
		if (Contains(path, "/"))
		{
			std::vector<String> paths = SplitString(path, "/");
			action->name = paths.back();
			paths.pop_back();

			PathAction* pathAction = s_Instance->CreateOrFindPaths(paths);

			if (!pathAction)
			{
				std::cout << "[DebugMenu] Add() - pathAction is null." << std::endl;
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			if (!pathAction->ContainsAction(action->name))
				pathAction->actionList.push_back(action);
			else
				gedel action;
		}
		else
		{
			s_Instance->m_ActionList.push_back(action);
		}
		s_Instance->Refresh();
	}

	void DebugMenu::Remove(const String& path)
	{
		if (Contains(path, "/"))
		{
			std::vector<String> paths = SplitString(path, "/");
			String name = paths.back();
			paths.pop_back();

			PathAction* pathAction = s_Instance->CreateOrFindPaths(paths);

			if (!pathAction)
			{
				std::cout << "[DebugMenu] Add() - pathAction is null." << std::endl;
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			if (!pathAction->ContainsAction(name))
			{
				if (pathAction->actionList.size() == 1)
				{
					PathAction* parent = pathAction->parent;

					if (parent)
					{
						parent->DeleteChild(pathAction);
					}
					else
					{
						for (uint i = 0; i < s_Instance->m_ActionList.size(); i++)
						{
							if (s_Instance->m_ActionList[i] == pathAction)
							{
								gedel s_Instance->m_ActionList[i];
								s_Instance->m_ActionList.erase(s_Instance->m_ActionList.begin() + i);
								break;
							}
						}
					}

					while (parent)
					{
						gedel pathAction;
						pathAction = pathAction->parent;
					}
				}
				else
				{
					ActionList& actions = pathAction->actionList;
					for (uint i = 0; i < actions.size(); i++)
					{
						if (actions[i]->name == name)
						{
							actions.erase(actions.begin() + i);
							break;
						}
					}
				}
			}
		}
		else
		{
			ActionList& actions = s_Instance->m_ActionList;
			for (uint i = 0; i < actions.size(); i++)
			{
				if (actions[i]->name == path)
				{
					gedel actions[i];
					actions.erase(actions.begin() + i);
					break;
				}
			}
		}

		s_Instance->Refresh();
	}

	void DebugMenu::Add(const String& path)
	{
		Add(path, genew EmptyAction(path));
	}

	void DebugMenu::Add(const String& path, const std::function<void()>& function)
	{
		Add(path, genew CustomAction(path, function));
	}

	void DebugMenu::Add(const String& path, bool* value)
	{
		Add(path, genew BooleanAction(path, [value]() { return *value; }, [value](bool v) { *value = v; }));
	}

	void DebugMenu::Add(const String& path, float* value)
	{
		Add(path, value, 0.0f, 100.0f);
	}

	void DebugMenu::Add(const String& path, float* value, float min, float max)
	{
		Add(path, genew FloatAction(path, [value]() { return *value; }, [value](float v) { *value = v; }, min, max));
	}

	void DebugMenu::Add(const String& path, math::Vector2f* value, float min, float max)
	{
		Add(path, genew Vec2Action(path, [value]() { return *value; }, [value](math::Vector2f v) { *value = v; }, math::Vector2f(min, min), math::Vector2f(max, max)));
	}

	void DebugMenu::Add(const String& path, math::Vector3f* value, float min, float max)
	{
		Add(path, genew Vec3Action(path, [value]() { return *value; }, [value](math::Vector3f v) { *value = v; }, math::Vector3f(min, min, min), math::Vector3f(max, max, max)));
	}

	void DebugMenu::Add(const String& path, math::Vector4f* value, float min, float max)
	{
		Add(path, genew Vec4Action(path, [value]() { return *value; }, [value](math::Vector4f v) { *value = v; }, math::Vector4f(min, min, min, min), math::Vector4f(max, max, max, max)));
	}

} }