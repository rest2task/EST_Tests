local CLAMP_PRS_MAX         = 0x32130002  --开关模压力上限
local CHG_SPD_MAX           = 0x3213000b  --储料速度上限
local CLSLOWPRS_PRS_MAX     = 0x3213002e  --关模低压压力上限
local CLSHIPRS_PRS_MAX      = 0x3213000c  --关模高压压力上限
local CLSHIPRS_SPD_MAX      = 0x3213000d  --关模高压高速上限
local CLSHIPRS2_SPD         = 0x20140022  --关模高压2速度
local CLSHIPRS_SPD          = 0x20140009  --关模高压速度
local CHARGE_PRS_MAX        = 0x3213000a  --储料压力上限
local EJECT_PRS_MAX         = 0x32130033  --托模压力上限
local ADJUST_PRS_MAX        = 0x32130003  --调模压力上限
local CHGBKPRS_PRS_MAX      = 0x32130009  --储料背压上限
local OTHER_PRS_MAX         = 0x32130004  --小动作压力上限
local SYSTEM_PRS_MAX        = 0x00120021  --系统压力上限
local SYS_MACH_CUSTOMID     = 0x00120018  --客户及机器代码

local CHGBACK_ADJ_CMD = 0x0020
local DAADJUST_DACHANEL = 0x3312004f
local INJECT_PORT_CHRGBAcK = 0x10150015
local SYSTEM_DAPRESS_BACKLIMIT = 0x32130009
local CLAMP_MOLD_FL_CLSOPEN_MODE = 0x20130060 --开关模模式

local TMP_SYSTEM_PASSWORD  = 0x3315003a 	--20200302密码变量 临时变量58
local TMP_SYSTEMPAR1_SHOW = 0x3315003b 		--系统参数1根据输入的密码是否正确，来显示
local TMP_SPECIAL_CAL = 0x3315003c 			--金鹰特殊计算用的密码
local SYS_CLAMP_PROTECT_FUNC= 0x331200DE	--开关模压力上限密码保护功能是否使用

local SYSTEM_MACH_DAPRES_CLPLIMIT = 0x32130002 	--开关模压力上限
local CLAMP_ACT_DAPRES_CLS1 = 0x20140000  		--关模一段压力
local CLAMP_ACT_DAPRES_CLS2 = 0x20140001  		--关模二段压力
local CLAMP_ACT_DAPRES_CLS3 = 0x20140002  		--关模三段压力

local CLAMP_ACT_DAPRES_OPEN2 = 0x2014000F  		--开模二段压力
local CLAMP_ACT_DAPRES_OPEN3 = 0x20140010  		--开模三段压力
local CLAMP_ACT_DAPRES_OPEN4 = 0x20140011  		--开模四段压力
local CLAMP_ACT_DAPRES_OPENEND = 0x20140012  	--开模五段压力


--20241017 chj 威力士增加压力上限限制
local SYS_FL_HYBDRAUIC = 0x0012000f 			--液压控制
local WLSMACHINE_TYPE = 0x331200fc 				--机器类型
local TMP_CLSHIPRSMAXLIMIT  = 0x33150064 		--临时变量100 关模高压压力上限最大值
local pp_MACHSET_HIPRS_LIMIT_MAX  = 0x331200E1 	--关模高压压力上限限制

local pp_MACHSET_CHGBKPRS_LIMIT_MAX_REAL  = 0x331200E4 		--储料背压压力上限最大值
local pp_MACHSET_CHGBKPRS_LIMIT_MAX  = 0x331200E2 			--储料背压压力上限限制

local TMP_CHGSPDMAXLIMIT  = 0x33150066 				--临时变量102 储料速度最大值
local pp_MACHSET_CHGSPD_LIMIT_MAX  = 0x331200E3 	--储料速度上限限制

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

function PageCreate()

end

function PageShow()
	SetPageLockMode(1)

	local prsaddr={CLAMP_PRS_MAX,CLSLOWPRS_PRS_MAX,CHARGE_PRS_MAX,EJECT_PRS_MAX,ADJUST_PRS_MAX,OTHER_PRS_MAX}
	local pdata
	--prs max process
	local value = VarAdrToUI16(SYSTEM_PRS_MAX)
	for i=1,6 do
		if(VarAdrToUI16(prsaddr[i])>value) then
			VarAdrSetInt(prsaddr[i],value)
			VarAdrSaveInt(prsaddr[i])
		end
	end

	--20220223 dyl 高压速度上限变化跟随变化
	if(VarAdrToUI16(CLSHIPRS2_SPD)>VarAdrToUI16(CLSHIPRS_SPD_MAX)) then
		CtlAdrInt(CLSHIPRS2_SPD,VarAdrToUI16(CLSHIPRS_SPD_MAX))
	end
	if(VarAdrToUI16(CLSHIPRS_SPD)>VarAdrToUI16(CLSHIPRS_SPD_MAX)) then
		CtlAdrInt(CLSHIPRS_SPD,VarAdrToUI16(CLSHIPRS_SPD_MAX))
	end

	--20200302 进入画面密码框值设0
	VarAdrSetInt(TMP_SYSTEM_PASSWORD,0)
    --20241017 chj 宁塑/金鹰厂家要求，增加系统参数密码设定
    if(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) == 0x7500) then
		WGTSetVisible("labelpsw", 1)
        WGTSetVisible("editpsw", 1)
		VarAdrSetInt(TMP_SYSTEMPAR1_SHOW,0)
    elseif(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) == 0x6400)then
		if((VarAdrToInt(SYS_CLAMP_PROTECT_FUNC)==1))then --开关模压力上限密码保护功能使用
			WGTSetVisible("labelpsw", 1)
			WGTSetVisible("editpsw", 1)
			VarAdrSetInt(TMP_SYSTEMPAR1_SHOW,1)
        else
			WGTSetVisible("labelpsw", 0)
			WGTSetVisible("editpsw", 0)
			VarAdrSetInt(TMP_SYSTEMPAR1_SHOW,1)
        end

    else
		WGTSetVisible("labelpsw", 0)
        WGTSetVisible("editpsw", 0)
		VarAdrSetInt(TMP_SYSTEMPAR1_SHOW,1)
    end

    --20241017 chj 金鹰厂家要求,开关模压力上限密码保护功能使用
    if(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) == 0x6400)then
		if((VarAdrToInt(SYS_CLAMP_PROTECT_FUNC)==1))then --开关模压力上限密码保护功能使用
			ReadOnly("edit31", true)
			ReadOnly("edit41", true)
			ReadOnly("edit43", true)
		else
			ReadOnly("edit31", false)
			ReadOnly("edit41", false)
			ReadOnly("edit43", false)
		end
    else
			ReadOnly("edit31", false)
			ReadOnly("edit41", false)
			ReadOnly("edit43", false)
    end


end

function PageUpdate()


end

function PageChange(name,adr,oldvalue,newvalue)

	if(name == "edit34") then --储料背压压力上限
		local pdata1 = VarAdrToUI16(DAADJUST_DACHANEL)
		local pdata2 = VarAdrToUI16(INJECT_PORT_CHRGBAcK)
		local pdata3 = VarAdrToUI16(SYSTEM_DAPRESS_BACKLIMIT)
		ProtSysCtrl(CHGBACK_ADJ_CMD, 3, pdata1, pdata2, pdata3)
	end


    if(adr == SYSTEM_MACH_DAPRES_CLPLIMIT) then
		if((VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) == 3) or (VarAdrToInt(CLAMP_MOLD_FL_CLSOPEN_MODE) == 4)) then
			SyncClsAndOpnPrs()
        end
    end

    --20220223 dyl 高压速度上限变化跟随变化
    if(adr == CLSHIPRS_SPD_MAX) then
		if(VarAdrToUI16(CLSHIPRS2_SPD)>VarAdrToUI16(CLSHIPRS_SPD_MAX)) then
			CtlAdrInt(CLSHIPRS2_SPD,VarAdrToUI16(CLSHIPRS_SPD_MAX))
		end
		if(VarAdrToUI16(CLSHIPRS_SPD)>VarAdrToUI16(CLSHIPRS_SPD_MAX)) then
			CtlAdrInt(CLSHIPRS_SPD,VarAdrToUI16(CLSHIPRS_SPD_MAX))
		end
	end

	if(adr == TMP_SYSTEM_PASSWORD) then
		if(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) == 0x7500) then--20241017 chj 宁塑厂家要求，增加系统参数密码设定
			if(newvalue == 625238) then
				VarAdrSetInt(TMP_SYSTEMPAR1_SHOW,1)
				SetFocus("edit31")
			else
				VarAdrSetInt(TMP_SYSTEMPAR1_SHOW,0)
			end
		elseif(band(VarAdrToInt(SYS_MACH_CUSTOMID),0xff00) == 0x6400)then--20241017 chj 金鹰厂家要求,开关模压力上限密码保护功能使用
			CalClampPrsMaxProtectPSW(TMP_SPECIAL_CAL)
			if(VarAdrToInt(TMP_SYSTEM_PASSWORD) == VarAdrToInt(TMP_SPECIAL_CAL)) then
				ReadOnly("edit31", false)
				ReadOnly("edit41", false)
				ReadOnly("edit43", false)
				SetFocus("edit31")
			else
				ReadOnly("edit31", true)
				ReadOnly("edit41", true)
				ReadOnly("edit43", true)
			end
		end

	end

	return 0

end

function PageHide()
	SetPageLockMode(0)

	--20200302 退出画面密码框值设0，方便通用的润滑模式颜色变化也可以用开关模上限编辑框的颜色变量
	VarAdrSetInt(TMP_SYSTEM_PASSWORD,0)
end
