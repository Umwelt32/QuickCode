##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert KORAD_APP
## References:
##############
##############
##################################################

import PySimpleGUI as sg
import korad_ctl
import korad_psu
import math

#theme
sg.theme('DarkGrey5')
#comm_devs
m_comm_brates   = ['110', '300', '600', '1200', '2400', '4800', '9600', '14400', '19200', '38400', '57600', '115200', '128000', '256000']
m_comm_dev_list = [dev[0] for dev in korad_ctl._get_serial_list()]
#layout
m_layout = [
    [sg.Text('COM_DEV:',size=(15, 1)),sg.Combo(m_comm_dev_list,size=(15, 1),key='-c_dev-'),sg.Combo(m_comm_brates,size=(10, 1),default_value='9600', key='-c_rate-'),sg.Button('OPEN',size=(10, 1)),sg.Button('CLOSE',size=(10, 1))],
    [sg.Text('DEVICE:', size=(15, 1)),sg.Text('<NOT OPENED!>',size=(25, 1), key='-l_dev_name-')],
    [sg.Text('STATUS:', size=(15, 1)),sg.Canvas(size=(25, 25), background_color='red', key='-l_dev_status_led-'),sg.Text('N/A',key='-l_dev_status-',size=(10, 1)),sg.Button('ON',size=(10, 1)),sg.Button('OFF',size=(10, 1))],
    [sg.Text('OUTPUT:', size=(15, 1)),sg.Text('N/A',key='-l_dev_output-',size=(30, 1))],
    [sg.Text('VOLTAGE:',size=(15, 1)),sg.Slider(range=(0, 30), orientation='h', size=(15, 15), default_value=12, key='-s_dev_vset-'),sg.Slider(range=(0, 9), orientation='h', size=(10, 15), default_value=0, key='-s_dev_vsetc-'),sg.Button('SET',size=(10, 1),key='-b_dev_vset-')],
    [sg.Text('CURRENT_MAX:',size=(15, 1)),sg.Slider(range=(0, 10), orientation='h', size=(15, 15), default_value=5, key='-s_dev_cset-'),sg.Slider(range=(0, 9), orientation='h', size=(10, 15), default_value=0, key='-s_dev_csetc-'),sg.Button('SET',size=(10, 1),key='-b_dev_cset-')],
    [sg.Button('EXIT',size=(10, 1)),sg.Button('POLL',size=(15, 1))]
]

class korad_gui:
    def __init__(self,parent,debug=False):
        self.m_parent             = parent
        self.m_psu                = korad_psu.korad_psu(debug)
        self.m_shall_exit         = False
        self.m_dev_name           = None
        self.m_dev_output_enabled = None
        self.m_dev_output_v       = None
        self.m_dev_output_a       = None
        self.m_dev_setpoint_v     = None
        self.m_dev_setpoint_a     = None
        self.m_dev_ocp_enabled    = None
    def isExit(self):
        return self.m_shall_exit
    def close(self):
        self.m_psu.close()
    def open(self,dev,br):
        self.m_psu.open(str(dev),int(br))
        if self.m_psu.isOpen():
            self.psu_poll_update()
        else:
            sg.popup_error(korad_ctl._getLastError(),title='Error')
    def psu_poll_update(self):
        if self.m_psu:
            self.m_dev_name           = self.m_psu.get_identity()
            self.m_dev_output_enabled = self.m_psu.get_output_state()
            self.m_dev_output_v       = self.m_psu.get_output_voltage()
            self.m_dev_output_a       = self.m_psu.get_output_current()
            self.m_dev_setpoint_v     = self.m_psu.get_voltage_setpoint()
            self.m_dev_setpoint_a     = self.m_psu.get_current_setpoint()
            self.m_dev_ocp_enabled    = self.m_psu.get_ovp_ocp_state()
    def gui_update(self):
        vc,v = math.modf(round(self.m_dev_setpoint_v,1))
        cc,c = math.modf(round(self.m_dev_setpoint_a,1))
        self.m_parent['-l_dev_name-'].update(str(self.m_dev_name))
        self.m_parent['-l_dev_status-'].update('ON' if self.m_dev_output_enabled else 'OFF')
        self.m_parent['-l_dev_status_led-'].Widget.config(background='green' if self.m_dev_output_enabled else 'red')
        self.m_parent['-l_dev_output-'].update(str(self.m_dev_output_v)+' V ; '+str(self.m_dev_output_a)+' A')
        self.m_parent['-s_dev_vset-'].update(v)
        self.m_parent['-s_dev_vsetc-'].update(vc*10)
        self.m_parent['-s_dev_cset-'].update(c)
        self.m_parent['-s_dev_csetc-'].update(cc*10)
    def gui_poll_events(self):
        event, values = self.m_parent.read()
        if event == sg.WIN_CLOSED or event == 'EXIT':
            self.m_shall_exit = True
        elif event == 'OPEN':
            self.open(values['-c_dev-'],values['-c_rate-'])
        elif event == 'CLOSE':
            self.close()
        elif event == 'ON':
            self.m_psu.set_output_state(True)
        elif event == 'OFF':
            self.m_psu.set_output_state(False)
        elif event == '-b_dev_vset-':
            v = float(float(values['-s_dev_vset-'])+(float(values['-s_dev_vsetc-'])/float(10)))
            self.m_psu.set_voltage_setpoint(v)
        elif event == '-b_dev_cset-':
            a = float(float(values['-s_dev_cset-'])+(float(values['-s_dev_csetc-'])/float(10)))
            self.m_psu.set_current_setpoint(a)
        elif event == 'POLL':
            pass
        self.psu_poll_update()
        self.gui_update()

def _main(debug=False):
    global m_layout
    window = sg.Window('PSU_GUI', m_layout, resizable=False)
    node = korad_gui(window,debug)
    while node.isExit()==False:
        node.gui_poll_events()
    window.close()

if __name__ == "__main__":
    _main()
