##############################################
# 日期: 2022-12-20
# 作者: quliuliu
# 说明：顶层Makefile
##############################################

TARGET := core_lib

SRC_INC_LAL := src/include/lal
SRC_INC_SDK := src/include/sdk
SRC_INC_CU := src/include/component
SRC_LIB_SDK := src/lib/sdk/
SRC_LIB_LAL := src/lib/lal/
SRC_LIB_CU := src/lib/component/
SRC_CORE := src/core/
RELEASE := release/
INC := src/include/
LIB := ../open_lib/zlog-1.2.15/lib/
libs = -L../open_lib/zlog-1.2.15/lib/
RELEASE_LIB_PATH := release/opt/lib
RELEASE_BIN_PATH := release/opt/bin

$(TARGET):

	cp -r build/opt release/
	@echo "************************* 拷贝LAL头文件到/include/lal *********************************************"
	mkdir -p $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_adc/lal_adc.h $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_can/lal_can.h $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_gpio/lal_gpio.h $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_i2c/lal_i2c.h $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_spi/lal_spi.h $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_uart/lal_uart.h $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_rtc/lal_rtc.h $(SRC_INC_LAL)
	cp $(SRC_LIB_LAL)lal_version/lal_version.h $(SRC_INC_LAL)	

	@echo "************************* 拷贝SDK头文件到include/sdk *********************************************"
	mkdir -p $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_file/sdk_file.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_print/sdk_print.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_ipc/sdk_ipc.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_log/sdk_log.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_watchdog/sdk_watchdog.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_led/sdk_led.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_buzzer/sdk_buzzer.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_drm/sdk_drm.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_dido/sdk_dido.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_para/sdk_para.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_record/sdk_record.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_fs/sdk_fs.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_public/sdk_public.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_version/sdk_version.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_adc/sdk_adc.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_key/sdk_key.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_modbus/sdk_modbus.h $(SRC_INC_SDK)
	cp $(SRC_LIB_SDK)sdk_store/sdk_store.h $(SRC_INC_SDK)

	@echo "************************* 拷贝COMPONENT/CU头文件到include/component *********************************************"
	mkdir -p $(SRC_INC_CU)
	cp $(SRC_LIB_CU)cu_sofar_sci/cu_sofar_sci.h $(SRC_INC_CU)

	cp -r $(INC) $(RELEASE)

	@echo "************************* 生成LAL和SDK库文件 *********************************************"
	# make -C $(SRC_LIB_LAL) && cp -p $(LIB)lib_lal.so $(RELEASE_LIB_PATH)
	# make -C $(SRC_LIB_SDK) && cp -p $(LIB)lib_sdk.so $(RELEASE_LIB_PATH)
	make -C $(SRC_LIB_LAL) 
	make -C $(SRC_LIB_SDK)
	cp -p $(SRC_LIB_LAL)/../*.so $(RELEASE_LIB_PATH)
	cp -p $(SRC_LIB_LAL)/../*.a $(RELEASE_LIB_PATH)
	cp -p $(SRC_LIB_SDK)/../*.so $(RELEASE_LIB_PATH)
	cp -p $(SRC_LIB_SDK)/../*.a $(RELEASE_LIB_PATH)
	

	make -C $(SRC_CORE)daemon && cp -p $(SRC_CORE)daemon/app_start $(RELEASE_BIN_PATH)
	make -C $(SRC_CORE)communication && cp -p $(SRC_CORE)communication/communication $(RELEASE_BIN_PATH)
	make -C $(SRC_CORE)ipc_server && cp -p $(SRC_CORE)ipc_server/core_ipc_server $(RELEASE_BIN_PATH)


	cd release/opt && tar -czf ../opt.tar.gz ./*
clean :
	cd $(SRC_LIB_SDK) && make clean
	cd $(SRC_LIB_LAL) && make clean
	cd $(SRC_CORE)daemon && make clean
	cd $(SRC_CORE)communication && make clean	
	rm -rf $(SRC_INC_LAL)
	rm -rf $(SRC_INC_SDK)
	rm -rf release/*



