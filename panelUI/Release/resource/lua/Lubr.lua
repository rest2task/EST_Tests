local OPMODE_INDEX = 0x33160013 --Operation mode index
local MODE_MANUAL =        0x00 --手动模式
local VW_MSG_MANUAL = 50       --请在手动模式操作此功能!

local LUB1_MACH_FL_LUBMODE = 0x30120000 --润滑模式1
local LUB2_MACH_FL_LUBMODE = 0x30220000 --润滑模式2
local LUB3_MACH_FL_LUBMODE = 0x30320000 --润滑模式3
local wUse15DaysLub1 = 0x331200ba --润滑1间歇时间
local wUse15DaysLub2 = 0x331200bb --润滑2间歇时间
local wUse15DaysLub3 = 0x331200bc --润滑3间歇时间
local COMM_SUBCMD_NEEDLUB = 0x0046 --强制润滑

local SYSTEM_MACH_CUSTOMID = 0x00120018 --20200317客户机器代码
--金鹰
local SYS_CLAMP_PROTECT_FUNC= 0x331200DE	--开关模压力上限密码保护功能是否使用
local SYS_SPC_KEY_USE_JY= 0x331200E8		--金鹰快速机
local TMP_LUBRMODEPSW_VISIBLE = TMPVAL(99) 	--润滑模式密码可见
local TMP_LUBRMODEPSW = TMPVAL(100) 		--润滑模式输入密码
local TMP_LUBRMODEPSW_CAL = TMPVAL(101) 	--润滑模式金鹰特殊公式计算出来的密码
--金鹰end

--威力士
local LubCnt=			0x331200e5 		--威力士特殊修改，显示几组润滑
local TMP_LUB2_SHOW=	TMPVAL(102)		--第二组润滑显示
local TMP_LUB3_SHOW=	TMPVAL(103)		--第三组润滑显示
--威力士end

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

function LubrPageReadOnly(status)
	if(status == true) then
		WGTSetEnable("stbutton1", 0)
		WGTSetEnable("stbutton2", 0)
        WGTSetEnable("stbutton3", 0)

		ReadOnly("edit12", true)
		ReadOnly("edit13", true)

		ReadOnly("edit26", true)
		ReadOnly("edit27", true)

		ReadOnly("edit100", true)
		ReadOnly("edit101", true)
	else
		WGTSetEnable("stbutton1", 1)
		WGTSetEnable("stbutton2", 1)
        WGTSetEnable("stbutton3", 1)

		if(VarAdrToInt(LUB1_MACH_FL_LUBMODE) == 0) then
			ReadOnly("edit12", true)
			ReadOnly("edit13", true)
		else
			ReadOnly("edit12", false)
			ReadOnly("edit13", false)
		end
		if(VarAdrToInt(LUB2_MACH_FL_LUBMODE) == 0) then
			ReadOnly("edit26", true)
			ReadOnly("edit27", true)
		else
			ReadOnly("edit26", false)
			ReadOnly("edit27", false)
		end
		if(VarAdrToInt(LUB3_MACH_FL_LUBMODE) == 0) then
			ReadOnly("edit100", true)
			ReadOnly("edit101", true)
		else
			ReadOnly("edit100", false)
			ReadOnly("edit101", false)
		end

	end

	ReadOnly("edit9", status)
	ReadOnly("edit10", status)
	ReadOnly("edit11", status)
	ReadOnly("edit14", status)

	ReadOnly("edit23", status)
	ReadOnly("edit24", status)
	ReadOnly("edit25", status)
	ReadOnly("edit28", status)

	ReadOnly("edit97", status)
	ReadOnly("edit98", status)
	ReadOnly("edit99", status)
	ReadOnly("edit102", status)
end


--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()
	SetPageLockMode(1)

    --威力士根据润滑组数进行显示隐藏
	if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x7A00) then
		if(VarAdrToInt(LubCnt)==1)then
			VarAdrSetInt(TMP_LUB2_SHOW,1)
			VarAdrSetInt(TMP_LUB3_SHOW,0)
		elseif(VarAdrToInt(LubCnt)==2)then
			VarAdrSetInt(TMP_LUB2_SHOW,1)
			VarAdrSetInt(TMP_LUB3_SHOW,1)
		else
			VarAdrSetInt(TMP_LUB2_SHOW,0)
			VarAdrSetInt(TMP_LUB3_SHOW,0)
		end
    else
		VarAdrSetInt(TMP_LUB2_SHOW,1)
		VarAdrSetInt(TMP_LUB3_SHOW,1)
    end

	G10LubGuideSwtich() --20211206 dyl
	if(VarAdrToInt(LUB1_MACH_FL_LUBMODE) == 3) then --20210507
		WGTSetVisible("edit14", 1)
        WGTSetVisible("edit15", 1)
        WGTSetVisible("label74", 1)
        WGTSetVisible("label75", 1)
        WGTSetVisible("label76", 1)
        WGTSetVisible("label77", 1)

        ReadOnly("edit12", false)
        ReadOnly("edit13", false)
    else
		WGTSetVisible("edit14", 0)
        WGTSetVisible("edit15", 0)
        WGTSetVisible("label74", 0)
        WGTSetVisible("label75", 0)
        WGTSetVisible("label76", 0)
        WGTSetVisible("label77", 0)
        SetLubHour(1, 0)
        SetLeftLubHour(1, VarAdrToInt(wUse15DaysLub1)*60-GetLubHour(1))

		if(VarAdrToInt(LUB1_MACH_FL_LUBMODE) == 0) then
			ReadOnly("edit12", true)
			ReadOnly("edit13", true)
		else
			ReadOnly("edit12", false)
			ReadOnly("edit13", false)
		end
    end

    if(VarAdrToInt(LUB2_MACH_FL_LUBMODE) == 3 and VarAdrToInt(TMP_LUB2_SHOW) == 1) then --20210507
		WGTSetVisible("edit28", 1)
        WGTSetVisible("edit29", 1)
        WGTSetVisible("label78", 1)
        WGTSetVisible("label79", 1)
        WGTSetVisible("label80", 1)
        WGTSetVisible("label81", 1)

        ReadOnly("edit26", false)
        ReadOnly("edit27", false)
    else
		WGTSetVisible("edit28", 0)
        WGTSetVisible("edit29", 0)
        WGTSetVisible("label78", 0)
        WGTSetVisible("label79", 0)
        WGTSetVisible("label80", 0)
        WGTSetVisible("label81", 0)
        SetLubHour(2, 0)
        SetLeftLubHour(2, VarAdrToInt(wUse15DaysLub2)*60-GetLubHour(2))

		if(VarAdrToInt(LUB2_MACH_FL_LUBMODE) == 0) then
			ReadOnly("edit26", true)
			ReadOnly("edit27", true)
		else
			ReadOnly("edit26", false)
			ReadOnly("edit27", false)
		end
    end


    if(VarAdrToInt(LUB3_MACH_FL_LUBMODE) == 3 and VarAdrToInt(TMP_LUB3_SHOW) == 1) then --20210507
		WGTSetVisible("edit102", 1)
        WGTSetVisible("edit103", 1)
        WGTSetVisible("label82", 1)
        WGTSetVisible("label83", 1)
        WGTSetVisible("label84", 1)
        WGTSetVisible("label85", 1)

        ReadOnly("edit100", false)
        ReadOnly("edit101", false)
    else
		WGTSetVisible("edit102", 0)
        WGTSetVisible("edit103", 0)
        WGTSetVisible("label82", 0)
        WGTSetVisible("label83", 0)
        WGTSetVisible("label84", 0)
        WGTSetVisible("label85", 0)
        SetLubHour(3, 0)
        SetLeftLubHour(3, VarAdrToInt(wUse15DaysLub3)*60-GetLubHour(3))

		if(VarAdrToInt(LUB3_MACH_FL_LUBMODE) == 0) then
			ReadOnly("edit100", true)
			ReadOnly("edit101", true)
		else
			ReadOnly("edit100", false)
			ReadOnly("edit101", false)
		end
    end

    --密码模式下变灰
    if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400) then --金鹰
    	if(VarAdrToInt(SYS_CLAMP_PROTECT_FUNC)==1) then --密码保护功能使用
			VarAdrSetInt(TMP_LUBRMODEPSW_VISIBLE,1)
			VarAdrSetInt(TMP_LUBRMODEPSW,1234)
			LubrPageReadOnly(true)
		else
			VarAdrSetInt(TMP_LUBRMODEPSW_VISIBLE,0)
			LubrPageReadOnly(false)
		end

    elseif(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x8700) then--海星
			VarAdrSetInt(TMP_LUBRMODEPSW_VISIBLE,1)
			VarAdrSetInt(TMP_LUBRMODEPSW,1234)
			LubrPageReadOnly(true)

	elseif(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x8F00) then --海航
			VarAdrSetInt(TMP_LUBRMODEPSW_VISIBLE,0) --从其他厂商切换到海航时，进行初始化
			LubrPageReadOnly(false)

		if(GetCurrentUserId() >= 4) then --海航润滑模式默认值为1，只有最高用户等级才可以修改润滑模式，其他等级不可修改
			ReadOnly("edit9", false)
			ReadOnly("edit23", false)
			ReadOnly("edit97", false)
		else
			ReadOnly("edit9", true)
			ReadOnly("edit23", true)
			ReadOnly("edit97", true)
		end

    else
			VarAdrSetInt(TMP_LUBRMODEPSW_VISIBLE,0)
			LubrPageReadOnly(false)

    end

    --金鹰快速机模式下，润滑二按键独立隐藏，显示润滑按键功能选择
    if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400) then --金鹰

		if(VarAdrToInt(SYS_SPC_KEY_USE_JY)==1) then --金鹰快速机
			WGTSetVisible("label73", 0)
			WGTSetVisible("stbutton2", 0)
			WGTSetVisible("label89", 1)
			WGTSetVisible("stbutton3", 1)
		else
			WGTSetVisible("label73", 1)
			WGTSetVisible("stbutton2", 1)
			WGTSetVisible("label89", 0)
			WGTSetVisible("stbutton3", 0)
		end

    else
		WGTSetVisible("label73", 1)
        WGTSetVisible("stbutton2", 1)
		WGTSetVisible("label89", 0)
        WGTSetVisible("stbutton3", 0)
    end



end

--画面隐藏--
function PageHide()
	SetPageLockMode(0)
end

--画面刷新--
function PageUpdate()

	if(VarAdrToInt(LUB1_MACH_FL_LUBMODE) == 3) then
		EditSetNum("edit15", GetLeftLubHour(1)/6.0, 1)
    end

    if(VarAdrToInt(LUB2_MACH_FL_LUBMODE) == 3) then
		EditSetNum("edit29", GetLeftLubHour(2)/6.0, 1)
    end

    if(VarAdrToInt(LUB3_MACH_FL_LUBMODE) == 3) then
		EditSetNum("edit103", GetLeftLubHour(3)/6.0, 1)
    end

end

--画面修改--
function PageChange(name,adr,oldvalue,newvalue)

	if(name == "edit9") then  --润滑1模式
		if(VarAdrToInt(OPMODE_INDEX)~=MODE_MANUAL) then
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
        else
            if(newvalue == 3) then --20210507
				WGTSetVisible("edit14", 1)
				WGTSetVisible("edit15", 1)
				WGTSetVisible("label74", 1)
				WGTSetVisible("label75", 1)
				WGTSetVisible("label76", 1)
				WGTSetVisible("label77", 1)

				ReadOnly("edit12", false)
				ReadOnly("edit13", false)

                SaveLubStartTm(1)
			else
				WGTSetVisible("edit14", 0)
				WGTSetVisible("edit15", 0)
				WGTSetVisible("label74", 0)
				WGTSetVisible("label75", 0)
				WGTSetVisible("label76", 0)
				WGTSetVisible("label77", 0)
				SetLubHour(1, 0)
				SetLeftLubHour(1, VarAdrToInt(wUse15DaysLub1)*60-GetLubHour(1))

				if(newvalue == 0) then
					ReadOnly("edit12", true)
					ReadOnly("edit13", true)
				else
					ReadOnly("edit12", false)
					ReadOnly("edit13", false)
				end
			end
		end
	end

	if(name == "edit23") then  --润滑2模式
		if(VarAdrToInt(OPMODE_INDEX)~=MODE_MANUAL) then
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		else
			if(newvalue == 3 and VarAdrToInt(TMP_LUB2_SHOW) == 1) then --20210507
				WGTSetVisible("edit28", 1)
				WGTSetVisible("edit29", 1)
				WGTSetVisible("label78", 1)
				WGTSetVisible("label79", 1)
				WGTSetVisible("label80", 1)
				WGTSetVisible("label81", 1)

				ReadOnly("edit26", false)
				ReadOnly("edit27", false)

                SaveLubStartTm(2)
			else
				WGTSetVisible("edit28", 0)
				WGTSetVisible("edit29", 0)
				WGTSetVisible("label78", 0)
				WGTSetVisible("label79", 0)
				WGTSetVisible("label80", 0)
				WGTSetVisible("label81", 0)
				SetLubHour(2, 0)
				SetLeftLubHour(2, VarAdrToInt(wUse15DaysLub2)*60-GetLubHour(2))

				if(newvalue == 0) then
					ReadOnly("edit26", true)
					ReadOnly("edit27", true)
				else
					ReadOnly("edit26", false)
					ReadOnly("edit27", false)
				end
			end
		end
	end

	if(name == "edit97") then  --润滑3模式
		if(VarAdrToInt(OPMODE_INDEX)~=MODE_MANUAL) then
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		else
			if(newvalue == 3 and VarAdrToInt(TMP_LUB3_SHOW) == 1) then --20210507
				WGTSetVisible("edit102", 1)
				WGTSetVisible("edit103", 1)
				WGTSetVisible("label82", 1)
				WGTSetVisible("label83", 1)
				WGTSetVisible("label84", 1)
				WGTSetVisible("label85", 1)

				ReadOnly("edit100", false)
				ReadOnly("edit101", false)

                SaveLubStartTm(3)
			else
				WGTSetVisible("edit102", 0)
				WGTSetVisible("edit103", 0)
				WGTSetVisible("label82", 0)
				WGTSetVisible("label83", 0)
				WGTSetVisible("label84", 0)
				WGTSetVisible("label85", 0)
				SetLubHour(3, 0)
				SetLeftLubHour(3, VarAdrToInt(wUse15DaysLub3)*60-GetLubHour(3))

				if(newvalue == 0) then
					ReadOnly("edit100", true)
					ReadOnly("edit101", true)
				else
					ReadOnly("edit100", false)
					ReadOnly("edit101", false)
				end
			end
		end
	end

    if(name == "edit14") then
		if(GetLubHour(1) < newvalue*60) then
			SetLeftLubHour(1, newvalue*60 - GetLubHour(1))
        else
			ProtSysCtrl(COMM_SUBCMD_NEEDLUB, 1, 1)
			SaveLubStartTm(1)
        end
    end

    if(name == "edit28") then
		if(GetLubHour(2) < newvalue*60) then
			SetLeftLubHour(2, newvalue*60 - GetLubHour(2))
        else
			ProtSysCtrl(COMM_SUBCMD_NEEDLUB, 1, 2)
			SaveLubStartTm(2)
        end
    end

    if(name == "edit102") then
		if(GetLubHour(3) < newvalue*60) then
			SetLeftLubHour(3, newvalue*60 - GetLubHour(3))
        else
			ProtSysCtrl(COMM_SUBCMD_NEEDLUB, 1, 3)
			SaveLubStartTm(3)
        end
    end

	if(name == "editpwd") then	--输入密码才能修改
		if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400) then --金鹰

			CalClampPrsMaxProtectPSW(TMP_LUBRMODEPSW_CAL)
			if(VarAdrToInt(TMP_LUBRMODEPSW) == VarAdrToInt(TMP_LUBRMODEPSW_CAL) ) then
				LubrPageReadOnly(false)
			end

        elseif(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x8700) then--海星

			if(VarAdrToInt(TMP_LUBRMODEPSW) == 8836 ) then
				LubrPageReadOnly(false)
			end

        else

        end

	end

	return 0

end
