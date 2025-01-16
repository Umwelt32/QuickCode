import serial,numpy,threading,time,queue
import serial.tools.list_ports

m_comm_devices_list = None
m_last_error = None

class serial_io:
    def __init__(self,use_thread=False,debug=False):
        self.m_debug  = debug
        self.m_serial = None
        self.m_comm_devices_list = None
        self._get_serial_list()
        self.m_shall_run  = True
        self.m_fifo       = queue.Queue()
        self.m_last_error = None
        self.m_rx_thread  = threading.Thread(target=self._rx_thread) if use_thread else None
        if self.m_rx_thread: self.m_rx_thread.start()
    def shutdown(self):
        self.m_shall_run = False
        if self.m_rx_thread: self.m_rx_thread.join()
        self.close()
    def getLastError(self):
        e = self.m_last_error
        self.m_last_error = None
        return e
    def _debug_print(self,msg,fnc=''):
        _smsg = str(fnc)+';'+str(msg)
        self.m_last_error = _smsg
        if self.m_debug: print(_smsg)
    def _rx_thread(self):
        while self.m_shall_run:
            if self.isOpen():
                byte = None
                try:
                    byte = self.m_serial.read(1)
                except:
                    pass
                else:
                    if len(byte)>0: self.m_fifo.put(int(byte[0]))
    def open(self, dev_name,brate=9600,bsize=8,timeout=2,stopbits=serial.STOPBITS_ONE):
        try:
            if self.isOpen():self.close()
            self.m_serial = serial.Serial(port=str(dev_name), baudrate=brate, bytesize=bsize, timeout=timeout, stopbits=serial.STOPBITS_ONE)
        except Exception as e:
            self.m_serial = None
            self._debug_print(str(e),'open')
    def close(self):
        if self.m_serial:
            self.m_serial.close()
            self.m_fifo   = queue.Queue()
            self.m_serial = None
    def isOpen(self):
        if self.m_serial:
            return True if self.m_serial.is_open else False
        else:
            return False
    def get_serial_list(self):
        return self._get_serial_list()
    def send(self,s,text=False):
        try:
            self.m_serial.write(s.encode() if text else bytes(s))
        except Exception as e:
            self._debug_print(str(e),'send_text')
            return False
        else:
            return True
    def read(self,num=0):
        data=[]
        if self.m_rx_thread:
            while not self.m_fifo.empty(): data.append(self.m_fifo.get())
        else:
            pass
        return None if len(data)==0 else data
    def _get_serial_list(self):
        if self.m_comm_devices_list==None:
            e = _get_serial_list()
            self.m_comm_devices_list = e[0]
            if e[1]: self._debug_print(str(e[1]),'_get_serial_list')
        return self.m_comm_devices_list


def _get_serial_list():
    global m_comm_devices_list
    global m_last_error
    m_last_error = None
    if m_comm_devices_list==None:
        try:
            ports = serial.tools.list_ports.comports()
            m_comm_devices_list = [[str(x),str(y),str(z)] for x,y,z in sorted(ports)]
        except Exception as e:
            m_comm_devices_list = None
            m_last_error = str(e)
            print(m_last_error)
    return [m_comm_devices_list,m_last_error]