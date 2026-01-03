local SYS_FL_QVR  =  0x00120013 --国标
local SYS_FL_MACH_CODE0 = 0x00120000 --code0
local OPMODE_INDEX       =  0x33160013 --Operation mode index
local MODE_MANUAL        = 0x00
local VW_MSG_MANUAL =50
function PageCreate()

end

function PageShow()
	SetPageLockMode(1)

	if(band(VarAdrToInt(SYS_FL_MACH_CODE0),0x0800) ~= 0) then
		DropSetCurIndex("stbutton9", 0)
    else
		DropSetCurIndex("stbutton9", 1)
    end

end

function PageUpdate()

end

function PageChange(name,adr,oldvalue,newvalue)


	if(name == "stbutton1")  then --座进检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0001))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0001)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton2") then --托模进检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0002))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0002)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton3") then --开模检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0004))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0004)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton4") then --中子检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0008))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0008)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton5") then --储料检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0010))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0010)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton6") then --射出检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0020))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0020)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton7") then --储料检测射出防护罩
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0040))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0040)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton8") then --座进检测射出防护罩
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0080))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0080)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton9") then --调模进检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			if(newvalue==1) then
				VarAdrSetInt(SYS_FL_MACH_CODE0,band(VarAdrToInt(SYS_FL_MACH_CODE0),bnot(0x0800)))
			else
				VarAdrSetInt(SYS_FL_MACH_CODE0,bor(VarAdrToInt(SYS_FL_MACH_CODE0),0x0800))
			end
			CMD_CodeSendSave()
		else
			DropSetCurIndex("stbutton9", oldvalue)
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton10") then --调模退检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0200))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0200)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "stbutton11") then --托模退检测安全门
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL) then
			CMD_CodeSendSave()
		else
			if(oldvalue==1) then
				VarAdrSetInt(SYS_FL_QVR,bor(VarAdrToInt(SYS_FL_QVR),0x0400))
			else
				VarAdrSetInt(SYS_FL_QVR,band(VarAdrToInt(SYS_FL_QVR),bnot(0x0400)))
			end
			ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	end

	return 0

end

function PageHide()
	SetPageLockMode(0)
end
