
app_path = "res/games/block_game/main.lua"

local make_main = function()
	
	local main = {}

	main.__index = main

	main.current_program = nil

	function main:init()
	end

	function main:update(dt)	--delta time
	end

	function main:render()
	end

	function main:gui()

	end

	return main
end

g_main_app = make_main()
g_main_app:init()