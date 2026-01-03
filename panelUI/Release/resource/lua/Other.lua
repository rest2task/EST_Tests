local SYS_FL_MACH_CODE1  =  0x00120001 -- code1
local SYS_FL_MACH_CODE0  =  0x00120000 -- code0
local SYS_FL_MACH_CODE54 =  0x0012000E -- code54
local SYS_FL_METER =  0x00120011 -- 位置尺控制
local SYSTEM_MOLD_FL_OPNDURINGCOOL = 0x00130000 --冷却开模功能 

local NOZZLE_MACH_NZL_METERCH = 0x11120000  --座台电子尺通道
local SYSTEM_MACH_FL_CTRLMODEL = 0x0012001c --控制器型号IV5200  IV3200
local ADMETER_ADPOSI_RELATIVE4 = 0x07160005 --座台电子尺
local ADMETER_ADPOSI_RELATIVE_EXT1 = 0x07160022 --20200324扩展电子尺

local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50
local VW_MSG_OPENLINKERR = 99 --储料开模功能使用时不能选择此功能!

local INJECT_MOLD_FL_INCACC = 0x10130000 --射出增速储能功能 
local INJSTOR_MACHSET_INJSTOR_METERCH = 0x12120000 --射出储能压力传感器通道号
local SERVO_CURRENT_NUM = 0x331300E8 --伺服个数
local PUMP1_MACHSET_PRESSAD_CH = 0x0F120005	--压力传感器AD编号
local VW_MSG_METERCHANNELUSED = 117
local d_machine1_VERSION_model_and_cat = 0x00140001
local FL_AUTONOZFWD = 0x11130007 --自动全程输出座进阀 --20250306 jhh 非手动情况下提示

--20200324座台电子尺地址切换
function SetNozMeter()
	local noz_chanel = VarAdrToInt(NOZZLE_MACH_NZL_METERCH)

--	if(VarAdrToInt(SYSTEM_MACH_FL_CTRLMODEL)==0x5200 or VarAdrToInt(SYSTEM_MACH_FL_CTRLMODEL)==0x5300) then --20220120 dyl 电子尺通道统一处理
	if(GetADMaxNumber()==8) then --20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
		if(noz_chanel>=4 and noz_chanel<=7) then
			WGTSetAttrByName("edit193","text",ADMETER_ADPOSI_RELATIVE4+(noz_chanel-4))
		end
--    elseif(VarAdrToInt(SYSTEM_MACH_FL_CTRLMODEL)==0x3100 and (band(VarAdrToInt(d_machine1_VERSION_model_and_cat),0x1FFF)==3101)) then
	elseif(GetADMaxNumber()==6) then --20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
		if(noz_chanel==7) then
			WGTSetAttrByName("edit193","text",ADMETER_ADPOSI_RELATIVE_EXT1)
		elseif(noz_chanel>=4 and noz_chanel<=6) then
			WGTSetAttrByName("edit193","text",ADMETER_ADPOSI_RELATIVE4+(noz_chanel-4))
		end
	else
		if(noz_chanel==4) then
			WGTSetAttrByName("edit193","text",ADMETER_ADPOSI_RELATIVE4)
		elseif(noz_chanel>=5 and noz_chanel<=7) then
			WGTSetAttrByName("edit193","text",ADMETER_ADPOSI_RELATIVE_EXT1+(noz_chanel-5))
		end
	end
end

--20200628 检查电子尺通道是否已使用
function CheckChannelUsed(value)
	if(VarAdrToInt(INJECT_MOLD_FL_INCACC) == 2) then
		if(value == VarAdrToInt(INJSTOR_MACHSET_INJSTOR_METERCH)) then
			return 1
        end
    end
    
    if(band(VarAdrToInt(SYS_FL_MACH_CODE1),0x0004)~=0) then --20220923 dyl ECAT模式下才判断座台通道号
		for i=0,VarAdrToInt(SERVO_CURRENT_NUM)-1,1 do  
			if(value == VarAdrToInt(PUMP1_MACHSET_PRESSAD_CH+i*0x100000)) then
				return 1
			end
		end 
	end
    
    return 0
end

--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()
	SetPageLockMode(1)
	
	if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",1,true)
        TabPageSetSubVisble("tabpage1",2,true)
        WGTSetVisible("stTips", 1)
    else 
		TabPageSetSubVisble("tabpage1",1,false)
        TabPageSetSubVisble("tabpage1",2,false)
        WGTSetVisible("stTips", 0)
	end
	
	if(band(VarAdrToInt(SYS_FL_MACH_CODE1),0x0004)~=0) then --20220923 dyl ECAT模式下才判断座台通道号
		if(band(VarAdrToInt(SYS_FL_METER),0x0004) ~= 0) then
			WGTSetVisible("tabpage1_Tb2_label19", 1)
			WGTSetVisible("tabpage1_Tb2_edit1", 1)
		else
			WGTSetVisible("tabpage1_Tb2_label19", 0)
			WGTSetVisible("tabpage1_Tb2_edit1", 0)
		end
	else
		WGTSetVisible("tabpage1_Tb2_label19", 0)
		WGTSetVisible("tabpage1_Tb2_edit1", 0)
	end
	
	--20200324 座台电子尺地址切换
	SetNozMeter()
    
    TabPageSelPage("tabpage1",0)
    FkeyTab("tabpage1", 0x206)

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
	print("Other PageChange")
    if(name == "cb51") then
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			if(band(VarAdrToInt(SYS_FL_MACH_CODE1),0x100)~=0 and (VarAdrToInt(SYSTEM_MOLD_FL_OPNDURINGCOOL) == 1)) then
				if(VarAdrToInt(adr) == 2) then
					CtlAdrInt(adr,oldvalue)
					INFO_ShowMsg(VW_MSG_OPENLINKERR)
					return -1
                end
			end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "tabpage1_Tb2_stbt11") then  --座进前检测锁模终
		if(VarAdrToInt(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave() --发送code码给下位机，然后发送保存命令
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_MACH_CODE4,bor(VarAdrToInt(SYS_FL_MACH_CODE4),0x0100))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE4,band(VarAdrToInt(SYS_FL_MACH_CODE4),bnot(0x0100)))
			end
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "tabpage1_Tb2_edit210") then --20200324 座台电子尺地址切换
		if(CheckChannelUsed(newvalue) == 1) then
			CtlAdrInt(adr,oldvalue)
			ShowMsg(VW_MSG_METERCHANNELUSED)
			return -1
		else
			SetNozMeter()
		end
    end
    
	return 0
end
