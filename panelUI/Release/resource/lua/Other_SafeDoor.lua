local SYS_FL_MACH_CODE0 = 0x00120000 --code0
local SYS_FL_MACH_CODE3 = 0x00120003 --code3
local SYS_FL_MACH_CODE4 = 0x00120004 --code4
local SYS_FL_MACH_CODE53= 0x0012000D --code53
local FL_PWRDOORCTRL = 0x29130000 --自动安全门

local TMP_AUTODOOR = 0x33150000 --自动安全门使用临时变量0

local FL_PWRDOOROPEN = 0x29130001  --安全门开模式
local AUTO_SAFE_DOOR_ENABLE = 0x33140158    --自动安全门是否使用

local TMP_SAFEDR_KEY_MODE = 0x33150001 --自动安全门按键模式

--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()
	SetPageLockMode(1)
	
    --自动安全门使用临时变量0
    if((band(VarAdrToInt(SYS_FL_MACH_CODE4),0x80)~=0) and (VarAdrToInt(FL_PWRDOORCTRL)==1)) then
		VarAdrSetInt(TMP_AUTODOOR, 1)
    else
		VarAdrSetInt(TMP_AUTODOOR, 0)
    end
    
    if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",1,true)
        WGTSetVisible("stTips", 1)
    else 
		TabPageSetSubVisble("tabpage1",1,false)
        WGTSetVisible("stTips", 0)
    end
    
	--自动安全门按键模式临时变量1
    if(band(VarAdrToInt(SYS_FL_MACH_CODE0),0x8000)~=0) then
		VarAdrSetInt(TMP_SAFEDR_KEY_MODE, 0)
    else
		VarAdrSetInt(TMP_SAFEDR_KEY_MODE, 1)
    end
    
    TabPageSelPage("tabpage1",0)
	FkeyTab("tabpage1", 0x201)
end

--画面隐藏--
function PageHide()
	SetPageLockMode(0)
end

--画面刷新--
function PageUpdate()
    --在安全门开模式选择5-开模中途时开放设置，否则变灰
    if((VarAdrToInt(FL_PWRDOOROPEN) == 5) and (VarAdrToInt(AUTO_SAFE_DOOR_ENABLE) ~= 0))then
		WGTSetEnable("editdropenpos",1)
        WGTSetEnable("labeldropenpos_unit",1)
        WGTSetEnable("labeldropenpos",1)
    else
		WGTSetEnable("editdropenpos",0)
        WGTSetEnable("labeldropenpos_unit",0)
        WGTSetEnable("labeldropenpos",0)
    end
end

--画面修改--
local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50
function PageChange(name,adr,oldvalue,newvalue)
	print("SafeDoor PageChange")
	if(name == "stbt11") then --自动安全门
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			CtlAdrInt(FL_PWRDOORCTRL, newvalue) --自动安全门使用临时变量0
			if(newvalue == 1) then
				VarAdrSetInt(SYS_FL_MACH_CODE4,bor(VarAdrToInt(SYS_FL_MACH_CODE4),0x80))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE4,band(VarAdrToUI16(SYS_FL_MACH_CODE4),0xFF7F))
			end
			CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
		else
			CtlAdrInt(TMP_AUTODOOR,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "cb21") then --安全门类型选择
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_MACH_CODE3,bor(VarAdrToInt(SYS_FL_MACH_CODE3),0x0200))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE3,band(VarAdrToInt(SYS_FL_MACH_CODE3),bnot(0x0200)))
			end
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "cb31") then --安全门油路选择
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_MACH_CODE53,bor(VarAdrToInt(SYS_FL_MACH_CODE53),0x1000))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE53,band(VarAdrToInt(SYS_FL_MACH_CODE53),bnot(0x1000)))
			end
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "cb41") then --安全门开模式
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
        
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "dropdownkeymode") then --自动安全门按键模式
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			if(newvalue == 0) then
				VarAdrSetInt(SYS_FL_MACH_CODE0,bor(VarAdrToInt(SYS_FL_MACH_CODE0),0x8000))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE0,band(VarAdrToInt(SYS_FL_MACH_CODE0),bnot(0x8000)))
			end
			CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
		else
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
        
    end

    return 0
end
