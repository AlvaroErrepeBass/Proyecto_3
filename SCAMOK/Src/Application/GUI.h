#pragma once
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <glm/glm.hpp>


	class GUI {
	public:
		void init(const std::string& resourceDirectory);
		void destroy();

		void draw();

		void loadScheme(const std::string& schemeFile);
		void setFont(const std::string& fontFile);
		CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
		static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);

		// Getters
		static CEGUI::OgreRenderer* getRenderer() { return m_renderer; }
		const CEGUI::GUIContext* getContext() { return m_context; }
	private:
		static CEGUI::OgreRenderer* m_renderer;
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Window* m_root = nullptr;
	};
