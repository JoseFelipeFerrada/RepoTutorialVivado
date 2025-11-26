set_property PACKAGE_PIN R19 [get_ports {en_0[0]}]
set_property PACKAGE_PIN T19 [get_ports {en_0[1]}]
set_property PACKAGE_PIN G14 [get_ports {en_0[2]}]
set_property PACKAGE_PIN J15 [get_ports {btn_0}]
set_property PACKAGE_PIN Y16 [get_ports {pwm_out_0[0]}]
set_property PACKAGE_PIN Y17 [get_ports {pwm_out_0[1]}]
set_property PACKAGE_PIN R14 [get_ports {pwm_out_0[2]}]
## Voltage Standard Assignments (This was missing)
set_property IOSTANDARD LVCMOS33 [get_ports {en_0[*]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_out_0[*]}]
set_property IOSTANDARD LVCMOS33 [get_ports btn_0]

## Configuration Bank Settings
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]