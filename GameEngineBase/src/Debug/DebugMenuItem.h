#pragma once

#include "Common.h"
#include "Math/Maths.h"
#include "Graphics/UI/Button.h"

namespace gebase { namespace debug {

	struct IAction;

	class DebugMenuItem : public graphics::ui::Button
	{
	protected:
		IAction* m_Action;
	public:
		DebugMenuItem(IAction* action, const math::Rectangle& bounds);

		void OnUpdate(float delta) override;
		inline const IAction* getAction() const { return m_Action; }
	};

} }