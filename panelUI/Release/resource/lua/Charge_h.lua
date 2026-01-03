local INJECT_MOLDSET_SEC_CHG = 0x10130018 -- 储料使用段数
local INJECT_ACTPARA_ADPOSI_CHG1END = 0x10140055 --储料一段终止位置
local INJECT_ACTPARA_ADPOSI_CHG2END = 0x10140056 --储料二段终止位置
local INJECT_ACTPARA_ADPOSI_CHG3END = 0x10140057 --储料三段终止位置
local INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE = 0x1014005D --射退距离
local INJECT_ACTPARA_ADPOSI_SUCKBACKEND = 0x1014005C --射退终止位置
local INJECT_MOLD_FL_SUCK_SOOMTH = 0x10130047 --射退平稳
local ADDR_MAX_ADINJECT = 0x07160012 --射出电子尺最大值

local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00 --手动模式
local VW_MSG_MANUAL = 50 --请在手动模式操作此功能!

local SYSTEM_PRS_MAX = 0x00120021 --系统压力上限
local SYSTEM_MACH_CUSTOMID = 0x00120018 --20200317客户机器代码
local SWCYCLEMOLDCURRENTCNT = 0x33120073  --20200317周期计数当前模数

local SYS_FL_MACH_CODE54 = 0x0012000E --20211227 dyl
local INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD = 0x10130010 --再次储料位置 --20211227 dyl
local INJECT_MOLDSET_TM_CHGFLOWMOLDING = 0x10130011 --再次储料时间 --20211227 dyl
local INJECT_MOLDSET_REMELT_FUNC = 0x331200d3 --再次储料模式 --20211227 dyl

local SYS_FL_MACH_CODE1 = 0x00120001 --20230518 chj
local FL_OPNDURINGCOOL = 0x130000 --储料开模联动

local FL_ELECTRIC_CHG = 0x10120058 --电储料功能
local FL_ELECTRIC_NOZFWD = 0x10120059 --电储料同步座进

local FL_ELECTRIC_SUCK = 0x10120067 --电储料射退模式 --20250306 jhh 非手动情况下提示
local FL_ELECCHG_HYDR = 0x10120068 --电储料液压
local PORT_ELECCHG_HYDR = 0x1012006b --电储料液压通道
local PORT_ELECCHG_DA = 0x1012006c --电储料通道
local PORT_ELECTRIC_CHG_MODE = 0x1012006d --电储料输出方式

--20250103 chj 座台慢速调试压力和速度变灰控制
function SetNzDbgVis()

    if(VarAdrToInt(FL_ELECTRIC_NOZFWD) == 0) then
		WGTSetVisible("tabpage1_Tb8_labelnzdbgprs", 1)
        WGTSetVisible("tabpage1_Tb8_labelnzdbgprs_unit", 1)
        WGTSetVisible("tabpage1_Tb8_editnzdbgprs", 1)
        
		WGTSetVisible("tabpage1_Tb8_labelnzdbgspd", 1)
        WGTSetVisible("tabpage1_Tb8_labelnzdbgspd_unit", 1)
        WGTSetVisible("tabpage1_Tb8_editnzdbgspd", 1)
    else
		WGTSetVisible("tabpage1_Tb8_labelnzdbgprs", 0)
        WGTSetVisible("tabpage1_Tb8_labelnzdbgprs_unit", 0)
        WGTSetVisible("tabpage1_Tb8_editnzdbgprs", 0)
        
		WGTSetVisible("tabpage1_Tb8_labelnzdbgspd", 0)
        WGTSetVisible("tabpage1_Tb8_labelnzdbgspd_unit", 0)
        WGTSetVisible("tabpage1_Tb8_editnzdbgspd", 0)
    end

end

function SetPrsEnable()
	if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x7A00) then
		if(VarAdrToInt(FL_ELECTRIC_CHG) == 1) then
			WGTSetEnable("edit35", 0)
			WGTSetEnable("edit36", 0)
			WGTSetEnable("edit37", 0)
			WGTSetEnable("label10", 0)
		else
			WGTSetEnable("edit35", 1)
			WGTSetEnable("edit36", 1)
			WGTSetEnable("edit37", 1)
			WGTSetEnable("label10", 1)
		end
	else
		WGTSetEnable("edit35", 1)
		WGTSetEnable("edit36", 1)
		WGTSetEnable("edit37", 1)
		WGTSetEnable("label10", 1)
	end
end

function SetClsChgWaitVis()

	if( (band(VarAdrToUI16(SYS_FL_MACH_CODE1),0x100)~=0) and (VarAdrToInt(FL_OPNDURINGCOOL) == 1) ) then
		WGTSetEnable("tabpage1_Tb1_label214", 1)
        WGTSetEnable("tabpage1_Tb1_dropdown1", 1)
        WGTSetEnable("tabpage1_Tb8_labelelechgopnlinkskbk", 1)
        WGTSetEnable("tabpage1_Tb8_stbtelechgopnlinkskbk", 1)
	else
		WGTSetEnable("tabpage1_Tb1_label214", 0)
        WGTSetEnable("tabpage1_Tb1_dropdown1", 0)
		WGTSetEnable("tabpage1_Tb8_labelelechgopnlinkskbk", 0)
        WGTSetEnable("tabpage1_Tb8_stbtelechgopnlinkskbk", 0)
	end
end

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

function PageCreate()

end

function PageShow()
	SetPageLockMode(1)
	local suckendpos

	if(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==1) then  --储料段数 --20211110 dyl 储料1~3段
		suckendpos = VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END)
		CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND,suckendpos)
    elseif(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==2) then  --储料段数
		suckendpos = VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)
		CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND,suckendpos)
	elseif(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==3) then --储料段数
		suckendpos = VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)
		CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND,suckendpos)
	end

	--202412 fqh 电储料画面是否可见
	if(VarAdrToInt(FL_ELECTRIC_CHG) == 1) then
		TabPageSetSubVisble("tabpage1",7,true)
	else
		TabPageSetSubVisble("tabpage1",7,false)
	end
    
    --20250103 chj 座台慢速调试压力和速度变灰控制
    SetNzDbgVis()
    
    SetPrsEnable()
    
    if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",4,true)
        if(VarAdrToInt(INJECT_MOLD_FL_SUCK_SOOMTH) > 0) then
			TabPageSetSubVisble("tabpage1",5,false)
            TabPageSetSubVisble("tabpage1",6,true)
        else
			TabPageSetSubVisble("tabpage1",5,true)
            TabPageSetSubVisble("tabpage1",6,false)
        end
    else
        TabPageSetSubVisble("tabpage1",4,false)
        TabPageSetSubVisble("tabpage1",5,false)
        TabPageSetSubVisble("tabpage1",6,false)
    end

    --20211227 dyl 再次储料功能
    if((VarAdrToInt(INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD) ==0) and (VarAdrToInt(INJECT_MOLDSET_TM_CHGFLOWMOLDING) == 0)) then
		CtlAdrInt(INJECT_MOLDSET_REMELT_FUNC, 0)
	else
		if(band(VarAdrToUI16(SYS_FL_MACH_CODE54),0x80)~=0) then
			if(VarAdrToInt(INJECT_MOLDSET_TM_CHGFLOWMOLDING) ~=0) then
				CtlAdrInt(INJECT_MOLDSET_REMELT_FUNC, 1)
			elseif(VarAdrToInt(INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD) ~=0) then
				CtlAdrInt(INJECT_MOLDSET_REMELT_FUNC, 2)
			end
		else
			CtlAdrInt(INJECT_MOLDSET_REMELT_FUNC, 0)
		end
    end


    TabPageSelPage("tabpage1",0)
	FkeyTab("tabpage1", 0x203)

    SetClsChgWaitVis()

end

local SLOPE_MAX_SUCK_PRES = 0x10130046 --射退最大斜率(压力)
local SUCK_PRES_STARTON = 0x1013004A --射退启动斜率(压力)
local SUCK_PRES_HALFWAY = 0x1013004B --射退中间斜率(压力)
local SUCK_PRES_STOPOFF = 0x1013004C --射退停止斜率(压力)
local SLOPE_MAX_SUCK_SPD = 0x10130045 --射退最大斜率(速度)
local SUCK_SPD_STARTON = 0x1013004D --射退启动斜率(速度)
local SUCK_SPD_HALFWAY = 0x1013004E --射退中间斜率(速度)
local SUCK_SPD_STOPOFF = 0x1013004F --射退停止斜率(速度)
function PageUpdate()
	EditSetNum("tabpage1_Tb7_dt14",GetPrsMS(SLOPE_MAX_SUCK_PRES,SUCK_PRES_STARTON),0)
	EditSetNum("tabpage1_Tb7_dt24",GetPrsMS(SLOPE_MAX_SUCK_PRES,SUCK_PRES_HALFWAY),0)
	EditSetNum("tabpage1_Tb7_dt34",GetPrsMS(SLOPE_MAX_SUCK_PRES,SUCK_PRES_STOPOFF),0)
    EditSetNum("tabpage1_Tb7_dt16",GetSpdMS(SLOPE_MAX_SUCK_SPD,SUCK_SPD_STARTON),0)
	EditSetNum("tabpage1_Tb7_dt26",GetSpdMS(SLOPE_MAX_SUCK_SPD,SUCK_SPD_HALFWAY),0)
	EditSetNum("tabpage1_Tb7_dt36",GetSpdMS(SLOPE_MAX_SUCK_SPD,SUCK_SPD_STOPOFF),0)
end

local NOZZLE_MOLD_FL_NOZRET = 0x11130002 --射退方式
function PageChange(name,adr,oldvalue,newvalue)
	local wmax
	local suckendpos
    print("Charge PageChange")
    if(name == "btxCURV") then
		PanelShowPageByName("Charge_Curve")
	elseif(name == "ed41") then --一段终止位置 --20211110 dyl 储料1~3段
		if(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==1) then
			if(VarAdrToInt(ADDR_MAX_ADINJECT) >= VarAdrToInt(adr)) then
				if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE) > (VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(adr))) then
					CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE,(VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(adr)))
				end
			end
			suckendpos = VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END)
			CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND,suckendpos)
        end
    elseif(name == "ed42") then --二段终止位置
        if(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==2) then
			if(VarAdrToInt(ADDR_MAX_ADINJECT) >= VarAdrToInt(adr)) then
				if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE) > (VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(adr))) then
					CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE,(VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(adr)))
				end
			end
			suckendpos = VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)
			CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND,suckendpos)
        end
    elseif(name == "ed43") then --三段终止位置
        if(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==3) then
			if(VarAdrToInt(ADDR_MAX_ADINJECT) >= VarAdrToInt(adr)) then
				if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE) > (VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(adr))) then
					CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE,(VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(adr)))
				end
			end
			suckendpos = VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)
			CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND,suckendpos)
        end
	elseif(name == "ed72") then --射退距离
		if(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==1) then --20211110 dyl 储料1~3段
			wmax= (VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END)+newvalue)
			suckendpos =  VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END)
        elseif(VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==2) then
			wmax= (VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)+newvalue)
			suckendpos =  VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)
        elseif (VarAdrToInt(INJECT_MOLDSET_SEC_CHG) ==3) then
			wmax= (VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)+newvalue)
            suckendpos =  VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)
		end
		CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND,suckendpos)
	elseif(name == "tabpage1_Tb1_ed11")  then --储料段数
		if(newvalue ==1) then --20211110 dyl 储料1~3段
			if(VarAdrToInt(ADDR_MAX_ADINJECT) >= VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END)) then
				if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE) > (VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END))) then
					CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE,(VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END)))
				end
			end
			CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND, (VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END)))
		elseif(newvalue ==2) then
			if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END) > VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)) then --20211110 dyl 储料1~3段
				CtlAdrInt(INJECT_ACTPARA_ADPOSI_CHG2END,VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END))
            end
			if(VarAdrToInt(ADDR_MAX_ADINJECT) >= VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)) then
				if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE) > (VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END))) then
					CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE,(VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)))
				end
			end
			CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND, (VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)))
		elseif(newvalue ==3) then
			if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END) > VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END)) then --20211110 dyl 储料1~3段
				CtlAdrInt(INJECT_ACTPARA_ADPOSI_CHG2END,VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG1END))
            end
			if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END) > VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)) then
				CtlAdrInt(INJECT_ACTPARA_ADPOSI_CHG3END,VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG2END))
            end
            if(VarAdrToInt(ADDR_MAX_ADINJECT) >= VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)) then
				if(VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE) > (VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END))) then
					CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE,(VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)))
				end
			end
			CtlAdrInt(INJECT_ACTPARA_ADPOSI_SUCKBACKEND, (VarAdrToInt(INJECT_ACTPARA_ADPOSI_SUCKBACKDISTANCE)+ VarAdrToInt(INJECT_ACTPARA_ADPOSI_CHG3END)))
		end
    elseif(name == "tabpage1_Tb1_cb12") then --射退模式
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then

		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "tabpage1_Tb1_cb22") then --储前射退模式
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then

		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "tabpage1_Tb1_stbt51") then --储料开模联动
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			if(VarAdrToInt(adr) == 1) then
				if(VarAdrToInt(NOZZLE_MOLD_FL_NOZRET) == 2) then --射退方式选择开模前
					CtlAdrInt(NOZZLE_MOLD_FL_NOZRET,0)
                end
            end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end

        SetClsChgWaitVis()
	elseif(name == "tabpage1_Tb2_cbRemelt") then --再次储料模式 --20211227 dyl
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			if(newvalue == 1) then
				VarAdrSetInt(SYS_FL_MACH_CODE54,bor(VarAdrToInt(SYS_FL_MACH_CODE54),0x0080))
				CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
				if(VarAdrToInt(INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD) ~= 0) then
					CtlAdrInt(INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD,0)
                end
			elseif(newvalue == 2) then
				VarAdrSetInt(SYS_FL_MACH_CODE54,bor(VarAdrToInt(SYS_FL_MACH_CODE54),0x0080))
				CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
				if(VarAdrToInt(INJECT_MOLDSET_TM_CHGFLOWMOLDING) ~= 0) then
					CtlAdrInt(INJECT_MOLDSET_TM_CHGFLOWMOLDING,0)
                end
			else
				VarAdrSetInt(SYS_FL_MACH_CODE54,band(VarAdrToUI16(SYS_FL_MACH_CODE54),0xFF7F))
				CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
				if(VarAdrToInt(INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD) ~= 0) then
					CtlAdrInt(INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD,0)
                end
                if(VarAdrToInt(INJECT_MOLDSET_TM_CHGFLOWMOLDING) ~= 0) then
					CtlAdrInt(INJECT_MOLDSET_TM_CHGFLOWMOLDING,0)
                end
            end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "tabpage1_Tb2_edit12") then
		if(newvalue == 0) then
			if(VarAdrToInt(INJECT_MOLDSET_ADPOSI_CHGFLOWMOLD) == 0) then
				if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
					VarAdrSetInt(SYS_FL_MACH_CODE54,band(VarAdrToUI16(SYS_FL_MACH_CODE54),0xFF7F))
					CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
				else
					CtlAdrInt(adr,oldvalue)
					INFO_ShowMsg(VW_MSG_MANUAL)
					return -1
				end
			end
		end
	elseif(name == "tabpage1_Tb2_edit13") then
		if(newvalue == 0) then
			if(VarAdrToInt(INJECT_MOLDSET_TM_CHGFLOWMOLDING) == 0) then
				if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
					VarAdrSetInt(SYS_FL_MACH_CODE54,band(VarAdrToUI16(SYS_FL_MACH_CODE54),0xFF7F))
					CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
				else
					CtlAdrInt(adr,oldvalue)
					INFO_ShowMsg(VW_MSG_MANUAL)
					return -1
				end
			end
		end
    elseif(name == "tabpage1_Tb8_dropdown24") then
    
		SetNzDbgVis()
	end
	--20200317 威力士真空发生器 模数重置
	if(name == "tabpage1_Tb2_bt61") then
		print("Charge Ptabpage1_Tb2_bt61")
		VarAdrSetInt(SWCYCLEMOLDCURRENTCNT, 0)
        VarAdrSaveInt(SWCYCLEMOLDCURRENTCNT)
	end

	if(adr == FL_ELECTRIC_SUCK) then --电储料射退模式 --20250306 jhh 非手动情况下提示
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
end
