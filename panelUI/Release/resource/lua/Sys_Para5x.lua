local CLAMP_MOLD_FL_CLSOPEN_MODE = 0x20130060 --开关模模式
local CLAMP_MOLD_SMOOTH_FUNCTION = 0x331200da --开关模平稳功能
local FL_LOCATE_DA = 0x20130072   --开关模平稳使用比例阀
local FL_CLS_LOCATE = 0x2013006e  --关模平稳功能
local FL_OPN_LOCATE = 0x2013006a  --开模平稳功能
local SYS_MACH_CUSTOMID  = 0x00120018  --客户及机器代码

--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()
	SetPageLockMode(1)
    
	--20241025 chj 宁塑厂商代码开放
	if(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) == 0x7500) then
		WGTSetVisible("labelsafemoudle", 1)
        WGTSetVisible("labelsafemoudle_unit", 1)
        WGTSetVisible("editsafemoudle", 1)
        
		WGTSetVisible("labelsafemoudleuse", 1)
        WGTSetVisible("dpsafemoudleuse", 1)
        
        
		WGTSetVisible("labelsaferelay", 1)
        WGTSetVisible("labelsaferelay_unit", 1)
        WGTSetVisible("editsaferelay", 1)
        
        WGTSetVisible("labelsaferelayuse", 1)
        WGTSetVisible("dpsaferelayuse", 1)
        
    else
		WGTSetVisible("labelsafemoudle", 0)
        WGTSetVisible("labelsafemoudle_unit", 0)
        WGTSetVisible("editsafemoudle", 0)
        
		WGTSetVisible("labelsafemoudleuse", 0)
        WGTSetVisible("dpsafemoudleuse", 0)
        
        
		WGTSetVisible("labelsaferelay", 0)
        WGTSetVisible("labelsaferelay_unit", 0)
        WGTSetVisible("editsaferelay", 0)
        
        WGTSetVisible("labelsaferelayuse", 0)
        WGTSetVisible("dpsaferelayuse", 0)
    end
end

--画面隐藏--
function PageHide()
	SetPageLockMode(0)
end

--画面刷新--
function PageUpdate()

end

--画面修改--
function PageChange(name,adr,oldvalue,newvalue)


    return 0
end

