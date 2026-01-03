
local SYS_TM_CYCLE_WHOLE  =0x0016001F --全程计时，包括再循环时间，开关安全门
local PROD_SHOTCNTPERHOUR =0x3316000e --每小时开模数
local PROD_REMAINTIMEH    =0x3316000f --剩余时间小时
local PROD_REMAINTIMEM    =0x33160010 --剩余时间分钟
local	PROD_TARGETCNT		=0x33120019
local	PROD_MOLDREACHCHK	=0x2013000A
local	PROD_OPENCNTSET1	=0x2013000B
local	PROD_OPENCNTSET2	=0x2013000C

local	PROD_PERCNT			=0x33120017
local	CLAMP_MOLD_FL_AUTO_PRU = 0x2013003B --自动时不良品监控
local	CLAMP_MOLD_FL_MAUL_PRU = 0x2013003C --手动时不良品监控

local totalOpenCnt = 0x3316000c
local totalOpenPrd = 0x3316000d

local PROD_BOOTTIMEPWD = 0x33150014 --20211112 dyl 开机总时间、运行总计时统计
local PROD_PRODTIMEPWD = 0x33150016

local function ProdManageUpdate()
	local cyc_time
	local prodcnt_set
	local left_hour
	local left_min
    local per_hour_prod

	cyc_time = VarAdrToInt(SYS_TM_CYCLE_WHOLE) --调整生产画面的每小时开模数算法，上一版本以制品时间来计算的，这一版本以全程计时（包含开关安全门的时间）来计算

	if(cyc_time ~=0) then
		per_hour_prod = 3600/cyc_time*100
        
		VarAdrSetInt(PROD_SHOTCNTPERHOUR,3600/cyc_time*1000)
		prodcnt_set = bor(lshift(VarAdrToUI16(PROD_OPENCNTSET1),16),VarAdrToUI16(PROD_OPENCNTSET2))
		if(prodcnt_set > 0) then
			local prod_left = 0
			if((VarAdrToUI16(PROD_MOLDREACHCHK) ==1) and (VarAdrToInt(PROD_PERCNT)~=0)) then --20241230 chj 增加非0判断
				prod_left = prodcnt_set -VarAdrToInt(totalOpenPrd)/VarAdrToInt(PROD_PERCNT)
			else
				prod_left = prodcnt_set -VarAdrToInt(totalOpenCnt)
			end

			if((prod_left > 0) and (per_hour_prod~=0)) then
				left_hour=prod_left/per_hour_prod
				left_min=(prod_left%per_hour_prod)/per_hour_prod*60+0.9
				VarAdrSetInt(PROD_REMAINTIMEH,left_hour)
				VarAdrSetInt(PROD_REMAINTIMEM,left_min)
			else
				VarAdrSetInt(PROD_REMAINTIMEH,0)
				VarAdrSetInt(PROD_REMAINTIMEM,0)
			end
		else
			VarAdrSetInt(PROD_REMAINTIMEH,0)
			VarAdrSetInt(PROD_REMAINTIMEM,0)
		end
	else
		VarAdrSetInt(PROD_SHOTCNTPERHOUR,0)
		VarAdrSetInt(PROD_REMAINTIMEH,0)
		VarAdrSetInt(PROD_REMAINTIMEM,0)
	end

	ProdSetRealTimeCount()--20211112 dyl 开机总时间、运行总计时统计
end

function PageCreate()

end

--页面显示--
function PageShow()
	SetPageLockMode(1)
	
	G10ProdManageGuideSwitch() --20201104

	local cnt = bor(lshift(VarAdrToInt(PROD_OPENCNTSET1),16) , VarAdrToInt(PROD_OPENCNTSET2))
	if(VarAdrToInt(PROD_MOLDREACHCHK)==1) then
		cnt = cnt*VarAdrToInt(PROD_PERCNT)
	end
	VarAdrSetInt(PROD_TARGETCNT,cnt)

	--20211231 dyl
	if(GetCurrentUserId() > 3)then
		WGTSetVisible("label205",1)
		WGTSetVisible("edit113",1)
	else
		WGTSetVisible("label205",0)
		WGTSetVisible("edit113",0)
	end
	--20211112 dyl 开机总时间、运行总计时统计
	VarAdrSetInt(PROD_BOOTTIMEPWD,0)
	VarAdrSetInt(PROD_PRODTIMEPWD,0)
	WGTSetEnable("btClear1",0)
	WGTSetEnable("btClear2",0)
end

--页面刷新--
function PageUpdate()

	ProdManageUpdate()
end

--页面修改--
function PageChange(name,adr,oldvalue,newvalue)
	if(adr == PROD_PERCNT) then
		ProdSetPerMold()
	elseif(adr == PROD_MOLDREACHCHK) then
		ProdChgMode()
	elseif(adr == PROD_TARGETCNT) then
		ProdSetGoalCnt()
	end

	if(name == "button8") then
		ProdClrCnt()
	elseif(name == "button9") then
		ProdClrPkg()
	end
    
    if(name == "stbt62") then
		if(newvalue == 0) then
			if(VarAdrToInt(CLAMP_MOLD_FL_AUTO_PRU) ~= 0) then
				CtlAdrInt(CLAMP_MOLD_FL_AUTO_PRU, 0)
            end
            
            if(VarAdrToInt(CLAMP_MOLD_FL_MAUL_PRU) ~= 0) then
				CtlAdrInt(CLAMP_MOLD_FL_MAUL_PRU, 0)
            end
        end
    end
	if(name == "editpwd1") then --20211112 dyl 开机总时间、运行总计时统计
		if(newvalue == 9595) then
			WGTSetEnable("btClear1",1)
		else
			WGTSetEnable("btClear1",0)
		end
	end
	if(name == "editpwd2") then --20211112 dyl 开机总时间、运行总计时统计
		if(newvalue == 9595) then
			WGTSetEnable("btClear2",1)
		else
			WGTSetEnable("btClear2",0)
		end
	end
	if(name == "btClear1") then --20211112 dyl 开机总时间、运行总计时统计
		ProdClrBootTime()
	end
	if(name == "btClear2") then --20211112 dyl 开机总时间、运行总计时统计
		ProdClrProdTime()
	end
	return 0

end

function PageHide()
	SetPageLockMode(0)
	
	--20211112 dyl 开机总时间、运行总计时统计
	VarAdrSetInt(PROD_BOOTTIMEPWD,0)
	VarAdrSetInt(PROD_PRODTIMEPWD,0)
end

