local logger = Demi.DiLogManager:GetInstance()
function DI_WARNING (log)
	logger:Output(Demi.LOG_LEVEL_WARNING,log)
end

DI_WARNING('hello')
local vec2 = Demi.DiVec2:new_local(4,3)
print(vec2:length())