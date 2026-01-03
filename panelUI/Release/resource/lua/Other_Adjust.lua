local OPMODE_INDEX = 0x33160013 --Operation mode index
local VM_MSG_CHANGETOADJMOLD = 116 -- 请切换到调模状态 20211224 dyl 
local _KEY_PANEL_PITCHSELFLEARN = 0x4066 --齿距自学习
local _KEY_PANEL_NULL = 0x00F0 --空键
local TEXT_START = 4
local TEXT_STOP = 5

function SetToothMode() --20211224 dyl 齿距自学习启动文本显示
	if(VarAdrToInt(OPMODE_INDEX) == 15) then
		ButtonSetText("tabpage1_Tb2_btstart",GetTextTran(TEXT_STOP))
	else
		ButtonSetText("tabpage1_Tb2_btstart",GetTextTran(TEXT_START))
	end
end

--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()
	SetPageLockMode(1)
	
	G10SeqVlvGuideSwitch()

	if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",2,true)
        TabPageSetSubVisble("tabpage1",3,true)
    else 
		TabPageSetSubVisble("tabpage1",2,false)
        TabPageSetSubVisble("tabpage1",3,false)
	end
	
	TabPageSelPage("tabpage1",0)
	FkeyTab("tabpage1", 0x200)
end

--画面隐藏--
function PageHide()
	SetPageLockMode(0)
end

--画面刷新--
function PageUpdate()
	SetToothMode() --20211224 dyl 齿距自学习启动文本显示
end

--画面修改--
local SYS_FL_MACH_CODE0 = 0x00120000 -- code0
local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50
local ADJUST_MACH_ADPOSI_ADJCUR = 0x24120006 --模板校正厚度 
local ADJUST_MACH_ADPOSI_ADJMAX = 0x24120008 --最大模厚 
local ADJUST_MACH_ADPOSI_ADJTARG = 0x2412000b  --调模更换位置 机器参数
local SYSTEM_MACH_ADPOSI_ADJTARG = 0x32130037 --模具厚度 

function PageChange(name,adr,oldvalue,newvalue)
	print("Adjust PageChange")
	if(name == "cb42") then --自动调模方式
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_MACH_CODE0,bor(VarAdrToInt(SYS_FL_MACH_CODE0),0x0008))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE0,band(VarAdrToInt(SYS_FL_MACH_CODE0),bnot(0x0008)))
			end
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "ed61") then --模具厚度
		CtlAdrInt(ADJUST_MACH_ADPOSI_ADJTARG,newvalue)
    elseif(name == "tabpage1_Tb2_edit24") then --机器最大模厚
		if(VarAdrToInt(ADJUST_MACH_ADPOSI_ADJCUR) > newvalue) then
			CtlAdrInt(ADJUST_MACH_ADPOSI_ADJCUR,newvalue)
        end
        
        if(VarAdrToInt(SYSTEM_MACH_ADPOSI_ADJTARG) > newvalue) then
			CtlAdrInt(SYSTEM_MACH_ADPOSI_ADJTARG,newvalue)
        end
    elseif(name == "tabpage1_Tb2_edit23") then --机器最小模厚
		if(VarAdrToInt(ADJUST_MACH_ADPOSI_ADJCUR) < newvalue) then
			CtlAdrInt(ADJUST_MACH_ADPOSI_ADJCUR,newvalue)
        end
        
        if(VarAdrToInt(SYSTEM_MACH_ADPOSI_ADJTARG) < newvalue) then
			CtlAdrInt(SYSTEM_MACH_ADPOSI_ADJTARG,newvalue)
        end
        
        if(VarAdrToInt(ADJUST_MACH_ADPOSI_ADJMAX) < newvalue) then
			CtlAdrInt(ADJUST_MACH_ADPOSI_ADJMAX,newvalue)
        end
	elseif(name == "tabpage1_Tb2_btstart") then  --20211224 dyl 齿距自学习
		if(VarAdrToInt(OPMODE_INDEX)== 4 or VarAdrToInt(OPMODE_INDEX) == 15) then
			SendPanelKey(_KEY_PANEL_PITCHSELFLEARN,1)
			SendPanelKey(_KEY_PANEL_NULL,1)
		else
			INFO_ShowMsg(VM_MSG_CHANGETOADJMOLD)
			return -1
		end
    elseif(name == "tabpage1_Tb3_bt61") then  --松模初始化
		CMD_ReleaseMdInit()
	end
    return 0
end
