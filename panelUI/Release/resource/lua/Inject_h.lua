local INJECT_TURNTOHOLD = 0x10130009 --保压转换方式 0:压力/时间 1:压力 2:速度 3:时间 4:位置 
local INJECT_CUREMODE = 0x3312001e   --射出曲线模式

local SYSTEM_MACH_CUSTOMID = 0x00120018 --客户机器代码 //20241226 chj 威力士快速机模式下，部分参数变灰：漏油检测功能
local p_PP_MACHSET_WLSMACHINE_TYPE = 0x331200fc --机器类型

local OPMODE_INDEX = 0x33160013 --Operation mode index --20250306 jhh 非手动情况下提示
local MODE_MANUAL = 0x00 --手动模式
local VW_MSG_MANUAL = 50 --请在手动模式操作此功能!

--20241214 chj 针阀：dic
local d_inject1_MACHSET_FL_INj_SPC = 0x1012003b 		--胶口针阀
local d_inject1_MACHSET_INJ_SPC_FUNC_CLS = 0x10120056 	--针阀关闭方式
local d_inject1_MACHSET_FL_INJPRESS = 0x1012005a 		--射出压力独立检测
local d_inject1_MACHSET_FL_INJSPC_AIR  = 0x1012005c     --针阀同步吹气点1
local d_inject1_MACHSET_FL_INJSPC_AIR1 = 0x1012005d     --针阀同步吹气点2
local FL_BLAST1 = 0x23130000 							--吹气1方式
local SYSTEM_MACH_CUSTOMID = 0x00120018 				--客户机器代码
local d_inject1_MACHSET_FL_INJ_SPC_FUNC = 0x1012004d    --针阀控制方式

--20241214 chj 针阀：针阀提前关时间选项变灰设置
function SetNeedVlvEarlyOffTmVis()
    if((VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) == 1) and (VarAdrToInt(d_inject1_MACHSET_INJ_SPC_FUNC_CLS) == 0))then
        print("PageShow1") 
        WGTSetEnable("tabpage1_Tb5_labelNdlOffTm", 1)
        WGTSetEnable("tabpage1_Tb5_labelNdlOffTm_unit", 1)
        WGTSetEnable("tabpage1_Tb5_edNdlOffTm", 1)
    
    else 
		print("PageShow2") 
        WGTSetEnable("tabpage1_Tb5_labelNdlOffTm", 0)
        WGTSetEnable("tabpage1_Tb5_labelNdlOffTm_unit", 0)
        WGTSetEnable("tabpage1_Tb5_edNdlOffTm", 0)
    end
end


--20250106 chj 针阀开变灰控制
function SetNdOpenShow()

    if((VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) ~= 0) and (VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC) == 1)) then

        WGTSetEnable("tabpage1_Tb5_labelNdlOnTm", 1)
        WGTSetEnable("tabpage1_Tb5_edNdlOnTm", 1)
        WGTSetEnable("tabpage1_Tb5_labelNdlOnTm_unit", 1)

    else
   
        WGTSetEnable("tabpage1_Tb5_labelNdlOnTm", 0)
        WGTSetEnable("tabpage1_Tb5_edNdlOnTm", 0)
        WGTSetEnable("tabpage1_Tb5_labelNdlOnTm_unit", 0)
    end

    if((VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) ~= 0) and (VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC) == 0))then

        WGTSetEnable("tabpage1_Tb5_labelNdlDltOnTm", 1)
        WGTSetEnable("tabpage1_Tb5_edNdlDltOnTm", 1)
        WGTSetEnable("tabpage1_Tb5_labelNdlDltOnTm_unit", 1)

    else

        WGTSetEnable("tabpage1_Tb5_labelNdlDltOnTm", 0)
        WGTSetEnable("tabpage1_Tb5_edNdlDltOnTm", 0)
        WGTSetEnable("tabpage1_Tb5_labelNdlDltOnTm_unit", 0)
	end

end

--20241214 chj 针阀： 胶口针阀页逻辑 使用针阀同步输出吹气时，此吹气不可用--将动作时间设置为 0
function Inj_SPC_Func()
    local tmpval1 = 0
    local tmpval2 = 0
    tmpval1 = VarAdrToInt(d_inject1_MACHSET_FL_INJSPC_AIR)
    tmpval2 = VarAdrToInt(d_inject1_MACHSET_FL_INJSPC_AIR1)
    
    if(tmpval1 ~=0 )then
        CtlAdrInt(FL_BLAST1 + 0x100000*(tmpval1-1),256)
    end
    
    if(tmpval2 ~=0 )then
        CtlAdrInt(FL_BLAST1 + 0x100000*(tmpval2-1),256)
	end
end

function SetTurn()
	local value = GetAttrValue("cb52","value")
	local offset = GetAttrValue("cb52", "xo") - 25
	if(value ==0) then--时间/位置
		WGTSetAttrByName("ed66","text",0x1013000b) 
		SetAttrValue("dtInject", "xo", 28+offset)
		SetAttrValue("btxCURV", "xo", 26+offset)
	elseif(value ==1) then--压力
		WGTSetAttrByName("ed66","text",0x1013000a)
		
		--转保压方式为压力，射出压力独立检测为使用下，射出压力和射出压力传感器压力位同一个dic
		if(VarAdrToInt(d_inject1_MACHSET_FL_INJPRESS) == 1)then
			WGTSetAttrByName("dtInject","text",0x10160075)
		else
			WGTSetAttrByName("dtInject","text",0x7160005)
		end
        
		SetAttrValue("dtInject", "xo", 26+offset)
		SetAttrValue("btxCURV", "xo", 28+offset)
	elseif(value == 2) then--速度
		WGTSetAttrByName("ed66","text",0x1013000d)
        WGTSetAttrByName("dtInject","text",0x10160010)
		SetAttrValue("dtInject", "xo", 26+offset)
		SetAttrValue("btxCURV", "xo", 28+offset)
	elseif(value == 3) then--时间
		WGTSetAttrByName("ed66","text",0x1013000b)	
		SetAttrValue("dtInject", "xo", 28+offset)
		SetAttrValue("btxCURV", "xo", 26+offset)
	elseif(value == 4) then--位置
		WGTSetAttrByName("ed66","text",0x1013000b)	
		SetAttrValue("dtInject", "xo", 28+offset)
		SetAttrValue("btxCURV", "xo", 26+offset)
	else
		SetAttrValue("dtInject", "xo", 28+offset)
		SetAttrValue("btxCURV", "xo", 26+offset)
	end
	PageFrameXSort()
end

function PageCreate()

end

function PageShow()
	print("PageShow")  
	SetPageLockMode(1)  
    
    --转保压方式
    SetTurn()     
    
    --20241226 chj 威力士快速机模式下，部分参数变灰：漏油检测功能
    if((VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE) == 1) and (band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x7A00)) then 
		WGTSetEnable("tabpage1_Tb2_label73", 0)
        WGTSetEnable("tabpage1_Tb2_stbutton5", 0)
    else
		WGTSetEnable("tabpage1_Tb2_label73", 1)
        WGTSetEnable("tabpage1_Tb2_stbutton5", 1)
    end
    
    if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",2,true)
        TabPageSetSubVisble("tabpage1",3,true)
    else 
        TabPageSetSubVisble("tabpage1",2,false)
        TabPageSetSubVisble("tabpage1",3,false)
    end
    
    --20241214 chj 针阀：金鹰厂商显示
    if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400) then 
		TabPageSetSubVisble("tabpage1",4,true)
    else
		TabPageSetSubVisble("tabpage1",4,false)
    end
    
    --20241214 chj 针阀：针阀提前关时间选项变灰设置
	SetNeedVlvEarlyOffTmVis()
    
    --20250106 chj 针阀开变灰控制
    SetNdOpenShow();
    
    TabPageSelPage("tabpage1",0)
    FkeyTab("tabpage1", 0x202)
end

function PageUpdate()

end

local INJECT_HYDR_SERVO_KP_INJCT = 0x10150003 --注射动作的PID_KP
local INJECT_HYDR_SERVO_KI_INJCT = 0x10150004 --注射动作的PID_KI
local INJECT_HYDR_SERVO_KP_HOLD = 0x10150008 --保压动作的PID_KP
local INJECT_HYDR_SERVO_KI_HOLD = 0x10150009 --保压动作的PID_KI
local SYS_SERVO_KP_INJCT_HIGH = 0x32130056 --注射动作的PID_KP_HIGH
local SYS_SERVO_KI_INJCT_HIGH = 0x32130057 --注射动作的PID_KI_HIGH
local SYS_SERVO_KP_INJCT_MID = 0x32130058 --注射动作的PID_KP_MID
local SYS_SERVO_KI_INJCT_MID = 0x32130059 --注射动作的PID_KI_MID
local SYS_SERVO_KP_INJCT_LOW = 0x3213005A --注射动作的PID_KP_LOW
local SYS_SERVO_KI_INJCT_LOW = 0x3213005B --注射动作的PID_KI_LOW
local SYS_SERVO_KP_HOLD_HIGH = 0x3213005C --保压动作的PID_KP_HIGH
local SYS_SERVO_KI_HOLD_HIGH = 0x3213005D --保压动作的PID_KI_HIGH
local SYS_SERVO_KP_HOLD_MID = 0x3213005E --保压动作的PID_KP_MID
local SYS_SERVO_KI_HOLD_MID = 0x3213005F --保压动作的PID_KI_MID
local SYS_SERVO_KP_HOLD_LOW = 0x32130060 --保压动作的PID_KP_LOW
local SYS_SERVO_KI_HOLD_LOW = 0x32130061 --保压动作的PID_KI_LOW
function PageChange(name,adr,oldvalue,newvalue)
	print("clamp PageChange")
    if(name == "btxCURV") then
		if(VarAdrToInt(INJECT_CUREMODE) == 1) then
			PanelShowPageByName("Inject_Curve_1")
        else
			PanelShowPageByName("Inject_Curve")
        end
    elseif(adr == INJECT_TURNTOHOLD) then --转保压方式
		SetTurn()
    elseif(name == "tabpage1_Tb2_cbinjfluidity") then --20200819 原料流动性模式
		if(newvalue == 0) then
			CtlAdrInt(INJECT_HYDR_SERVO_KP_INJCT,VarAdrToInt(SYS_SERVO_KP_INJCT_HIGH))
            CtlAdrInt(INJECT_HYDR_SERVO_KI_INJCT,VarAdrToInt(SYS_SERVO_KI_INJCT_HIGH))

            CtlAdrInt(INJECT_HYDR_SERVO_KP_HOLD,VarAdrToInt(SYS_SERVO_KP_HOLD_HIGH))
            CtlAdrInt(INJECT_HYDR_SERVO_KI_HOLD,VarAdrToInt(SYS_SERVO_KI_HOLD_HIGH))
        elseif(newvalue == 1) then
			CtlAdrInt(INJECT_HYDR_SERVO_KP_INJCT,VarAdrToInt(SYS_SERVO_KP_INJCT_MID))
            CtlAdrInt(INJECT_HYDR_SERVO_KI_INJCT,VarAdrToInt(SYS_SERVO_KI_INJCT_MID))

            CtlAdrInt(INJECT_HYDR_SERVO_KP_HOLD,VarAdrToInt(SYS_SERVO_KP_HOLD_MID))
            CtlAdrInt(INJECT_HYDR_SERVO_KI_HOLD,VarAdrToInt(SYS_SERVO_KI_HOLD_MID))
        elseif(newvalue == 2) then
			CtlAdrInt(INJECT_HYDR_SERVO_KP_INJCT,VarAdrToInt(SYS_SERVO_KP_INJCT_LOW))
            CtlAdrInt(INJECT_HYDR_SERVO_KI_INJCT,VarAdrToInt(SYS_SERVO_KI_INJCT_LOW))

            CtlAdrInt(INJECT_HYDR_SERVO_KP_HOLD,VarAdrToInt(SYS_SERVO_KP_HOLD_LOW))
            CtlAdrInt(INJECT_HYDR_SERVO_KI_HOLD,VarAdrToInt(SYS_SERVO_KI_HOLD_LOW))
        end
    elseif(adr == d_inject1_MACHSET_FL_INJSPC_AIR) then 
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then--20250306 jhh 非手动情况下提示
			Inj_SPC_Func()--20241205 chj 针阀：页逻辑 使用针阀同步输出吹气时，此吹气不可用--将动作时间设置为 0
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(adr == d_inject1_MACHSET_FL_INJSPC_AIR1) then 
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then--20250306 jhh 非手动情况下提示
			Inj_SPC_Func()--20241205 chj 针阀：页逻辑 使用针阀同步输出吹气时，此吹气不可用--将动作时间设置为 0
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(adr == d_inject1_MACHSET_FL_INj_SPC) then 
		Inj_SPC_Func()--20241205 chj 针阀：页逻辑 使用针阀同步输出吹气时，此吹气不可用--将动作时间设置为 0
        SetNeedVlvEarlyOffTmVis()--20241214 chj 针阀：提前关时间选项变灰设置
        SetNdOpenShow()--20250106 chj 针阀开变灰控制
    
    elseif(adr == d_inject1_MACHSET_INJ_SPC_FUNC_CLS) then 
		SetNeedVlvEarlyOffTmVis()--20241214 chj 针阀：提前关时间选项变灰设置
      
	elseif(adr == d_inject1_MACHSET_FL_INJ_SPC_FUNC) then 
		SetNdOpenShow()--20250106 chj 针阀开变灰控制
	elseif(adr == d_inject1_MACHSET_FL_INJPRESS) then 
		SetTurn()--转保压方式为压力，射出压力独立检测为使用下，射出压力和射出压力传感器压力位同一个dic
          
	end
    
	return 0
end

function PageHide()
	SetPageLockMode(0)
end
