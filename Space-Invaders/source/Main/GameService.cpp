#ifndef MAIN_GAMESERVICE_H
#define MAIN_GAMESERVICE_H

#include "../../header/Main/GameService.h"
#include "../../header/Graphics/GraphicService.h"
#include "../../header/Event/EventService.h"
#include "../../header/UI/UIService.h"

namespace Main
{
	class GameService
	{
	private:
		ServiceLocator* service_locator;
		GraphicWindow* game_window;
		GameState current_state;

	public:
		GameService();
		~GameService();

		void ignite();
		void initialize();
		void initializeVariables();
		void showSplashScreen();
		bool isRunning();
		void update();
		void render();
		void destroy();
		void setGameState(GameState new_state);
		GameState getGameState();
	};

} 

#include "GameService.h"

namespace Main
{
	GameService::GameService() : service_locator(nullptr), current_state(GameState::BOOT), game_window(nullptr) {}

	GameService::~GameService() { destroy(); }

	void GameService::ignite()
	{
		service_locator = ServiceLocator::getInstance();
		if (service_locator == nullptr)
		{
			return;
		}
		initialize();
	}

	void GameService::initialize()
	{
		service_locator->initialize();
		initializeVariables();
		showSplashScreen();
	}

	void GameService::initializeVariables()
	{
		if (service_locator == nullptr || service_locator->getGraphicService() == nullptr)
		{
			return;
		}
		game_window = service_locator->getGraphicService()->getGameWindow();
	}

	void GameService::showSplashScreen()
	{
		setGameState(GameState::SPLASH_SCREEN);
		UIService* uiService = ServiceLocator::getInstance()->getUIService();
		if (uiService == nullptr)
		{
			return;
		}
		uiService->showScreen();
	}

	bool GameService::isRunning()
	{
		if (service_locator == nullptr || service_locator->getGraphicService() == nullptr)
		{
			return false;
		}
		return service_locator->getGraphicService()->isGameWindowOpen();
	}

	void GameService::update()
	{
		if (service_locator == nullptr || service_locator->getEventService() == nullptr)
		{
			return;
		}
		service_locator->getEventService()->processEvents();
		service_locator->update();
	}

	void GameService::render()
	{
		if (game_window == nullptr || service_locator == nullptr)
		{
			return;
		}
		game_window->clear();
		service_locator->render();
		game_window->display();
	}

	void GameService::destroy()
	{
		if (service_locator != nullptr)
		{
			service_locator->deleteServiceLocator();
			service_locator = nullptr;
		}
	}

	void GameService::setGameState(GameState new_state)
	{
		current_state = new_state;
	}

	GameState GameService::getGameState()
	{
		return current_state;
	}
} 
//change
