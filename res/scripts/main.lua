
app_path = "res/games/block_game/main.lua"

local make_main = function()
	
	local main = {}

	main.__index = main

	main.current_program = nil

	main.rect_center = Vec2.New(0, 0)
	main.rect_dimen  = Vec2.New(1, 1)

	function main:init()
	end

	function main:update(dt)	--delta time
	end

	function main:render()
		-- ImmGFX.DrawRect(self.rect_center, self.rect_dimen)
	end

	function main:gui()
		ImGui.Begin("Lua menu");

		self.rect_center.x = ImGui.SliderFloat("Center", self.rect_center.x, -2, 2 )
		self.rect_center.y = ImGui.SliderFloat("", self.rect_center.y, -2, 2 )

		ImGui.Seperator()

		self.rect_dimen.x = ImGui.SliderFloat("Dimen", self.rect_dimen.x, -2, 2 )
		self.rect_dimen.y = ImGui.SliderFloat("", self.rect_dimen.y, -2, 2 )

		ImGui.End()
	end

	return main
end

g_main_app = make_main()
g_main_app:init()