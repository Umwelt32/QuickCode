##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## profile-recognition and polygon-breaking algorithm
## References:
############## DOI:10.1016/j.cageo.2006.06.007
##############
##################################################

import cv2,os,sys,numpy,math

m_img    = None
m_points = None

class profile_point:
    def __init__(self, x,y):
        self.m_x=x
        self.m_y=y

def getImgPoint(x,y):
    global m_img
    value = None
    if (m_img.shape[0] < x) and (m_img.shape[1] < y):
        value = m_img[x][y]
    return value

def loadImg(path):
    global m_img
    global m_points
    m_img = cv2.imread(path, cv2.CV_LOAD_IMAGE_GRAYSCALE)

if __name__ == "__main__":
    print("stub")