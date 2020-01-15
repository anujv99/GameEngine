
a = "gg"

Game = {
	Update = function()
		while (true) do
			ImGui.Begin("Lua Window")
			a = ImGui.TextInput("Window Title", a, 100)
			ImGui.End()

			Window.Title = a

			coroutine.yield()
		end
	end
}

co = coroutine.create(Game.Update)