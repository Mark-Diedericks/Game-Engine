#include "ge.h"
#include "DebugMenuItem.h"
#include "DebugMenuAction.h"
#include "DebugMenu.h"

#include "Graphics/Font/FontManager.h"

namespace gebase { namespace debug {

	DebugMenuItem::DebugMenuItem(IAction* action, const math::Rectangle& bounds) : Button(action->ToString(), bounds, [action]() { action->OnAction(); }), m_Action(action)
	{

	}

	void DebugMenuItem::OnUpdate(float delta)
	{
		m_Label = m_Action->ToString();
		m_Font = graphics::FontManager::Get((uint)DebugMenu::Get()->getSettings().fontSize);
	}

} }