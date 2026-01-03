local SYS_FL_MACH_CODE1  =  0x00120001 -- code1
local CLAMP_MOLD_FL_CLSE_SMOOTH  = 0x2013004A  --关模平稳 0  不使用   1   关模平稳  默认0

local SYSTEM_PRS_MAX = 0x00120021 --系统压力上限
local PASSWORD = 0x33150069 

--金鹰 20241029 chj 金鹰厂商增加对开模启动速度的密码限制	
local TMP_PSW_CAL = 0x33150070 					--润滑模式金鹰特殊公式计算出来的密码
local SYSTEM_MACH_CUSTOMID = 0x00120018 		--客户机器代码
local AUTOCTRL_DAFLOW_LIMIT = 0x3213002F		--开关模速度上限
local SYS_CLAMP_PROTECT_FUNC= 0x331200DE		--开关模压力上限密码保护功能是否使用
local PP_OBJ_DAFLOW_OPENSPD1_LIMIT = 0x331200E0	--PID快速开模启动速度最大值
local TMP_DAFLOW_OPENSPD1_LIMIT = TMPVAL(200)	--临时变量_PID快速开模启动速度最大值
--金鹰end

local CLAMP_MACHSET_EFFCPR = 0x20120028  --开模位置有效区
local CLAMP_ACT_ADPOS_OPENSPD3 = 0x20140029  --快速开模结束位置
local CLAMP_ACTPARA_OPN5 = 0x2014001C  --开模五段终止位置
local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50

local CLAMP_ACT_DAPRES_CLSHIGH = 0x20140004 --关模高压压力
local CLAMP_ACT_DAPRES_CLSHIPRES2 = 0x20140021 --关模高压2段压力
local CLAMP_ACT_ADPOSI_OPENENDEND_MOLD = 0x20140047 --开模五段模板位置

local CLAMP_MOLD_ADPOSI_EJTTRAVELCLOSE = 0x2013002E --关模联动位置 
local CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL = 0x20130008 --开模联动位置 
local CLAMP_MACH_ADPOS_OPNSTART = 0x2012003E --途中开模启动距离
local CLAMP_MACH_ADPOS_CLSMIDSTART = 0x2012005A --途中关模启动距离 
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

local SYSTEM_MACH_CUSTOMID = 0x00120018 --客户机器代码

function PageCreate()
	print("clamp PageCreate")

	SetPushTable("Clampx_h","edit69")
	SetPushTable("Clampx_h","edit70")

	SetPushTable("Clampx_h","edit76")

	SetPushTable("Clampx_h","edit74")
	--SetPushTable("Clampx_h","edit192") --20220228 dyl
end

function PageShow()
	print("clampx pageshow")
	SetPageLockMode(1)
    
    VarAdrSetInt(PASSWORD, 0)
    
    if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",3,true)
        TabPageSetSubVisble("tabpage1",4,true)
        TabPageSetSubVisble("tabpage1",5,true)
        TabPageSetSubVisble("tabpage1",6,true)
    else 
		TabPageSetSubVisble("tabpage1",3,false)
        TabPageSetSubVisble("tabpage1",4,false)
        TabPageSetSubVisble("tabpage1",5,false)
        TabPageSetSubVisble("tabpage1",6,false)
    end
    
    if(band(VarAdrToInt(SYS_FL_MACH_CODE1),0x2000)~=0) then
		TabPageSetSubVisble("tabpage1",1,true)
    else
		TabPageSetSubVisble("tabpage1",1,false)
	end
    --20200304
	if (VarAdrToInt(CLAMP_ACT_ADPOS_OPENSPD3) == VarAdrToInt(CLAMP_ACTPARA_OPN5)) then
        
    else
		CtlAdrInt(CLAMP_ACTPARA_OPN5,VarAdrToInt(CLAMP_ACT_ADPOS_OPENSPD3))
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
    end
    
	ReadOnly("edit194", true)
	ReadOnly("edit195", true)
	ReadOnly("edit196", true)
	ReadOnly("edit197", true)
	ReadOnly("edit198", true)
	ReadOnly("edit199", true)
    
    --关模高压二段压力与一段相同
	if(VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIPRES2) ~= VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH)) then
		CtlAdrInt(CLAMP_ACT_DAPRES_CLSHIPRES2, VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH))
    end
	
    --20241029 chj 金鹰厂商增加对开模启动速度的密码限制		
	if(VarAdrToInt(SYS_CLAMP_PROTECT_FUNC)==1 and (band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400)) then --密码保护功能使用
		ReadOnly("edit62", true)
		VarAdrSetInt(TMP_DAFLOW_OPENSPD1_LIMIT,VarAdrToInt(PP_OBJ_DAFLOW_OPENSPD1_LIMIT))
	else
		ReadOnly("edit62", false)
		VarAdrSetInt(TMP_DAFLOW_OPENSPD1_LIMIT,VarAdrToInt(AUTOCTRL_DAFLOW_LIMIT))
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

function PageUpdate()

end

function ReadOnly(wgt_name, status)
	if(status == true) then
		SetAttrValue(wgt_name, "readOnly", 1)
		EditSetBKColor(wgt_name, 0xdedede)
		EditSetTextColor(wgt_name, 0x000000)
	else
		SetAttrValue(wgt_name, "readOnly", 0)
		EditSetBKColor(wgt_name, 0xFFFFFF)
		EditSetTextColor(wgt_name, 0x000000)
	end
end

local USE_PUSHPARA = 0x3312007F --是否使用五支点
function PageChange(name,adr,oldvalue,newvalue)
	print("clampx PageChange")
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
	elseif(adr == CLAMP_ACT_ADPOS_OPENSPD3) then --20200304
		if (VarAdrToInt(adr) == VarAdrToInt(CLAMP_ACTPARA_OPN5)) then
        
        else
			CtlAdrInt(CLAMP_ACTPARA_OPN5,newvalue)
			CtlAdrInt(CLAMP_ACT_ADPOSI_OPENENDEND_MOLD,newvalue)--20200731

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
            
            if(VarAdrToInt(CLAMP_MACH_ADPOS_CLSMIDSTART) > newvalue) then
				CtlAdrInt(CLAMP_MACH_ADPOS_CLSMIDSTART,newvalue)
			end
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
    elseif(name == "edit62") then --快速开模启动位置 --20210219
        if(PUSH_Type()) then
			if(VarAdrToInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL) < newvalue/10) then
				CtlAdrInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL,newvalue/10)
			end
        else
			if(VarAdrToInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL) < newvalue) then
				CtlAdrInt(CLAMP_MOLD_ADPOSI_OPNEJTTRAVEL,newvalue)
			end
        end
    elseif(adr == PASSWORD) then
		if(VarAdrToInt(PASSWORD) == 5858) then
			ReadOnly("edit194", false)
			ReadOnly("edit195", false)
			ReadOnly("edit196", false)
			ReadOnly("edit197", false)
			ReadOnly("edit198", false)
			ReadOnly("edit199", false)
            SetFocus("edit44")
        else
			ReadOnly("edit194", true)
			ReadOnly("edit195", true)
			ReadOnly("edit196", true)
			ReadOnly("edit197", true)
			ReadOnly("edit198", true)
			ReadOnly("edit199", true)
        end
        if((band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400))then --20241029 chj 金鹰厂商增加对开模启动速度的密码限制	
			CalClampPrsMaxProtectPSW(TMP_PSW_CAL)
			if(VarAdrToInt(PASSWORD) == VarAdrToInt(TMP_PSW_CAL)) then
				ReadOnly("edit62", false)
				SetFocus("edit58") --跳转至edit58
			end
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
	print("clampx PageHide")
	SetPageLockMode(0)
end

