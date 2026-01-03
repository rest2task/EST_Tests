local CORE_SELECT = 0x33120016 --中子选择
local CORE_SELECT_SHOW = 0x331501F4 --中子选择显示TMPSTR(0)
local PART_OFFSET = 0x00100000   --部件偏移 
local COREA_MOLD_FL_CORE_CTRL = 0x22130001 --中子功能选择 (0=不用 1=中子 2=绞牙)
local COREA_MOLD_FL_CORE_TRAVEL = 0x22130002 --中子进控制方式
local COREA_MOLD_FL_CORE_OUTTRAVEL = 0x22130003 --中子退控制方式
local COREA_MOLD_FL_ADPOSI_CORE_FWD = 0x22130006 --中子进动作位置
local COREA_MOLD_FL_ADPOSI_CORE_BWD = 0x22130007 --中子退动作位置 
local COREA_ACT_DAPRES_COREIN = 0x22140000 --中子进压力
local COREA_ACT_DAPRES_COREOUT = 0x22140001 --中子退压力
local COREA_ACT_DAFLOW_COREIN = 0x22140002 --中子进流量
local COREA_ACT_DAFLOW_COREOUT = 0x22140003 --中子退流量
local COREA_ACT_ADPOSI_COREINACT = 0x22140008 --中子进动作位置
local COREA_ACT_ADPOSI_COREOUTACT = 0x22140009 --中子退动作位置
local COREA_ACT_TM_COREIN = 0x22140004 --中子进计时
local COREA_ACT_TM_COREOUT = 0x22140005 --中子退计时
local COREA_ACT_CN_SCREWIN = 0x22140006 --绞牙进计数
local COREA_ACT_CN_SCREWOUT = 0x22140007 --绞牙退计数
       
--local COREA_MOLD_FL_COREPROTECT = 0x22130000 --中子保护功能 
local COREA_MOLD_FL_INJ_FWDOFF = 0x22130005 --射出时中子进保持

local COREA_MACH_DAPRES_CORESETUP = 0x22120004 --中子慢速调试压力
local COREA_MACH_DAFLOW_CORESETUP = 0x22120005 --中子慢速调试流量
       
local COREA_MACH_TM_COREINCTDLY = 0x22120006 --中子进延迟开
local COREA_MACH_TM_COREOUTCTDLY = 0x22120007 --中子退延迟开
local COREA_MACH_TMDLY_VLV_ONCOREIN = 0x2212000e --中子进阀开延迟
local COREA_MACH_TMDLY_VLV_OFFCOREIN = 0x2212000f --中子进阀关延迟
local COREA_MACH_TMDLY_VLV_ONCOREOUT = 0x22120010 --中子退阀开延迟
local COREA_MACH_TMDLY_VLV_OFFCOREOUT = 0x22120011 --中子退阀关延迟
local COREA_MACH_TM_COREENDDLYIN = 0x22120012 --中子进动作结束延迟计时
local COREA_MACH_TM_COREENDDLYOUT = 0x22120013 --中子退动作结束延迟计时
local COREA_MOLD_TM_COREINHOLDTM  =  0x2213000C   --中子进保持计时
local COREA_MOLD_TM_COREOUTHOLDTM =  0x2213000D   --中子退保持计时
        
local COREA_MACH_DAPRES_CORERAMP = 0x22120000 --中子启动压力斜率
local COREA_MACH_DAFLOW_CORERAMP = 0x22120001 --中子启动流量斜率
local COREA_MACH_DAPRES_COREOFFRAMP = 0x22120002 --中子关闭压力斜率
local COREA_MACH_DAFLOW_COREOFFRAMP = 0x22120003 --中子关闭流量斜率 

local SYSTEM_MACH_CODE50 = 0x0012000a --机器配置CODE50
local COREA_MOLD_PRI_FWD = 0x22130008  --中子A进优先级
local COREA_MOLD_PRI_BWD = 0x22130009  --中子A退优先级

--20200302
local COREA_IN_BACKCOLOR =  0x331402c2 --中子A进背景色
local COREA_OUT_BACKCOLOR = 0x331402c3 --中子A退背景色

--20211221 dyl
local COREB_ACT_ADPOSI_COREINACT = 0x22240008 --中子B进动作位置
local COREB_ACT_ADPOSI_COREOUTACT = 0x22240009 --中子B退动作位置
local COREC_ACT_ADPOSI_COREINACT = 0x22340008 --中子C进动作位置
local COREC_ACT_ADPOSI_COREOUTACT = 0x22340009 --中子C退动作位置
local CORED_ACT_ADPOSI_COREINACT = 0x22440008 --中子D进动作位置
local CORED_ACT_ADPOSI_COREOUTACT = 0x22440009 --中子D退动作位置
local COREE_ACT_ADPOSI_COREINACT = 0x22540008 --中子E进动作位置
local COREE_ACT_ADPOSI_COREOUTACT = 0x22540009 --中子E退动作位置
local COREF_ACT_ADPOSI_COREINACT = 0x22640008 --中子F进动作位置
local COREF_ACT_ADPOSI_COREOUTACT = 0x22640009 --中子F退动作位置
local COREA_MACH_ADPOS_EFFCOREINPLUS = 0x22120014 --中子A进位置有效区(+)
local COREA_MACH_ADPOS_EFFCOREOUTPLUS = 0x22120016 --中子A退位置有效区(+)
local COREB_MACH_ADPOS_EFFCOREINPLUS = 0x22220014 --中子B进位置有效区(+)
local COREB_MACH_ADPOS_EFFCOREOUTPLUS = 0x22220016 --中子B退位置有效区(+)
local COREC_MACH_ADPOS_EFFCOREINPLUS = 0x22320014 --中子C进位置有效区(+)
local COREC_MACH_ADPOS_EFFCOREOUTPLUS = 0x22320016 --中子C退位置有效区(+)
local CORED_MACH_ADPOS_EFFCOREINPLUS = 0x22420014 --中子D进位置有效区(+)
local CORED_MACH_ADPOS_EFFCOREOUTPLUS = 0x22420016 --中子D退位置有效区(+)
local COREE_MACH_ADPOS_EFFCOREINPLUS = 0x22520014 --中子E进位置有效区(+)
local COREE_MACH_ADPOS_EFFCOREOUTPLUS = 0x22520016 --中子E退位置有效区(+)
local COREF_MACH_ADPOS_EFFCOREINPLUS = 0x22620014 --中子F进位置有效区(+)
local COREF_MACH_ADPOS_EFFCOREOUTPLUS = 0x22620016 --中子F退位置有效区(+)
local CLAMP_ACT_ADPOSI_OPENEND = 0x2014001C --开模五段终止位置


local OPMODE_INDEX = 0x33160013 --Operation mode index
local MODE_MANUAL = 0x00 --手动模式
local VW_MSG_MANUAL = 50 --请在手动模式操作此功能!

--金鹰
local SYS_SPC_KEY_USE_JY= 0x331200E8		--金鹰快速机
local SYSTEM_MACH_CUSTOMID = 0x00120018 --20200317客户机器代码

function SetCoreParam()
	local coreselect = VarAdrToInt(CORE_SELECT)
    local offset = PART_OFFSET*coreselect*2
    local offset2 = offset + PART_OFFSET
    WGTSetAttrByName("cb22","value",COREA_MOLD_FL_CORE_CTRL+offset)
    WGTSetAttrByName("cb31","value",COREA_MOLD_FL_CORE_TRAVEL+offset)
    WGTSetAttrByName("cb32","value",COREA_MOLD_FL_CORE_OUTTRAVEL+offset)
    WGTSetAttrByName("cb31_2","value",COREA_MOLD_FL_CORE_TRAVEL+offset)
    WGTSetAttrByName("cb32_2","value",COREA_MOLD_FL_CORE_OUTTRAVEL+offset)
    WGTSetAttrByName("cbposin1","value",COREA_MOLD_FL_ADPOSI_CORE_FWD+offset)
    WGTSetAttrByName("cbposout1","value",COREA_MOLD_FL_ADPOSI_CORE_BWD+offset)
    WGTSetAttrByName("ed41","text",COREA_ACT_DAPRES_COREIN+offset)
    WGTSetAttrByName("ed42","text",COREA_ACT_DAPRES_COREOUT+offset)
    WGTSetAttrByName("ed51","text",COREA_ACT_DAFLOW_COREIN+offset)
    WGTSetAttrByName("ed52","text",COREA_ACT_DAFLOW_COREOUT+offset)
    WGTSetAttrByName("ed61","text",COREA_ACT_ADPOSI_COREINACT+offset)
    WGTSetAttrByName("ed62","text",COREA_ACT_ADPOSI_COREOUTACT+offset)
    WGTSetAttrByName("ed71","text",COREA_ACT_TM_COREIN+offset)
    WGTSetAttrByName("ed72","text",COREA_ACT_TM_COREOUT+offset)
    WGTSetAttrByName("ed81","text",COREA_ACT_CN_SCREWIN+offset)
    WGTSetAttrByName("ed82","text",COREA_ACT_CN_SCREWOUT+offset)
    --func
    WGTSetAttrByName("tabpage1_Tb1_stbt31","value",COREA_MOLD_FL_INJ_FWDOFF+offset)
    --debug
    WGTSetAttrByName("tabpage1_Tb2_ed11","text",COREA_MACH_DAPRES_CORESETUP+offset)
    WGTSetAttrByName("tabpage1_Tb2_ed21","text",COREA_MACH_DAFLOW_CORESETUP+offset)
    --control1
    WGTSetAttrByName("tabpage1_Tb3_ed11","text",COREA_MACH_TM_COREINCTDLY+offset)
    WGTSetAttrByName("tabpage1_Tb3_ed21","text",COREA_MACH_TM_COREOUTCTDLY+offset)
    
    WGTSetAttrByName("tabpage1_Tb3_edadvon1","text",COREA_MACH_TMDLY_VLV_ONCOREIN+offset)  
    WGTSetAttrByName("tabpage1_Tb3_ed41","text",COREA_MACH_TMDLY_VLV_OFFCOREIN+offset)  
     
    WGTSetAttrByName("tabpage1_Tb3_edreton1","text",COREA_MACH_TMDLY_VLV_ONCOREOUT+offset) 
    WGTSetAttrByName("tabpage1_Tb3_ed61","text",COREA_MACH_TMDLY_VLV_OFFCOREOUT+offset)
    
    WGTSetAttrByName("tabpage1_Tb3_ed51","text",COREA_MACH_TM_COREENDDLYIN+offset)
    WGTSetAttrByName("tabpage1_Tb3_ed71","text",COREA_MACH_TM_COREENDDLYOUT+offset)
    
    --control2
	WGTSetAttrByName("tabpage1_Tb4_ed81","text",COREA_MOLD_TM_COREINHOLDTM+offset)
	WGTSetAttrByName("tabpage1_Tb4_ed91","text",COREA_MOLD_TM_COREOUTHOLDTM+offset)
--    WGTSetAttrByName("tabpage1_Tb3_stbt11","value",COREA_MOLD_FL_COREPROTECT+offset)
    --ramp
    WGTSetAttrByName("tabpage1_Tb5_ed11","text",COREA_MACH_DAPRES_CORERAMP+offset)
    WGTSetAttrByName("tabpage1_Tb5_ed12","text",COREA_MACH_DAFLOW_CORERAMP+offset)
    WGTSetAttrByName("tabpage1_Tb5_ed21","text",COREA_MACH_DAPRES_COREOFFRAMP+offset)
    WGTSetAttrByName("tabpage1_Tb5_ed22","text",COREA_MACH_DAFLOW_COREOFFRAMP+offset)   

    --20200302动作步背景色
    WGTSetAttrByName("label155","backColor",COREA_IN_BACKCOLOR+coreselect*4)
    WGTSetAttrByName("label156","backColor",COREA_OUT_BACKCOLOR+coreselect*4)
    
    WGTSetAttrByName("cb24","value",COREA_MOLD_FL_CORE_CTRL+offset2)
    WGTSetAttrByName("cb33","value",COREA_MOLD_FL_CORE_TRAVEL+offset2)
    WGTSetAttrByName("cb34","value",COREA_MOLD_FL_CORE_OUTTRAVEL+offset2)
    WGTSetAttrByName("cb33_2","value",COREA_MOLD_FL_CORE_TRAVEL+offset2)
    WGTSetAttrByName("cb34_2","value",COREA_MOLD_FL_CORE_OUTTRAVEL+offset2)
    WGTSetAttrByName("cbposin2","value",COREA_MOLD_FL_ADPOSI_CORE_FWD+offset2)
    WGTSetAttrByName("cbposout2","value",COREA_MOLD_FL_ADPOSI_CORE_BWD+offset2)
    WGTSetAttrByName("ed43","text",COREA_ACT_DAPRES_COREIN+offset2)
    WGTSetAttrByName("ed44","text",COREA_ACT_DAPRES_COREOUT+offset2)
    WGTSetAttrByName("ed53","text",COREA_ACT_DAFLOW_COREIN+offset2)
    WGTSetAttrByName("ed54","text",COREA_ACT_DAFLOW_COREOUT+offset2)
    WGTSetAttrByName("ed63","text",COREA_ACT_ADPOSI_COREINACT+offset2)
    WGTSetAttrByName("ed64","text",COREA_ACT_ADPOSI_COREOUTACT+offset2)
    WGTSetAttrByName("ed73","text",COREA_ACT_TM_COREIN+offset2)
    WGTSetAttrByName("ed74","text",COREA_ACT_TM_COREOUT+offset2)
    WGTSetAttrByName("ed83","text",COREA_ACT_CN_SCREWIN+offset2)
    WGTSetAttrByName("ed84","text",COREA_ACT_CN_SCREWOUT+offset2)
    --func
    WGTSetAttrByName("tabpage1_Tb1_stbt32","value",COREA_MOLD_FL_INJ_FWDOFF+offset2)
    --debug
    WGTSetAttrByName("tabpage1_Tb2_ed31","text",COREA_MACH_DAPRES_CORESETUP+offset2)
    WGTSetAttrByName("tabpage1_Tb2_ed41","text",COREA_MACH_DAFLOW_CORESETUP+offset2)
    --control1
    WGTSetAttrByName("tabpage1_Tb3_ed12","text",COREA_MACH_TM_COREINCTDLY+offset2)
    WGTSetAttrByName("tabpage1_Tb3_ed22","text",COREA_MACH_TM_COREOUTCTDLY+offset2)
    
    WGTSetAttrByName("tabpage1_Tb3_edadvon2","text",COREA_MACH_TMDLY_VLV_ONCOREIN+offset2)  
    WGTSetAttrByName("tabpage1_Tb3_ed42","text",COREA_MACH_TMDLY_VLV_OFFCOREIN+offset2)
    
    WGTSetAttrByName("tabpage1_Tb3_edreton2","text",COREA_MACH_TMDLY_VLV_ONCOREOUT+offset2) 
    WGTSetAttrByName("tabpage1_Tb3_ed62","text",COREA_MACH_TMDLY_VLV_OFFCOREOUT+offset2)
    
    WGTSetAttrByName("tabpage1_Tb3_ed52","text",COREA_MACH_TM_COREENDDLYIN+offset2)
    WGTSetAttrByName("tabpage1_Tb3_ed72","text",COREA_MACH_TM_COREENDDLYOUT+offset2)
    
    --control2
	WGTSetAttrByName("tabpage1_Tb4_ed82","text",COREA_MOLD_TM_COREINHOLDTM+offset2)
	WGTSetAttrByName("tabpage1_Tb4_ed92","text",COREA_MOLD_TM_COREOUTHOLDTM+offset2)
--    WGTSetAttrByName("tabpage1_Tb3_stbt12","value",COREA_MOLD_FL_COREPROTECT+offset2)
    --ramp
    WGTSetAttrByName("tabpage1_Tb5_ed31","text",COREA_MACH_DAPRES_CORERAMP+offset2)
    WGTSetAttrByName("tabpage1_Tb5_ed32","text",COREA_MACH_DAFLOW_CORERAMP+offset2)
    WGTSetAttrByName("tabpage1_Tb5_ed41","text",COREA_MACH_DAPRES_COREOFFRAMP+offset2)
    WGTSetAttrByName("tabpage1_Tb5_ed42","text",COREA_MACH_DAFLOW_COREOFFRAMP+offset2) 
    
    WGTSetAttrByName("label196","backColor",COREA_IN_BACKCOLOR+coreselect*4+2)
    WGTSetAttrByName("label197","backColor",COREA_OUT_BACKCOLOR+coreselect*4+2)
end

function SetCoreShow()
	local coreselect = VarAdrToInt(CORE_SELECT)
    if(coreselect == 0) then
		VarAdrSetStr(CORE_SELECT_SHOW,"A B")
        SetAttrValue("image1","fileName","BackCoreA.png")
        SetAttrValue("image2","fileName","BackCoreB.png")
        WGTSetVisible("subpage1", 1)
        WGTSetVisible("tabpage1_Tb1_subpage1", 1)
        WGTSetVisible("tabpage1_Tb2_subpage1", 1)
        WGTSetVisible("tabpage1_Tb3_subpage1", 1)
        WGTSetVisible("tabpage1_Tb4_subpage1", 1)
        WGTSetVisible("tabpage1_Tb5_subpage1", 1)
        WGTSetVisible("subpage2", 0)
        WGTSetVisible("tabpage1_Tb1_subpage2", 0)
        WGTSetVisible("tabpage1_Tb2_subpage2", 0)
        WGTSetVisible("tabpage1_Tb3_subpage2", 0)
        WGTSetVisible("tabpage1_Tb4_subpage2", 0)
        WGTSetVisible("tabpage1_Tb5_subpage2", 0)
        WGTSetVisible("subpage3", 0)
        WGTSetVisible("tabpage1_Tb1_subpage3", 0)
        WGTSetVisible("tabpage1_Tb2_subpage3", 0)
        WGTSetVisible("tabpage1_Tb3_subpage3", 0)
        WGTSetVisible("tabpage1_Tb4_subpage3", 0)
        WGTSetVisible("tabpage1_Tb5_subpage3", 0)
    elseif(coreselect == 1) then
		VarAdrSetStr(CORE_SELECT_SHOW,"C D")
        SetAttrValue("image1","fileName","BackCoreC.png")
        SetAttrValue("image2","fileName","BackCoreD.png")
        WGTSetVisible("subpage1", 0)
        WGTSetVisible("tabpage1_Tb1_subpage1", 0)
        WGTSetVisible("tabpage1_Tb2_subpage1", 0)
        WGTSetVisible("tabpage1_Tb3_subpage1", 0)
        WGTSetVisible("tabpage1_Tb4_subpage1", 0)
        WGTSetVisible("tabpage1_Tb5_subpage1", 0)
        WGTSetVisible("subpage2", 1)
        WGTSetVisible("tabpage1_Tb1_subpage2", 1)
        WGTSetVisible("tabpage1_Tb2_subpage2", 1)
        WGTSetVisible("tabpage1_Tb3_subpage2", 1)
        WGTSetVisible("tabpage1_Tb4_subpage2", 1)
        WGTSetVisible("tabpage1_Tb5_subpage2", 1)
        WGTSetVisible("subpage3", 0)
        WGTSetVisible("tabpage1_Tb1_subpage3", 0)
        WGTSetVisible("tabpage1_Tb2_subpage3", 0)
        WGTSetVisible("tabpage1_Tb3_subpage3", 0)
        WGTSetVisible("tabpage1_Tb4_subpage3", 0)
        WGTSetVisible("tabpage1_Tb5_subpage3", 0)
    elseif(coreselect == 2) then
		VarAdrSetStr(CORE_SELECT_SHOW,"E F")
        SetAttrValue("image1","fileName","BackCoreE.png")
        SetAttrValue("image2","fileName","BackCoreF.png")
        WGTSetVisible("subpage1", 0)
        WGTSetVisible("tabpage1_Tb1_subpage1", 0)
        WGTSetVisible("tabpage1_Tb2_subpage1", 0)
        WGTSetVisible("tabpage1_Tb3_subpage1", 0)
        WGTSetVisible("tabpage1_Tb4_subpage1", 0)
        WGTSetVisible("tabpage1_Tb5_subpage1", 0)
        WGTSetVisible("subpage2", 0)
        WGTSetVisible("tabpage1_Tb1_subpage2", 0)
        WGTSetVisible("tabpage1_Tb2_subpage2", 0)
        WGTSetVisible("tabpage1_Tb3_subpage2", 0)
        WGTSetVisible("tabpage1_Tb4_subpage2", 0)
        WGTSetVisible("tabpage1_Tb5_subpage2", 0)
        WGTSetVisible("subpage3", 1)
        WGTSetVisible("tabpage1_Tb1_subpage3", 1)
        WGTSetVisible("tabpage1_Tb2_subpage3", 1)
        WGTSetVisible("tabpage1_Tb3_subpage3", 1)
        WGTSetVisible("tabpage1_Tb4_subpage3", 1)
        WGTSetVisible("tabpage1_Tb5_subpage3", 1)
    end
end

local COREA_MOLD_FL_SPECIALCODE = 0x22130004 --特殊中子功能
function SetSpecCoreFunc()
	if(VarAdrToInt(COREA_MOLD_FL_CORE_CTRL) == 0) then
		CtlAdrInt(COREA_MOLD_FL_SPECIALCODE,0)
    end
end

function CorePrivMutual(oldvalue , newvalue, bflag ,num) --20200224中子进优先级设置互斥 bflag表示中子进退，num表示中子A-F
    local CORE_NUM = 6 --中子个数

    if(bflag ==0) then  
      for i=0, CORE_NUM-1  do
        if((VarAdrToInt(COREA_MOLD_PRI_FWD+i*0x100000) ==newvalue) and (i~=num)) then 
          CtlAdrInt(COREA_MOLD_PRI_FWD+i*0x100000,oldvalue)
        end
      end
    elseif (bflag ==1) then
      for i=0, CORE_NUM-1 do
        if((VarAdrToInt(COREA_MOLD_PRI_BWD+i*0x100000) ==newvalue) and (i~=num)) then 
          CtlAdrInt(COREA_MOLD_PRI_BWD+i*0x100000,oldvalue)
        end
      end
    end
end

function CorePosByPoint(coreselect, bflag_iscorein, num) --20211221 dyl
    local offset = PART_OFFSET*coreselect*2
    local offset2 = offset + PART_OFFSET
    local origin_addr = COREA_MOLD_FL_ADPOSI_CORE_FWD
    local originpos_addr = COREA_ACT_ADPOSI_COREINACT
    if(bflag_iscorein == 1) then
		if(num == 0) then
			origin_addr = origin_addr + offset
			originpos_addr = originpos_addr + offset
		elseif(num == 1) then
			origin_addr = origin_addr + offset2
			originpos_addr = originpos_addr + offset2
		end
		if(VarAdrToInt(origin_addr) == 0) then
			CtlAdrInt(originpos_addr, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENEND))
		elseif(VarAdrToInt(origin_addr) == 2) then
			CtlAdrInt(originpos_addr, 0)
		end
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(originpos_addr, COREA_MACH_ADPOS_EFFCOREINPLUS+(originpos_addr-COREA_ACT_ADPOSI_COREINACT))				
		end
	else
		if(num == 0) then
			origin_addr = origin_addr + offset + 1
			originpos_addr = originpos_addr + offset + 1
		elseif(num == 1) then
			origin_addr = origin_addr + offset2 + 1
			originpos_addr = originpos_addr + offset2 + 1
		end
		if(VarAdrToInt(origin_addr) == 0) then
			CtlAdrInt(originpos_addr, VarAdrToInt(CLAMP_ACT_ADPOSI_OPENEND))
		elseif(VarAdrToInt(origin_addr) == 2) then
			CtlAdrInt(originpos_addr, 0)
		end
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(originpos_addr, COREA_MACH_ADPOS_EFFCOREOUTPLUS+(originpos_addr-COREA_ACT_ADPOSI_COREOUTACT))				
		end
    end
end

function PageCreate()
	--20211221 dyl
    for i=0, 3  do
		CorePosByPoint(i, 1, 0)
		CorePosByPoint(i, 0, 0)
		CorePosByPoint(i, 1, 1)
		CorePosByPoint(i, 0, 1)
    end
end

function PageShow()
	SetPageLockMode(1)
	
	SetCoreParam() --地址切换
    SetCoreShow() --当前中子文本显示
    SetSpecCoreFunc() --特殊中子功能可用
    
	if(GetCurrentUserId() > 1) then
		TabPageSetSubVisble("tabpage1",2,true)
		TabPageSetSubVisble("tabpage1",3,true)
		TabPageSetSubVisble("tabpage1",4,true)
		if(band(VarAdrToInt(SYSTEM_MACH_CODE50),0x800)~=0) then --20200224中子优先级
			TabPageSetSubVisble("tabpage1",5,true)
		else
			TabPageSetSubVisble("tabpage1",5,false)
		end
	else 
		TabPageSetSubVisble("tabpage1",2,false)
		TabPageSetSubVisble("tabpage1",3,false)
		TabPageSetSubVisble("tabpage1",4,false)
		TabPageSetSubVisble("tabpage1",5,false)
	end
    
	--金鹰快速机模式下，中子按键独立选择
    if(band(VarAdrToInt(SYSTEM_MACH_CUSTOMID),0xFF00)==0x6400) then --金鹰
    
		if(VarAdrToInt(SYS_SPC_KEY_USE_JY)==1) then --金鹰快速机
			WGTSetVisible("dropdownJYKeySel", 1)
			WGTSetVisible("labelJYKeySel", 1)
		else
			WGTSetVisible("dropdownJYKeySel", 0)
			WGTSetVisible("labelJYKeySel", 0)
		end

    else
			WGTSetVisible("dropdownJYKeySel", 0)
			WGTSetVisible("labelJYKeySel", 0)
    end
    
	TabPageSelPage("tabpage1",0)
	FkeyTab("tabpage1", 0x204)

end

function PageUpdate()

end

function PageChange(name,adr,oldvalue,newvalue)
	print("Core PageChange")
  if(name == "cb12") then --中子选择
		SetCoreParam()
		SetCoreShow()
  elseif(name == "cb22") then --功能选择
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			if(VarAdrToInt(CORE_SELECT) == 0) then
				SetSpecCoreFunc() --特殊中子功能可用
			end
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
  elseif(name == "cb24") then --功能选择
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
		end
	elseif(name == "cbposin1") then --20211221 dyl
		CorePosByPoint(VarAdrToInt(CORE_SELECT), 1, 0)
	elseif(name == "cbposout1") then --20211221 dyl
		CorePosByPoint(VarAdrToInt(CORE_SELECT),0, 0)
	elseif(name == "cbposin2") then --20211221 dyl
		CorePosByPoint(VarAdrToInt(CORE_SELECT),1, 1)
	elseif(name == "cbposout2") then --20211221 dyl
		CorePosByPoint(VarAdrToInt(CORE_SELECT),0, 1)
	elseif(adr == COREA_ACT_ADPOSI_COREINACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREA_MACH_ADPOS_EFFCOREINPLUS) --中子有效区位置
		end
	elseif(adr == COREA_ACT_ADPOSI_COREOUTACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREA_MACH_ADPOS_EFFCOREOUTPLUS) --中子有效区位置
		end
	elseif(adr == COREB_ACT_ADPOSI_COREINACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREB_MACH_ADPOS_EFFCOREINPLUS) --中子有效区位置
		end
	elseif(adr == COREB_ACT_ADPOSI_COREOUTACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREB_MACH_ADPOS_EFFCOREOUTPLUS) --中子有效区位置
		end
	elseif(adr == COREC_ACT_ADPOSI_COREINACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREC_MACH_ADPOS_EFFCOREINPLUS) --中子有效区位置
		end
	elseif(adr == COREC_ACT_ADPOSI_COREOUTACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREC_MACH_ADPOS_EFFCOREOUTPLUS) --中子有效区位置
		end
	elseif(adr == CORED_ACT_ADPOSI_COREINACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, CORED_MACH_ADPOS_EFFCOREINPLUS) --中子有效区位置
		end
	elseif(adr == CORED_ACT_ADPOSI_COREOUTACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, CORED_MACH_ADPOS_EFFCOREOUTPLUS) --中子有效区位置
		end
	elseif(adr == COREE_ACT_ADPOSI_COREINACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREE_MACH_ADPOS_EFFCOREINPLUS) --中子有效区位置
		end
	elseif(adr == COREE_ACT_ADPOSI_COREOUTACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREE_MACH_ADPOS_EFFCOREOUTPLUS) --中子有效区位置
		end
	elseif(adr == COREF_ACT_ADPOSI_COREINACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREF_MACH_ADPOS_EFFCOREINPLUS) --中子有效区位置
		end
	elseif(adr == COREF_ACT_ADPOSI_COREOUTACT) then --20211221 dyl
		if(PUSH_Type()) then
			CoreEffeAreaByPushtab(adr, COREF_MACH_ADPOS_EFFCOREOUTPLUS) --中子有效区位置
		end
--  elseif(name == "tabpage1_Tb3_stbt11") then --中子保护功能
--		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then

--		else
--			CtlAdrInt(adr,oldvalue)
--			INFO_ShowMsg(VW_MSG_MANUAL)
--			return -1
--		end
--  elseif(name == "tabpage1_Tb3_stbt12") then --中子保护功能
--		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then

--		else
--			CtlAdrInt(adr,oldvalue)
--			INFO_ShowMsg(VW_MSG_MANUAL)
--			return -1
--		end
  elseif(name == "tabpage1_Tb1_cb41") then --特殊中子功能
		if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL) then
			CorePointByLink()
		else
			CtlAdrInt(adr,oldvalue)
			INFO_ShowMsg(VW_MSG_MANUAL)
			return -1
    end
  elseif(name == "tabpage1_Tb1_btinfo") then --特殊中子功能说明 --20211221 dyl
	ShowSpcCoreMsg(VW_MSG_MANUAL)
  --20200224中子优先级互斥  
  elseif(name == "tabpage1_Tb6_ed11") then
    CorePrivMutual(oldvalue,newvalue,0,0) --CORE A
  elseif(name == "tabpage1_Tb6_ed12") then
    CorePrivMutual(oldvalue,newvalue,0,1) --CORE B
  elseif(name == "tabpage1_Tb6_ed13") then
    CorePrivMutual(oldvalue,newvalue,0,2) --CORE C
  elseif(name == "tabpage1_Tb6_ed14") then
    CorePrivMutual(oldvalue,newvalue,0,3) --CORE D
  elseif(name == "tabpage1_Tb6_ed15") then
    CorePrivMutual(oldvalue,newvalue,0,4) --CORE E
  elseif(name == "tabpage1_Tb6_ed16") then
    CorePrivMutual(oldvalue,newvalue,0,5) --CORE F
  elseif(name == "tabpage1_Tb6_ed21") then
    CorePrivMutual(oldvalue,newvalue,1,0)
  elseif(name == "tabpage1_Tb6_ed22") then
    CorePrivMutual(oldvalue,newvalue,1,1)
  elseif(name == "tabpage1_Tb6_ed23") then
    CorePrivMutual(oldvalue,newvalue,1,2)
  elseif(name == "tabpage1_Tb6_ed24") then
    CorePrivMutual(oldvalue,newvalue,1,3)
  elseif(name == "tabpage1_Tb6_ed25") then
    CorePrivMutual(oldvalue,newvalue,1,4)
  elseif(name == "tabpage1_Tb6_ed26") then
    CorePrivMutual(oldvalue,newvalue,1,5)
  end

	return 0
end

function PageHide()
	SetPageLockMode(0)
end
