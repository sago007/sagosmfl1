/*
===========================================================================
blockattack - Block Attack - Rise of the Blocks
Copyright (C) 2014 Poul Sander

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

Source information and contacts persons can be found at
http://blockattack.sf.net
===========================================================================
*/

#ifndef GAMESTATE_HPP
#define	GAMESTATE_HPP

#include "SagoCommandQueue.hpp"

namespace sago {

class GameState {
public:
    /**
     * Is the state active. If this returns false then the State-manager will pop the state object
     * @return true if active
     */
    virtual bool IsActive() = 0;
    /**
     * Is the state blocking states under it?
     * @return true if blocking update
     */
    virtual bool IsBlockingDraw() { return false; };
    /**
     * Tells the state to draw itself to target
     * @param target The RenderWindow to draw to
     */
    virtual void Draw(sf::RenderWindow &target) = 0;
    /**
     * Is the state blocking lower states from updating?
     * @return true if blocking
     */
    virtual bool IsBlockingUpdate() { return false; };
    /**
     * Called reguarly by the game loop if not blocking
     * This is the only input given to a state!
     * @param fDeltaTime mili seconds since last update
     * @param input The input queue. Note that the state cannot push commands
     */
    virtual void Update(float fDeltaTime, const sago::SagoCommandQueue &input) = 0;
    /**
     * Allows the state to push events to the event queue
     * The state should not read from this!
     * @param inout The state may add events to this
     */
    virtual void UpdateCommandQueue(sago::SagoCommandQueue &inout) {};
	
	/**
	 * Process a console command. The console command is given as input. 
	 * arg[0] is the command as is always filled. Arguments thereafter are optional arguments
	 * @return true if the command was accpeted. False if the GameStateManager should pass the command to the next layer.
	 */
	virtual bool ProcessConsoleCommand(const std::vector<std::string>& arg) { return false; };
	
	virtual ~GameState() = default;
};

}

#endif	/* GAMESTATE_HPP */

