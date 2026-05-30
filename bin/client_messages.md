# Sokit Message List
# Format: checked|content|description|order|delay_ms
# checked: 1=checked, 0=unchecked

0|[02]IC,C,,,[03]|IC: 初始化连接 IC(Initial Connection)|1|0
0|[02]QT,C,,,[03]|QT: 查询测试文件列表 QT(Query Testfile)|2|0
0|[02]GT,C,,,TestFileName[03]|GT: 查询测试文件的图形列表GT|3|0
0|[02]ST,C,,,1,1,TestFileName[03]|ST: 切换测试文件 ST(Set Testfile)|4|0
1|[02]SP,C,,,1,1,1,1[03]|SP: 开关电 SP(Switch Power) On|5|0
1|[02]SP,C,,,1,1,1,0[03]|SP: 开关电 SP(Switch Power) Off|6|0
0|[02]CP,C,,,1,1,1,1[03]|CP: 序号切图 CP(Cut Pattern)|7|0
0|[02]NP,C,,,1,1,1,Pattern.bmp[03]|NP: 图片名字切图 NP(Named Pattern)|8|0
0|[02]HP,C,,,1,1,1,0,1920,1080,Pattern,0[03]|HP: Hando切图 HP(Hando Pattern)|9|0
0|[02]GP,C,,,1,1,1,255,255,255[03]|GP: 灰阶切图 GP(Gray Pattern)|10|0
0|[02]SB,C,,,1,1,1,1[03]|SB: 开关背光 SB(Switch Backlight) Off|11|0
0|[02]SB,C,,,1,1,1,0[03]|SB: 开关背光 SB(Switch Backlight) On|12|0
0|[02]CLEARAVI,C,,,[03]|CLEARAVI: 清除AVI|13|0
0|[02]BK,C,,,1,1,1,1,255,0,0,0,255,0,1000[03]|BK: Blink功能 BK(Blink)|14|0
0|[02]BKF,C,,,1,1,1,1,255,0,0,0,255,0,60[03]|BKF: 按频率Blink功能 BKF(Blink by Frequency)|15|0
0|[02]GCV,C,,,1,1,1[03]|GCV: 获取电流电压 GCV(Get Current Voltage)|16|0
0|[02]SPWR,C,,,1,1,1[03]|SPWR: 设置电源报警主动上报 SPWR(Set PowerWarning Report)|17|0
0|[02]SVV,C,,,1,1,1,Vdd1,5000[03]|SVV: 设置电压值 SVV(Set Voltage Value)|18|0
0|[02]VPS,C,,,1,1,1[03]|VPS: 查询电源状态 VPS(View Power Stats)|19|0
0|[02]CT,C,,,1,1,1,1280,720,30,30,50,50,4231991232,0,0[03]|CT: CrossTalk设置 CT(CrossTalk)|20|0
0|[02]FLK,C,,,1,1,1,0,255,255,255[03]|FLK: SetFlickerPtn|21|0
0|[02]FLKN,C,,,1,1,1,0,0,255,255,255,0,0,0[03]|FLKN: SetFlickerPtnNew|22|0
0|[02]AF,C,,,1,1,1,1,2[03]|AF: AutoFlicker流程 AF(Auto Flicker)|23|0
0|[02]QPS,C,,,1,1[03]|QPS: 查询PG状态 QPS(Query PG's Status)|24|0
0|[02]RDVCOM,C,,,1,1,1,1[03]|RDVCOM: 读取Vcom值|25|0
0|[02]WRVCOM,C,,,1,1,1,0,5.5,1[03]|WRVCOM: 写入Vcom值|26|0
0|[02]HDT,C,,,1,1,1,D:\\A.bmp[03]|HDT: Hando模式打图 HDT(hand to)|27|0
0|[02]PSM,C,,,1,1,1,1,0,0,100,100,200,200,10,10[03]|PSM: PG显示坐标 PSM(PG Show Mark)|28|0
0|[02]CM,C,,,1,1,1,1,1280,720,255,255,255,0,0,0,0,1,1,0[03]|CM: 十字光标CrossMask设置 CM(CrossMask)|29|0
0|[02]DISTXT,C,,,1,1,1,1,1,0,100,100,0,0,Test[03]|DISTXT: 显示字符 DISTXT(DISPLAY TXT)|30|0
0|[02]RIC,C,,,1,1,1,0,0,0,,Delay 200[03]|RIC: 运行InitCode RIC(Run InitCode)|31|0
0|[02]NEXT,C,,,1,1,1,0[03]|NEXT: 向下切图 NEXT(NEXT Pattern)|32|0
0|[02]NEXT,C,,,1,1,1,1[03]|NEXT: 向上切图(Previous)|33|0
0|[02]SVCC,C,,,1,1,1,1,0,1,0,0[03]|SVCC: VCResult 电压系数设置|34|0
0|[02]SICC,C,,,1,1,1,1,0,1,0,0[03]|SICC: ICResult 电流系数设置|35|0
0|[02]PINF,C,,,1,1,1,SN12345,PID67890[03]|PINF: Panel信息 PINF(Panel Info)|36|0
0|[02]PINFS,C,,,1,1,1,SN,PI[03]|PINFS: Panel信息(HEX格式)|37|0
0|[02]PED,C,,,1,1,1,EDID_DATA[03]|PED: Panel EDID数据 PED(Panel EDID DATA)|38|0
0|[02]CTGMFSH,C,,,1,1,1,RegKey,RegValue[03]|CTGMFSH: GAMMA完成 CTGMFSH(COLOR TEST GAMMA FINISH)|39|0
0|[02]CTGMDR,C,,,1,1,1,Gray,Band,x,xmin,xmax,y,ymin,ymax,lv,lvmin,lvmax[03]|CTGMDR: GAMMA数据上报 CTGMDR(COLOR TEST GAMMA DATA REPORT)|40|0
0|[02]READ_OTP_DATA,C,,,1,1,1,D:\\Tmp\\PanelIDA,PanelId,LuaFile,Reserve[03]|READ_OTP_DATA: 读取Demura烧录数据|41|0
0|[02]READ_BARCODE,C,,,1,1,1,D:\\Tmp\\PanelIDA,PanelId,LuaFile,Reserve[03]|READ_BARCODE: 读取烧录在模组里面的条码|42|0
0|[02]CLS,C,,,1,1[03]|CLS: CELL连接状态 CLS(Cell Link state)|43|0
0|[02]CIC,C,,,1,1[03]|CIC: CELL初始化连接 CIC(Cell Initial Connection)|44|0
0|[02]CQV,C,,,1,1[03]|CQV: Cell查询ARM和FPGA的版本号 CQV(Cell Query Version)|45|0
0|[02]CST,C,,,1,1,ModuleName,TestFileName,1[03]|CST: CELL设置测试文件 CST(CELL Set Testfile)|46|0
0|[02]CON,C,,,1,1,1,15[03]|CON: CELL开电 CON(Cell Power On)|47|0
0|[02]COFF,C,,,1,1,1,15[03]|COFF: CELL关电 COFF(Cell Power Off)|48|0
0|[02]CCP,C,,,1,1,1,1[03]|CCP: CELL切图 CCP(Cell Cut Pattern)|49|0
0|[02]CCSP,C,,,1,1,1,1,1[03]|CCSP: 子画面切图 CCSP(Cell Cut Sub Pattern)|50|0
0|[02]CCFP,C,,,1,1,1,Para1,Para2[03]|CCFP: 固定画面切图 CCFP(Cell Cut Fixed Pattern)|51|0
0|[02]CCPD,C,,,1,1,1,0[03]|CCPD: CELL按方向切图 CCPD(Cell Cut Pattern Direction)|52|0
0|[02]CCPA,C,,,1,1[03]|CCPA: 自动切图 CCPA(Cell Cut Pattern Auto)|53|0
0|[02]CCPE,C,,,1,1[03]|CCPE: 终止切图 CCPE(Cell Cut Pattern End)|54|0
0|[02]CGC,C,,,1,1[03]|CGC: CELL获取电流 CGC(Cell Get Current)|55|0
0|[02]CGM,C,,,[03]|CGM: CELL获取模组 CGM(Cell Get Module)|56|0
0|[02]CGMT,C,,,1,1[03]|CGMT: 获取当前模组测试文件|57|0
0|[02]CGMTT,C,,,1,1[03]|CGMTT: CELL获取当前模组测试文件aging总时间与画面锁秒时间|58|0
0|[02]CGT,C,,,ModuleName,1[03]|CGT: CELL获取测试文件 CGT(Cell Get Test File Name)|59|0
0|[02]CGP,C,,,ModuleName,TestName,1[03]|CGP: CELL获取图片 CGP(Cell Get Pattern Name)|60|0
0|[02]CGPLT,C,,,ModuleName,TestFileName,1[03]|CGPLT: 获取测试文件下的图片和对应的锁秒时间|61|0
0|[02]CPIS,C,,,0[03]|CPIS: CELL参数项目切换 CPIS(Cell Para Item Switch)|62|0
0|[02]CPVA,C,,,0,1[03]|CPVA: CELL参数调节 CPVA(Cell Para ValueAdjust)|63|0
0|[02]CQVA,C,,,1,1,1,AdjNum,AdjIndex1,AdjIndex2[03]|CQVA: CELL查询调节参数值 CQVA(Cell Query Value Adjust)|64|0
0|[02]CVCOM,C,,,ModuleName,PatternName[03]|CVCOM: CELL获取Vcom值 CVCOM(Cell VCOM)|65|0
0|[02]CIMP,C,,,1,1,1,15[03]|CIMP: CELL获取阻抗 CIMP(Cell Impedance)|66|0
0|[02]CPLTC,C,,,1,1[03]|CPLTC: CELL栈板到达 CPLTC(CELL Pallet come)|67|0
0|[02]CMPVA,C,,,1,1,ParaNum,AdjIndex1,AdjValue1[03]|CMPVA: 多参数目标电压值设置|68|0
0|[02]CSDA,C,,,1,1,Flag,ChanNum,Value,Reserve[03]|CSDA: 设置DA或者电压值|69|0
0|[02]CCFS,C,,,1,1,Type,FileName[03]|CCFS: 校准文件同步|70|0
0|[02]CGA,C,,,1,1,1,R,G,B[03]|CGA: CELL灰阶调节 CGA(Cell Gray Adjust)|71|0
0|[02]CGAMAEN,C,,,1,1,1[03]|CGAMAEN: CELL GAMA使能 CGAMAEN(CELL GAMMA ENABLE)|72|0
0|[02]CGAMAFUN,C,,,1,1,PanelNum,PanelId1,PanelId2[03]|CGAMAFUN: CELL GAMA功能 CGAMAFUN(CELL GAMMA FUNCTION)|73|0
0|[02]CAF,C,,,1,1[03]|CAF: AOI通知AF使能|74|0
0|[02]CEDCSTART,C,,,1,1,Cplatform1,PanelId1,Cplatform2,PanelId2[03]|CEDCSTART: CELL EDC开始 CEDCSTART(CELL EDCSTART)|75|0
0|[02]PANRDY,C,,,1,1,PanelNum,PanelId1,PanelId2[03]|PANRDY: CELL PG屏压接 PANNELREADY(CellPannelReady)|76|0
0|[02]PANRDYAGING,C,,,1,1,PalletId,PanelNum,PanelId1,PanelId2[03]|PANRDYAGING: 屏已经压接好(用于华星Agin机台)|77|0
0|[02]CHECKCIE,C,,,1,1,1[03]|CHECKCIE: CELL YW CheckCIE(CellCheckCIE)|78|0
0|[02]CGAMMAR,C,,,1,1,1[03]|CGAMMAR: CELL GAMMA CGAMMAR(CELL GAMMA Report)|79|0
0|[02]OSRC,C,,,1,1,1,cmd[03]|OSRC: 光学探头原始指令 OSRC(optics sensor raw cmd)|80|0
0|[02]CAGEN,C,,,1,1[03]|CAGEN: CELL AGING使能 CAGEN(CELL AGING ENABLE)|81|0
0|[02]CSETIO,C,,,1,1,ChanNo,OpType[03]|CSETIO: 设置IO|82|0
0|[02]CGPI,C,,,1,1,PalletId,ProcessType,ProcessSkipFlag[03]|CGPI: 通过tibco获取panel信息|83|0
0|[02]CSPDI,C,,,1,1,PanelGrade1,ResultList,PalletId,ProcessType,Model[03]|CSPDI: 通过tibco上传panel判级信息|84|0
0|[02]CSPDT,C,,,1,1,PanelGrade1,PanelJudgeInformation,ResultList,PalletId[03]|CSPDT: 界面提交判级(T4项目)|85|0
0|[02]SWITCHMODEL,C,,,1,Model[03]|SWITCHMODEL: 通过tibco上传当前模式信息(T4项目)|86|0
0|[02]QCLS,C,,,1,1,1[03]|QCLS: CELL查询PG是否设置测试文件|87|0
0|[02]SIMP,C,,,1,1,1[03]|SIMP: CELL开始ITO (Cell Start ITO)|88|0
0|[02]SENDPANELID,C,,,1,1,PanelId[03]|SENDPANELID: CELL PG SENPANELID(CellSendPanelId)|89|0
0|[02]JUDGEEND,C,,,1,1,PortId[03]|JUDGEEND: CELL PG JUDGEEND(CellJudgeEnd)|90|0
0|[02]ICTPC,C,,,[03]|ICTPC: ICTPC定时连接(ICTP Connect TP<->PG)|91|0
0|[02]ICTPRTR,C,,,2,TP1,0,TP2,0[03]|ICTPRTR: ICTPRTR上报测试结果(ICTP Report Test Result)|92|0
0|[02]ICTPRONOFF,C,,,2,TP1,ON,TP2,ON[03]|ICTPRONOFF: ICTPRONOFF上报开始结束(ICTP Report ONOFF)|93|0
0|[02]ICTPEVENT,C,,,2,TP1,0,TP2,0[03]|ICTPEVENT: ICTPEVENT请求特殊事件(TP需求特殊事件响应)|94|0
0|[02]PC,C,,,1,1[03]|PC: 查询PG连接状态(PG Connection)|95|0
0|[02]ROMI,C,,,1,1,1,Para1,Para2[03]|ROMI: 传输demura相关信息|96|0
0|[02]ADR,C,,,1,1,1[03]|ADR: 自动开始Demura调节(Auto Demura Regulate)|97|0
