local OPMODE_INDEX = 0x33160013 --Operation mode index
local MODE_MANUAL = 0x00 --手动模式
local VW_MSG_MANUAL = 50 --请在手动模式操作此功能!
local VW_EJECT_SENSER_USE = 90 --电眼检出需使用
local VW_EJECT_BLOWMODE_NEEDCHANGE = 91 --请更改吹气方式到非电眼后模式
local EJECTFUNC_SENSOR = 0x33120052 --电眼检出选择
local FL_BLAST1 = 0x23130000 --吹气1方式
local _KEY_PANEL_TIMEAUTO = 0x4037
local MODE_SENSORAUTO = 0x04
local MODE_TIMEAUTO = 0x08

local EJECT1_MOLD_FL_EJEF_SMOOTH = 0x21130021 --托进平稳
local SYSTEM_PRS_MAX = 0x00120021 --系统压力上限
local SYSTEM_MACH_CUSTOMID = 0x00120018 --客户机器代码

local CLAMP_ACTPARA_OPN5 = 0x2014001C  --开模五段终止位置
local ADDR_MAX_ADEJECT = 0x07160014 --托模电子尺最大值
local ADPOSI_EJTADVSTARTFIRSTEND = 0x21140008 --托进一段终止位置
local ADPOSI_EJTADVFASTEND = 0x21140009 --托进二段终止位置
local ADPOSI_EJTFFIRSTEND1 = 0x2114000e --托模进一段首次位置
local ADPOSI_EJTFFIRSTEND2 = 0x2114000f --托模进二段首次位置

local EJECT1_FL_EJTCTRL = 0x21130002 --托模方式
local EJECT1_CN_EJT = 0x21130003 --托模次数
local AUTOCTRL_FL_EJTCTRL = 0x3213007F --托模方式
local EJECT1_ADPOSI_MOVINGBLASTSTART = 0x23130001 --是否使用28组吹气功能
--20211115 dyl 机械手
local FL_ROBOTCTRL = 0x31130000   --机械手功能选择
local FL_CORESEC1  = 0x31130002   --中子信号1连接
local FL_CORESEC2  = 0x31130003   --中子信号2连接

--20211201 dyl 吹气方式
local PANEL_FL_BLASTMODE1 = 0x331200be
local PANEL_FL_BLASTMODE14 = 0x331200cb

local FL_AIR_EXTEND28 = 0x0013003a              --吹气28组

--20241219 chj 吹气28组开放后，吹气1,2不显示
local CUT_FL_FUNC = 0x21130019

function GetPrsMS(MaxRmp,SetRmp)
    local dwData = 0
    if((VarAdrToInt(MaxRmp)*VarAdrToInt(SetRmp)) ~= 0) then
		dwData = VarAdrToInt(SYSTEM_PRS_MAX)*1000*100/(VarAdrToInt(MaxRmp)*VarAdrToInt(SetRmp))
    end
    return dwData
end
function GetSpdMS(MaxRmp,SetRmp)
    local dwData = 0
    if((VarAdrToInt(MaxRmp)*VarAdrToInt(SetRmp)) ~= 0) then
		dwData = 100*1000*100/(VarAdrToInt(MaxRmp)*VarAdrToInt(SetRmp))
    end
	return dwData
end

function SetVibrateMode()
	local mode = GetAttrValue("cb82","value")
	if(mode == 0) then
		WGTSetAttrByName("ed92","text",0x21130005)
	else
		WGTSetAttrByName("ed92","text",0x21130006)
	end
end

--20211115 dyl 机械手
function SetRobotMode()
	if(VarAdrToInt(FL_ROBOTCTRL) == 3) then
		if(VarAdrToInt(FL_CORESEC1) == VarAdrToInt(FL_CORESEC2)) then
			CtlAdrInt(FL_CORESEC1, 1)
			CtlAdrInt(FL_CORESEC2, 2)
		end
	end
end

function PageCreate()

end

function PageShow()   
	SetPageLockMode(1)
	
--	--20200304通用切料头功能
--	if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x8B00) then
--		TabPageSetSubVisble("tabpage1",2,true)
--	else
--		TabPageSetSubVisble("tabpage1",2,false)
--	end

	--20241219 chj 吹气28组开放后，吹气1,2不显示
	if(VarAdrToInt(FL_AIR_EXTEND28) > 0) then
		TabPageSetSubVisble("tabpage1",0,false)
        TabPageSetSubVisble("tabpage1",1,false)
        TabPageSelPage("tabpage1",2)
	else
		TabPageSetSubVisble("tabpage1",0,true)
        TabPageSetSubVisble("tabpage1",1,true)
        TabPageSelPage("tabpage1",0)
	end
    
	--20211214 dyl 切料头功能不使用页面不显示
	if(VarAdrToInt(CUT_FL_FUNC) > 0) then
		TabPageSetSubVisble("tabpage1",3,true)
	else
		TabPageSetSubVisble("tabpage1",3,false)
	end
	
    if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",5,true)
        if(VarAdrToInt(EJECT1_MOLD_FL_EJEF_SMOOTH) > 0) then
			TabPageSetSubVisble("tabpage1",6,false)
            TabPageSetSubVisble("tabpage1",7,true)
        else
			TabPageSetSubVisble("tabpage1",6,true)
            TabPageSetSubVisble("tabpage1",7,false)
        end
    else 
		TabPageSetSubVisble("tabpage1",5,false)
        TabPageSetSubVisble("tabpage1",6,false)
        TabPageSetSubVisble("tabpage1",7,false)
    end
    
    
    FkeyTab("tabpage1", 0x201)
    
    --20210318 --20211111 dyl
--    local max_value = VarAdrToInt(CLAMP_ACTPARA_OPN5)
--    if(max_value > VarAdrToInt(ADDR_MAX_ADEJECT) and VarAdrToInt(ADDR_MAX_ADEJECT) > 0) then
--		max_value = VarAdrToInt(ADDR_MAX_ADEJECT)
--	end
	local max_value = VarAdrToInt(ADDR_MAX_ADEJECT)
    
    if(VarAdrToInt(ADPOSI_EJTADVFASTEND) > max_value) then
		CtlAdrInt(ADPOSI_EJTADVFASTEND, max_value)
        
        if(VarAdrToInt(ADPOSI_EJTADVSTARTFIRSTEND) > max_value) then
			CtlAdrInt(ADPOSI_EJTADVSTARTFIRSTEND, max_value)
		end
    end
    if(VarAdrToInt(ADPOSI_EJTFFIRSTEND2) > max_value) then
		CtlAdrInt(ADPOSI_EJTFFIRSTEND2, max_value)
        
        if(VarAdrToInt(ADPOSI_EJTFFIRSTEND1) > max_value) then
			CtlAdrInt(ADPOSI_EJTFFIRSTEND1, max_value)
		end
    end
    
    
	SetVibrateMode() --震动托模方式
	
	SetRobotMode() -- 20211115 dyl 机械手
 
	local ejt_mod = VarAdrToInt(EJECT1_FL_EJTCTRL)
	if(ejt_mod ~= 1) then
		if(ejt_mod == 2 and VarAdrToInt(EJECT1_CN_EJT) == 0) then
			VarAdrSetInt(AUTOCTRL_FL_EJTCTRL, 0)
        else 
			VarAdrSetInt(AUTOCTRL_FL_EJTCTRL, ejt_mod+1)
        end
    else
		if(VarAdrToInt(EJECT1_CN_EJT) > 0) then
			VarAdrSetInt(AUTOCTRL_FL_EJTCTRL, 2)
        else
			VarAdrSetInt(AUTOCTRL_FL_EJTCTRL, 0)
        end
    end
    
    --20211201 dyl 吹气方式
    local blow_mode
    for i=0,13,1 do
		blow_mode = VarAdrToInt(FL_BLAST1+0x100000*i)
		if(blow_mode == 256) then
			VarAdrSetInt(PANEL_FL_BLASTMODE1+i, 0)
		else
			VarAdrSetInt(PANEL_FL_BLASTMODE1+i, blow_mode+1)
		end
	end
    
end

local EJEF_MAX_PRES = 0x2113001E --托模进最大斜率(压力)
local EJTF_PRES_STARTON = 0x2113002A --托进开始压力斜率
local EJTF_PRES_HALFWAY = 0x2113002B --托进中间压力斜率
local EJTF_PRES_STOPOFF = 0x2113002C --托进停止压力斜率
local EJEF_MAX_SPD = 0x2113001D --托模进最大斜率(速度)
local EJTF_SPD_STARTON = 0x21130027 --托进开始速度斜率
local EJTF_SPD_HALFWAY = 0x21130028 --托进中间速度斜率
local EJTF_SPD_STOPOFF = 0x21130029 --托进停止速度斜率
local EJEB_MAX_PRES = 0x21130020 --托模退最大斜率(压力)
local EJTB_PRES_STARTON = 0x21130030 --托退开始压力斜率
local EJTB_PRES_HALFWAY = 0x21130031 --托退中间压力斜率
local EJTB_PRES_STOPOFF = 0x21130032 --托退停止压力斜率
local EJEB_MAX_SPD = 0x2113001F --托模退最大斜率(速度)
local EJTB_SPD_STARTON = 0x2113002D --托退开始速度斜率
local EJTB_SPD_HALFWAY = 0x2113002E --托退中间速度斜率
local EJTB_SPD_STOPOFF = 0x2113002F --托退停止速度斜率
function PageUpdate()
	--adv
	EditSetNum("tabpage1_Tb8_dt12",GetPrsMS(EJEF_MAX_PRES,EJTF_PRES_STARTON),0)
	EditSetNum("tabpage1_Tb8_dt22",GetPrsMS(EJEF_MAX_PRES,EJTF_PRES_HALFWAY),0)
	EditSetNum("tabpage1_Tb8_dt32",GetPrsMS(EJEF_MAX_PRES,EJTF_PRES_STOPOFF),0)
    EditSetNum("tabpage1_Tb8_dt14",GetSpdMS(EJEF_MAX_SPD,EJTF_SPD_STARTON),0)
	EditSetNum("tabpage1_Tb8_dt24",GetSpdMS(EJEF_MAX_SPD,EJTF_SPD_HALFWAY),0)
	EditSetNum("tabpage1_Tb8_dt34",GetSpdMS(EJEF_MAX_SPD,EJTF_SPD_STOPOFF),0)
    --ret
    EditSetNum("tabpage1_Tb8_dt16",GetPrsMS(EJEB_MAX_PRES,EJTB_PRES_STARTON),0)
	EditSetNum("tabpage1_Tb8_dt26",GetPrsMS(EJEB_MAX_PRES,EJTB_PRES_HALFWAY),0)
	EditSetNum("tabpage1_Tb8_dt36",GetPrsMS(EJEB_MAX_PRES,EJTB_PRES_STOPOFF),0)
    EditSetNum("tabpage1_Tb8_dt18",GetSpdMS(EJEB_MAX_SPD,EJTB_SPD_STARTON),0)
	EditSetNum("tabpage1_Tb8_dt28",GetSpdMS(EJEB_MAX_SPD,EJTB_SPD_HALFWAY),0)
	EditSetNum("tabpage1_Tb8_dt38",GetSpdMS(EJEB_MAX_SPD,EJTB_SPD_STOPOFF),0)

end

local EJECT1_FL_EJTFIRSTSET = 0x2113000A --托膜首次独立行程
function PageChange(name,adr,oldvalue,newvalue)
	print("PageChange")

	if(name == "cb62") then
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			if(newvalue == 0) then
				CtlAdrInt(EJECT1_FL_EJTCTRL, 1)
                CtlAdrInt(EJECT1_CN_EJT, 0)
            else
				CtlAdrInt(EJECT1_FL_EJTCTRL, newvalue-1)
            end
            
			local ejtmod = VarAdrToInt(EJECT1_FL_EJTCTRL)
            if(ejtmod == 0) then
				CtlAdrInt(EJECT1_FL_EJTFIRSTSET, 0)
			elseif(ejtmod == 2) then
				CtlAdrInt(EJECT1_FL_EJTFIRSTSET, 0)
			end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "cb82") then
		SetVibrateMode()
--	elseif(band(adr,0xFF0FFFFF) == 0x23030000) then --吹气方式
--		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
--			if((newvalue == 7) and (VarAdrToInt(EJECTFUNC_SENSOR) == 0)) then
--				CtlAdrInt(adr,oldvalue)
--				INFO_ShowMsg(VW_EJECT_SENSER_USE)
--				return -1
--            end
            
--			VarSendByAdr(adr + 1)
			
	--20211201 dyl 吹气方式
	elseif(adr >= PANEL_FL_BLASTMODE1 and adr <= PANEL_FL_BLASTMODE14) then --吹气方式
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then	
			local addoffset = adr-PANEL_FL_BLASTMODE1
			if(newvalue == 0) then 
				CtlAdrInt(FL_BLAST1+0x100000*addoffset, 256)
            elseif((newvalue == 8) and (VarAdrToInt(EJECTFUNC_SENSOR) == 0)) then
				CtlAdrInt(adr,oldvalue)
				INFO_ShowMsg(VW_EJECT_SENSER_USE)
				return -1
			else
				CtlAdrInt(FL_BLAST1+0x100000*addoffset, newvalue-1)
			end
            
            VarSendByAdr(EJECT1_ADPOSI_MOVINGBLASTSTART+0x100000*addoffset) --20241210 chj 28组吹气:下拉框改变，对应的位置也要再发送下
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "tabpage1_Tb4_stbt21") then --电眼检出选择
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			if(newvalue == 0) then
				local use_aftsensor = false
				for i=0,13,1 do
					if(VarAdrToInt(FL_BLAST1+0x100000*i) == 7) then
						use_aftsensor = true
					end
				end

				if(use_aftsensor) then
					CtlAdrInt(adr,oldvalue)
					INFO_ShowMsg(VW_EJECT_BLOWMODE_NEEDCHANGE)
					return -1
				end
			end
    
			if(newvalue==1) then
				if(VarAdrToInt(OPMODE_INDEX) == 3) then
					SendPanelKey(_KEY_PANEL_TIMEAUTO,1)
					SendPanelKey(_KEY_PANEL_TIMEAUTO,0)
				end
			else
				if(VarAdrToInt(OPMODE_INDEX) == 2) then
					SendPanelKey(_KEY_PANEL_TIMEAUTO,1)
					SendPanelKey(_KEY_PANEL_TIMEAUTO,0)
				end
			end
        else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	--20211115 dyl 机械手
	elseif(name == "tabpage1_Tb4_cb12") then --机械手
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			SetRobotMode()
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "tabpage1_Tb4_cb71") then --20211214 dyl 切料头功能不使用页面不显示
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			if(VarAdrToInt(CUT_FL_FUNC) > 0) then
				TabPageSetSubVisble("tabpage1",3,true)
			else
				TabPageSetSubVisble("tabpage1",3,false)
			end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	end
	
	return 0

end

function PageHide()
	print("PageHide")
	SetPageLockMode(0)
end

