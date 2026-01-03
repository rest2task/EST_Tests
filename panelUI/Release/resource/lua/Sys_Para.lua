local CLSHIPRS_SPD_MAX      = 0x3213000d  --关模高压高速上限
local CLSHIPRS2_SPD         = 0x20140022  --关模高压2速度
local CLSHIPRS_SPD          = 0x20140009  --关模高压速度
local SYS_MACH_CUSTOMID     = 0x00120018  --客户及机器代码


local CLAMP_MOLD_FL_CLSE_SMOOTH = 0x2013004a


local TMP_SYSTEM_PASSWORD  = 0x3315003a --20200302密码变量 临时变量58 
local TMP_SYSTEMPAR1_SHOW = 0x3315003b --系统参数1根据输入的密码是否正确，来显示

local SYS_FL_MACH_CODE1 = 0x00120001 --20200303 code1
local SYS_FL_MACH_CODE53 = 0x0012000D --code53
local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50

local CLAMP_ACT_DAPRES_CLSHIGH = 0x20140004 --关模高压压力
local CLAMP_ACT_DAPRES_CLSHIPRES2 = 0x20140021 --关模高压2段压力

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

local CLAMP_MOLD_FL_CLSOPEN_MODE = 0x20130060 	--开关模模式
local CLAMP_MOLD_SMOOTH_FUNCTION = 0x331200da 	--开关模平稳功能
local FL_CLS_LOCATE = 0x2013006e  				--关模平稳功能
local FL_OPN_LOCATE = 0x2013006a  				--开模平稳功能
local FL_LOCATE_DA = 0x20130072   				--开关模平稳使用比例阀

local FL_AIR_EXTEND28 = 0x0013003a              --吹气28组

local VW_MSG_OPCUA = 165

function SyncClsAndOpnPrs()
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
end

function SyncClsAndOpnSpdPos()
	if(VarAdrToInt(CLAMP_ACT_DAFLOW_CLS2) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_CLS2, VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1))
    end
    
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_CLS3) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_CLS3, VarAdrToInt(CLAMP_ACT_DAFLOW_CLS1))
    end
    
    if(VarAdrToInt(CLAMP_ACT_ADPOSI_CLS1END) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END)) then
		CtlAdrInt(CLAMP_ACT_ADPOSI_CLS1END, VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END))
    end
    
    if(VarAdrToInt(CLAMP_ACT_ADPOSI_CLS2END) ~= VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END)) then
		CtlAdrInt(CLAMP_ACT_ADPOSI_CLS2END, VarAdrToInt(CLAMP_ACT_ADPOSI_CLS3END))
    end
    
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_OPEN2) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_OPEN2, VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND))
    end
    
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_OPEN3) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_OPEN3, VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND))
    end
    
    if(VarAdrToInt(CLAMP_ACT_DAFLOW_OPEN4) ~= VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND)) then
		CtlAdrInt(CLAMP_ACT_DAFLOW_OPEN4, VarAdrToInt(CLAMP_ACT_DAFLOW_OPENEND))
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

--开关模平稳模式 20241017 chj 增加开关模平稳
function OpenSmoothShow()
	if(VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) == 0) then
		WGTSetEnable("dpClampSteady",1)
        WGTSetEnable("labelClpStd",1)
	else
		WGTSetEnable("dpClampSteady",0)
        WGTSetEnable("labelClpStd",0)
    end
end

function SetSmoothFunc()
	if(VarAdrToInt(CLAMP_MOLD_SMOOTH_FUNCTION) == 0) then
		if(VarAdrToInt(FL_LOCATE_DA) ~=0)then
			CtlAdrInt(FL_LOCATE_DA, 0)
		end
        
		if(VarAdrToInt(FL_CLS_LOCATE) ~=0)then
			CtlAdrInt(FL_CLS_LOCATE, 0)
		end
        
		if(VarAdrToInt(FL_OPN_LOCATE) ~=0)then
			CtlAdrInt(FL_OPN_LOCATE, 0)
		end
	end
end

function PageCreate()

end

function PageShow()
	SetPageLockMode(1)

	--20220223 dyl 高压速度上限变化跟随变化
	if(VarAdrToUI16(CLSHIPRS2_SPD)>VarAdrToUI16(CLSHIPRS_SPD_MAX)) then
		CtlAdrInt(CLSHIPRS2_SPD,VarAdrToUI16(CLSHIPRS_SPD_MAX))
	end
	if(VarAdrToUI16(CLSHIPRS_SPD)>VarAdrToUI16(CLSHIPRS_SPD_MAX)) then
		CtlAdrInt(CLSHIPRS_SPD,VarAdrToUI16(CLSHIPRS_SPD_MAX))
	end

	OpenSmoothShow()--开关模平稳模式 20241017 chj 增加开关模平稳
    SetSmoothFunc()
    
    --20241017 chj 威力士增加压力上限限制
    if(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) ~= 0x7A00) then
		NaviSetIndex(0)
    else
		NaviSetIndex(1)
    end
    
end

function PageUpdate()

    
end

function PageChange(name,adr,oldvalue,newvalue)
	
    if(name == "dropdown2") then
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			if(newvalue == 1) then
				CtlAdrInt(CLAMP_MOLD_FL_CLSE_SMOOTH, 1)
            elseif(newvalue == 2) then
				--PID模式关模高压二段压力与一段相同
				if(VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIPRES2) ~= VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH)) then
					CtlAdrInt(CLAMP_ACT_DAPRES_CLSHIPRES2, VarAdrToInt(CLAMP_ACT_DAPRES_CLSHIGH))
                end
                
				CtlAdrInt(CLAMP_MOLD_FL_CLSE_SMOOTH, 0)
            elseif((newvalue == 3) or (newvalue == 4)) then
				SyncClsAndOpnPrs()
                SyncClsAndOpnSpdPos()
                CtlAdrInt(CLAMP_MOLD_FL_CLSE_SMOOTH, 1)
			else
				CtlAdrInt(CLAMP_MOLD_FL_CLSE_SMOOTH, 0)
			end
            
		OpenSmoothShow()--开关模平稳模式 20241017 chj 增加开关模平稳
    
        else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	end
    --20200303
	if(name == "dropdown6") then --射出监控异常停机方式
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_MACH_CODE1,bor(VarAdrToInt(SYS_FL_MACH_CODE1),0x0001))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE1,band(VarAdrToInt(SYS_FL_MACH_CODE1),bnot(0x0001)))
			end
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	end
    
    if(name == "dpClampSteady") then --开关模平稳功能
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			SetSmoothFunc()
    
        else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	end    
    
    if(adr == FL_AIR_EXTEND28) then
		if(VarAdrToInt(OPMODE_INDEX) ~= MODE_MANUAL) then
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    end
    
	return 0

end

function PageHide()
	SetPageLockMode(0)
	
	--20200302 退出画面密码框值设0，方便通用的润滑模式颜色变化也可以用开关模上限编辑框的颜色变量
	VarAdrSetInt(TMP_SYSTEM_PASSWORD,0)
end
