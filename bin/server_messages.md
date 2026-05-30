# Sokit Message List
# Format: checked|content|description|order|delay_ms
# checked: 1=checked, 0=unchecked

1|[02]IC,S,0,[03]|IC: 初始化连接应答 (0:OK, 1:NG)|1|0
0|[02]QT,S,0,,FileName1,FileName2,FileName3[03]|QT: 查询测试文件列表应答|2|0
0|[02]GT,S,0,,PatternName1,PatternName2[03]|GT: 查询测试文件的图形列表应答|3|0
0|[02]ST,S,0,,1,1[03]|ST: 切换测试文件应答|4|0
1|[02]SP,S,0,,1,1,1[03]|SP: 开关电应答|5|0
1|[02]CP,S,0,,1,1,1[03]|CP: 序号切图应答|6|0
0|[02]NP,S,0,,1,1,1[03]|NP: 图片名字切图应答|7|0
0|[02]HP,S,0,,1,1,1[03]|HP: Hando切图应答|8|0
0|[02]GP,S,0,,1,1,1[03]|GP: 灰阶切图应答|9|0
0|[02]SB,S,0,,1,1,1[03]|SB: 开关背光应答|10|0
0|[02]CLEARAVI,S,0,[03]|CLEARAVI: 清除AVI应答|11|0
0|[02]BK,S,0,,1,1,1[03]|BK: Blink功能应答|12|0
0|[02]BKF,S,0,,1,1,1[03]|BKF: 按频率Blink功能应答|13|0
0|[02]GCV,S,0,,1,1,1,Vdd1,Value1,Idd1,Value1[03]|GCV: 获取电流电压应答|14|0
0|[02]SPWR,S,0,,1,1,1,Vddh1,2,Vddl1,Value[03]|SPWR: 设置电源报警主动上报应答|15|0
0|[02]SVV,S,0,,1,1[03]|SVV: 设置电压值应答|16|0
0|[02]VPS,S,0,,1,1,LinkOnStats[03]|VPS: 查询电源状态应答|17|0
0|[02]CT,S,0,,1,1,1[03]|CT: CrossTalk设置应答|18|0
0|[02]FLK,S,0,,1,1,1[03]|FLK: SetFlickerPtn应答|19|0
0|[02]FLKN,S,0,,1,1,1[03]|FLKN: SetFlickerPtnNew应答|20|0
0|[02]AF,S,0,,1,1,1,1,VcomValue,FlickerValue[03]|AF: AutoFlicker流程应答|21|0
0|[02]QPS,S,0,,1,1,OnlineStatus,SetStatus,OnOffStatus[03]|QPS: 查询PG状态应答|22|0
0|[02]RDVCOM,S,0,,1,1,1,VcomValue[03]|RDVCOM: 读取Vcom值应答|23|0
0|[02]WRVCOM,S,0,,1,1,1,VcomType,VcomValue[03]|WRVCOM: 写入Vcom值应答|24|0
0|[02]HDT,S,0,,1,1,1[03]|HDT: Hando模式打图应答|25|0
0|[02]PSM,S,0,,1,1,1[03]|PSM: PG显示坐标应答|26|0
0|[02]CM,S,0,,1,1,1[03]|CM: 十字光标CrossMask设置应答|27|0
0|[02]DISTXT,S,0,,1,1,1[03]|DISTXT: 显示字符应答|28|0
0|[02]RIC,S,0,,1,1,1[03]|RIC: 运行InitCode应答|29|0
0|[02]CP,S,0,,1,1,1[03]|NEXT: 切图应答|30|0
0|[02]SVCC,S,0,,1,1,1[03]|SVCC: VCResult 电压系数设置应答|31|0
0|[02]SICC,S,0,,1,1,1[03]|SICC: ICResult 电流系数设置应答|32|0
0|[02]PINF,S,0,,1,1,1[03]|PINF: Panel信息应答|33|0
0|[02]PINFS,S,0,,1,1,1[03]|PINFS: Panel信息(HEX格式)应答|34|0
0|[02]PED,S,0,,1,1,1[03]|PED: Panel EDID数据应答|35|0
0|[02]CTGMFSH,S,0,,1,1,1[03]|CTGMFSH: GAMMA完成应答|36|0
0|[02]CTGMDR,S,0,,1,1,1[03]|CTGMDR: GAMMA数据上报应答|37|0
1|[02]CTGMSTR,C,0,,1,1,1[03]|CTGMSTR: GAMMA开始(TEMS->第三方)|38|0
0|[02]CTGMSTP,C,0,,1,1,1[03]|CTGMSTP: GAMMA停止(TEMS->第三方)|39|0
0|[02]READ_OTP_DATA,S,0,,1,1,1[03]|READ_OTP_DATA: 读取Demura烧录数据应答|40|0
0|[02]READ_BARCODE,S,0,,1,1,1[03]|READ_BARCODE: 读取烧录在模组里面的条码应答|41|0
0|[02]CLS,S,0,,LinkState[03]|CLS: CELL连接状态应答|42|0
0|[02]CIC,S,0,[03]|CIC: CELL初始化连接应答|43|0
0|[02]CQV,S,0,,ArmVersionNo,FpgaVersionNo[03]|CQV: Cell查询ARM和FPGA的版本号应答|44|0
0|[02]CST,S,0,[03]|CST: CELL设置测试文件应答|45|0
0|[02]CON,S,0,[03]|CON: CELL开电应答|46|0
0|[02]COFF,S,0,[03]|COFF: CELL关电应答|47|0
0|[02]CCP,S,0,[03]|CCP: CELL切图应答|48|0
0|[02]CCSP,S,0,,1,1[03]|CCSP: 子画面切图应答|49|0
0|[02]CCFP,S,0,,1,1[03]|CCFP: 固定画面切图应答|50|0
0|[02]CCPD,S,0,,1,1,1,PatternIndex[03]|CCPD: CELL按方向切图应答|51|0
0|[02]CCPA,S,0,,1,1[03]|CCPA: 自动切图应答|52|0
0|[02]CCPE,S,0,,1,1[03]|CCPE: 终止切图应答|53|0
0|[02]CGC,S,0,,Idd1,Idd2[03]|CGC: CELL获取电流应答|54|0
0|[02]CGM,S,0,,ModuleName1,ModuleName2[03]|CGM: CELL获取模组应答|55|0
0|[02]CGMT,S,0,,TestFileName[03]|CGMT: 获取当前模组测试文件应答|56|0
0|[02]CGMTT,S,0,,ModuleName,TestName,TotalTime,PatternNum,PatternName1,LockTime1[03]|CGMTT: CELL获取当前模组测试文件aging总时间与画面锁秒时间应答|57|0
0|[02]CGT,S,0,,TestName1,TestName2[03]|CGT: CELL获取测试文件应答|58|0
0|[02]CGP,S,0,,PatternName1,PatternName2[03]|CGP: CELL获取图片应答|59|0
0|[02]CGPLT,S,0,,PatternName1,LockTime1,PatternName2,LockTime2[03]|CGPLT: 获取测试文件下的图片和对应的锁秒时间应答|60|0
0|[02]CPIS,S,0,,ItemName[03]|CPIS: CELL参数项目切换应答|61|0
0|[02]CPVA,S,0,,Step[03]|CPVA: CELL参数调节应答|62|0
0|[02]CQVA,S,0,,1,1,1,AdjNum,AdjIndex1,AdjValue1,AdjIndex2,AdjValue2[03]|CQVA: CELL查询调节参数值应答|63|0
0|[02]CVCOM,S,0,,Vcom1,Vcom2[03]|CVCOM: CELL获取Vcom值应答|64|0
0|[02]CIMP,S,0,,Group,PortNum,PortNo1,Result,ImpNum,Imp1Result[03]|CIMP: CELL获取阻抗应答|65|0
0|[02]CPLTC,S,0,[03]|CPLTC: CELL栈板到达应答|66|0
0|[02]CMPVA,S,0,,1,1,Flag[03]|CMPVA: 多参数目标电压值设置应答|67|0
0|[02]CSDA,S,0,,1,1,Flag[03]|CSDA: 设置DA或者电压值应答|68|0
0|[02]CCFS,S,0,,1,1[03]|CCFS: 校准文件同步应答|69|0
0|[02]CGA,S,0,,1,1,1[03]|CGA: CELL灰阶调节应答|70|0
0|[02]CAF,S,0,,1,1[03]|CAF: AOI通知AF使能应答|71|0
0|[02]CEDCSTART,S,0,,1,1[03]|CEDCSTART: CELL EDC开始应答|72|0
0|[02]PANRDY,S,0,OK[03]|PANRDY: CELL PG屏压接应答|73|0
0|[02]PANRDYAGING,S,0,[03]|PANRDYAGING: 屏已经压接好应答|74|0
0|[02]CHECKCIE,S,0,OK[03]|CHECKCIE: CELL YW CheckCIE应答|75|0
0|[02]CGAMMAR,S,0,,1,1,1,PatternIndex,Result[03]|CGAMMAR: CELL GAMMA应答|76|0
0|[02]OSRC,S,0,,1,1,1,cmd[03]|OSRC: 光学探头原始指令应答|77|0
0|[02]CSETIO,S,0,,1,1,ChannelNo[03]|CSETIO: 设置IO应答|78|0
0|[02]CGPI,S,0,,1,1[03]|CGPI: 通过tibco获取panel信息应答|79|0
0|[02]CSPDI,S,0,,1,1[03]|CSPDI: 通过tibco上传panel判级信息应答|80|0
0|[02]CSPDT,S,0,,1,1[03]|CSPDT: 界面提交判级应答|81|0
0|[02]SWITCHMODEL,S,0,,1,1[03]|SWITCHMODEL: 通过tibco上传当前模式信息应答|82|0
0|[02]QCLS,S,0,,Status,1,1,1[03]|QCLS: CELL查询PG是否设置测试文件应答|83|0
0|[02]SIMP,S,0,,Group,PortNum,PortNo1,Result,ImpNum,Imp1Result[03]|SIMP: CELL开始ITO应答|84|0
0|[02]SENDPANELID,S,0,OK[03]|SENDPANELID: CELL PG SENPANELID应答|85|0
0|[02]ICTPC,S,0,[03]|ICTPC: ICTPC定时连接应答|86|0
0|[02]ICTPRTR,S,0,[03]|ICTPRTR: ICTPRTR上报测试结果应答|87|0
0|[02]ICTPRONOFF,S,0,[03]|ICTPRONOFF: ICTPRONOFF上报开始结束应答|88|0
0|[02]ICTPEVENT,S,0,[03]|ICTPEVENT: ICTPEVENT请求特殊事件应答|89|0
0|[02]ICTPNR,S,0,,PanelID1,PanelID2[03]|ICTPNR: ICTPNR TP名上报比对(TEMS->TP)|90|0
0|[02]ICTPONOFF,S,,,2,TP1,ON,TP2,ON[03]|ICTPONOFF: ICTPONOFF开始结束(TEMS->TP)|91|0
0|[02]PC,S,0,,1,PgStats[03]|PC: 查询PG连接状态应答|92|0
0|[02]GPMVTID,S,,,1,1,1[03]|GPMVTID: 获取PannelId/ModuleId/VerId/TestId等信息(TEMS->Client)|93|0
0|[02]RHVD,S,,,1,1,1,H-Demura,V-Demura,BurnCount[03]|RHVD: 上报H-Demura V-Demura(TEMS->Client)|94|0
0|[02]ROMI,S,0,,1,1,1,Para1,Para2[03]|ROMI: 传输demura相关信息应答|95|0
0|[02]ADR,S,0,,1,1,1[03]|ADR: 自动开始Demura调节应答|96|0
