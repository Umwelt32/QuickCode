##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## KORAD_APP
## References:
##############
##############
##################################################

import PySimpleGUI as sg
import serial_io
import threading
import math
import time

#theme
sg.theme('DarkGrey5')
#comm_devs
m_comm_brates   = ['110', '300', '600', '1200', '2400', '4800', '9600', '14400', '19200', '38400', '57600', '115200', '128000', '256000']
m_comm_dev_list = [dev[0] for dev in serial_io._get_serial_list()[0]]
#layout
m_tab_config          = [[sg.Combo(m_comm_dev_list,size=(15, 1),default_value='select device',key='-c_dev-'),sg.Combo(m_comm_brates,size=(10, 1),default_value='9600', key='-c_rate-'),sg.Button('OPEN',size=(10, 1)),sg.Button('CLOSE',size=(10, 1))],
                        [sg.Text('DEVICE_STATUS:',size=(15, 1)),sg.Text('Disconnected',size=(10, 1),key='-l_dev_status-'),sg.Canvas(size=(25, 25), background_color='red', key='-l_dev_status_led-')]]
m_tab_serial_console  = [[sg.Multiline(size=(64, 10),disabled=True,key='-l_dev_output-')],[sg.HorizontalSeparator()],[sg.Input(size=(54, 10),key="-i_dev_input-"),sg.Button('SEND',key='-b_dev_send-',size=(10, 1))]]
m_tab_options         = [[]]
# Create the Tab elements
tab1 = sg.Tab("Serial IO", m_tab_serial_console)
tab2 = sg.Tab("Serial Configuration", m_tab_config)
tab3 = sg.Tab("Options", m_tab_options)

# Create the TabGroup
tab_group = sg.TabGroup([[tab1, tab2,tab3]])

# Define the window layout
m_layout = [[tab_group]]

class serial_gui:
    def __init__(self,parent,use_thread=False,debug=False):
        self.m_parent              = parent
        self.m_dev                 = serial_io.serial_io(True,debug)
        self.m_shall_run           = True
        self.m_console_text        = ''
        self.m_thread  = threading.Thread(target=self._thread_func) if use_thread else None
        if self.m_thread: self.m_thread.start()
    def isRunning(self):
        return self.m_shall_run
    def close(self):
        self.m_dev.close()
    def open(self,dev,br):
        self.m_dev.open(dev,br)
        e = self.m_dev.getLastError()
        if e: sg.popup_error(e,title='Error')
    def poll(self):
        self._gui_poll_events()
    def send(self,data):
        self.m_dev.send(data)
    def _thread_func(self):
        while self.m_shall_run:
            time.sleep(0.5)
            d = self.m_dev.read()
            if d: self.m_console_text=self.m_console_text+str(d)
            self._gui_update()
    def _gui_update(self):
        if self.m_shall_run:
            self.m_parent['-l_dev_status-'].update('Connected' if self.m_dev.isOpen() else 'Disconnected')
            self.m_parent['-l_dev_status_led-'].Widget.config(background='green' if self.m_dev.isOpen() else 'red')
            self.m_parent['-l_dev_output-'].update(str(self.m_console_text))
    def _gui_poll_events(self):
        if self.m_shall_run:
            event, values = self.m_parent.read()
            if event == sg.WIN_CLOSED or event == 'EXIT':
                self.m_shall_run = False
            elif event == 'OPEN':
                self.open(values['-c_dev-'],values['-c_rate-'])
            elif event == 'CLOSE':
                self.close()
            elif event == '-b_dev_send-':
                self.send([int(x) for x in values['-i_dev_input-'].split(' ')])
            elif event == 'POLL':
                pass

def _main(thread=True,debug=False):
    global m_layout
    window = sg.Window('SERIAL_IO', m_layout, finalize=True,resizable=False)
    node = serial_gui(window,thread,debug)
    while node.isRunning():
        node.poll()
    window.close()

if __name__ == "__main__":
    _main(True,True)
    exit(0)
