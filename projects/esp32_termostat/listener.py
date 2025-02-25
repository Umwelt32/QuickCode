import socket
import struct
import numpy
import paho.mqtt.publish as publish


def _handle_packet_C3(packet):
    if packet[0]!=0xA5: return None
    if packet[1]==0x06:
        if packet[5]!=0x74: return None
        tempnr = packet[10]
        temp   = str(chr(packet[11]))+''+str(packet[12])+''+str(packet[13])
        temp   = temp[:-1]+'.'+temp[-1]
        print("Read Temp["+str(tempnr)+"]: "+str(temp)+'\n')
    if packet[1]==0x07:
        if packet[5]!=0x62: return None
        tempnr = packet[10]
        temp   = str(chr(packet[11]))+''+str(packet[12])+''+str(packet[13])
        print("Read Para["+str(tempnr)+"]: "+str(temp)+'\n')

def _handle_packet_C2(packet):
    if packet[0]!=0xA5: return None
    if packet[3]!=0xC2: return None
    device = packet[7]
    uniq   = packet[2]
    if packet[1]==0x06:
        if packet[6]!=0x74: return None
        temp   = ''.join([chr(x) for x in packet[10:15]])
        temp   = temp[:-1]+'.'+temp[-1]
        temp   = float(temp)
        nr = str(chr(packet[8]+0x30))+''+str(chr(packet[9]+0x30))
        nr = uniq
        publish.single(topic="compit/temp/"+str(device)+"/"+str(nr), payload=str(temp), hostname="localhost")
        print("Read Temp["+str(device)+"]["+str(nr)+"]: "+str(temp))
    if packet[1]==0x07:
        if packet[6]!=0x63: return None
        nr       = ''.join([chr(x) for x in packet[10:12]])
        para     = ''.join([chr(x) for x in packet[13:15]])
        nr = int(nr)
        para = int(para)
        publish.single(topic="compit/params/"+str(device)+"/"+str(nr), payload=str(para), hostname="localhost")
        print("Read Para["+str(nr)+"]: "+str(para))

def _handle_packet_sensors(packet):
    if packet[0]!=0xA5: return None
    if packet[3]!=0x00: return None
    if packet[1]!=0x01: return None
    data = packet[5:]
    _sensors = numpy.frombuffer(data, dtype=numpy.int32, count=5, offset=0)
    publish.single(topic="compit/temp/dht11_t",payload=str(_sensors[0]), hostname="localhost")
    publish.single(topic="compit/temp/dht11_h",payload=str(_sensors[1]), hostname="localhost")
    print('DHT11: Temp: '+str(_sensors[0])+' Humi: '+ str(_sensors[1]))

MULTICAST_GROUP = '239.0.0.2'  # Change this to match your multicast group
PORT = 7171  # The port to listen on

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind to the multicast address and port
sock.bind(('192.168.0.132', PORT))

# Join the multicast group
mreq = struct.pack('4s4s', socket.inet_aton(MULTICAST_GROUP), socket.inet_aton('0.0.0.0'))
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

print(f"Listening for multicast messages on {MULTICAST_GROUP}:{PORT}")

while True:
    data, addr = sock.recvfrom(1024)
    _handle_packet_C2(data)
    _handle_packet_sensors(data)
    #print(f"Received from {addr}: "+str([hex(x) for x in data]))