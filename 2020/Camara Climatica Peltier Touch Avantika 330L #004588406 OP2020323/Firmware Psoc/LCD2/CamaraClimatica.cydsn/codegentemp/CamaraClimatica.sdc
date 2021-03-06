# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\Laboratorio\Camara Climatica\2020\Camara Climatica Peltier Touch Avantika 330L #004588406 OP2020323\Firmware Psoc\LCD2\CamaraClimatica.cydsn\CamaraClimatica.cyprj
# Date: Thu, 05 Nov 2020 18:22:51 GMT
#set_units -time ns
create_clock -name {ADC_Ext_CP_Clk(routed)} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dclk_0}]]
create_clock -name {ADC_SAR_IntClock(routed)} -period 1000 -waveform {0 500} [list [get_pins {ClockBlock/dclk_1}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_clock -name {ADC_Ext_CP_Clk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {ADC_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 9 19} [list [get_pins {ClockBlock/aclk_glb_0}]]
create_generated_clock -name {ADC_SAR_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 25 49} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {UART_1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 313 627} [list [get_pins {ClockBlock/dclk_glb_2}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for D:\Laboratorio\Camara Climatica\2020\Camara Climatica Peltier Touch Avantika 330L #004588406 OP2020323\Firmware Psoc\LCD2\CamaraClimatica.cydsn\TopDesign\TopDesign.cysch
# Project: D:\Laboratorio\Camara Climatica\2020\Camara Climatica Peltier Touch Avantika 330L #004588406 OP2020323\Firmware Psoc\LCD2\CamaraClimatica.cydsn\CamaraClimatica.cyprj
# Date: Thu, 05 Nov 2020 18:22:36 GMT
