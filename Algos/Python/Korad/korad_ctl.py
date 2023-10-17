##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert KORAD_APP
## References:
##############
##############
##################################################

import koradctl
import serial.tools.list_ports

m_comm_devices_list = None
m_last_error = 'No error.'

def _get_serial_list():
    global m_comm_devices_list
    global m_last_error
    if m_comm_devices_list==None:
        try:
            ports = serial.tools.list_ports.comports()
            m_comm_devices_list = [[str(x),str(y),str(z)] for x,y,z in sorted(ports)]
        except Exception as e:
            m_last_error = str(e)
            print(m_last_error)
    return m_comm_devices_list

def _getPsuCtlNode(port_name,brate=9600):
    global m_last_error
    try:
        port = koradctl.get_port(str(port_name),int(brate))
        psu = koradctl.PowerSupply(port)
    except Exception as e:
        m_last_error = str(e)
        print(m_last_error)
        psu = None
    return psu

def _getLastError():
    global m_last_error
    return str(m_last_error)
