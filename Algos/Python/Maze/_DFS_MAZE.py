##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert DFS algorithm
## References:
##############
##############
##################################################

import cv2,os,sys,numpy,math

m_maze  = None
m_stack = None

def _init(n,seed):
    global m_maze
    global m_stack
    numpy.random.seed(seed)
    m_stack = [[_rnd_rand_int(0,n)*2,_rnd_rand_int(0,n)*2]]
    m_maze  = numpy.zeros((n*2, n*2), dtype=numpy.uint8)
    for x in range(0,m_maze.shape[0],2):
        for y in range(0,m_maze.shape[0],2):
            m_maze[x][y]=2

def _rnd_rand_int(s,e):
    if (s==e): return int(s)
    return int(numpy.random.randint(s,e))

def _dfs_stack():
    global m_stack
    while len(m_stack)>0:
        v = m_stack.pop(0)
        _dfs(v[0],v[1],_rnd_rand_int(1,16))

def _dfs(x,y,stack):
    global m_stack
    xy = [[[x+1,y],[x+2,y]],[[x-1,y],[x-2,y]],[[x,y+1],[x,y+2]],[[x,y-1],[x,y-2]]]
    if _setCellValue(x,y,1)==False: return False
    if stack==0: m_stack.append([x,y])
    if stack==0: return False
    for idx in range(len(xy)):
        if _getCellValue(xy[idx][1][0],xy[idx][1][1])==2:
            _setCellValue(xy[idx][0][0],xy[idx][0][1],1)
            _dfs(xy[idx][1][0],xy[idx][1][1],stack-1)

def _save_img(filename):
    global m_maze
    m_maze[m_maze==1]=255
    m_maze[m_maze==2]=128
    cv2.imwrite(filename, m_maze)

def _getCellValue(x,y):
    global m_maze
    try:
        value = m_maze[x][y]
    except:
        value = None
    return value

def _setCellValue(x,y,v):
    global m_maze
    try:
        m_maze[x][y]=v
    except:
        return False
    else:
        return True

if __name__ == "__main__":
    _init(128,128)
    _dfs_stack()
    _save_img('out.bmp')
    
