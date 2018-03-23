#pragma once

#include "Common.h"
#include "DebugMenu.h"

#include "Graphics/UI/Slider.h"
#include "Math/Maths.h"

namespace gebase { namespace debug {

	struct IAction
	{
		enum class ActionType
		{
			NONE = 0,
			EMPTY,
			PATH,
			BOOLEAN,
			VALUE,
			VEC2,
			VEC3,
			VEC4
		};

		String name;
		ActionType type;

		virtual void OnAction() = 0;
		virtual String ToString() const = 0;
	};

	typedef std::vector<IAction*> ActionList;

	struct EmptyAction : public IAction
	{
		EmptyAction(const String& name)
		{
			this->name = name;
			type = ActionType::EMPTY;
		}

		void OnAction() override {}
		String ToString() const override { return name; }
	};

	struct CustomAction : public IAction
	{
	private:
		std::function<void()> m_Function;
	public:
		CustomAction(const String& name, const std::function<void()>& function) : m_Function(function)
		{
			this->name = name;
		}

		void OnAction() override { m_Function(); }

		String ToString() const override { return name; }
	};

	struct BackAction : public IAction
	{
		PathAction* destination;

		BackAction(PathAction* destination)
		{
			this->name = "../";
			this->destination = destination;
			this->type = ActionType::PATH;
		}

		void OnAction() override { DebugMenu::setPath(destination); }
		
		String ToString() const override { return name; }
	};

	struct PathAction : public IAction
	{
		ActionList actionList;
		PathAction* parent;

		PathAction(const String& name, PathAction* parent)
		{
			this->name = name;
			this->parent = parent;
			this->type = ActionType::PATH;
		}

		void OnAction() override { DebugMenu::setPath(this); }
		
		String ToString() const override { return name; }

		bool ContainsAction(const String& name)
		{
			for (IAction* a : actionList)
				if (a->name == name)
					return true;
			return false;
		}

		bool DeleteChild(PathAction* child)
		{
			for (uint i = 0; i < actionList.size(); i++)
			{
				if (actionList[i] == child)
				{
					gedel actionList[i];
					actionList.erase(actionList.begin() + i);
					return true;
				}
			}
			return false;
		}

		PathAction* FindPath(const String& name)
		{
			for (IAction* a : actionList)
			{
				if (a->type == ActionType::PATH)
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
	};

	struct BooleanAction : public IAction
	{
		using Getter = std::function<bool()>;
		using Setter = std::function<void(bool)>;

	private:
		Getter m_Getter;
		Setter m_Setter;
	public:
		BooleanAction(const String& name, const Getter& getter, const Setter& setter) : m_Getter(getter), m_Setter(setter)
		{
			this->name = name;
			this->type = ActionType::BOOLEAN;
		}

		void OnAction() override { m_Setter(!m_Getter()); }

		String ToString() const override { return name + ": " + (m_Getter() ? "true" : "false"); }
	};

	template <typename T>
	struct ValueAction : public IAction
	{
		using Getter = std::function<T()>;
		using Setter = std::function<void(T)>;
	
	private:
		Getter m_Getter;
		Setter m_Setter;
		T m_Min;
		T m_Max;
	public:
		ValueAction(const String& name, const Getter& getter, const Setter& setter, T min, T max) : m_Getter(getter), m_Setter(setter), m_Min(min), m_Max(max)
		{
			this->name = name;
			this->type = ActionType::VALUE;
		}

		void OnAction() override
		{
			utils::LogUtil::WriteLine("ERROR", "[ValueAction] OnAction() - Not implemented");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		String ToString() const { return name + ": " + StringFormat::ToString(m_Getter); }
	};

	template<>
	void ValueAction<float>::OnAction()
	{
		float vals[1] = { (m_Getter() - m_Min) / (m_Max - m_Min) };
		graphics::ui::Slider::ValueChangedCallback callback[1] = { [&](float value) { m_Setter(value * (m_Max - m_Min) + m_Min); } };
		DebugMenu::Get()->EditValues(name, vals, 1, callback);
	}

	template<>
	String ValueAction<float>::ToString() const
	{
		return name + ": " + StringFormat::Float(m_Getter());
	}

	template<>
	void ValueAction<int32>::OnAction()
	{
		int32 vals[1] = { (m_Getter() - m_Min) / (m_Max - m_Min) };
		graphics::ui::Slider::ValueChangedCallback callback[1] = { [&](int32 value) { m_Setter(value * (m_Max - m_Min) + m_Min); } };
		DebugMenu::Get()->EditValues(name, vals, 1, callback);
	}

	template<>
	String ValueAction<int32>::ToString() const
	{
		return name + ": " + StringFormat::ToString(m_Getter());
	}

	template<>
	void ValueAction<math::Vector2f>::OnAction()
	{
		float vals[2] = {
			(m_Getter().x - m_Min.x) / (m_Max.x - m_Min.x),
			(m_Getter().y - m_Min.y) / (m_Max.y - m_Min.y)
		};
		graphics::ui::Slider::ValueChangedCallback callback[2] = {
			[&](float value) { m_Setter(math::Vector2f(value * (m_Max.x - m_Min.x) + m_Min.x, m_Getter().y)); },
			[&](float value) { m_Setter(math::Vector2f(m_Getter().x, value * (m_Max.y - m_Min.y) + m_Min.y)); }
		};
		DebugMenu::Get()->EditValues(name, vals, 2, callback);
	}

	template<>
	String ValueAction<math::Vector2f>::ToString() const
	{
		return name + ": " + StringFormat::Float(m_Getter().x) + ", " + StringFormat::Float(m_Getter().y);
	}

	template<>
	void ValueAction<math::Vector3f>::OnAction()
	{
		float vals[3] = {
			(m_Getter().x - m_Min.x) / (m_Max.x - m_Min.x),
			(m_Getter().y - m_Min.y) / (m_Max.y - m_Min.y),
			(m_Getter().z - m_Min.z) / (m_Max.z - m_Min.z)
		};
		graphics::ui::Slider::ValueChangedCallback callback[3] = {
			[&](float value) { m_Setter(math::Vector3f(value * (m_Max.x - m_Min.x) + m_Min.x, m_Getter().y, m_Getter().z)); },
			[&](float value) { m_Setter(math::Vector3f(m_Getter().x, value * (m_Max.y - m_Min.y) + m_Min.y, m_Getter().z)); },
			[&](float value) { m_Setter(math::Vector3f(m_Getter().x, m_Getter().y, value * (m_Max.z - m_Min.z) + m_Min.z)); }
		};
		DebugMenu::Get()->EditValues(name, vals, 3, callback);
	}

	template<>
	String ValueAction<math::Vector3f>::ToString() const
	{
		return name + ": " + StringFormat::Float(m_Getter().x) + ", " + StringFormat::Float(m_Getter().y) + ", " + StringFormat::Float(m_Getter().z);
	}

	template<>
	void ValueAction<math::Vector4f>::OnAction()
	{
		float vals[4] = {
			(m_Getter().x - m_Min.x) / (m_Max.x - m_Min.x),
			(m_Getter().y - m_Min.y) / (m_Max.y - m_Min.y),
			(m_Getter().z - m_Min.z) / (m_Max.z - m_Min.z),
			(m_Getter().w - m_Min.w) / (m_Max.w - m_Min.w)
		};
		graphics::ui::Slider::ValueChangedCallback callback[4] = {
			[&](float value) { m_Setter(math::Vector4f(value * (m_Max.x - m_Min.x) + m_Min.x, m_Getter().y, m_Getter().z, m_Getter().w)); },
			[&](float value) { m_Setter(math::Vector4f(m_Getter().x, value * (m_Max.y - m_Min.y) + m_Min.y, m_Getter().z, m_Getter().w)); },
			[&](float value) { m_Setter(math::Vector4f(m_Getter().x, m_Getter().y, value * (m_Max.z - m_Min.z) + m_Min.z, m_Getter().w)); },
			[&](float value) { m_Setter(math::Vector4f(m_Getter().x, m_Getter().y, m_Getter().z, value * (m_Max.w - m_Min.w) + m_Min.w)); }
		};
		DebugMenu::Get()->EditValues(name, vals, 4, callback);
	}

	template<>
	String ValueAction<math::Vector4f>::ToString() const
	{
		return name + ": " + StringFormat::Float(m_Getter().x) + ", " + StringFormat::Float(m_Getter().y) + ", " + StringFormat::Float(m_Getter().z) + ", " + StringFormat::Float(m_Getter().w);
	}

	typedef ValueAction<int32> IntAction;
	typedef ValueAction<float> FloatAction;
	typedef ValueAction<math::Vector2f> Vec2Action;
	typedef ValueAction<math::Vector3f> Vec3Action;
	typedef ValueAction<math::Vector4f> Vec4Action;

} }