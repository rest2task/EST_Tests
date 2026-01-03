--金鹰
local SYSTEM_MACH_CUSTOMID = 0x00120018 --20200317客户机器代码
local TMP_JY_SHOW = TMPVAL(100) 		--金鹰数据保护内容是否显示
--金鹰 end

--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()

	if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400) then --金鹰
		VarAdrSetInt(TMP_JY_SHOW,1)
    else
		VarAdrSetInt(TMP_JY_SHOW,0)
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

    return 0
end
