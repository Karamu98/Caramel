#pragma once
#include "Caramel/Core/Core.h"
#include "EditorPanel.h"

namespace Caramel
{
	class Scene;
	class SceneViewPanel : public EditorPanel
	{
	public:
		inline void SetTargetScene(Ref<Scene> target) { m_targetScene = target; };

	protected:
		virtual void InternalRender() override;

	private:
		Ref<Scene> m_targetScene;
	};
}