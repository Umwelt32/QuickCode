##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert .hgt file's to .bmp format
## References:
############## https://en.wikipedia.org/wiki/Shuttle_Radar_Topography_Mission
##############
##################################################

import cv2,os,sys,numpy,math

m_img    = None
m_points = None

def loadImg(path):
    global m_img
    global m_points
    m_img = cv2.imread(path, cv2.CV_LOAD_IMAGE_GRAYSCALE)

if __name__ == "__main__":
    print("stub")