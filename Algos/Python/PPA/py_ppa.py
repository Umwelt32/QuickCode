##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## profile-recognition and polygon-breaking algorithm
## References:
############## DOI:10.1016/j.cageo.2006.06.007
##############
##################################################

import cv2,os,sys,numpy,math

m_img        = None
m_points     = None
m_directions = [[0,1],[1,1],[1,0],[1,-1]]

class profile_point:
    def __init__(self, x,y,k,k2):
        self.m_x=x
        self.m_y=y
        self.m_k=k
        self.m_k2=k2
        self.m_isTarget=False
    def TargetRecognition(self,t):
        global m_directions
        global m_img
        self.m_isTarget=False
        point_value = getImgPoint(self.m_x,self.m_y)
        kern_size   = self.m_k2
        for step in m_directions:
            if self.m_isTarget:
                break
            for x in range(1,kern_size):
                if self.m_isTarget:
                    break
                step_size_x = (step[0]*x)
                step_size_y = (step[1]*x)
                p1 = [self.m_x+step_size_x,self.m_y+step_size_y]
                p2 = [self.m_x-step_size_x,self.m_y-step_size_y]
                v1 = int(getImgPoint(p1[0],p1[1]))
                v2 = int(getImgPoint(p2[0],p2[1]))
                input_val = evaluatePoints(t,point_value,v1,v2)
                if (input_val):
                    self.m_isTarget=True
                else:
                    continue
                
def evaluatePoints(t,v_in,v1,v2):
    eval = False
    v_in=int(v_in)
    v1=int(v1)
    v2=int(v2)
    d1=v_in-v1
    d2=v_in-v2
    if (d1<0 and d2<0) or (d1>0 and d2>0):
        d1=int(abs(d1))
        d2=int(abs(d2))
        if d1>t and d2>t:
            eval=True
    return eval

def getImgPoint(x,y):
    global m_img
    m_x = x
    m_y = y
    value = 0
    m_x=min(m_img.shape[0]-1,m_x)
    m_y=min(m_img.shape[1]-1,m_y)
    m_x=max(0,m_x)
    m_y=max(0,m_y)
    value = m_img[m_x][m_y]     
    return int(value)

def loadImg(path):
    global m_img
    global m_points
    m_img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)

def create_profile_points(k_size,k2_size):
    global m_img
    global m_points
    m_points = []
    for x in range(k_size,m_img.shape[0]-k_size,k_size):
        for y in range(k_size,m_img.shape[1]-k_size,k_size):
            m_points.append(profile_point(x,y,k_size,k2_size))
            
def TargetRecognition(t):
    global m_points
    for point in m_points:
        point.TargetRecognition(t)

def DrawCirclesOnTargets(r):
    global m_points
    global m_img
    for point in m_points:
        if point.m_isTarget:
            cv2.circle(m_img,(point.m_x,point.m_y), r, (0,0,255), -1)
      
if __name__ == "__main__":
    loadImg('input.bmp')
    create_profile_points(8,8)
    TargetRecognition(32)
    DrawCirclesOnTargets(4)
    cv2.imwrite('out.bmp', m_img)
    print("stub")
