import socket
import numpy

m_socket = None

def udp_send(target_ip,target_port,data):
    global m_socket
    try:
        m_socket.sendto(data, (target_ip, target_port))
        print('UDP packet sent successfully.')
    except socket.error as e:
        print('Error while sending UDP packet:', e)
        return False
    else:
        return True

def udp_listen(listen_ip,listen_port):
    global m_socket
    m_socket.bind((listen_ip, listen_port))

def udp_recv(buffer_size):
    global m_socket
    try:
        data, address = m_socket.recvfrom(buffer_size)
    except socket.error as e:
        print('recv ex: ', e)
        return None
    else:
        print('recv: '+str(address))
        return [data,address]

def udp_send_raw(target_ip,target_port,data):
    r_data = numpy.array(data,dtype=numpy.uint8)
    return udp_send(target_ip,target_port,r_data)

if __name__ == "__main__":
    m_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_listen('127.0.0.1', 12345)
    udp_send_raw('127.0.0.1',1234,[0,0,0,0,0])
    l = udp_recv(512)
    m_socket.close()
