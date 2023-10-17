##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert KORAD_APP
## References:
##############
##############
##################################################

import korad_ctl

class korad_psu:
    def __init__(self):
        self.close()
    def open(self, dev_name,brate=9600):
        self.m_dev_psu = korad_ctl._getPsuCtlNode(dev_name,brate)
        self.m_dev_label_name = None
    def close(self):
        self.m_dev_psu = None
        self.m_dev_label_name = None
    def isOpen(self):
        return False if self.m_dev_psu==None else True
    def get_identity(self):
        if self.m_dev_label_name==None:
            try:
                self.m_dev_label_name = str(self.m_dev_psu.get_identity())
            except:
                self.m_dev_label_name = '<NOT OPENED!>' if self.m_dev_psu==None else 'Unknow_Name'
        return str(self.m_dev_label_name)
    def get_output_state(self):
        try:
            return self.m_dev_psu.get_output_state()
        except:
            return False
    def get_output_voltage(self):
        try:
            return float(self.m_dev_psu.get_output_voltage()[0])
        except:
            return float(0)
    def get_output_current(self):
        try:
            return float(self.m_dev_psu.get_output_current()[0])
        except:
            return float(0)
    def get_voltage_setpoint(self):
        try:
            return float(self.m_dev_psu.get_voltage_setpoint()[0])
        except:
            return float(0)
    def get_current_setpoint(self):
        try:
            return float(self.m_dev_psu.get_current_setpoint()[0])
        except:
            return float(0)
    def get_ovp_ocp_state(self):
        try:
            return self.m_dev_psu.get_ovp_ocp_state()
        except:
            return False
    def set_current_setpoint(self,a):
        try:
            self.m_dev_psu.set_current_setpoint(a)
        except:
            pass
    def set_ocp_state(self,s):
        try:
            self.m_dev_psu.set_ocp_state(s)
        except:
            pass
    def set_ovp_state(self,s):
        try:
            self.m_dev_psu.set_ovp_state(s)
        except:
            pass 
    def set_output_state(self,state):
        try:
            self.m_dev_psu.set_output_state(state)
        except:
            pass
    def set_voltage_setpoint(self,v):
        try:
            self.m_dev_psu.set_voltage_setpoint(v)
        except:
            pass
