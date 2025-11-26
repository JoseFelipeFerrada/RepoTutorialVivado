## GPIO Pin Assignments
set_property PACKAGE_PIN R19 [get_ports {gpio_rtl_tri_i[0]}]
set_property PACKAGE_PIN T19 [get_ports {gpio_rtl_tri_i[1]}]
set_property PACKAGE_PIN G14 [get_ports {gpio_rtl_tri_i[2]}]
set_property PACKAGE_PIN J15 [get_ports {gpio_rtl_tri_i[3]}]

## PWM Pin Assignments
set_property PACKAGE_PIN Y16 [get_ports {pwm0_0}]
set_property PACKAGE_PIN Y17 [get_ports {pwm0_1}]
set_property PACKAGE_PIN R14 [get_ports {pwm0_2}]

## Voltage Standard Assignments (Corrected)
# I have matched these exactly to the ports listed above
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_rtl_tri_i[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_rtl_tri_i[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_rtl_tri_i[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_rtl_tri_i[3]}]

set_property IOSTANDARD LVCMOS33 [get_ports {pwm0_0}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm0_1}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm0_2}]

## Configuration Bank Settings
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]