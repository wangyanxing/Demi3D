function DI_WARNING (log)
	Demi.DiLogManager:GetInstance():Output(Demi.LOG_LEVEL_WARNING,log)
end

function DI_LOG (log)
	Demi.DiLogManager:GetInstance():Output(Demi.LOG_LEVEL_LOG,log)
end

function DI_DEBUG (log)
	Demi.DiLogManager:GetInstance():Output(Demi.LOG_LEVEL_DEBUG,log)
end

DI_LOG('hello lua')
local vec2 = Demi.DiVec2:new_local(4,3)
DI_LOG(vec2:length())