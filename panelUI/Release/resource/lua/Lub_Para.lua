local machineset_lubcountmax  =  0x00120014 --通过配置工具实现润滑模数上限
local p_PP_MACHSET_wLUBCNTMAX = 0x331200cc --自动润滑模数上限值

local p_PP_MACHSET_wLUBR1_CNT_MIN = 0x331200db --自动润滑一模数下限
local p_PP_MACHSET_wLUBR2_CNT_MIN = 0x331200dc --自动润滑二模数下限
local p_PP_MACHSET_wLUBR3_CNT_MIN = 0x331200dd --自动润滑三模数下限

local OPMODE_INDEX = 0x33160013 	--Operation mode index
local MODE_MANUAL = 0x00
local VW_MSG_MANUAL = 50

local SYS_FL_MACH_CODE5 = 0x00120005 --code5

local d_lub2_MACHSET_FL_LUBERR_AGAIN = 0x3022000b --润滑异常立即润滑2按键
local d_lub3_MACHSET_FL_LUBERR_AGAIN = 0x3032000b --润滑异常立即润滑3按键
local d_lub2_MACHSET_FL_LUBERR_AUTOMODE = 0x3022000c --润滑异常后自动润滑2按键
local d_lub3_MACHSET_FL_LUBERR_AUTOMODE = 0x3032000c --润滑异常后自动润滑3按键
local d_lub2_MACHSET_FL_LUBERR_AUTOKEY = 0x3022000d --自动使用润滑2按键
local d_lub3_MACHSET_FL_LUBERR_AUTOKEY = 0x3032000d --自动使用润滑3按键

function SetLubLimit()
	if(VarAdrToUI16(machineset_lubcountmax)>0) then

		if(VarAdrToInt(machineset_lubcountmax) < VarAdrToInt(p_PP_MACHSET_wLUBR1_CNT_MIN)) then
			VarAdrSetInt(p_PP_MACHSET_wLUBR1_CNT_MIN,VarAdrToInt(machineset_lubcountmax))
			VarAdrSaveInt(p_PP_MACHSET_wLUBR1_CNT_MIN)
		end

		if(VarAdrToInt(machineset_lubcountmax) < VarAdrToInt(p_PP_MACHSET_wLUBR2_CNT_MIN)) then
			VarAdrSetInt(p_PP_MACHSET_wLUBR2_CNT_MIN,VarAdrToInt(machineset_lubcountmax))
			VarAdrSaveInt(p_PP_MACHSET_wLUBR2_CNT_MIN)
		end

		if(VarAdrToInt(machineset_lubcountmax) < VarAdrToInt(p_PP_MACHSET_wLUBR3_CNT_MIN)) then
			VarAdrSetInt(p_PP_MACHSET_wLUBR3_CNT_MIN,VarAdrToInt(machineset_lubcountmax))
			VarAdrSaveInt(p_PP_MACHSET_wLUBR3_CNT_MIN)
		end

    else
		if(VarAdrToInt(p_PP_MACHSET_wLUBCNTMAX) < VarAdrToInt(p_PP_MACHSET_wLUBR1_CNT_MIN)) then
			VarAdrSetInt(p_PP_MACHSET_wLUBR1_CNT_MIN,VarAdrToInt(p_PP_MACHSET_wLUBCNTMAX))
			VarAdrSaveInt(p_PP_MACHSET_wLUBR1_CNT_MIN)
		end

		if(VarAdrToInt(p_PP_MACHSET_wLUBCNTMAX) < VarAdrToInt(p_PP_MACHSET_wLUBR2_CNT_MIN)) then
			VarAdrSetInt(p_PP_MACHSET_wLUBR2_CNT_MIN,VarAdrToInt(p_PP_MACHSET_wLUBCNTMAX))
			VarAdrSaveInt(p_PP_MACHSET_wLUBR2_CNT_MIN)
		end

		if(VarAdrToInt(p_PP_MACHSET_wLUBCNTMAX) < VarAdrToInt(p_PP_MACHSET_wLUBR3_CNT_MIN)) then
			VarAdrSetInt(p_PP_MACHSET_wLUBR3_CNT_MIN,VarAdrToInt(p_PP_MACHSET_wLUBCNTMAX))
			VarAdrSaveInt(p_PP_MACHSET_wLUBR3_CNT_MIN)
		end

	end

end

--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()

--20241023 chj 润滑参数权限为超级权限的时候，润滑限制才可见
if(GetCurrentUserId() > 3) then
	WGTSetVisible("groupbox1",1)

    WGTSetVisible("label30",1)
    WGTSetVisible("label204",1)
    WGTSetVisible("label205",1)
    WGTSetVisible("label206",1)
    WGTSetVisible("label32",1)
    WGTSetVisible("label33",1)
    WGTSetVisible("label34",1)

    	--20211206 dyl
	if(VarAdrToUI16(machineset_lubcountmax)>0) then
		WGTSetVisible("editset",0)
		WGTSetVisible("editmach",1)
	else
		WGTSetVisible("editset",1)
		WGTSetVisible("editmach",0)
	end

    WGTSetVisible("edit10",1)
    WGTSetVisible("edit14",1)
    WGTSetVisible("edit15",1)
    WGTSetVisible("edit11",1)
    WGTSetVisible("edit12",1)
    WGTSetVisible("edit13",1)

	WGTSetVisible("label61",1)
    WGTSetVisible("label62",1)
    WGTSetVisible("label63",1)
else

	WGTSetVisible("groupbox1",0)

    WGTSetVisible("label30",0)
    WGTSetVisible("label204",0)
    WGTSetVisible("label205",0)
    WGTSetVisible("label206",0)
    WGTSetVisible("label32",0)
    WGTSetVisible("label33",0)
    WGTSetVisible("label34",0)


	WGTSetVisible("editset",0)
	WGTSetVisible("editmach",0)


    WGTSetVisible("edit10",0)
    WGTSetVisible("edit14",0)
    WGTSetVisible("edit15",0)
    WGTSetVisible("edit11",0)
    WGTSetVisible("edit12",0)
    WGTSetVisible("edit13",0)

	WGTSetVisible("label61",0)
    WGTSetVisible("label62",0)
    WGTSetVisible("label63",0)

end

    SetLubLimit()

if(band(VarAdrToInt(SYS_FL_MACH_CODE5),0x2000)~=0) then

	WGTSetEnable("labelrelub",1)
	WGTSetEnable("dprelub",1)

else
	WGTSetEnable("labelrelub",0)
	WGTSetEnable("dprelub",0)
end

if(band(VarAdrToInt(SYS_FL_MACH_CODE5),0x40)~=0) then
	WGTSetEnable("labellubmod",1)
	WGTSetEnable("dprelubmod",1)

else
	WGTSetEnable("labellubmod",0)
	WGTSetEnable("dprelubmod",0)
end

end

--画面隐藏--
function PageHide()

end

--画面刷新--
function PageUpdate()

end

--画面修改--
function PageChange(name,adr,oldvalue,newvalue)

    if(name == "editset") then
		SetLubLimit()
    elseif(name == "dprelub") then
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			CtlAdrInt(d_lub2_MACHSET_FL_LUBERR_AGAIN,newvalue)--2024 fqh 润滑异常立即润滑2,3按键
			CtlAdrInt(d_lub3_MACHSET_FL_LUBERR_AGAIN,newvalue)
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "dprelubmod") then
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			CtlAdrInt(d_lub2_MACHSET_FL_LUBERR_AUTOMODE,newvalue)--2024 fqh 润滑异常后自动润滑2,3按键
			CtlAdrInt(d_lub3_MACHSET_FL_LUBERR_AUTOMODE,newvalue)
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
    elseif(name == "btautolubkey")then
		CtlAdrInt(d_lub2_MACHSET_FL_LUBERR_AUTOKEY,newvalue)
        CtlAdrInt(d_lub3_MACHSET_FL_LUBERR_AUTOKEY,newvalue)
    end


    return 0
end
