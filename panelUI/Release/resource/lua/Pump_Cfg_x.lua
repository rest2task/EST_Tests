
SYSTEM_MACHSET_FL_HYDRAUIC=0x0012000F --408  液压控制选择码
SYSTEM_MACHSET_FL_MACHINTERNAL6=0x0012000E
HYDR_MACHSET_FL_PWRMATCHTBL=0x09120001 --使用面板PUMP配置表
SYSTEM_MACHSET_FL_MACHINECTRL0 = 0x00120000-- 400-405 机器动作控制码 80H

local useServo5 = band(VarAdrToInt(SYSTEM_MACHSET_FL_MACHINTERNAL6),0x1020) ~=0 and (band(SYSTEM_MACHSET_FL_MACHINECTRL0,0x0004) == 0)

local isMultPump = band(VarAdrToInt(SYSTEM_MACHSET_FL_HYDRAUIC),0x800) ~=0

local isPumpAdjust = VarAdrToInt(HYDR_MACHSET_FL_PWRMATCHTBL) == 1
local isViceStrStop = isMultPump and isPumpAdjust and useServo5
local isPumpAdj = isMultPump and isPumpAdjust

local TMP_VICE_STARTSPD2	=TMPVAL(0)  --副泵启动速度
local TMP_VICE_STARTSPD3	=TMPVAL(1)  --副泵启动速度
local TMP_VICE_STARTSPD4	=TMPVAL(2)  --副泵启动速度
local TMP_VICE_STARTSPD5	=TMPVAL(3)  --副泵启动速度
local TMP_VICE_STARTSPD6	=TMPVAL(4)  --副泵启动速度
local TMP_VICE_STARTSPD7	=TMPVAL(5)  --副泵启动速度

local TMP_VICE_STOPSPD2	=TMPVAL(10)  --副泵退出速度
local TMP_VICE_STOPSPD3	=TMPVAL(11)  --副泵退出速度
local TMP_VICE_STOPSPD4	=TMPVAL(12)  --副泵退出速度
local TMP_VICE_STOPSPD5	=TMPVAL(13)  --副泵退出速度
local TMP_VICE_STOPSPD6	=TMPVAL(14)  --副泵退出速度
local TMP_VICE_STOPSPD7	=TMPVAL(15)  --副泵退出速度

local SYS_FL_MACH_CODE0    =0x00120000 -- code0
local	SYS_FL_MACH_CODE54   =0x0012000E -- code54
local SYS_FL_HYDRAUIC      =0x0012000F --液压控制
local	SERVO_STATE_CANST1   =0x0A160012  --伺服CAN通信状态1

local		CAN_TX_READ 		=		0x03
local		CAN_TX_WRITE    	=		0x06
local		CAN_TX_IDRESPONSE  	=		0x08
local		CAN_TX_MAINTEST    	=		0x0C
local     CAN_TX_MAINPRG       =       0x0D

local	SERVO_STATE_CANST1=   0x0A160012  --伺服CAN通信状态1

local ID_A322= 38
local ID_A320= 40
local VW_MSG_MANUAL =50
local MODE_MANUAL     =    0x00

local function PumpCfgPageIn()
	if((band(VarAdrToUI16(SYS_FL_MACH_CODE0),0x04)==0) and  (band(VarAdrToUI16(SYS_FL_MACH_CODE54), 0x1020)~=0)) then
		if(band(VarAdrToUI16(SYS_FL_HYDRAUIC),0x0800)~=0)then
			for i=2,7 do
				if(band(VarAdrToUI16(SERVO_STATE_CANST1+i-1) , 0x80)~=0) then--online
					Send_TaskCan(CAN_TX_READ,i,0xA322,0)--副泵启动
					Send_TaskCan(CAN_TX_READ,i,0xA320,0)--副泵退出
				end
			end
		end
	end
	--vice pump start or stop speed set */
	for i=0,5 do
		VarAdrSetInt(TMP_VICE_STARTSPD2+i,GetServoData(2+i,ID_A322)/10)
		VarAdrSetInt(TMP_VICE_STOPSPD2+i,GetServoData(2+i,ID_A320)/10)
	end
end



function PageCreate()

end


function PageShow()
	SetPageLockMode(2)
	
	--setEnable()

	PumpCfgPageIn()
end

function PageUpdate()

end

function PageChange(name,adr,oldvalue,newvalue)
	if((adr >= TMP_VICE_STARTSPD2) and (adr <= TMP_VICE_STARTSPD7)) then
		local index = adr - TMP_VICE_STARTSPD2
		if(GetOPMode() == MODE_MANUAL) then
			if(band(VarAdrToUI16(SERVO_STATE_CANST1+1+index),0x80)~=0) then
				local value = VarAdrToUI16(TMP_VICE_STARTSPD2+index)
				Send_TaskCan(CAN_TX_WRITE,index+2,0xA322,value*10)
				Send_TaskCan(CAN_TX_WRITE,index+2,0xA323,value*10)
				Send_TaskCan(CAN_TX_READ, index+2,0xA322,0)
			end
		else
			CtlAdrInt(addr,msg_value)--原始值
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif((adr >= TMP_VICE_STOPSPD2) and (adr <= TMP_VICE_STOPSPD7)) then
		if(GetOPMode() == MODE_MANUAL) then
			if(band(VarAdrToUI16(SERVO_STATE_CANST1+1+index),0x80)~=0) then
				local value = VarAdrToUI16(TMP_VICE_STARTSPD2+index)
				Send_TaskCan(CAN_TX_WRITE,index+2,0xA320,value*10)
				Send_TaskCan(CAN_TX_WRITE,index+2,0xA320,value*10)
				Send_TaskCan(CAN_TX_READ, index+2,0xA320,0)
			end
		else
			CtlAdrInt(addr,msg_value)--原始值
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(adr == HYDR_MACHSET_FL_PWRMATCHTBL)then
		--setEnable()
	end

	return 0

end

function PageHide()
	SetPageLockMode(0)
end
