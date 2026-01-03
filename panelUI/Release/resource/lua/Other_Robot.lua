
local FL_ROBOTCTRL = 0x31130000   --机械手功能选择
local FL_CORESEC1  = 0x31130002   --中子信号1连接
local FL_CORESEC2  = 0x31130003   --中子信号2连接
local VW_MSG_CORESELECTED = 51 --此中子已选择！

function SetRobotMode()
	if(VarAdrToInt(FL_ROBOTCTRL) == 3) then
		if((VarAdrToInt(FL_CORESEC1) == VarAdrToInt(FL_CORESEC2)) and (VarAdrToInt(FL_CORESEC1) ~= 0)) then
			CtlAdrInt(FL_CORESEC1, 1)
			CtlAdrInt(FL_CORESEC2, 2)
		end
	end
end

-- 中子信号连接选择
function RbtCoreLink(addr,old_value)
	local value
	local valuex
	value = VarAdrToInt(FL_CORESEC1)
	valuex = VarAdrToInt(FL_CORESEC2)

	if(VarAdrToInt(FL_ROBOTCTRL)==3) then

		if(valuex==1) then
			if(value==1) then
				CtlAdrInt(addr,old_value)
				INFO_ShowMsg(VW_MSG_CORESELECTED)
			end
		elseif(valuex==2) then
			if(value==2) then
				CtlAdrInt(addr,old_value)
				INFO_ShowMsg(VW_MSG_CORESELECTED)
			end
		elseif(valuex==3) then
			if(value==3) then
				CtlAdrInt(addr,old_value)
				INFO_ShowMsg(VW_MSG_CORESELECTED)
			end
		elseif(valuex==4) then
			if(value==4) then
				CtlAdrInt(addr,old_value)
				INFO_ShowMsg(VW_MSG_CORESELECTED)
			end
		elseif(valuex==5) then
			if(value==5) then
				CtlAdrInt(addr,old_value)
				INFO_ShowMsg(VW_MSG_CORESELECTED)
			end
		elseif(valuex==6) then
			if(value==6) then
				CtlAdrInt(addr,old_value)
				INFO_ShowMsg(VW_MSG_CORESELECTED)
			end
		end
	end
end

--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()
	SetPageLockMode(1)
	
	SetRobotMode()
    
end

--画面隐藏--
function PageHide()
	SetPageLockMode(0)
end

--画面刷新--
function PageUpdate()

end

--画面修改--
local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50
local CLAMP_MACHSET_FL_ADPOSROBOT = 0x20120030  --开模途中机械手选择
local CLAMP_MACHSET_TM_MANUALOUT  = 0x20120035  --手动输出时间
function PageChange(name,adr,oldvalue,newvalue)
	print("Robot PageChange")
	if(name == "cb11") then --机械手
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			SetRobotMode()
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "stbt21") then --途中机械手功能
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			CtlAdrInt(CLAMP_MACHSET_FL_ADPOSROBOT,newvalue) --机器参数转模具参数
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "ed51") then --途中机械手输出时间
		CtlAdrInt(CLAMP_MACHSET_TM_MANUALOUT,newvalue)
    elseif(name == "cb61") then --中子信号1连接
		RbtCoreLink(FL_CORESEC1,oldvalue)
	elseif(name == "cb71") then --中子信号2连接
		RbtCoreLink(FL_CORESEC2,oldvalue)
	end
    return 0
end
