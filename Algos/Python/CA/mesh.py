##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## canny
## References:
##############
##############
##################################################
import os,sys,math,numpy,random,cv2

m_img_data   = None
m_mmesh_data = None

def _create_2d_array(img):
    i = numpy.zeros(img.shape, dtype=numpy.dtype([('x', 'int'), ('y', 'int'),('v', 'int'),('s', '0')]))
    for x in range(img.shape[0]):
        for y in range(img.shape[1]):
            i[x][y]=(x, y,img[x][y],[None,None,None,None])
    return i

if __name__ == "__main__":
    m_img_data   = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
    m_img_data[m_img_data!=0]=1
    l = _create_2d_array(m_img_data)
    print(str(l))
    #cv2.imwrite('canny2.bmp', edges)
    exit(0)
