/*-------------------------------------------------------------------------\
| File: GAMESTATE.H															|
| Desc: Provides declarations for a gamestate								|
| Definition File: GAMESTATE.CPP											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <vector>

namespace GameFramework
{
	enum GameStateID
	{
		MENU,
		INGAME
	};

	class GameState
    {
	public:
		GameState();
		GameState(GameStateID stateID);
		virtual ~GameState();

		GameState(const GameState& param);
		virtual GameState& operator=(const GameState& param);

		void Enable();
		void Disable();
		bool IsEnabled() const;

		GameStateID ID();

		virtual void Update();

	private:
		GameStateID id;
		bool enabled;

		virtual void Initialize();
	};

	class DrawableGameState : public GameState
	{
	public:
		DrawableGameState();
        DrawableGameState(GameStateID stateID);
        virtual ~DrawableGameState();

        DrawableGameState(const DrawableGameState& param);
        virtual DrawableGameState& operator=(const DrawableGameState& param);

		void SetVisibility(bool visible);
		bool IsVisible() const;

        virtual void Render();
	private:
		bool visible;
	};

    class GameStateCollection
    {
        public:
            GameStateCollection();
            ~GameStateCollection();

            GameStateCollection(const GameStateCollection& param);
            GameStateCollection& operator=(const GameStateCollection& param);

            void AddState(GameState* newState);
            void RemoveState(GameStateID stateID);
            void EnableState(GameStateID stateID);

            std::vector<GameState*>* States();

        private:
            std::vector<GameState*> states;

            std::vector<GameState*>::iterator srch(GameStateID stateID);
    }; 
}

#endif // _GAMESTATE_H_