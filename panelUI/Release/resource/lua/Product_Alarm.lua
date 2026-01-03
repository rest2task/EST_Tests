
local SYSTEM_STATE_FL_AUTOALARM = 0x00160004 --自动警报状态
local ALARM_STATE = 0x33120023
local INJECT_STATE_ADPOSI_INJCUSHION = 0x10160005  --60C 射出转保压位置
local INJECT_STATE_ADPOSI_INJHOLDEND = 0x10160004  --60B 射出终点位置实际值

local SYSTEM_MACHSET_FL_MACHINECODE48 = 0x00120008 --code48
local SYSTEM_MACHSET_FL_MACHINECTRL2 = 0x00120002 -- 402
local isatuoArmState = band(VarAdrToInt(SYSTEM_MACHSET_FL_MACHINECODE48),0x100) ~= 0
local isInjEndPos = band(VarAdrToInt(SYSTEM_MACHSET_FL_MACHINECTRL2),0x01) ~= 0

function PageCreate()

end

function PageShow()
	SetPageLockMode(1)
--自动警报
	if(isatuoArmState) then
		WGTSetAttrByName("tabpage1_Tb1_stbt11","value",SYSTEM_STATE_FL_AUTOALARM)
	else
		WGTSetAttrByName("tabpage1_Tb1_stbt11","value",ALARM_STATE)
	end


--射出监控位置
	if(isInjEndPos) then
		WGTSetAttrByName("tabpage1_Tb1_edb2","text",INJECT_STATE_ADPOSI_INJHOLDEND)
	else
		WGTSetAttrByName("tabpage1_Tb1_edb2","text",INJECT_STATE_ADPOSI_INJCUSHION)
	end
    
    TabPageSelPage("tabpage1",0)
	FkeyTab("tabpage1", 0x201)
end

function PageUpdate()

end

function PageChange(name,adr,oldvalue,newvalue)

	return 0
end

function PageHide()
	SetPageLockMode(0)
end
