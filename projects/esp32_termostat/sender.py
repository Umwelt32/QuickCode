import socket
import time,numpy

MULTICAST_GROUP = '239.0.0.2'
PORT = 7171
MESSAGE = "Hello, multicast!"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
sock.bind(('192.168.0.132', 12346))

def construct_C3_frame(dst,src,p1,data):
    if data is None: data = [0,0,0,0,0,0]
    if dst is None:  dst  = 0xE4 #0xE4 is sent to everybody 100 | 0x80
    if len(data)<6: data=data+[0,0,0,0,0,0]
    data = data[0:6] # data size is always 6bytes
    dst = numpy.bitwise_or(dst, 0x80) #add Tx bit to address 
    frame = numpy.array([dst] + [p1] + [src] + [0x00] + data + [0x23],dtype=numpy.uint8)
    crc   = numpy.sum(frame[2:-1]) #crc sum bytes byte[2]...byte[-1] presuming that initially byte[3] is 0
    frame[3] = numpy.bitwise_and(crc, 0x7F) #and crc with 0x7F
    list_frame = [int(x) for x in frame]
    return list_frame

def build_package(op,unique,proto,len,data):
    _package = [0x5A,op,unique,proto,len] + data + [0]*64
    _buffer  = numpy.array(_package[0:62], dtype=numpy.uint8).tobytes()
    _buffer  = numpy.frombuffer(_buffer, dtype=numpy.uint16)
    _crc     = numpy.uint16(0)
    for e in _buffer:
        _crc=_crc+e
        if _crc < e:_crc=_crc+1
    _crc=numpy.bitwise_and(_crc, 0xFFFF)
    _crc=numpy.bitwise_not(_crc)
    _crc=numpy.bitwise_and(_crc, 0xFFFF)
    _buffer = numpy.array(_buffer[0:62], dtype=numpy.uint16)
    _buffer = numpy.append(_buffer, [_crc])
    _package = numpy.array(_buffer, dtype=numpy.uint16).tobytes()
    return _package

#C,T,X char stands for something
while True:
    sock.sendto(build_package(1,0,0,11,[0,0]), (MULTICAST_GROUP, PORT))
    time.sleep(2)
    sock.sendto(build_package(7,0,0xC2,11,[70,3]), (MULTICAST_GROUP, PORT))
    time.sleep(2)
    for i in range(1,3,1):
        sock.sendto(build_package(6,i,0xC2,11,[70,i]), (MULTICAST_GROUP, PORT))
        time.sleep(2)