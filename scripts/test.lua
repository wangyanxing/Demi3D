require "common"

DI_LOG('hello lua')
local vec2 = Demi.DiVec2:new_local(4,3)
DI_LOG(vec2:length())