local SYS_FL_MACH_CODE1  =  0x00120001 -- code1
local CLAMP_MOLD_FL_CLSE_SMOOTH  = 0x2013004A  --关模平稳 0  不使用   1   关模平稳  默认0
local CLAMP_MOLD_FL_CLSOPEN_MODE = 0x20130060 --开关模模式
local CLAMP_ACT_ADPOSI_OPENENDEND_MOLD = 0x20140047 --开模五段模板位置
local TMP_CLSOPEN_MODE = 0x33150000 --功能页面开关模模式临时变量
local CLAMP_ACT_DAPRES_CLSHIGH = 0x20140004 --关模高压压力
local CLAMP_ACT_DAPRES_CLSHIPRES2 = 0x20140021 --关模高压2段压力
local SYS_MACH_CUSTOMID = 0x00120018 --客户及机器代码
local CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE = 0x2013002E --关模联动位置 
local CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL = 0x20130008 --开模联动位置 
local CLAMP_MACH_ADPOS_OPNSTART = 0x2012003E --途中开模启动距离
local CLAMP_ACT_ADPOS_ULT_CLPOPEN =0x2014004A --开模差动结束位置
local FL_OPENSLOW_USE_ULT = 0x20130080 --开模慢速阀使用差动阀 --20250306 jhh 非手动情况下提示

--20211221 dyl
local COREA_ADPOSI_CORE_FWD = 0x22130006 --中子进动作点
local COREA_ACT_ADPOSI_COREINACT = 0x22140008 --中子进动作位置
local COREA_MACH_ADPOS_EFFCOREINPLUS = 0x22120014 --中子A进位置有效区（+）
local COREA_ADPOSI_CORE_BWD = 0x22130007 --中子退动作点
local COREA_ACT_ADPOSI_COREOUTACT = 0x22140009 --中子退动作位置
local COREA_MACH_ADPOS_EFFCOREOUTPLUS = 0x22120016 --中子A退位置有效区（+）
local SYSTEM_MOLD_FL_CLOSJTCORE = 0x00130012 --关模连动
local SYSTEM_MOLD_FL_OPNEJTLINK = 0x00130003 --开模连动

local SYSTEM_PRS_MAX = 0x00120021 --系统压力上限

local SYSTEM_MACH_CUSTOMID = 0x00120018 --客户机器代码

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

local CLAMP_ACT_DAFLOW_CLS1 = 0x20140005  		--关模一段流量
local CLAMP_ACT_DAFLOW_CLS2 = 0x20140006  		--关模二段流量
local CLAMP_ACT_DAFLOW_CLS3 = 0x20140007  		--关模三段流量
local SYSTEM_MACH_DAPRES_CLPLIMIT = 0x32130002 	--开关模压力上限
local CLAMP_ACT_DAPRES_CLS1 = 0x20140000  		--关模一段压力
local CLAMP_ACT_DAPRES_CLS2 = 0x20140001  		--关模二段压力
local CLAMP_ACT_DAPRES_CLS3 = 0x20140002  		--关模三段压力
local CLAMP_ACT_ADPOSI_CLS1END = 0x2014000A  	--关模一段终止位置
local CLAMP_ACT_ADPOSI_CLS2END = 0x2014000B  	--关模二段终止位置
local CLAMP_ACT_ADPOSI_CLS3END = 0x2014000C  	--关模三段终止位置
function SyncCls()
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_CLS2) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_CLS2, VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1))
    end
    
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_CLS3) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_CLS3, VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1))
    end
    
    if(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) ~= 0x7800) then --20200930
		if(VarAdrToInt(CLAMP_ACT_DAPRES_CLS1) ~= VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_CLS1, VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT))
		end
        
        if(VarAdrToInt(CLAMP_ACT_DAPRES_CLS2) ~= VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_CLS2, VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT))
		end
    
		if(VarAdrToInt(CLAMP_ACT_DAPRES_CLS3) ~= VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_CLS3, VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT))
		end
    else
		if(VarAdrToInt(CLAMP_ACT_DAPRES_CLS2) ~= VarAdrToInt(CLAMP_ACT_DAPRES_CLS1)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_CLS2, VarAdrToInt(CLAMP_ACT_DAPRES_CLS1))
		end
    
		if(VarAdrToInt(CLAMP_ACT_DAPRES_CLS3) ~= VarAdrToInt(CLAMP_ACT_DAPRES_CLS1)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_CLS3, VarAdrToInt(CLAMP_ACT_DAPRES_CLS1))
		end
    end
    
    
    
    if(VarAdrToInt(CLAMP_ACT_ADPOSI_CLS1END) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END)) then
		CtlAdrInt(CLAMP_ACT_ADPOSI_CLS1END, VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END))
    end
    
    if(VarAdrToInt(CLAMP_ACT_ADPOSI_CLS2END) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END)) then
		CtlAdrInt(CLAMP_ACT_ADPOSI_CLS2END, VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END))
    end
end

local CLAMP_ACT_DAFLOW_OPEN2 = 0x20140014  		--开模二段流量
local CLAMP_ACT_DAFLOW_OPEN3 = 0x20140015  		--开模三段流量
local CLAMP_ACT_DAFLOW_OPEN4 = 0x20140016  		--开模四段流量
local CLAMP_ACT_DAFLOW_OPENEND = 0x20140017  	--开模五段流量
local CLAMP_ACT_DAPRES_OPEN2 = 0x2014000F  		--开模二段压力
local CLAMP_ACT_DAPRES_OPEN3 = 0x20140010  		--开模三段压力
local CLAMP_ACT_DAPRES_OPEN4 = 0x20140011  		--开模四段压力
local CLAMP_ACT_DAPRES_OPENEND = 0x20140012  	--开模五段压力
local CLAMP_ACT_ADPOSI_OPENSLOW = 0x20140018  	--开模一段终止位置
local CLAMP_ACT_ADPOSI_OPEN2 = 0x20140019  		--开模二段终止位置
local CLAMP_ACT_ADPOSI_OPEN3 = 0x2014001A  		--开模三段终止位置
local CLAMP_ACT_ADPOSI_OPEN4 = 0x2014001B  		--开模四段终止位置
function SyncOpn()
	if(VarAdrToInt(CLAMP_ACT_DAFLOW_OPEN2) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_OPEN2, VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND))
    end
    
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_OPEN3) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_OPEN3, VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND))
    end
    
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_OPEN4) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_OPEN4, VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND))
    end
    
    if(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) ~= 0x7800) then --20200930
		if(VarAdrToInt(CLAMP_ACT_DAPRES_OPEN2) ~= VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_OPEN2, VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT))
		end
    
		if(VarAdrToInt(CLAMP_ACT_DAPRES_OPEN3) ~= VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_OPEN3, VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT))
		end
    
		if(VarAdrToInt(CLAMP_ACT_DAPRES_OPEN4) ~= VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_OPEN4, VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT))
		end
    
		if(VarAdrToInt(CLAMP_ACT_DAPRES_OPENEND) ~= VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_OPENEND, VarAdrToInt(SYSTEM_MACH_DAPRES_CLPLIMIT))
		end
    else
		if(VarAdrToInt(CLAMP_ACT_DAPRES_OPEN2) ~= VarAdrToInt(CLAMP_ACT_DAPRES_OPENEND)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_OPEN2, VarAdrToInt(CLAMP_ACT_DAPRES_OPENEND))
		end
    
		if(VarAdrToInt(CLAMP_ACT_DAPRES_OPEN3) ~= VarAdrToInt(CLAMP_ACT_DAPRES_OPENEND)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_OPEN3, VarAdrToInt(CLAMP_ACT_DAPRES_OPENEND))
		end
    
		if(VarAdrToInt(CLAMP_ACT_DAPRES_OPEN4) ~= VarAdrToInt(CLAMP_ACT_DAPRES_OPENEND)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_OPEN4, VarAdrToInt(CLAMP_ACT_DAPRES_OPENEND))
		end
    end
    
    
	if(PUSH_Type()) then
		if(VarAdrToInt(CLAMP_ACT_ADPOSI_OPEN2) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)/10) then
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPEN2, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)/10)
        end
    else
		if(VarAdrToInt(CLAMP_ACT_ADPOSI_OPEN2) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)) then
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPEN2, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW))
        end
    end
    
    if(PUSH_Type()) then
		if(VarAdrToInt(CLAMP_ACT_ADPOSI_OPEN3) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)/10) then
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPEN3, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)/10)
        end
    else
		if(VarAdrToInt(CLAMP_ACT_ADPOSI_OPEN3) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)) then
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPEN3, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW))
        end
    end
    
    if(PUSH_Type()) then
		if(VarAdrToInt(CLAMP_ACT_ADPOSI_OPEN4) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)/10) then
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPEN4, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)/10)
        end
    else
		if(VarAdrToInt(CLAMP_ACT_ADPOSI_OPEN4) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW)) then
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPEN4, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENSLOW))
        end
    end
end

function PageCreate()
	print("clamp PageCreate")
	SetPushTable("Clamp_Smart","edit69")
	SetPushTable("Clamp_Smart","edit70")
	SetPushTable("Clamp_Smart","edit71") --20211105 dyl HighPrs#2
	SetPushTable("Clamp_Smart","edit73")
	SetPushTable("Clamp_Smart","edit72")

	--SetPushTable("Clamp_Smart","edit192") --20200316 --20220228 dyl
end

function PageShow()
	print("Clamp_Smart pageshow")
	SetPageLockMode(1)
    
    if(VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) > 1) then --20200819
		VarAdrSetInt(TMP_CLSOPEN_MODE, 1)
    else
		VarAdrSetInt(TMP_CLSOPEN_MODE, 0)
    end
    
    
	--20241231 chj 新增威力士开关模切换方式：普通+智能；标准为拉链式开模+智能
    if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x7A00) then 
		DropSetItems("tabpage1_Tb1_cbmode",GetTextTran(128))
    else
		DropSetItems("tabpage1_Tb1_cbmode",GetTextTran(127))
    end
    
    if(VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) > 1 and VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) ~= 3) then --20200819
		WGTSetVisible("tabpage1_Tb1_labelmode", 0)
        WGTSetVisible("tabpage1_Tb1_cbmode", 0)
        WGTSetVisible("tabpage1_Tb1_label195", 0)
        WGTSetVisible("tabpage1_Tb1_label196", 0)
        WGTSetVisible("tabpage1_Tb1_label197", 0)
        --SetAttrValue("tabpage1_Tb1_lblinfo", "y", 89)
    else
		WGTSetVisible("tabpage1_Tb1_labelmode", 1)
        WGTSetVisible("tabpage1_Tb1_cbmode", 1)
        WGTSetVisible("tabpage1_Tb1_label195", 1)
        WGTSetVisible("tabpage1_Tb1_label196", 1)
        WGTSetVisible("tabpage1_Tb1_label197", 1)
        --SetAttrValue("tabpage1_Tb1_lblinfo", "y", 117)
    end
    
    if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",3,true)
        TabPageSetSubVisble("tabpage1",5,true)
        if(VarAdrToInt(CLAMP_MOLD_FL_CLSE_SMOOTH) > 0) then
			TabPageSetSubVisble("tabpage1",4,false)
            TabPageSetSubVisble("tabpage1",6,true)
            TabPageSetSubVisble("tabpage1",7,true)
            if(VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) == 4) then
				TabPageSetSubVisble("tabpage1",8,true)
            else
				TabPageSetSubVisble("tabpage1",8,false)
            end
        else
			TabPageSetSubVisble("tabpage1",4,true)
            TabPageSetSubVisble("tabpage1",6,false)
            TabPageSetSubVisble("tabpage1",7,false)
            TabPageSetSubVisble("tabpage1",8,false)
        end
    else 
		TabPageSetSubVisble("tabpage1",3,false)
        TabPageSetSubVisble("tabpage1",4,false)
        TabPageSetSubVisble("tabpage1",5,false)
        TabPageSetSubVisble("tabpage1",6,false)
        TabPageSetSubVisble("tabpage1",7,false)
        TabPageSetSubVisble("tabpage1",8,false)
    end
    
    if(band(VarAdrToInt(SYS_FL_MACH_CODE1),0x2000)~=0 and VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) ~= 4) then
		TabPageSetSubVisble("tabpage1",1,true)
    else
		TabPageSetSubVisble("tabpage1",1,false)
    end
    
    --20200930
    if((band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) == 0x7800) and (GetCurrentUserId() > 2)) then
		WGTSetEnable("edit37", 1)
        WGTSetEnable("edit51", 1)
    else
		WGTSetEnable("edit37", 0)
        WGTSetEnable("edit51", 0)
    end
    
    SyncCls()
    SyncOpn()
    
    --关模高压二段压力与一段相同
	if(VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIPRES2) ~= VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH)) then
		CtlAdrInt(CLAMP_ACT_DAPRES_CLSHIPRES2, VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH))
    end

	--金鹰厂商代码下才显示关模机械手滤波时间 20241205 chj
	if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400) then 
		WGTSetVisible("tabpage1_Tb4_lblclsrobtm", 1)
        WGTSetVisible("tabpage1_Tb4_edclsrobtm", 1)
        WGTSetVisible("tabpage1_Tb4_lblclsrobtm_unit", 1)
        
    else
		WGTSetVisible("tabpage1_Tb4_lblclsrobtm", 0)
        WGTSetVisible("tabpage1_Tb4_edclsrobtm", 0)
        WGTSetVisible("tabpage1_Tb4_lblclsrobtm_unit", 0)
    end
    
    TabPageSelPage("tabpage1",0)
    FkeyTab("tabpage1", 0x200)
end

local SLOPE_MAX_CLS_PRES = 0x2013004D --关模最大斜率(压力)
local SLOPE_TM_CLS_PRES_STARTON = 0x2013005C --关模开始斜率(压力)
local SLOPE_TM_CLS_PRES_HALFWAY = 0x2013005D --关模中间斜率(压力)
local SLOPE_TM_CLS_PRES_STOPOFF = 0x2013005E --关模停止斜率(压力)
local SLOPE_MAX_CLS_SPD = 0x2013004B --关模最大斜率(速度)
local SLOPE_TM_CLS_SPD_STARTON = 0x20130056 --关模开始斜率(速度)
local SLOPE_TM_CLS_SPD_HALFWAY = 0x20130057 --关模中间斜率(速度)
local SLOPE_TM_CLS_SPD_STOPOFF = 0x20130058 --关模停止斜率(速度)
local FLOWSTART_CLOSE_MID = 0x20130042 --关模手动启动速度斜率
local FLOWSTOP_CLOSE_MID = 0x20130044 --关模手动停止速度斜率
local SLOPE_MAX_OPN_PRES = 0x2013004E --开模最大斜率(压力)
local SLOPE_TM_OPN_PRES_STARTON = 0x20130059 --开模启动斜率(压力)
local SLOPE_TM_OPN_PRES_HALFWAY = 0x2013005A --开模中间斜率(压力)
local SLOPE_TM_OPN_PRES_STOPOFF = 0x2013005B --开模停止斜率(压力)
local SLOPE_MAX_OPN_SPD = 0x2013004C --开模最大斜率(速度)
local SLOPE_TM_OPN_SPD_STARTON = 0x20130053 --开模启动斜率(速度)
local SLOPE_TM_OPN_SPD_HALFWAY = 0x20130054 --开模中间斜率(速度)
local SLOPE_TM_OPN_SPD_STOPOFF = 0x20130055 --开模停止斜率(速度)
local FLOWSTART_OPEN_MID = 0x20130046 --开模手动启动速度斜率
local FLOWSTOP_OPEN_MID = 0x20130048 --开模手动停止速度斜率
function PageUpdate()
	--cls
	EditSetNum("tabpage1_Tb7_dt12",GetPrsMS(SLOPE_MAX_CLS_PRES,SLOPE_TM_CLS_PRES_STARTON),0)
	EditSetNum("tabpage1_Tb7_dt22",GetPrsMS(SLOPE_MAX_CLS_PRES,SLOPE_TM_CLS_PRES_HALFWAY),0)
	EditSetNum("tabpage1_Tb7_dt32",GetPrsMS(SLOPE_MAX_CLS_PRES,SLOPE_TM_CLS_PRES_STOPOFF),0)
    EditSetNum("tabpage1_Tb7_dt14",GetSpdMS(SLOPE_MAX_CLS_SPD,SLOPE_TM_CLS_SPD_STARTON),0)
	EditSetNum("tabpage1_Tb7_dt24",GetSpdMS(SLOPE_MAX_CLS_SPD,SLOPE_TM_CLS_SPD_HALFWAY),0)
	EditSetNum("tabpage1_Tb7_dt34",GetSpdMS(SLOPE_MAX_CLS_SPD,SLOPE_TM_CLS_SPD_STOPOFF),0)
    EditSetNum("tabpage1_Tb7_dt44",GetSpdMS(SLOPE_MAX_CLS_SPD,FLOWSTART_CLOSE_MID),0)
    EditSetNum("tabpage1_Tb7_dt54",GetSpdMS(SLOPE_MAX_CLS_SPD,FLOWSTOP_CLOSE_MID),0)
    --opn
    EditSetNum("tabpage1_Tb7_dt16",GetPrsMS(SLOPE_MAX_OPN_PRES,SLOPE_TM_OPN_PRES_STARTON),0)
	EditSetNum("tabpage1_Tb7_dt26",GetPrsMS(SLOPE_MAX_OPN_PRES,SLOPE_TM_OPN_PRES_HALFWAY),0)
	EditSetNum("tabpage1_Tb7_dt36",GetPrsMS(SLOPE_MAX_OPN_PRES,SLOPE_TM_OPN_PRES_STOPOFF),0)
    EditSetNum("tabpage1_Tb7_dt18",GetSpdMS(SLOPE_MAX_OPN_SPD,SLOPE_TM_OPN_SPD_STARTON),0)
	EditSetNum("tabpage1_Tb7_dt28",GetSpdMS(SLOPE_MAX_OPN_SPD,SLOPE_TM_OPN_SPD_HALFWAY),0)
	EditSetNum("tabpage1_Tb7_dt38",GetSpdMS(SLOPE_MAX_OPN_SPD,SLOPE_TM_OPN_SPD_STOPOFF),0)
    EditSetNum("tabpage1_Tb7_dt48",GetSpdMS(SLOPE_MAX_OPN_SPD,FLOWSTART_OPEN_MID),0)
    EditSetNum("tabpage1_Tb7_dt58",GetSpdMS(SLOPE_MAX_OPN_SPD,FLOWSTOP_OPEN_MID),0)
end

local CLAMP_MACHSET_EFFCPR = 0x20120028  --开模位置有效区
local CLAMP_ACTPARA_OPN5 = 0x2014001C  --开模五段终止位置
local CLAMP_ACT_ADPOS_OPENSPD3 = 0x20140029  --快速开模结束位置
local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50
local USE_PUSHPARA = 0x3312007F --是否使用五支点
function PageChange(name,adr,oldvalue,newvalue)
	print("Clamp_Smart PageChange")
    SyncCls()
    SyncOpn()
    
    if(name == "btxPUSH") then
		if(VarAdrToInt(USE_PUSHPARA) > 0) then
			PanelShowPageByName("Pushtable2")
        else
			PanelShowPageByName("Pushtable")
        end
    elseif(name == "btxCURV") then
		PanelShowPageByName("Clamp_Curve")
    elseif(adr == CLAMP_MACHSET_EFFCPR) then
		if(PUSH_Type()) then
			local ppart = band(rshift(CLAMP_MACHSET_EFFCPR,16),0xFFF0)
			local subid = band(rshift(CLAMP_MACHSET_EFFCPR,16),0xf)
			local offset = band(CLAMP_MACHSET_EFFCPR,0xFFFF)
			local value = VarAdrToInt(CLAMP_MACHSET_EFFCPR)
			local value1 = VarAdrToInt(CLAMP_ACTPARA_OPN5) - value
			local value2 = UnTransMeter(VarAdrToInt(CLAMP_ACTPARA_OPN5),false) - UnTransMeter(value1,false)
			local array = newArray(1)
			setArray(array,1,value2)
			ProtSendPartSubData(ppart,subid,offset,array)
			return -1 --返回-1是为了开模位置有效期通过上面已经发送，不需要再次发送
		end
	elseif(adr == CLAMP_ACTPARA_OPN5) then --20200304
		if(VarAdrToInt(CLAMP_ACTPARA_OPN5) ~= VarAdrToInt(CLAMP_ACT_ADPOS_OPENSPD3)) then --20200413 开模终位置与PID模式开模终位置保持同步
			CtlAdrInt(CLAMP_ACT_ADPOS_OPENSPD3,newvalue)
        end
        
        if(VarAdrToInt(CLAMP_ACTPARA_OPN5) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_OPENENDEND_MOLD)) then --20200731
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPENENDEND_MOLD,newvalue)
        end
        
		if(PUSH_Type()) then
			local ppart = band(rshift(CLAMP_MACHSET_EFFCPR,16),0xFFF0)
			local subid = band(rshift(CLAMP_MACHSET_EFFCPR,16),0xf)
			local offset = band(CLAMP_MACHSET_EFFCPR,0xFFFF)
			local value = VarAdrToInt(CLAMP_MACHSET_EFFCPR)
			local value1 = VarAdrToInt(CLAMP_ACTPARA_OPN5) - value
			local value2 = UnTransMeter(VarAdrToInt(CLAMP_ACTPARA_OPN5),false) - UnTransMeter(value1,false)
			local array = newArray(1)
			setArray(array,1,value2)
			ProtSendPartSubData(ppart,subid,offset,array)
		end
        
        --20210219
        if(VarAdrToInt(CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE) > newvalue) then
			CtlAdrInt(CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE,newvalue)
        end
        
        if(VarAdrToInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL) > newvalue) then
			CtlAdrInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL,newvalue)
        end
        
        if(VarAdrToInt(CLAMP_MACH_ADPOS_OPNSTART) > newvalue) then
			CtlAdrInt(CLAMP_MACH_ADPOS_OPNSTART,newvalue)
        end
        SyncCoreAdvRetPos(adr) --20211221 dyl
    elseif(name == "edit48") then --低压模保位置 --20210219
        if(PUSH_Type()) then
			if(VarAdrToInt(CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE) < newvalue/10) then
				CtlAdrInt(CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE,newvalue/10)
			end
        else
			if(VarAdrToInt(CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE) < newvalue) then
				CtlAdrInt(CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE,newvalue)
			end
        end
    elseif(name == "edit60") then --开模一段位置 --20210219
        if(PUSH_Type()) then
			if(VarAdrToInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL) < newvalue/10) then
				CtlAdrInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL,newvalue/10)
			end
        else
			if(VarAdrToInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL) < newvalue) then
				CtlAdrInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL,newvalue)
			end
        end
        
        if(VarAdrToInt(CLAMP_ACT_ADPOS_ULT_CLPOPEN) > newvalue) then --20210518
			CtlAdrInt(CLAMP_ACT_ADPOS_ULT_CLPOPEN,newvalue)
		end
    elseif(name == "tabpage1_Tb2_cb11") then --关模联动
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			--20211221 dyl 中子动作点
			if (newvalue==1) then
                --20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
                CorePointByLink()
            elseif(newvalue>1) then
                CtlAdrInt(COREA_ADPOSI_CORE_FWD+(newvalue-1)*0x100000,1)
                CtlAdrInt(COREA_ACT_ADPOSI_COREINACT+(newvalue-1)*0x100000,VarAdrToInt(CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE))
                --20210730 dyl 中子有效区位置
                if(PUSH_Type()) then
                    CoreEffeAreaByPushtab(COREA_ACT_ADPOSI_COREINACT+(newvalue-1)*0x100000, COREA_MACH_ADPOS_EFFCOREINPLUS+(newvalue-1)*0x100000)
                end
            end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "tabpage1_Tb2_cb12") then --开模联动
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			--20211221 dyl 中子动作点
            if(newvalue==2) then
                --20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
                CorePointByLink()
            elseif(newvalue>2) then
                CtlAdrInt(COREA_ADPOSI_CORE_BWD+(newvalue-2)*0x100000,1)
                CtlAdrInt(COREA_ACT_ADPOSI_COREOUTACT+(newvalue-2)*0x100000,VarAdrToInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL))
                --20210730 dyl 中子有效区位置
                if(PUSH_Type()) then
                    CoreEffeAreaByPushtab(COREA_ACT_ADPOSI_COREOUTACT+(newvalue-2)*0x100000, COREA_MACH_ADPOS_EFFCOREOUTPLUS+(newvalue-2)*0x100000)
                end
			end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "tabpage1_Tb2_ed21") then --关模联动位置 --20211221 dyl
		if(VarAdrToInt(SYSTEM_MOLD_FL_CLOSJTCORE)==1) then
            --20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
            CorePointByLink()
        elseif(VarAdrToInt(SYSTEM_MOLD_FL_CLOSJTCORE)>1) then
            CtlAdrInt(COREA_ACT_ADPOSI_COREINACT+(VarAdrToInt(SYSTEM_MOLD_FL_CLOSJTCORE)-1)*0x100000,newvalue)
            --20210730 dyl 中子有效区位置
            if(PUSH_Type()) then
                CoreEffeAreaByPushtab(COREA_ACT_ADPOSI_COREINACT+(VarAdrToInt(SYSTEM_MOLD_FL_CLOSJTCORE)-1)*0x100000, COREA_MACH_ADPOS_EFFCOREINPLUS+(VarAdrToInt(SYSTEM_MOLD_FL_CLOSJTCORE)-1)*0x100000)
            end
        end
   elseif(name == "tabpage1_Tb2_ed22") then --开模联动位置 --20211221 dyl
		if(VarAdrToInt(SYSTEM_MOLD_FL_OPNEJTLINK)==2) then
            --20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
            CorePointByLink()
        elseif(VarAdrToInt(SYSTEM_MOLD_FL_OPNEJTLINK)>2) then
            CtlAdrInt(COREA_ACT_ADPOSI_COREOUTACT+(VarAdrToInt(SYSTEM_MOLD_FL_OPNEJTLINK)-2)*0x100000,newvalue)
            --20210730 dyl 中子有效区位置
            if(PUSH_Type()) then
                CoreEffeAreaByPushtab(COREA_ACT_ADPOSI_COREOUTACT+(VarAdrToInt(SYSTEM_MOLD_FL_OPNEJTLINK)-2)*0x100000, COREA_MACH_ADPOS_EFFCOREOUTPLUS+(VarAdrToInt(SYSTEM_MOLD_FL_OPNEJTLINK)-2)*0x100000)
            end
        end
    elseif(adr == TMP_CLSOPEN_MODE) then --20200819
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then   
			if(newvalue == 0) then
				if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x7A00) then --20241231 chj 新增威力士开关模切换方式：普通+智能；标准为拉链式开模+智能
                    CtlAdrInt(CLAMP_MOLD_FL_CLSOPEN_MODE, 0)
					SetRecordByManual(CLAMP_MOLD_FL_CLSOPEN_MODE, 0, 0, 3)
                else
					CtlAdrInt(CLAMP_MOLD_FL_CLSOPEN_MODE, 1)
					SetRecordByManual(CLAMP_MOLD_FL_CLSOPEN_MODE, 0, 1, 3)
                end

                --20250103 chj 模式切换，强制进行数据同步
                if((VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) == 1) or (VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) == 3)) then
					CtlAdrInt(CLAMP_MOLD_FL_CLSE_SMOOTH, 1)
                else
                    CtlAdrInt(CLAMP_MOLD_FL_CLSE_SMOOTH, 0)
				end
                    
                PanelShowPageByName("Clamp_h")
            end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(adr == CLAMP_ACT_DAPRES_CLSHIGH) then    
        --关模高压二段压力与一段相同
		if(VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIPRES2) ~= VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH)) then
			CtlAdrInt(CLAMP_ACT_DAPRES_CLSHIPRES2, VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH))
		end
	elseif(adr == FL_OPENSLOW_USE_ULT) then --开模慢速阀使用差动阀 --20250306 jhh 非手动情况下提示
		if(VarAdrToInt(OPMODE_INDEX) ~= MODE_MANUAL) then
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    end
	return 0
end

function PageHide()
	print("Clamp_Smart PageHide")
	SetPageLockMode(0)
end

