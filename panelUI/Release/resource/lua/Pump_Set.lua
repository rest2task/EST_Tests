
--画面创建--
function PageCreate()

end

--画面显示--
function PageShow()
	SetPageLockMode(1)
	
	TabPageSelPage("tabpage1",0)
	FkeyTab("tabpage1", 0x202)
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

    return 0
end
